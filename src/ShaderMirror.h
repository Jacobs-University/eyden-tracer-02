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
        Vec3f shadingNormal = ray.hit->getNormal(ray);

        // Calculate the reflected ray origin and direction
        Ray reflected;

        float cos_alpha = -ray.dir.dot(shadingNormal);

        // The origin of the reflected ray is the hitpoint
        reflected.org = ray.org + ray.dir * ray.t;

        if (cos_alpha > 0)
            reflected.dir = normalize(ray.dir + 2 * cos_alpha * shadingNormal);
        else
            reflected.dir = ray.dir;

        return m_scene.RayTrace(reflected);
    }
    
    
private:
    CScene& m_scene;
};
