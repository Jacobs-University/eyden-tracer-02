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

	//we will apply the formula given

	virtual Vec3f shade(const Ray& ray) const override
	{
		// --- PUT YOUR CODE HERE ---
		//return RGB(0, 0, 0);
	
		Vec3f normal = ray.hit->getNormal(ray);

		if (normal.dot(ray.dir) > 0)
			normal = -normal;

		Vec3f reflect = normalize(ray.dir - 2 * normal.dot(ray.dir) * normal);

		Vec3f a_Intensity(1, 1, 1);

		Vec3f color = CShaderFlat::shade();

		Vec3f a_Color = m_ka * color;
		Vec3f r_final = a_Color.mul(a_Intensity);

		Ray shadow;
		shadow.org = ray.org + ray.t * ray.dir;

		for (auto pLight : m_scene.getLights()) {
			std::optional<Vec3f> l_Intensity = pLight->illuminate(shadow);
			if (l_Intensity) {
				float cos_l_Normal = shadow.dir.dot(normal);
				if (cos_l_Normal > 0) {
					if (m_scene.occluded(shadow))
						continue;

					Vec3f d_Color = m_kd * color;
					r_final += (d_Color * cos_l_Normal).mul(l_Intensity.value());
				}

				float cos_l_Reflect = shadow.dir.dot(reflect);
				if (cos_l_Reflect > 0) {
					Vec3f s_Color = m_ks * RGB(1, 1, 1); 
					r_final += (s_Color * powf(cos_l_Reflect, m_ke)).mul(l_Intensity.value());
				}
			}
		}

		for (int i = 0; i < 3; i++)
			if (r_final.val[i] > 1) r_final.val[i] = 1;

		return r_final;
	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
