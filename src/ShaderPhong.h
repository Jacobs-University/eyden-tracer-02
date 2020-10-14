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
	 * @param ks The specular reflection coefficients
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

	/*
	 * Lr = ka*ca*La + kd*cd Σl=0n-1 Ll(Il·N)+ ks*cs Σl=0n-1 Ll(Il·R)ke
	 * */
	virtual Vec3f shade(const Ray& ray) const override
	{
	    Vec3f normal = ray.hit->getNormal(ray);
	    if (ray.dir.dot(normal) > 0)
	        normal *= -1;
        // r = d−2(d⋅n)n
        Vec3f reflected = normalize(ray.dir - 2*(ray.dir.dot(normal))*normal);
        Vec3f color = CShaderFlat::shade(ray);
        Vec3f ambient = m_ka * color;
        auto ambientIntensity = RGB(1, 1, 1);
        // mul is what we need for element wise multiplication.
        Vec3f final = ambient.mul(ambientIntensity);
        Ray temp;
        temp.org = ray.org + ray.t * ray.dir;
	    for (const auto& light : m_scene.getLights()){
	        auto intensity = light->illuminate(temp);
	        if (!intensity.has_value())
	            continue;
	        double cosLN = temp.dir.dot(normal);
            double cosLR = temp.dir.dot(reflected);
            if (cosLN > 0){
	            /*
	            * "For the purpose of shadow mapping in particular we should note that objects that
	            * are directly lit by the light source reflect all 3 of the light components, while
	            * objects in the shadow only reflect the ambient component."
	            * https://blogs.igalia.com/itoral/2017/07/06/working_lights_shadows_parti_phong_reflection_model/
	            * */
	            if (m_scene.occluded(temp))
	                continue;
	            Vec3f diffuse = m_kd * color;
	            final += (diffuse * cosLN).mul(intensity.value());
	        }
	        // make sure we're in the front side.
	        if (cosLR > 0){
	            Vec3f specular = m_ks * RGB(1, 1, 1);
	            final += (specular * pow(cosLR, m_ke)).mul(intensity.value());
	        }
	    }
	    for (auto &x : final.val)
	        x = MIN(x, 1);
		return final;
	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
