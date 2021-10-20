#pragma once

#include "ILight.h"
#include "ray.h"

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
	{
		// --- PUT YOUR CODE HERE ---
		ray.hit = nullptr;
		ray.dir = m_org - ray.org;
        ray.t = norm(ray.dir);
        ray.dir = normalize(ray.dir);
        Vec3f res = this->m_intensity / (ray.t * ray.t);

        return res;

	}


private:
	Vec3f m_intensity;	///< The emission (red, green, blue)
	Vec3f m_org;		///< The light source origin
};
