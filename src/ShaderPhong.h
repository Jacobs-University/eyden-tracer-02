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

		Ray shadow;
		Vec3f diff = (0, 0, 0);
		Vec3f sular = (0, 0, 0);
		Vec3f amb = CShaderFlat::shade(ray);
		Vec3f Norm_ = ray.hit->getNormal(ray);

		if (ray.dir.dot(Norm_) > 0) { 
			Norm_ = -1 * Norm_;

		}
		shadow.org = ray.dir * ray.t + ray.org;

		for (auto beam : m_scene.getLights()) {
			
		    if (!beam->illuminate(shadow) && m_scene.occluded(shadow)) {
			    continue;
		    }
		    Vec3f smth = 2 * shadow.dir.dot(Norm_) * Norm_ - shadow.dir;

		    diff += *beam->illuminate(shadow) * std::max(0.f, shadow.dir.dot(Norm_));
			sular += *beam->illuminate(shadow) * std::pow(std::max(0.f, smth.dot(-1 * ray.dir)), m_ke);
			
		}

		Vec3f res = m_ks * RGB(1, 1, 1).mul(sular)+ m_ka * amb * 1.1  + m_kd * amb.mul(diff);

		return res;

	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
