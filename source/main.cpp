#include <cstddef>
#include <bgfx/bgfx.h>
import copium;
import copium.window;
import copium.render.pbr;
import copium.render.generate;
import copium.time;
import copium.logging;

int main()
{
    Copium::sayHi();

    Window window("copium cube", 100, 100, 1280, 720, {
        WindowControls::RESIZABLE
    });
    window.open();

    Logging::print("info", 0, "create render");
    PBR::PBRRenderer pbrRenderer(&window);
    pbrRenderer.enableBackfaceCulling(false);
    Logging::print("info", 0, "render created");

    // make a camera
    Logging::print("info", 0, "attempt cam");
    PBR::Camera camera;
    camera.initialize();
    
    camera.setPosition(2.0f, 0.0f, 2.0f);
    camera.setTarget(0.0f, 0.0f, 0.0f);
    camera.setAspectRatio(static_cast<float>(window.w) / static_cast<float>(window.h));
    
    Logging::print("info", 0, "made cam");

    // make a light
    PBR::LightManager lightManager;
    PBR::Light dirLight = lightManager.createDirectionalLight(2.0f, -2.0f, 0.0f);
    dirLight.setColor(1.0f, 1.0f, 1.0f);
    dirLight.setIntensity(5.0f);
    lightManager.addLight(dirLight);

    // environment with base lighting
    PBR::Environment environment;
    environment.setAmbientColor(0.2f, 0.2f, 0.2f);
    environment.setAmbientIntensity(1.0f);

    // red material
    PBR::Material material;
    material.setAlbedoColor(1.0f, 0.0f, 0.0f);
    material.setMetallicFactor(0.0f);
    material.setRoughnessFactor(0.5f);

    Logging::print("render", 0, "attempting cam assignment");
    pbrRenderer.setCamera(&camera);
    pbrRenderer.setLightManager(&lightManager);

    // add cube
    PBR::Mesh cubeMesh = PBR::MeshBuilder::createCube(1.0f);
    size_t cubeIdx = pbrRenderer.addObject(&cubeMesh, &material);
    PBR::RenderObject* cube = pbrRenderer.getObject(cubeIdx);
    if (cube)
    {
        cube->transform.setPosition(0.0f, 0.0f, 0.0f);
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
    float rot = 0;
    while (!window.closed)
    {
        WindowControls::pollEvents();
        
        if (window.shouldClose())
        {
            window.close();
        }

        if (fps.shouldFire())
        {
            rot += 0.05;
            cube->transform.setRotation(0, rot, 0);
            bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 
                              0x303030ff,
                              1.0f, 0);
            bgfx::setViewRect(0, 0, 0, window.w, window.h);


            environment.bind();
            pbrRenderer.render(0);
        }

        
        bgfx::frame();
    }

    bgfx::destroy(pbrProgram);
    bgfx::shutdown();

    return 0;
}