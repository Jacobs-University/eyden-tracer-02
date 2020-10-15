# Practical Assignment 2
**Dealine**: 15.10.2020

Please put your name here:  
**Name**: Shahin Mammadov
## Problem 1
### Encapsulate camera and primitives from main application logic (Points 5)
1. Fork the current repository
2. Study the new framework-code of 
    - IShader.h, ShaderFlat.h, ShaderEyeLight.h, ShaderPhong.h
    - ILight.h, LightOmni.h 
    - Scene.h and main.cpp
3. A pointer ```std::shared_ptr<const IPrim> hit``` is now contained in your ```Ray``` structure. After a ray has been successfully intersected with a primitive, store the primitive’s address in ```hit``` (if the hit ditance is smaller than ```ray.t```).
4. In the class ```CScene``` you find a method ```void add(const ptr_camera_t pCamera)```. Change your code accordingly using the appropriate vector defined in the class. See also method ```CScene::getActiveCamera()``` to understand how the member-variable ```m_activeCamera```should be also initialized.
5. In the class ```CScene``` you find a method ```void add(const ptr_prim_t pPrim)```. Change your code accordingly using the appropriate vector defined in the class.
6. Rather then intersecting each primitive in the main function we will now use the ```bool intersect(Ray& ray) const``` method of the ```Scene``` class. After modification the method should iterate over all primitives, intersect them and return true or false depending on if we had a valid hit with the scene data or not.
7. The loop of main.cpp calls the ```CScene::RayTrace(Ray& ray)``` method. This method should call ```bool intersect(Ray& ray)``` and depending on a hit or not return a white or black color.

 If you did everything correct the rendered image should look like:

<kbd>   
<img src="./doc/bool.jpg" alt="Boolean Shading" width="400px">
</kbd>

## Problem 2
### The Surface-Shader Concept (Points 10 + 10)
A _surface-shader_ is a small program that is assigned to a primitive and is responsible for computing the color of each ray hitting this primitive. For example, a _flat shader_ might just return a constant color for a primitive, whereas another shader might compute more complex effects such as lighting, shadows, or texturing.

In this exercise, you will add some missing parts of the given basic shader framework to your ray tracer and implement two simple shaders:
1. Implement a simple flat shader. Proceed as follows:
    - The shader class has a pure virtual function ```Vec3f IShader::shade(const Ray& ray)```, which has to be implemented in all derived shaders.
    - Implement the ```CShaderFlat::shade(const Ray& ray)``` method. The method should just return the color passed in the constructor of ```CShaderFlat```.
    - Each primitive has a pointer ```ptr_shader_t m_pShader```, which you can obtain via ```IShader::getShader()``` function in the new framework and a corresponding modified Constructor definition. Adjust the Constructor code appropriate. For example, our red sphere could be initialized using ```CSphere s1(std::make_shared<CShaderFlat>(RGB(1, 0, 0)), Vec3f(-2.0f, 1.7f, 0), 2);```. As we will see later some shaders need access to the scene data (_e.g_ for light or shadow calculations), this is why these shaders gets a reference to the scene objects (_e.g._ ```CShaderPhong```).
    - Finally, if, for instance, the primitive intersected by a ray has been stored in ```std::shared_ptr<const IPrim>	hit``` (you can use ```ray.hit = shared_from_this();``` in intersection algorithms), the appropriate color can then be computed by calling ```hit->getShader()->shade(ray)```; Change your code in ```CScene::RayTrace(Ray& ray)``` such that not black or white is returned but the color from the primitive with the closest hit or the background color if a ray does not hit a primitive.
2. Implement the ```CShaderEyelight::shade(const Ray& ray)``` method in the eye light shader, which uses the angle between the incoming ray and the surface normal at the hit point to achieve a better impression of the actual primitive’s shape. The resulting color should be calculated according to: 
_result_ = |cos(_theta_)|·_color_
where _theta_ is the angle between the primitive surface normal and the ray direction. As the shader now needs to know some information about the primitive (_i.e._ the surface normal), some modifications are necessary:
    - Implement the ```Vec3f CPrim::getNormal(const Ray& ray)``` method in all classes derived from ```IPrim```. ```getNormal(const Ray& ray)``` should return the normalized normal of the primitive. The ray parameter passed to ```getNormal(const Ray& ray)``` should be a ray that has been successfully intersected before, so you can assume that the intersection stored in this ray corresponds to the actual primitive. For example, ray.org + ray.t * ray.dir should be a point on the primitive.
    - Implement the shading function ```CShaderEyelight::shade(const Ray& ray)``` using ```ray.hit->getNormal(ray)``` to retrieve the surface normal of the primitive. With the surface normal the above given formula can be applied. If the test scene specified in main.cpp is rendered with these two shaders it should look like:
    
<img src="./doc/flat.jpg" alt="Flat Shading" width="400px"/> <img src="./doc/eye_light.jpg" alt="Eylight Shading" width="400px"/>

## Problem 3
### Phong Shading and Point Light sources (Points 30)
In the last exercise we implemented two simple surface shaders, which do not take light sources into account. A more advanced surface shading concept, the _phong shading model_, utilizes light sources to increase the rendering realism and give objects a plastic like appearance. Before we can implement the ```CShaderPhong::shade(const Ray& ray)``` method in ShaderPhong.h we have to implement a simple light source.
1. Implement a point light. Proceed as follows:
    - Study the base interface class ```ILight```. Each light source which we will derive from it has to implement an ```ILight::illuminate(Ray& ray)``` method.
    - Implement the ```CScene::add(const ptr_light_t pLight)``` method.
    - Implement the ```CLightOmni::illuminate(Ray& ray)``` method. The method should calculate the light intensity, as described in the lecture, which hits the surface point from the light source as well as the direction vector from the surface point to the light source. The direction vector will be later used for shadow computations.
2. Implement the _phong illumination model_
    - The value _L<sub>r</sub>_ returned by ```Vec3f CShaderPhong::Shade(const Ray& ray) const``` should be calculated according to:
    
    _L<sub>r</sub>_ = _k<sub>a</sub>c<sub>a</sub>L<sub>a</sub>_ + _k<sub>d</sub>c<sub>d</sub>_ Σ<sub>_l=0_</sub><sup>_n-1_</sup> _L<sub>l</sub>_(**I**<sub>_l_</sub>·**N**)+ _k<sub>s</sub>c<sub>s</sub>_ Σ<sub>_l=0_</sub><sup>_n-1_</sup> _L<sub>l</sub>_(**I**_<sub>l</sub>_·**R**)<sup>_k<sub>e</sub>_</sup>
    
    _c<sub>a</sub>_: Ambient color   
    _c<sub>d</sub>_: Diffuse color  
    _c<sub>s</sub>_: Specular color (Use _c<sub>s</sub>_ = (1, 1, 1) for white highlights)  
    
    _k<sub>a</sub>_: Ambient coefficient  
    _k<sub>d</sub>_: Diffuse coefficient  
    _k<sub>s</sub>_: Specular coefficient  
    _k<sub>e</sub>_: Exponent (_shine_ parameter)  
    
    _L<sub>a</sub>_: Ambient radiance  
    _L<sub>l</sub>_: Radiance arriving from light source _l_  
    
    **I**<sub>_l_</sub>: Direction to light source _l_  
    **N**: Shading normal  
    **R**: Reflected incident ray direction (must point away from the surface)
    
    _n_: Number of lights sources
    
### Notes:
- Sometimes an incident ray may hit the backside of a surface (_i.e._ the shading normal points to the other side.) Then, just turn the shading normal around to face forward.
- Only consider light sources that illuminate the primitive from its front-side (_i.e._ **I**<sub>_l_</sub>·**N** > 0).
    
## Problem 4
### Shadows (Points 20)
To add more realism to the phong model we want now to incorporate shadows into it. Proceed as follows:
- Implement the method ```CScene::occluded(Ray& ray)``` in the ```CScene``` class, which should check if something blocks the light.
- Modify ```CShaderPhong::shade(const Ray& ray)``` to check for occlusion.
If everything is implemented correct your images should look like this:

<img src="./doc/phong_no_shadows.jpg" alt="Phong Shading without shadows" width="400px"/> <img src="./doc/phong.jpg" alt="Phong Shading with shadows" width="400px"/>

## Problem 5
### Reflection | Recursive Raytracing (Points 25)
Now we will implement a _perfect mirror_ material with the ```CShaderMirror``` shader. Perfect mirror has no own color and reflects all the rays, thus in constructor it takes only a reference to the scene. Please implement the ```CShaderMirror::shade(const Ray& ray)``` method. To do so you need to calculate two verctors:
- **N**: Shading normal  
- **R**: Reflected incident ray direction (must point away from the surface)

and use scene object to trace recursively the reflected ray with ```CScene::rayTrace(Ray& ray)```method. 

Once you implement everything correct the rendered image should look like:

<kbd>   
<img src="./doc/mirror.jpg" alt="Boolean Shading" width="400px">
</kbd>

## Submission
Please submit the assignment by making a pull request.
**Important** : Please make sure that
- No _extra files_ are submitted (except those, which were mentioned in the assignment)
- The changes were made _only_ in those files where you were asked to write your code
- The Continiouse Integration system (appVeyor) can build the submitted code
- The rendered images are also submitted in the folder "renders" 
