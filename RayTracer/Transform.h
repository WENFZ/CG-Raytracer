#pragma once
#include "Object3D.h"
#include "matrix.h"



class Transform :
	public Object3D
{
public:
	Transform(Matrix& m, Object3D* o);
	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	virtual void paint();

	Matrix m_local2world;
	Matrix m_world2local;
	Matrix m_transposeOfworld2local;
	Object3D* m_obj;
};

