#include "Transform.h"
#include "glut.h"
Transform::Transform(Matrix& m, Object3D* o)
	:m_local2world(m)
{
	m_local2world.Inverse(m_world2local);
	m_world2local.Transpose(m_transposeOfworld2local);
	m_obj = o;
}
bool Transform::intersect(const Ray& r, Hit& h, float tmin)
{
	
	Vec3f newOrigin = r.getOrigin();
	m_world2local.Transform(newOrigin);

	Vec3f newDirection(r.getDirection());
	m_world2local.TransformDirection(newDirection);

	Ray localRay(newOrigin, newDirection);

	if (m_obj->intersect(localRay, h, tmin))
	{
		auto n = h.getNormal();
		m_transposeOfworld2local.TransformDirection(n);
		n.Normalize();
		h.set(h.getT(), h.getMaterial(), n, r);
		return true;
	}
	return false;
	
}

void Transform::paint()
{
	glPushMatrix();
	glMultMatrixf(m_local2world.glGet());
	m_obj->paint();
	glPopMatrix();
}
