#include <Windows.h>
#include "glut.h"
#include "Camera.h"
#include <limits>
#include "matrix.h"

Camera::Camera(Vec3f center, Vec3f direction, Vec3f up)
{
	m_center = center;
	direction.Normalize();
	m_direction = direction;
	m_up = up - up.Dot3(direction) * direction;
	m_up.Normalize();
	Vec3f::Cross3(m_horizontal, m_direction, m_up);
	
	printf("center of camera (%2f,%2f,%2f)\n", m_center.x(), m_center.y(), m_center.z());
	printf("direction of camera (%2f,%2f,%2f)\n", m_direction.x(), m_direction.y(), m_direction.z());
	printf("up of camera (%2f,%2f,%2f)\n", m_up.x(), m_up.y(), m_up.z());


}

void Camera::rotateCamera(float dx, float dy)
{
	auto horizontal=Matrix::MakeAxisRotation(m_up, dx);
	auto up = Matrix::MakeAxisRotation(m_horizontal, dy);
	auto total = horizontal*up ;
	total.Transform(m_center);
	total.TransformDirection(m_direction);
	total.TransformDirection(m_up);
	total.TransformDirection(m_horizontal);
	//printf("center of camera (%2f,%2f,%2f)\n", m_center.x(), m_center.y(), m_center.z());
	//printf("direction of camera (%2f,%2f,%2f)\n", m_direction.x(), m_direction.y(), m_direction.z());
	//printf("up of camera (%2f,%2f,%2f)\n", m_up.x(), m_up.y(), m_up.z());

}

void Camera::truckCamera(float x, float y)
{
	// translation
	m_center += m_horizontal * (x/10)+ m_up * (y/10);
	/*printf("center of camera (%2f,%2f,%2f)\n", m_center.x(), m_center.y(), m_center.z());
	printf("direction of camera (%2f,%2f,%2f)\n", m_direction.x(), m_direction.y(), m_direction.z());
	printf("up of camera (%2f,%2f,%2f)\n", m_up.x(), m_up.y(), m_up.z());*/

}

void Camera::dollyCamera(float distance)
{
	// zoom in out
	m_center += distance * m_direction*50;
	/*printf("center of camera (%2f,%2f,%2f)\n", m_center.x(), m_center.y(), m_center.z());
	printf("direction of camera (%2f,%2f,%2f)\n", m_direction.x(), m_direction.y(), m_direction.z());
	printf("up of camera (%2f,%2f,%2f)\n", m_up.x(), m_up.y(), m_up.z());*/

	//printf("%f", distance);
}

void Camera::glPlaceCamera()
{
	//glLoadMatrixf((getProjectionMatrix() * getViewMatrix()).glGet());
	//glLoadMatrixf(getViewMatrix().glGet());
	//glLoadMatrixf(getProjectionMatrix() .glGet());
	
	//glutWireTeapot(2);

}

void Camera::glInit(int w, int h)
{
	
	
}




OrthographicCamera:: OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size)
	:Camera(center,direction,up)
{
	cout << "using Orthographic Camera\n";
	m_size = size;
}

float OrthographicCamera::getTMin() const
{
	return -9999;
}
Ray OrthographicCamera::generateRay(Vec2f point)
{
	Vec3f orig=m_center+(point.x()-0.5)*m_size*m_horizontal+(point.y()-0.5)*m_size*m_up;
	return Ray(orig,m_direction);
}

void OrthographicCamera::glInit(int w, int h)
{
	m_width = w;
	m_height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-m_size/2, m_size/2, -m_size/2, m_size/2, -1000, 1000);
	//gluOrtho(0, w, 0, h);
}

void OrthographicCamera::glPlaceCamera()
{
	GLdouble eye[3] = { m_center.x(),m_center.y(),m_center.z() };
	GLdouble center[3] = { m_center.x() + m_direction.x(),m_center.y() + m_direction.y(),m_center.z() + m_direction.z() };
	GLdouble up[3] = { m_up.x(),m_up.y(), m_up.z() };
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
}


PerspectiveCamera::PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float angle)
	:Camera(center, direction, up)

{
	cout << "using Perspective Camera\n";

	m_angle = angle;
	
}
float PerspectiveCamera::getTMin() const
{
	return FLT_TRUE_MIN;
}
Ray PerspectiveCamera::generateRay(Vec2f point)
{
	// ¼ÆËãÆÁÄ»µÄ¿í¶È
	// |ox|=cos(2/costheta-2)
	// (ox,oy)=t*(horizontal,up)
	// 
	m_ox = sqrt(2 / cos(m_angle) - 2) * m_horizontal;
	m_oy = sqrt(2 / cos(m_angle) - 2) * m_up;
	auto fp = (point.x() - 0.5) * m_ox + (point.y() - 0.5) * m_oy;
	auto direction = m_direction + fp;
	direction.Normalize();
	return Ray(m_center, direction);
}

void PerspectiveCamera::glInit(int w, int h)
{
	m_width = w;
	m_height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(m_angle*180/3.1415926, (1.0 * w / h), 0.01, 100000);
}

void PerspectiveCamera::glPlaceCamera()
{
	GLdouble eye[3] = { m_center.x(),m_center.y(),m_center.z() };
	GLdouble center[3] = { m_center.x() + m_direction.x(),m_center.y() + m_direction.y(),m_center.z() + m_direction.z() };
	GLdouble up[3] = { m_up.x(),m_up.y(), m_up.z() };
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
}



//Matrix PerspectiveCamera::getProjectionMatrix()
//{
//	Matrix m; // zero matrix
//
//	float yScale = 1.f / tanf(0.5f * m_angle);
//	float xScale = yScale / (1.0*m_width/m_height);
//	float zFar = 1000;
//	float zNear = 0.1;
//
//	m.Set(0, 0, xScale);
//	m.Set(1, 1, yScale);
//	m.Set(3, 2, -1);
//
//
//	bool directX = false;
//	if (directX)
//	{
//		m.Set(2, 2, zFar / (zNear - zFar));
//		m.Set(2, 3, zNear * zFar / (zNear - zFar));
//	}
//	else
//	{
//		m.Set(2, 2, (zFar + zNear) / (zNear - zFar));
//		m.Set(2, 3, 2.f * zFar * zNear / (zNear - zFar));
//	}
//
//	return m;
//}
//
