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

		Vec3f normal = ray.hit->getNormal(ray);


		//theta is the angle between the primitive surface normal and the ray direction

		float theta;
		float dot = normal.dot(ray.dir);

		
		

		return CShaderFlat::shade(ray) * fabs(dot);
	}
};

