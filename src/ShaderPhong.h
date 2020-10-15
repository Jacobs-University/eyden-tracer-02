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
		Vec3f color_ambient = CShaderFlat::shade(ray) * m_ka;
		Ray ray2;
		Vec3f diff_s = 0;

		for (int i = 0; i < m_scene.getLights().size(); i++) 
		{
			ray2.org = ray.org + ray.t * ray.dir;
			std::optional<Vec3f> lRadiance = m_scene.getLights()[i]->illuminate(ray2);
			ray2.t = std::numeric_limits<float>::infinity();

			if (!m_scene.occluded(ray2)) 
			{
				if (lRadiance) 
				{
					float theta = max(0.0f, ray2.dir.dot(ray.hit->getNormal(ray)));
					diff_s += *lRadiance * theta;
				}
			}
		}

		Vec3f diffuse_color = m_kd * diff_s.mul(CShaderFlat::shade(ray));

		Vec3f specular_sum = 0;
		Ray ray3;

		for (int i = 0; i < m_scene.getLights().size(); i++) 
		{
			ray3.org = ray.org + ray.t * ray.dir;
			std::optional<Vec3f> lRadiance = m_scene.getLights()[i]->illuminate(ray3);
			ray3.t = std::numeric_limits<float>::infinity();
			if (!m_scene.occluded(ray2)) 
			{
				if (lRadiance) 
				{
					Vec3f reflected_distance = ray3.dir - 2 * (ray3.dir.dot(ray.hit->getNormal(ray))) * ray.hit->getNormal(ray);
					float theta = max(0.0f, ray.dir.dot(reflected_distance));

					specular_sum += *lRadiance * pow(theta, m_ke);
				}
			}
		}

		Vec3f specular_color = m_ks * RGB(1, 1, 1).mul(specular_sum);

		return color_ambient + diffuse_color + specular_color;
	}


private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
