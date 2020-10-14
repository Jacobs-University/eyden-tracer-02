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
	    Vec3f normal = ray.hit->getNormal(ray);
        if (ray.dir.dot(normal) > 0)
            normal *= -1;
        Vec3f reflected_dir = ray.dir - 2*(ray.dir.dot(normal))*normal;

        Ray reflected;
        reflected.dir = normalize(reflected_dir);
        reflected.t = std::numeric_limits<double>::infinity();
        reflected.org = ray.t * ray.dir + ray.org;
		return m_scene.RayTrace(reflected);
	}
	
	
private:
	CScene& m_scene;
};
