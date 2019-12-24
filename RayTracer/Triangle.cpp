#include "Triangle.h"
#include "glut.h"
#include "material.h"
Triangle::Triangle(Vec3f& a, Vec3f& b, Vec3f& c, Material* m)
{
	m_a = a;
	m_b = b;
	m_c = c;
	m_material = m;
}
bool Triangle::intersect(const Ray& r, Hit& h, float tmin)
{
	// P=O+Dt
	// P=xA+yB+(1-x-y)C
	// O+Dt=x(A-C)+y(B-C)+C
	// (C-A)x+(C-B)y+Dt=C-O
	// [C-A,C-B,D][x,y,t]=[C-O]
	auto AC = m_c - m_a;
	auto BC = m_c - m_b;
	auto D = r.getDirection();
	auto OC = m_c - r.getOrigin();
	struct
	{
		float operator()(Vec3f a,Vec3f b,Vec3f c)
		{
			return operator()
				(
					a.x(), b.x(), c.x(),
					a.y(), b.y(), c.y(),
					a.z(), b.z(), c.z()
				);
		}
		float operator()(float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22)
		{
			return
				(
					m00 * (m11 * m22 - m12 * m21)
					- m01 * (m10 * m22 - m12 * m20)
					+ m02 * (m10 * m21 - m11 * m20)
					);
		}
	}determinant3x3;
	
	float m = determinant3x3(AC, BC, D);
	float x = determinant3x3(OC, BC, D)/m;
	float y = determinant3x3(AC, OC, D)/m;
	float t = determinant3x3(AC, BC, OC)/m;

	if (!(t > tmin&& t < h.getT()))
		return false;
	if (x >= 0 && x <= 1 && y >= 0 && y <= 1&&(x+y)>=0&&(x+y)<=1)
	{
		Vec3f norm;
		Vec3f::Cross3(norm,AC,BC);
		norm.Normalize();
		h.set(t, m_material,norm,r);
		return true;
	}
	return false;
	
}

void Triangle::paint()
{
	auto ab = m_b - m_a;
	auto ac = m_c - m_a;
	Vec3f n;
	Vec3f::Cross3(n, ab, ac);
	n.Normalize();
	float m[4] = { m_material->getDiffuseColor().x(), m_material->getDiffuseColor().y(), m_material->getDiffuseColor().z() ,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m);
	m[0] = m_material->getSpecularColor().x();
	m[1] = m_material->getSpecularColor().y();
	m[2] = m_material->getSpecularColor().z();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m);
	glBegin(GL_TRIANGLES);
	glNormal3f(n.x(), n.y(), n.z());
	glVertex3f(m_a.x(), m_a.y(), m_a.z());
	glVertex3f(m_b.x(), m_b.y(), m_b.z());
	glVertex3f(m_c.x(), m_c.y(), m_c.z());
	glEnd();
}
