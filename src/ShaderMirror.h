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
		//flip surfaceNorm if on other side from incident ray
		Vec3f surfaceNorm = ray.hit->getNormal(ray);
		if (surfaceNorm.dot(ray.dir) > 0) {
			surfaceNorm *= -1;
		}
		
		//mirror ray
		Ray ray2;
		ray2.org = ray.org + ray.t*ray.dir;
		ray2.dir = normalize(ray.dir - ray.dir.dot(surfaceNorm) * surfaceNorm * 2);
		
		return m_scene.RayTrace(ray2);
	}
	
	
private:
	CScene& m_scene;
};
