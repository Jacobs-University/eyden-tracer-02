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
		Vec3f nrm = ray.hit->getNormal(ray);
		double ray_dnrm = ray.dir.dot(nrm);

		if(ray_dnrm > 0) { nrm = -nrm; }

		Vec3f ambient_col = CShaderFlat::shade(ray);
		Vec3f diffuse_col = 0;
		Vec3f specular_col = 0;

		Ray shade;

		int n = 1;

		shade.org = ray.dir * ray.t + ray.org;
		
		for(auto light : m_scene.getLights()) {
			for(int i = 0; i < n; i ++) {
				if(!light->illuminate(shade))
					if(m_scene.occluded(shade))
						continue; 

				Vec3f res = 2 * shade.dir.dot(nrm) * nrm - shade.dir;

				diffuse_col += *light->illuminate(shade) * std::max(0.f, shade.dir.dot(nrm));
				specular_col += *light->illuminate(shade) * std::pow(std::max(0.f, res.dot(-ray.dir)), m_ke);
			}
		}

		return  m_ka * ambient_col * 1.1 + 
				m_kd * ambient_col.mul(diffuse_col / n) + 
				m_ks * RGB(1, 1, 1).mul(specular_col / n);
	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
