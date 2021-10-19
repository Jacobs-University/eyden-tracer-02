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
		
		//flip surfaceNorm if on other side from incident ray
		Vec3f surfaceNorm = ray.hit->getNormal(ray);
		if (surfaceNorm.dot(ray.dir) > 0) {
			surfaceNorm *= -1;
		}
		
		//calc ambient term
		Vec3f ambient = m_ka * m_color;
		
		//calc defuse and specular terms
		Vec3f defuse;
		Vec3f specular;
		std::vector<ptr_light_t> lights = m_scene.getLights();
		for (auto light = lights.begin(); light != lights.end(); light++) {
			
			//get ray and intensity from light source
			Ray I;
			I.org = ray.org + ray.dir*ray.t;
			I.hit = ray.hit;
			Vec3f Li = light->get()->illuminate(I).value();
			
			if (!m_scene.occluded(I)) { //skip if light is blocked
				
				//incrament defuse and specular sums
				double IdotN = I.dir.dot(surfaceNorm);
				if (IdotN > 0) { //only consider front side
					defuse += Li * IdotN;
					
					double IdotR = I.dir.dot(normalize(ray.dir - ray.dir.dot(surfaceNorm) * surfaceNorm * 2));
					if (IdotR > 0) {
						specular += Li * pow(IdotR, m_ke);
					}
				}
			}
		}
		defuse = m_kd * m_color.mul(defuse);
		specular = m_ks * Vec3f(1, 1, 1).mul(specular);
		
		return ambient + defuse + specular;
	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
