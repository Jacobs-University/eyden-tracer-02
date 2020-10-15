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
		
		Vec3f snormal = ray.hit->getNormal(ray);//shading normal N
		//ray may hit the backside of a surface
		if (ray.dir.dot(snormal) > 0)
			snormal = snormal*-1;//turn around		
		//get reflection vector R
		Vec3f reflection = normalize(ray.dir - 2 * (ray.dir.dot(snormal)) * snormal);
		
		Vec3f color = CShaderFlat::shade();//get colour
		Vec3f ambientC = m_ka * color;//ambient coefficient * colour
		Vec3f ambience= RGB(1, 1, 1);//ambience intensity

		//vector elements multiplication.
		Vec3f AmbientVec = ambientC.mul(ambience);

		Ray L;//Direction Ray L for Light
		L.org = ray.org + ray.dir * ray.t;

		//check all light sources
		for (auto& it : m_scene.getLights())
		{
			//auto Light = m_scene.getLights();
			std::optional<Vec3f> intensity = it->illuminate(L);

			if (intensity)
			{//diffuse
				double CLN = L.dir.dot(snormal);

				//front side check
				if (CLN > 0)
				{//ambient occlusion
					if (m_scene.occluded(L))
					{
						Vec3f diffuseC = m_kd * color;
						AmbientVec = AmbientVec + (diffuseC * CLN).mul(intensity.value());
					}
				}
				//specular
				double CLR = reflection.dot(L.dir);
				if (CLR > 0)
				{
					Vec3f specular = m_ks * RGB(1, 1, 1);
					AmbientVec = AmbientVec + (specular * pow(CLR, m_ke)).mul(intensity.value());
				}
			}
		}

		for (auto& x : AmbientVec.val)
			if (x > 1)
				x = 1;

		return AmbientVec;
	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
