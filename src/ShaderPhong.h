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
		//return RGB(0, 0, 0);
		Vec3f normal = ray.hit->getNormal(ray);
		if (ray.dir.dot(normal) > 0) {
			normal *= -1;
		}
		//ambient intensity
		Vec3f ambienceIntensity(1, 1, 1);
		
		//ambient color c_a
		Vec3f reflect = normalize(ray.dir - 2 * (ray.dir.dot(normal)) * normal);
		Vec3f color = CShaderFlat::shade();
		Vec3f c_a = m_ka * color;
		Vec3f endproduct = c_a.mul(ambienceIntensity);
		Vec3f res = c_a.mul(ambienceIntensity);


		Ray shadow; 
		shadow.org = ray.org + ray.t * ray.dir;
		for (auto pLight : m_scene.getLights()) {
			std::optional<Vec3f> lightIntensity = pLight->illuminate(shadow);
			float cosLightNormal = shadow.dir.dot(normal);
			if (cosLightNormal > 0) {
				if (m_scene.occluded(shadow)) {
					continue;
				}
				Vec3f diffuseColor = m_kd * color;
				res += (diffuseColor * cosLightNormal).mul(cosLightNormal).mul(lightIntensity.value());
			}
			float cosLightReflect = shadow.dir.dot(reflect);
			if (cosLightReflect > 0) {
				Vec3f specularColor = m_ks * RGB(1, 1, 1);
				res += (specularColor * powf(cosLightReflect, m_ke)).mul(lightIntensity.value());
			}

		}
		for (int i = 0; i < 3; i++)
			if (res.val[i] > 1) res.val[i] = 1;
		return res;
	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
