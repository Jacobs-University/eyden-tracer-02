#pragma once

#include "ShaderFlat.h"

class CShaderPhong : public CShaderFlat
{
public:
	/**
	 * @brief Constructor
	 * @param scene Reference to the scene
	 * @param color The color of the object
	 * @param ka The ambient coefficient
	 * @param kd The diffuse reflection coefficients
	 * @param ks The specular refelection coefficients
	 * @param ke The shininess exponent
	 */
	CShaderPhong(CScene& scene, Vec3f color, float ka, float kd, float ks, float ke)
		: CShaderFlat(color)
		, m_scene(scene)
		, m_ka(ka)
		, m_kd(kd)
		, m_ks(ks)
		, m_ke(ke)
	{}
	virtual ~CShaderPhong(void) = default;

	virtual Vec3f shade(const Ray& ray) const override
	{
        Vec3f s(0, 0, 0);

        Vec3f color = CShaderFlat::shade(ray);

        Vec3f shadingNormal = ray.hit->getNormal(ray);

        // Calculate the reflected ray origin and direction
        Ray reflected;

        if (m_ks > 0) {
            float cos_alpha = -ray.dir.dot(shadingNormal);

            // The origin of the reflected ray is the hitpoint
            reflected.org = ray.org + ray.dir * ray.t;

		    if (cos_alpha > 0)
                reflected.dir = normalize(ray.dir + 2 * cos_alpha * shadingNormal);
            else
                reflected.dir = ray.dir;
        } else
            reflected = ray;

        // Ambient
        if (m_ka > 0)
            // Add the ambient color based on the coefficient to the shader
            // Ambient color is hardcoded
            s += m_ka * color.mul(Vec3f(0, 0, 0));

        if (m_kd > 0 || m_ke > 0) {
            // Set up the ray poiting from hitpoint to light source
            Ray I;
            I.org = ray.org + ray.t * ray.dir;

            // Calculate diffusion and specular reflection from each light source
            for (auto &pLight : m_scene.getLights()) {
                I.hit = ray.hit;

                // Retrieve radiance from light based on hitpoint and update direction of I
                auto radiance = pLight->illuminate(I);

                if (radiance && (!pLight->shadow() || !m_scene.occluded(I))) {
                    // Calculate diffusion of the light based on the object
                    if (m_kd > 0) {
                        float cosLightNormal = shadingNormal.dot(I.dir);

                        if (cosLightNormal > 0)
                            s += m_kd * cosLightNormal * color.mul(radiance.value());
                    }

                    // Calculate specular reflection of the light based on the object
                    if (m_ks > 0) {
                        float cosLightReflect = reflected.dir.dot(I.dir);

                        if (cosLightReflect > 0)
                            s += m_ks * powf(cosLightReflect, m_ke) * radiance.value();
                    }
                }
            }
        }

        return s;
	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
