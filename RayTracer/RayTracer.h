#pragma once
#include "hit.h"
class SceneParser;
class RayTracer
{
public:
	RayTracer(SceneParser* s, int max_bounces,float cutoff_weight,bool shadows,bool shade_back);
	Vec3f traceRay(Ray& ray, float tmin, int bounces, float weight,
		float indexOfRefraction, Hit& hit) const;


	Vec3f mirrorDirection(const Vec3f& normal, const Vec3f& incoming)const;
	bool transmittedDirection(const Vec3f& normal, const Vec3f& incoming,
		float index_i, float index_t, Vec3f& transmitted)const;
	SceneParser* m_scene;
	int m_maxBounces;
	float m_cutoffWeight;
	bool m_shadows;
	bool m_shadeBack;
};

