#include "Plane.h"
#include "glut.h"
#include "matrix.h"
#include "material.h"
Plane::Plane(Vec3f& normal, float d, Material* m)
{
	normal.Normalize();
	m_normal = normal;
	m_d = d;
	m_material = m;
}
bool Plane::intersect(const Ray& r, Hit& h, float tmin)
{
	// PN=d
	// P=O+Dt
	// (O+Dt)N=d
	// ON+DNt=d
	// t=(d-ON)/DN
	auto t = (m_d - r.getOrigin().Dot3(m_normal)) / (r.getDirection().Dot3(m_normal));
	if (t > tmin&& t < h.getT())
	{
		h.set(t, m_material, m_normal, r);
		return true;
	}
	return false;
	
}

void Plane::paint()
{
	float p1x = 100, p1z = -100, p1y = (m_d - m_normal.x() * p1x - m_normal.z() * p1z) / m_normal.y();
	float p2x = -100, p2z = -100, p2y = (m_d - m_normal.x() * p2x - m_normal.z() * p2z) / m_normal.y();
	float p3x = -100, p3z = 100, p3y = (m_d - m_normal.x() * p3x - m_normal.z() * p3z) / m_normal.y();
	float p4x = 100, p4z = 100, p4y = (m_d - m_normal.x() * p4x - m_normal.z() * p4z) / m_normal.y();
	auto m_normal = this->m_normal;
	//m_normal = m_normal * -1;

	float m[4] = { m_material->getDiffuseColor().x(), m_material->getDiffuseColor().y(), m_material->getDiffuseColor().z() ,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m);
	m[0] = m_material->getSpecularColor().x();
	m[1] = m_material->getSpecularColor().y();
	m[2] = m_material->getSpecularColor().z();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m);
	glBegin(GL_POLYGON);
	glNormal3f(m_normal.x(), m_normal.y(), m_normal.z());
	

	glVertex3f(p1x, p1y, p1z);
	glVertex3f(p2x, p2y, p2z);
	glVertex3f(p3x, p3y, p3z);
	glVertex3f(p4x, p4y, p4z);
	glEnd();



	/*glBegin(GL_TRIANGLES);

	glVertex3f(100, 100, (-m_d - 100 * m_normal.x() - 100 * m_normal.y() / m_normal.z()));

	glVertex3f(100, (-m_d - 100 * m_normal.x() - 100 * m_normal.z()) / m_normal.y(), 100);
	glVertex3f((-m_d - 100 * m_normal.y() - 100 * m_normal.z()) / m_normal.x(), 100, 100);
	glNormal3f(-m_normal.x(), -m_normal.y(), -m_normal.z());
	glEnd();*/
}
