#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color) 
  { 
	  diffuseColor = d_color; 
	  specularColor = Vec3f();
	  transparentColor = Vec3f();
	  exponent = 0;
	  reflectiveColor = Vec3f();
	  indexOfRefraction = 0;
  }
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f getSpecularColor()const { return specularColor; }
  virtual Vec3f getRelectiveColor()const { return reflectiveColor; }
  virtual Vec3f getTransparentColor()const { return transparentColor; }
  virtual float getExponent()const { return exponent; }
  virtual float getIndexOfRefraction()const { return indexOfRefraction; }
protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  Vec3f specularColor;
  float exponent;
  Vec3f reflectiveColor;
  Vec3f transparentColor;
  float indexOfRefraction;
};


class PhongMaterial :public Material
{
public:
	PhongMaterial(const Vec3f& diffuseColor,
		const Vec3f& specularColor,
		float exponent,
		const Vec3f& reflectiveColor,
		const Vec3f& transparentColor,
		float indexOfRefraction) :Material(diffuseColor)
	{
		this->specularColor = specularColor;
		this->exponent = exponent;
		this->reflectiveColor = reflectiveColor;
		this->transparentColor = transparentColor;
		this->indexOfRefraction = indexOfRefraction;
	}
	



};

// ====================================================================
// ====================================================================

#endif
