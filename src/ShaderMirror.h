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
		Ray shade;
		Vec3f nrm = ray.hit->getNormal(ray);
		double ray_dnrm = ray.dir.dot(nrm);

		if(ray_dnrm > 0) { nrm = -nrm; }

		shade.dir = normalize(ray.dir - ray_dnrm * nrm * 2);
		shade.t = std::numeric_limits<float>::max();
		shade.org = ray.dir * ray.t + ray.org;

		return m_scene.RayTrace(shade);
	}
	
	
private:
	CScene& m_scene;
};
