#pragma once
#include "ray.h"
#include "vectors.h"
#include "matrix.h"
class Camera
{
public:
	Camera(Vec3f center, Vec3f direction, Vec3f up);
	virtual ~Camera(){}
	virtual float getTMin() const = 0;
	virtual Ray generateRay(Vec2f point) = 0;
	void rotateCamera(float dx, float dy);
	void truckCamera(float x, float y);
	void dollyCamera(float distance);
	virtual void glPlaceCamera();
	virtual void glInit(int w, int h);


	Vec3f m_center;
	Vec3f m_direction;
	Vec3f m_horizontal;
	Vec3f m_up;

	int m_width;
	int m_height;


};


class OrthographicCamera :
	public Camera
{
public:
	OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size);
	virtual float getTMin() const;
	virtual Ray generateRay(Vec2f point);
	virtual void glInit(int w, int h);
	virtual void glPlaceCamera();

	float m_size;

};

class PerspectiveCamera :
	public Camera
{
public:
	PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float angle);
	virtual float getTMin() const;
	virtual Ray generateRay(Vec2f point);
	virtual void glInit(int w, int h);

	virtual void glPlaceCamera();

	float m_angle;
	Vec3f m_ox;
	Vec3f m_oy;
	
};

