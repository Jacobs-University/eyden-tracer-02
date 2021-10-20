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
		// --- PUT YOUR CODE HERE ---

		Vec3f finalColor(0, 0, 0);

		Vec3f color = CShaderFlat::shade(ray);

		Vec3f normal = ray.hit->getNormal(ray);

		Ray reflected = ray;

		Vec3f ambientColor(0, 0, 0);

		if (this->m_ka) {
			finalColor += this->m_ka * color.mul(ambientColor);
		}

		if (this->m_kd > 0 || this->m_ke > 0) {

			Ray I;

            I.org = ray.org + ray.t * ray.dir;

            for (auto &pLight : m_scene.getLights()) {
                I.hit = ray.hit;

                auto radiance = pLight->illuminate(I);

                if (radiance && !this->m_scene.occluded(I)) {
                    if (m_kd > 0) {
                        float cosAngleNormal = normal.dot(I.dir);

                        if (cosAngleNormal > 0)
                            finalColor += this->m_kd * cosAngleNormal * color.mul(radiance.value());
                    }

                    if (m_ks > 0) {
                        float cosAngleReflect = reflected.dir.dot(I.dir);

                        if (cosAngleReflect > 0)
                            finalColor += this->m_ks * powf(cosAngleReflect, this->m_ke) * radiance.value();
                    }
                }
            }
		}
		return finalColor;
	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
