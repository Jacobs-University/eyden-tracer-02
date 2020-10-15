#pragma once

#include "IShader.h"

class CShaderMirror : public IShader
{
public:
	/**
	 * @brief Constructor
	 * @param scene Reference to the scene
	 */
	CShaderMirror(CScene& scene)
		: m_scene(scene)
	{}
	virtual ~CShaderMirror(void) = default;

	virtual Vec3f shade(const Ray& ray) const override
	{/*
	 
	 source used:
	 http://www.cs.cornell.edu/courses/cs4620/2012fa/lectures/35raytracing.pdf
	 */
		// --- PUT YOUR CODE HERE ---
		Vec3f N = ray.hit->getNormal(ray);//shading normal
		Vec3f R = (2 * (ray.dir.dot(N)) * N) - ray.dir; //Reflected dir R
		//calculate reflection
		Ray Reflection;

		Reflection.t = INFINITY;
		Reflection.dir = normalize(R);
		Reflection.org = ray.org + (ray.dir * ray.t);

		return m_scene.RayTrace(Reflection);
	}
	
	
private:
	CScene& m_scene;
};
