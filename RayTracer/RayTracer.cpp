#include "RayTracer.h"
#include "scene_parser.h"
#include "Group.h"
#include "material.h"
#include "hit.h"
#include "light.h"
#include <algorithm>
#include "rayTree.h"

RayTracer::RayTracer(SceneParser* s, int max_bounces,float cutoff_weight, bool shadows,bool shade_back)
{
	m_scene = s;
	m_maxBounces = max_bounces;
	m_cutoffWeight = cutoff_weight;
	m_shadows = shadows;
	m_shadeBack = shade_back;
}

Vec3f RayTracer::traceRay(Ray& ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit& hit) const
{
	float epsilon = 0.001;
	if (bounces >= m_maxBounces + 1 || weight < m_cutoffWeight)
	{
		/*if (weight < m_cutoffWeight)
			cout << "weight cut off\n";
		else
			cout << "bounces cut off\n";*/
		return Vec3f();
	}

	if (!m_scene->getGroup()->intersect(ray, hit, tmin))
		return m_scene->getBackgroundColor();
	auto material = hit.getMaterial();
	auto normal = hit.getNormal();
	normal.Normalize();
	auto numLights = m_scene->getNumLights();
	auto color = material->getDiffuseColor() * m_scene->getAmbientLight();
	auto outdir = mirrorDirection(hit.getNormal(), ray.getDirection());

	for (int i = 0; i < numLights; i++)
	{
		Vec3f dir;
		Vec3f col;
		float distance;
		m_scene->getLight(i)->getIllumination(hit.getIntersectionPoint(), dir, col, distance);
		Ray r(hit.getIntersectionPoint(), dir);
		bool shadowsAndInway = m_shadows && m_scene->getGroup()->intersectShadowRay(r, distance);
		RayTree::AddShadowSegment(r, 0, distance);
		Vec3f n = normal;
		if (m_shadeBack && n.Dot3(dir)<0)
			n.Negate();
		if (!shadowsAndInway && n.Dot3(dir) > 0)
		{
			color += material->getDiffuseColor() * col * n.Dot3(dir);
		}
		if (!shadowsAndInway && outdir.Dot3(dir) > 0)
		{
			color += material->getSpecularColor() * col * (powf(outdir.Dot3(dir), material->getExponent()));
		}

		
	}
	
	if (material->getRelectiveColor().Length()>0)
	{
		Ray outgoing(hit.getIntersectionPoint(), outdir);
		Hit h(FLT_MAX, nullptr, Vec3f());
		auto c = traceRay(outgoing, epsilon, bounces + 1, weight/ hit.getT(), indexOfRefraction, h);
		color += material->getRelectiveColor() * c;
		RayTree::AddReflectedSegment(outgoing, epsilon, h.getT());

	}
	if (material->getTransparentColor().Length() > 0)
	{
		Vec3f transmitted;
		bool inside=transmittedDirection(normal, ray.getDirection(), indexOfRefraction, material->getIndexOfRefraction(), transmitted);
		Ray outgoing(hit.getIntersectionPoint(), transmitted);
		Hit h(FLT_MAX, nullptr, Vec3f());
		auto c = traceRay(outgoing, epsilon, bounces + 1, weight / hit.getT(), inside? 1:material->getIndexOfRefraction(), h);
		color += material->getTransparentColor() * c;
		RayTree::AddTransmittedSegment(outgoing, epsilon, h.getT());
	}

	return color;
}

Vec3f RayTracer::mirrorDirection(const Vec3f& normal, const Vec3f& incoming)const
{
	auto n = normal;
	if (n.Dot3(incoming) > 0)
	{
		n.Negate();
	}
	return incoming - 2 * n.Dot3(incoming) * n;
}

bool RayTracer::transmittedDirection(const Vec3f& n, const Vec3f& incoming, float index_i, float index_t, Vec3f& transmitted)const
{
	auto normal = n;
	bool inside = false;
	auto relativeIndex = index_i / index_t;

	if (normal.Dot3(incoming) > 0)
	{
		// inside the obj
		normal.Negate();
		inside = true;
	}
	auto costheta = -incoming.Dot3(normal);
	transmitted = (relativeIndex * costheta - sqrt(1 - relativeIndex * relativeIndex * (1 - costheta * costheta))) * normal + relativeIndex * incoming;

	return inside;
}

