#pragma once

#include "ShaderFlat.h"

/**
 * @brief Eye-light shader class
 */
class CShaderEyelight : public CShaderFlat
{
public:
	/**
	 * @brief Constructor
	 * @details This is a texture-free and light-source-free shader
	 * @param scene Reference to the scene
	 * @param color The color of the object
	 */
	CShaderEyelight(Vec3f color = RGB(0.5f, 0.5f, 0.5f))
		: CShaderFlat(color)
	{}
	virtual ~CShaderEyelight(void) = default;

	virtual Vec3f shade(const Ray& ray) const override
	{
		// --- PUT YOUR CODE HERE ---
		//shader needs to know information about primitive normal
		const auto PrimNormal = ray.hit->getNormal(ray);
		auto DotResult = PrimNormal.dot(ray.dir);

		//getting the angle between two vec3f by dot product
		auto PrimNormal2 = PrimNormal.dot(PrimNormal);
		auto RayDir2 = ray.dir.dot(ray.dir);
		double Angle = acos(DotResult / (PrimNormal2 * RayDir2));
		auto result = -cos(Angle) * CShaderFlat::shade();
		return result;
		//return RGB(0, 0, 0);
	}
};

