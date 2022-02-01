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
        Vec3f N=ray.hit->getNormal(ray);
        if (ray.dir.dot(N) > 0) {
            N = N * -1;
        }
        Vec3f R = ray.dir - 2 * (ray.dir.dot(N)) * N;
        Ray reflectedRay;
        reflectedRay.dir = normalize(R);
        reflectedRay.t = std::numeric_limits<float>::max();
        reflectedRay.org = ray.dir * ray.t + ray.org;
        return m_scene.RayTrace(reflectedRay);	}
	
	
private:
	CScene& m_scene;
};
