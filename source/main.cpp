#include <cstddef>
#include <bgfx/bgfx.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <string>
import copium;
import copium.window;
import copium.render.pbr;
import copium.render.generate;
import copium.time;
import copium.logging;
import copium.input;

int main()
{
    Copium::sayHi();

    Window window("copium cube", {
        // WindowControls::BORDERLESS,
        WindowControls::TRANSPARENT,
        WindowControls::RESIZABLE
    });
    window.setWindowPosition(0,0);
    window.setWindowSize(800, 600);
    window.open();


    // create renderer
    PBR::PBRRenderer pbrRenderer(&window);
    pbrRenderer.enableBackfaceCulling(false);

    // make a camera
    PBR::Camera camera;
    camera.initialize();
    
    camera.setPosition(0.0f, 0.0f, 3.0f);
    camera.setTarget(0.0f, 0.0f, 0.0f);
    camera.setAspectRatio(static_cast<float>(window.w) / static_cast<float>(window.h));

    // make a light
    PBR::LightManager lightManager;

    PBR::Light dirLight = lightManager.createDirectionalLight(2.0f, -2.0f, 0.0f);
    dirLight.setColor(1.0f, 1.0f, 1.0f);
    dirLight.setIntensity(5.0f);
    lightManager.addLight(dirLight);

    PBR::Light dirLight2 = lightManager.createDirectionalLight(-2.0f, -2.0f, 0.0f);
    dirLight2.setColor(0.8f, 1.0f, 1.0f);
    dirLight2.setIntensity(5.0f);
    lightManager.addLight(dirLight2);

    // environment with base lighting
    PBR::Environment environment;
    environment.setAmbientColor(0.2f, 0.2f, 0.2f);
    environment.setAmbientIntensity(1.0f);

    // red material
    PBR::Material material1;
    material1.setAlbedoColor(1.0f, 0.0f, 0.0f);
    material1.setMetallicFactor(1.0f);
    material1.setRoughnessFactor(1.0f);
    // yellow material
    PBR::Material material2;
    material2.setAlbedoColor(1.0f, 1.0f, 0.0f);
    material2.setMetallicFactor(1.0f);
    material2.setRoughnessFactor(1.0f);

    pbrRenderer.setCamera(&camera);
    pbrRenderer.setLightManager(&lightManager);

    // add cube
    PBR::Mesh cubeMesh = PBR::MeshBuilder::createCube(1.0f);
    size_t cubeIdx = pbrRenderer.addObject(&cubeMesh, &material1);
    size_t cubeIdx2 = pbrRenderer.addObject(&cubeMesh, &material2);
    PBR::RenderObject* cube = pbrRenderer.getObject(cubeIdx);
    PBR::RenderObject* cube2 = pbrRenderer.getObject(cubeIdx2);
    if (cube)
    {
        cube->transform.setPosition(1.0f, 0.0f, 0.0f);
    }
    if (cube2)
    {
        cube2->transform.setPosition(-1.0f, 0.0f, 0.0f);
    }

    // Load shader program
    Logging::print("render", 0, "attempting program load");
    bgfx::ProgramHandle pbrProgram = ShaderLoader::loadShaderProgram(
        "assets/shaders/pbr/vs_pbr.bin", 
        "assets/shaders/pbr/fs_pbr.bin"
    );
    
    if (!bgfx::isValid(pbrProgram)) {
        Logging::print("error", 0, "Failed to load PBR shaders!");
        if (!ShaderLoader::shaderExists("assets/shaders/pbr/vs_pbr.bin")) {
            Logging::print("error", 0, "Vertex shader not found");
        }
        if (!ShaderLoader::shaderExists("assets/shaders/pbr/fs_pbr.bin")) {
            Logging::print("error", 0, "Fragment shader not found");
        }
        return -1;
    }
    pbrRenderer.setShaderProgram(pbrProgram);

    Logging::print("info", 0, "Setup complete, entering render loop");

    Time::Delay fps(Time::FPS(60).getDuration(), true);

    float delta = 1/60;
    float roty = 0;
    float rotx = 0;
    InputManager input;
    while (!window.closed)
    {
        WindowControls::pollEvents();
        
        

        if (fps.shouldFire())
        {
            input.update();
            roty += 0.05;
            rotx += 0.05;

            cube->transform.setRotation(roty, rotx, 0);
            cube2->transform.setRotation(-roty, -rotx, 0);

            pbrRenderer.updateViewport(window.w, window.h, 0);

            bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 
                              0x00000000,
                              1.0f, 0);
            bgfx::setViewRect(0, 0, 0, window.w, window.h);


            environment.bind();
            pbrRenderer.render(0);

            Logging::print("info", 0, std::to_string(window.getWindowWidth()));
        }

        bgfx::frame();
        if (window.shouldClose())
        {
            window.close();
        }
        
    }

    bgfx::destroy(pbrProgram);
    bgfx::shutdown();

    return 0;
}