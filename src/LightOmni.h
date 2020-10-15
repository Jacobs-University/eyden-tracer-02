#pragma once

#include "ILight.h"

/**
 * @brief Point light source class
 */
class CLightOmni : public ILight
{
public:
	/**
	 * @brief Constructor
	 * @param intensity The emission color and strength of the light source
	 * @param org The position (origin) of the light source
	 * @param castShadow Flag indicatin whether the light source casts shadow
	 */
	CLightOmni(const Vec3f& intensity, const Vec3f& org, bool castShadow = true)
		: ILight(castShadow)
		, m_intensity(intensity)
		, m_org(org)
	{}
	virtual ~CLightOmni(void) = default;

	virtual std::optional<Vec3f> illuminate(Ray& ray) override
	{/*
	 source:
	 http://ogldev.atspace.co.uk/www/tutorial20/tutorial20.html
	 */
		// --- PUT YOUR CODE HERE ---
		Vec3f pRay = m_org - ray.org;
		ray.dir = normalize(pRay);
		ray.t = norm(pRay);
		double attenuation = (ray.t * ray.t);
		return m_intensity/attenuation;

	}


private:
	Vec3f m_intensity;	///< The emission (red, green, blue)
	Vec3f m_org;		///< The light source origin
};
