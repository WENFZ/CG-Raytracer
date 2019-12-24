#pragma once
#include "Object3D.h"
class Plane :
	public Object3D
{
public:
	Plane(Vec3f& normal, float d, Material* m);
	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	virtual void paint();

	Vec3f m_normal;
	float m_d;

};

