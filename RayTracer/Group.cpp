#include "Group.h"
Group::Group(int size) 
{
	m_objects = vector<Object3D*>(size, nullptr);
	m_size = size;

}
void Group::addObject(int index, Object3D* obj)
{
	assert(index >= 0 && index < m_size);
	m_objects[index] = obj;
}

void Group::paint()
{
	for (int i = 0; i < m_size; i++)
		m_objects[i]->paint();
}

bool Group::intersect(const Ray& r, Hit& h, float tmin)
{
	bool intersected = false;
	for (int i = 0; i < m_size; i++)
	{
		intersected |= m_objects[i]->intersect(r, h, tmin);
	}
	//for (auto obj : m_objects)
	//	intersected= intersected|| (obj->intersect(r, h, tmin));
	return intersected;
}
bool Group::intersectShadowRay(const Ray& r, float& t2light)
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_objects[i]->intersectShadowRay(r, t2light))
			return true;
	}
	return false;
}



