#pragma once
#include "Object3D.h"
#include <vector>
class Sphere :
	public Object3D
{
public:

	Sphere(Vec3f center, float radius, Material* material);
	virtual void paint();

	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	Vec3f m_center;
	float m_radius;

	vector<float> points;// t+1 pts
	vector<float> normals;
	vector<int> indices;
	int t;
};

