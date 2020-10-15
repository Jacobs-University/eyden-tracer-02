// Scene class
// Written by Sergey Kosov in 2019 for Jacobs University
#pragma once

#include "ILight.h"
#include "IPrim.h"
#include "CameraPerspective.h"

// ================================ Scene Class ================================
/**
 * @brief Scene class
 */
class CScene
{
public:
	/**
	 * @brief Constructor
	 * @param camera The reference to the camera
	 * @todo Background may be image
	 */
	CScene(Vec3f bgColor = RGB(0,0,0))
		: m_bgColor(bgColor)
	{}
  	~CScene(void) = default;
  
	/**
	 * @brief Adds a new primitive to the scene
	 * @param prim Pointer to the primitive
	 */
	void add(const ptr_prim_t pPrim)
	{
		// --- PUT YOUR CODE HERE ---
		m_vpPrims.push_back(pPrim);
	}
	/**
	 * @brief Adds a new light to the scene
	 * @param pLight Pointer to the light
	 */
	void add(const ptr_light_t pLight)
	{
		// --- PUT YOUR CODE HERE ---
		m_vpLights.push_back(pLight);
	}
	/**
	 * @brief Adds a new camera to the scene and makes it to ba active
	 * @param pCamera Pointer to the camera
	 */
	void add(const ptr_camera_t pCamera)
	{
		// --- PUT YOUR CODE HERE ---
		m_vpCameras.push_back(pCamera);
		std::vector<ptr_camera_t>::iterator it = std::find(m_vpCameras.begin(), m_vpCameras.end(), pCamera);
		if(it != m_vpCameras.end())
			m_activeCamera = std::distance(m_vpCameras.begin(), it);
		else
			std::cout << "new Camera couldnt be added - error" << std::endl;
	}
	/**
	 * @brief Returns the container with all scene light source objects
	 * @note This method is to be used only in OpenRT shaders
	 * @return The vector with pointers to the scene light sources
	 */
	const std::vector<ptr_light_t>	getLights(void) const { return m_vpLights; }
	/**
	 * @brief Returns the active camera
	 * @retval ptr_camera_t The pointer to active camera
	 * @retval nullptr If there are no cameras added yet into the scene
	 */
	ptr_camera_t getActiveCamera(void) const { return m_vpCameras.empty() ? nullptr : m_vpCameras.at(m_activeCamera); }
	/**
	 * @brief Checks intersection of ray \b ray with all contained objects
	 * @param ray The ray
	 * @retval true If ray \b ray intersects any object
	 * @retval false otherwise
	 */
	bool intersect(Ray& ray) const
	{
		// --- PUT YOUR CODE HERE ---
		bool isIntersected = false;
		for (auto it = begin(m_vpPrims); it != end(m_vpPrims); ++it)
		{
			isIntersected = isIntersected || (it->get()->intersect(ray));
		}
		return isIntersected;
	}

	/**
	 * find occluder
	 */
	bool occluded(Ray& ray)
	{
		// --- PUT YOUR CODE HERE ---
		for (auto& primitive : m_vpPrims)
		{
			if (primitive->occluded(ray))
				return true;
			else
				return false;
		}
	}

	/**
	 trace the given ray and shade it and
	 return the color of the shaded ray
	 */
	Vec3f RayTrace(Ray& ray) const
	{
		// --- PUT YOUR CODE HERE ---
		/*if (intersect(ray) == false)
			 return RGB(0, 0, 0);//Black
		else return RGB(1, 1, 1);//White
		*/
		Vec3f colour = m_bgColor;//if background
		for (auto it : m_vpPrims)
		{
			if (it->intersect(ray))
			{
				colour = ray.hit->getShader()->shade(ray);
			}
		}

		return colour;
	}


private:
	Vec3f						m_bgColor;    			///< background color
	std::vector<ptr_prim_t> 	m_vpPrims;				///< primitives
	std::vector<ptr_light_t>	m_vpLights;				///< lights
	std::vector<ptr_camera_t>	m_vpCameras;			//< Cameras
	size_t						m_activeCamera	= 0;	//< The index of the active camera
};
