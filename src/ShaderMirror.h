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
		Vec3f normal = ray.hit->getNormal(ray);
        Ray reflectedRay;
        float cosAngle = -ray.dir.dot(normal);

        reflectedRay.org = ray.org + ray.dir * ray.t;

        if (cosAngle > 0)
            reflectedRay.dir = normalize(ray.dir + 2 * cosAngle * normal);
        else
            reflectedRay.dir = ray.dir;

        return this->m_scene.RayTrace(reflectedRay);
	}
	
	
private:
	CScene& m_scene;
};
