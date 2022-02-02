#pragma once

#include "ShaderFlat.h"
#include "LightOmni.h"
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
		Vec3f Color = m_color;
		Vec3f AmbientColor = RGB(0, 0, 0);
		//Vec3f DiffuseColor;
		Vec3f SpecularColor = RGB(1, 1, 1);

		Vec3f AmibentRadiance;
		Vec3f RadianceArriving;

		Vec3f SurfaceNormal = ray.hit->getNormal(ray);
		if (SurfaceNormal.dot(ray.dir) > 0) {
			SurfaceNormal *= -1;
		}
		//getting each light sources
		auto LightSources = m_scene.getLights();

		//calucalte ambient diffuse specular beforehand
		Vec3f ambient = m_ka * Color;
		Vec3f SumDiffuse;
		Vec3f SumSpecular;

		for (auto EachLight : LightSources) {
			
			Ray TempRay, ReflectedRay;
			TempRay.org = ray.org + ray.dir * ray.t;
			TempRay.hit = ray.hit;
			ReflectedRay = ray;
			ReflectedRay.dir = ray.dir - ray.dir.dot(SurfaceNormal) * SurfaceNormal * 2;
			//if occuluded then skip
			//if (!m_scene.occluded(TempRay)) {
				//get each light radiance 
				Vec3f EachRadiance = EachLight->illuminate(TempRay).value();
				//for diffuse
				auto CosTheta = SurfaceNormal.dot(TempRay.dir);
				if (CosTheta > 0) {
					SumDiffuse += EachRadiance * CosTheta;
				}
				//for specular
					//calculating Angle
				double DotResult = TempRay.dir.dot(normalize(ReflectedRay.dir));
				if (DotResult > 0) {
					DotResult = pow(DotResult, m_ke);
					SumSpecular += EachRadiance * DotResult;
				}
			//}
				
		}
		SumDiffuse = SumDiffuse.mul(m_color) * m_kd;
		SumSpecular = SumSpecular.mul(SpecularColor) * m_ks;
		Vec3f FinalResult;
		FinalResult += ambient;
		//auto FinalResult = ambient + SumSpecular;
		//auto FinalResult = SumSpecular;
		FinalResult += SumDiffuse;
		FinalResult += SumSpecular;
		return FinalResult;
	}
	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
