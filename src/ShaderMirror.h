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
		Vec3f Norm_ = ray.hit->getNomal(ray);
		if (ray.dir.dot(Norm_) > 0 ) {
			Norm_ = -1 * Norm_ ;
		}
		Ray shadow;
		shadow.dir = normalize(ray.dir - 2* (ray.hit->getNomal(ray)) * Norm_ );
		shadow.org = ray.dir * ray.t + ray.org;
		shadow.t = std::numeric_limits<float>::max();
		Vec3f scene_shade = m_scene.RayTrace(shadow);
		return scene_shade;
	}
	
	
private:
	CScene& m_scene;
};
