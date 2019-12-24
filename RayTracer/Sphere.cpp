#include "Sphere.h"
#include "glut.h"
#include <vector>
#include "material.h"
#include "matrix.h"

Sphere::Sphere(Vec3f center, float radius, Material* material)
{
	m_center = center;
	m_radius = radius;
	m_material = material;
	t = 50;

	float PI = 3.1415926;
	for (int i = 0; i <= t; i++)
	{
		for (int k = 0; k <= t; k++)
		{
			auto x = m_radius * cos(k * 2 * PI / t) * cos(i * 2 * PI / t) + m_center.x();
			auto y = m_radius * sin(k * 2 * PI / t) + m_center.y();
			auto z = m_radius * cos(k * 2 * PI / t) * sin(i * 2 * PI / t) + m_center.z();

			points.push_back(x);
			points.push_back(y);
			points.push_back(z);

			normals.push_back((x-m_center.x()) / m_radius);
			normals.push_back((y - m_center.y()) / m_radius);
			normals.push_back((z - m_center.z()) / m_radius);

		}
	}

	for (int i = 0; i < t; i++)
	{
		for (int k = 0; k < t; k++)
		{
			indices.push_back(i * (t + 1) + k);
			indices.push_back((i + 1) * (t + 1) + k);
			indices.push_back((i + 1) * (t + 1) + k + 1);

			indices.push_back(i * (t + 1) + k);
			indices.push_back((i + 1) * (t + 1) + k + 1);
			indices.push_back(i * (t + 1) + k + 1);

		}
	}
}
void Sphere::paint()
{
	float m[4] = { m_material->getDiffuseColor().x(), m_material->getDiffuseColor().y(), m_material->getDiffuseColor().z() ,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m);
	
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < t; i++)
	{
		for (int k = 0; k < t; k++)
		{
			// µÚi*t+k¸ö¾ØÐÎ

			int index = indices[(i * t + k) * 6];

			glNormal3f(normals[index * 3], normals[index * 3 + 1], normals[index * 3 + 2]);


			glVertex3f(points[index * 3], points[index * 3 + 1], points[index * 3 + 2]);
			index = indices[(i * t + k) * 6 + 1];
			glVertex3f(points[index * 3], points[index * 3 + 1], points[index * 3 + 2]);
			index = indices[(i * t + k) * 6 + 2];
			glVertex3f(points[index * 3], points[index * 3 + 1], points[index * 3 + 2]);


			index = indices[(i * t + k) * 6 + 3];
			glNormal3f(normals[index * 3], normals[index * 3 + 1], normals[index * 3 + 2]);

			glVertex3f(points[index * 3], points[index * 3 + 1], points[index * 3 + 2]);
			index = indices[(i * t + k) * 6 + 4];
			glVertex3f(points[index * 3], points[index * 3 + 1], points[index * 3 + 2]);
			index = indices[(i * t + k) * 6 + 5];
			glVertex3f(points[index * 3], points[index * 3 + 1], points[index * 3 + 2]);

		}
	}
	glEnd();


	/*glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, points.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < t; i++)
	{
		for (int k = 0; k < t; k++)
		{
			glArrayElement(indices[(i * t + k) * 6 + 0]);
			glArrayElement(indices[(i * t + k) * 6 + 1]);
			glArrayElement(indices[(i * t + k) * 6 + 2]);

			glArrayElement(indices[(i * t + k) * 6 + 3]);
			glArrayElement(indices[(i * t + k) * 6 + 4]);
			glArrayElement(indices[(i * t + k) * 6 + 5]);
		}
	}
	glEnd();*/
	
}
bool Sphere::intersect(const Ray& r, Hit& h, float tmin)
{
	// P=O+Dt
	// cpcp=rr
	// (p-c)(p-c)=rr
	// pp-2p.c+cc=rr
	// (O+Dt)(O+Dt)-2(O+Dt)C+CC-rr=0
	// OO+2ODt+DDtt-2OC-2tDC+CC-rr=0
	// DDtt+(2OD-2DC)t+OO+CC-2OC-rr=0
	
	auto D = r.getDirection();
	auto O = r.getOrigin();
	auto C = m_center;


	auto a = D.Dot3(D);
	auto b = (O.Dot3(D) - D.Dot3(C)) * 2;
	auto c = O.Dot3(O) + C.Dot3(C) - 2 * O.Dot3(C) - m_radius * m_radius;
	//auto delta = b * b - 4 * c*r.getDirection().Dot3(r.getDirection());
	
	auto delta = b * b - 4 * a * c;
	if (delta >= 0)
	{
		auto t = (-b - sqrt(delta)) / 2/a;
		if (t < 0.001)	
			t = (-b + sqrt(delta)) / 2/ a;
		if (t > tmin&& t < h.getT())			// update the hit with t
		{
			auto norm = r.pointAtParameter(t) - m_center;
			norm.Normalize();
			h.set(t, m_material, norm,r);
			//h = Hit(t, m_material,norm);
			return true;
		}
	}
	return false;
}
