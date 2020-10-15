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
	{
		// --- PUT YOUR CODE HERE ---
		//return Vec3f(0,0,0);

		Vec3f normal = ray.hit->getNormal(ray);

		if (ray.dir.dot(normal) > 0)
			normal *= -1;

		Vec3f r_dir = ray.dir - 2 * (ray.dir.dot(normal)) * normal;

		Ray r;
		r.dir = normalize(r_dir);
		r.t = std::numeric_limits<double>::infinity();
		r.org = ray.t * ray.dir + ray.org;
		return m_scene.RayTrace(r);
	}
	
	
private:
	CScene& m_scene;
};
