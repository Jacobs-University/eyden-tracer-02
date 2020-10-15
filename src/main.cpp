#include "Scene.h"

#include "CameraPerspective.h"

#include "PrimSphere.h"
#include "PrimPlane.h"
#include "PrimTriangle.h"

#include "ShaderFlat.h"
#include "ShaderEyelight.h"
#include "ShaderPhong.h"
#include "ShaderMirror.h"

#include "LightOmni.h"

Mat RenderFrame(void)
{
	// Define a scene
	CScene scene;
	
	// Flat surface shaders
//	auto shd1 = std::make_shared<CShaderFlat>(RGB(1, 0, 0)); // red surface
//	auto shd2 = std::make_shared<CShaderFlat>(RGB(0, 1, 0)); // green surface
//	auto shd3 = std::make_shared<CShaderFlat>(RGB(0, 0, 1)); // blue surface
//	auto shd4 = std::make_shared<CShaderFlat>(RGB(1, 1, 0)); // yellow surface
//	auto shd5 = std::make_shared<CShaderFlat>(RGB(0, 1, 1)); // cyan surface
//	auto shd6 = std::make_shared<CShaderFlat>(RGB(1, 1, 1)); // white surface

	// EyeLight surface shaders
/*	auto shd1 = std::make_shared<CShaderEyelight>(RGB(1, 0, 0)); // red surface
	auto shd2 = std::make_shared<CShaderEyelight>(RGB(0, 1, 0)); // green surface
	auto shd3 = std::make_shared<CShaderEyelight>(RGB(0, 0, 1)); // blue surface
	auto shd4 = std::make_shared<CShaderEyelight>(RGB(1, 1, 0)); // yellow surface
	auto shd5 = std::make_shared<CShaderEyelight>(RGB(0, 1, 1)); // cyan surface
	auto shd6 = std::make_shared<CShaderEyelight>(RGB(1, 1, 1)); // white surface
	*/
	// Phong surface shaders
	auto shd1 = std::make_shared<CShaderPhong>(scene, RGB(1, 0, 0), 0.1f, 0.5f, 0.5f, 40); // red surface
	auto shd2 = std::make_shared<CShaderPhong>(scene, RGB(0, 1, 0), 0.1f, 0.5f, 0.5f, 40); // green surface
	auto shd3 = std::make_shared<CShaderPhong>(scene, RGB(0, 0, 1), 0.1f, 0.5f, 0.5f, 40); // blue surface
	auto shd4 = std::make_shared<CShaderPhong>(scene, RGB(1, 1, 0), 0.1f, 0.5f, 0.5f, 40); // yellow surface
	auto shd5 = std::make_shared<CShaderPhong>(scene, RGB(0, 1, 1), 0.1f, 0.5f, 0.5f, 40); // cyan surface
	auto shd6 = std::make_shared<CShaderPhong>(scene, RGB(1, 1, 1), 0.1f, 0.5f, 0.5f, 40); // white surface

	// Mirror shader
	auto shdM = std::make_shared<CShaderMirror>(scene);
	
	// Add camera to scene
	scene.add(std::make_shared<CCameraPerspective>(Size(800, 600), Vec3f(0,0,8), Vec3f(0,0,-1), Vec3f(0,1,0), 60));
	
	// Add primitives to scene
	scene.add(std::make_shared<CPrimSphere>(shd1, Vec3f(-2, 1.7f, 0), 2));
	scene.add(std::make_shared<CPrimSphere>(shd2, Vec3f(1, -1, 1), 2.2f));
	scene.add(std::make_shared<CPrimSphere>(shd3, Vec3f(3, 0.8f, -2), 2));
	scene.add(std::make_shared<CPrimPlane>(shd4, Vec3f(0, -1, 0), Vec3f(0, 1, 0)));
	scene.add(std::make_shared<CPrimTriangle>(shd5, Vec3f(-2, 3, 1),Vec3f(1, 2, 1),Vec3f(3, 2.8f, 3)));

	Vec3f pointLightIntensity(25, 25, 25);
	Vec3f lightPosition1(0, 17.5f, 0);
	Vec3f lightPosition2(-3, 5, +4);

	// Add light sources to scene
	scene.add(std::make_shared<CLightOmni>(pointLightIntensity, lightPosition1));
	scene.add(std::make_shared<CLightOmni>(pointLightIntensity, lightPosition2));


	auto pCamera = scene.getActiveCamera();
	if (!pCamera) {
		printf("Error: Can't find active camera. Exiting.\n");
		exit(0);
	}
	Mat img(scene.getActiveCamera()->getResolution(), CV_32FC3);		// image array
	Ray ray;                                          			// primary ray

	for (int y = 0; y < img.rows; y++)
		for (int x = 0; x < img.cols; x++) {
			scene.getActiveCamera()->InitRay(ray, x, y); // initialize ray
			img.at<Vec3f>(y, x) = scene.RayTrace(ray); 
		}
	
	img.convertTo(img, CV_8UC3, 255);
	return img;
}

int main(int argc, char* argv[])
{
	Mat img = RenderFrame();
	imshow("Image", img);
	waitKey();
	imwrite("flat.jpg", img);
	return 0;
}
