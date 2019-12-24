#pragma once
#include "ray.h"
#include "hit.h"
#include <assert.h>

class Object3D
{
	float epsilon = 0.001;
public:
	Object3D() { m_material = nullptr; }
	virtual ~Object3D(){}
	virtual void paint() = 0;
	virtual bool intersect(const Ray& r, Hit& h, float tmin) = 0;
	virtual bool intersectShadowRay(const Ray& r, float& t2light)
	{
		Hit h(t2light, nullptr, Vec3f());
		bool in= intersect(r, h, epsilon);
		t2light = h.getT();
		return in;
	}
	Material* m_material;
};