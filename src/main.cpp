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

enum ShaderType {
    Flat,
    Eyelight,
    Phong,
    Mirror
};

template<typename T, typename T2>
Mat computeMat(CScene &scene, T &shd1, T &shd2, T &shd3, T &shd4, T2 &shd5){
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

Mat RenderFrame(ShaderType shader)
{
	// Define a scene
	CScene scene;
	// Flat surface shaders
	if (shader == Flat) {
        auto shd1 = std::make_shared<CShaderFlat>(RGB(1, 0, 0)); // red surface
        auto shd2 = std::make_shared<CShaderFlat>(RGB(0, 1, 0)); // green surface
        auto shd3 = std::make_shared<CShaderFlat>(RGB(0, 0, 1)); // blue surface
        auto shd4 = std::make_shared<CShaderFlat>(RGB(1, 1, 0)); // yellow surface
        auto shd5 = std::make_shared<CShaderFlat>(RGB(0, 1, 1)); // cyan surface
        auto shd6 = std::make_shared<CShaderFlat>(RGB(1, 1, 1)); // white surface
        return computeMat(scene, shd1, shd2, shd3, shd4, shd5);
    }
	else if (shader == Eyelight)
	{
        auto shd1 = std::make_shared<CShaderEyelight>(RGB(1, 0, 0)); // red surface
        auto shd2 = std::make_shared<CShaderEyelight>(RGB(0, 1, 0)); // green surface
        auto shd3 = std::make_shared<CShaderEyelight>(RGB(0, 0, 1)); // blue surface
        auto shd4 = std::make_shared<CShaderEyelight>(RGB(1, 1, 0)); // yellow surface
        auto shd5 = std::make_shared<CShaderEyelight>(RGB(0, 1, 1)); // cyan surface
        auto shd6 = std::make_shared<CShaderEyelight>(RGB(1, 1, 1)); // white surface
        return computeMat(scene, shd1, shd2, shd3, shd4, shd5);
    }
	else
	{
        auto shd1 = std::make_shared<CShaderPhong>(scene, RGB(1, 0, 0), 0.1f, 0.5f, 0.5f, 40); // red surface
        auto shd2 = std::make_shared<CShaderPhong>(scene, RGB(0, 1, 0), 0.1f, 0.5f, 0.5f, 40); // green surface
        auto shd3 = std::make_shared<CShaderPhong>(scene, RGB(0, 0, 1), 0.1f, 0.5f, 0.5f, 40); // blue surface
        auto shd4 = std::make_shared<CShaderPhong>(scene, RGB(1, 1, 0), 0.1f, 0.5f, 0.5f, 40); // yellow surface
        auto shd5 = std::make_shared<CShaderPhong>(scene, RGB(0, 1, 1), 0.1f, 0.5f, 0.5f, 40); // cyan surface
        auto shdM = std::make_shared<CShaderMirror>(scene);
        auto shd6 = std::make_shared<CShaderPhong>(scene, RGB(1, 1, 1), 0.1f, 0.5f, 0.5f, 40); // white surface
        return shader == Phong ? computeMat(scene, shd1, shd2, shd3, shd4, shd5) : computeMat(scene, shd1, shd2, shd3, shd4, shdM);
	}
}

int main(int argc, char* argv[])
{
    ShaderType type = Mirror;
    std::string path;
    if (type == Flat)
    {
        path = "../render/flat.jpg";
    }
    else if (type == Eyelight)
    {
        path = "../render/eyelight.jpg";
    }
    else if (type == Phong)
    {
        path = "../render/phong.jpg";
    }
    else if (type == Mirror)
    {
        path = "../render/mirror.jpg";
    }
    else {
        printf("Error: Unsupported shading type. Exiting.\n");
        exit(0);
    }
	Mat img = RenderFrame(type);
    imwrite(path, img);
	imshow("Image", img);
	waitKey();
	return 0;
}
