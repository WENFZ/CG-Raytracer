#pragma once
#include "Object3D.h"
#include <vector>
class Group :
	public Object3D
{
public:
	Group(int size);
	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	void addObject(int index, Object3D* obj);
	virtual void paint();

	virtual bool intersectShadowRay(const Ray& r, float& t2light);

	vector<Object3D*> m_objects;
	int m_size;
};

