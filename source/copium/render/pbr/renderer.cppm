module;
#include <bgfx/bgfx.h>
#include <SDL3/SDL.h>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
export module copium.render.pbr.renderer;

import copium.render.pbr.material;
import copium.render.pbr.light;
import copium.render.pbr.mesh;
import copium.render.pbr.camera;
import copium.window;

export namespace PBR {

struct Transform
{
    float position[3] = {0.0f, 0.0f, 0.0f};
    float rotation[3] = {0.0f, 0.0f, 0.0f};
    float scale[3] = {1.0f, 1.0f, 1.0f};

    void
    setPosition(float x, float y, float z)
    {
        position[0] = x;
        position[1] = y;
        position[2] = z;
    }

    void
    setRotation(float x, float y, float z)
    {
        rotation[0] = x;
        rotation[1] = y;
        rotation[2] = z;
    }

    void
    setScale(float x, float y, float z)
    {
        scale[0] = std::max(0.0001f, x);
        scale[1] = std::max(0.0001f, y);
        scale[2] = std::max(0.0001f, z);
    }

    void
    setUniformScale(float s)
    {
        float validScale = std::max(0.0001f, s);
        scale[0] = validScale;
        scale[1] = validScale;
        scale[2] = validScale;
    }

    // Generates column-major 4x4 transformation matrix (standard for OpenGL/bgfx)
    void
    getMatrix(float* matrix)
    const
    {
        float sx = std::sin(rotation[0]);
        float cx = std::cos(rotation[0]);
        float sy = std::sin(rotation[1]);
        float cy = std::cos(rotation[1]);
        float sz = std::sin(rotation[2]);
        float cz = std::cos(rotation[2]);

        // Column 0
        matrix[0] = scale[0] * (cy * cz);
        matrix[1] = scale[0] * (cy * sz);
        matrix[2] = scale[0] * (-sy);
        matrix[3] = 0.0f;

        // Column 1
        matrix[4] = scale[1] * (sx * sy * cz - cx * sz);
        matrix[5] = scale[1] * (sx * sy * sz + cx * cz);
        matrix[6] = scale[1] * (sx * cy);
        matrix[7] = 0.0f;

        // Column 2
        matrix[8] = scale[2] * (cx * sy * cz + sx * sz);
        matrix[9] = scale[2] * (cx * sy * sz - sx * cz);
        matrix[10] = scale[2] * (cx * cy);
        matrix[11] = 0.0f;

        // Column 3 (translation)
        matrix[12] = position[0];
        matrix[13] = position[1];
        matrix[14] = position[2];
        matrix[15] = 1.0f;
    }
};

struct RenderObject
{
    Mesh* mesh = nullptr;
    Material* material = nullptr;
    Transform transform;
    bool visible = true;
};

class PBRRenderer
{
private:
    bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;
    Camera* camera = nullptr;
    LightManager* lightManager = nullptr;
    std::vector<RenderObject> objects;
    
    uint64_t state = 0
        | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CCW
        | BGFX_STATE_MSAA;

public:
    bgfx::Init init;
    Window *window;
    uint32_t renderFlags;

    PBRRenderer(Window *window)
        : window(window)
        , renderFlags(BGFX_RESET_VSYNC)
    {
        if (!window)
        {
            throw std::runtime_error("Window pointer cannot be null!");
        }

        SDL_Window* sdl_win = (SDL_Window*)window->getWindowContext();
        if (!sdl_win)
        {
            throw std::runtime_error("Invalid SDL window context!");
        }

        SDL_PropertiesID props = SDL_GetWindowProperties(sdl_win);
        const char* driver = SDL_GetCurrentVideoDriver();

        // Initialize platform data
        this->init.platformData = {};

        if (SDL_strcmp(driver, "wayland") == 0) {
            this->init.platformData.ndt  = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
            this->init.platformData.nwh  = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
            this->init.platformData.type = bgfx::NativeWindowHandleType::Wayland;
        } 
        else if (SDL_strcmp(driver, "x11") == 0) {
            this->init.platformData.ndt  = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
            this->init.platformData.nwh  = (void*)(uintptr_t)SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
            this->init.platformData.type = bgfx::NativeWindowHandleType::Default;
        } 
        else if (SDL_strcmp(driver, "windows") == 0) {
            this->init.platformData.nwh  = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
            this->init.platformData.type = bgfx::NativeWindowHandleType::Default;
        }
        else
        {
            throw std::runtime_error("Unsupported SDL video driver!");
        }

        this->init.type = bgfx::RendererType::Vulkan;
        this->init.resolution.width = window->w;
        this->init.resolution.height = window->h;
        this->init.resolution.reset = this->renderFlags;

        if (!bgfx::init(this->init)) {
            throw std::runtime_error("Failed to initialize bgfx!");
        }
    
        // Set up default view
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, window->w, window->h);
    }

    ~PBRRenderer()
    {
        if (bgfx::isValid(program)) bgfx::destroy(program);
        bgfx::shutdown();
    }

    PBRRenderer(const PBRRenderer&) = delete;
    PBRRenderer& operator=(const PBRRenderer&) = delete;

    PBRRenderer(PBRRenderer&& other) noexcept
        : program(other.program)
        , camera(other.camera)
        , lightManager(other.lightManager)
        , objects(std::move(other.objects))
        , state(other.state)
        , init(other.init)
        , window(other.window)
        , renderFlags(other.renderFlags)
    {
        other.program = BGFX_INVALID_HANDLE;
        other.camera = nullptr;
        other.lightManager = nullptr;
        other.window = nullptr;
    }

    PBRRenderer& operator=(PBRRenderer&& other) noexcept
    {
        if (this != &other)
        {
            // Clean up existing resources
            if (bgfx::isValid(program)) bgfx::destroy(program);

            // Move resources
            program = other.program;
            camera = other.camera;
            lightManager = other.lightManager;
            objects = std::move(other.objects);
            state = other.state;
            init = other.init;
            window = other.window;
            renderFlags = other.renderFlags;

            other.program = BGFX_INVALID_HANDLE;
            other.camera = nullptr;
            other.lightManager = nullptr;
            other.window = nullptr;
        }
        return *this;
    }

    void
    setShaderProgram(bgfx::ProgramHandle prog)
    {
        if (!bgfx::isValid(prog))
        {
            return; // TODO: maybe not best practice
        }

        if (bgfx::isValid(program)) bgfx::destroy(program);
        program = prog;
    }

    void
    setCamera(Camera* cam)
    {
        camera = cam;
    }

    void
    setLightManager(LightManager* lights)
    {
        lightManager = lights;
    }

    size_t
    addObject(Mesh* mesh, Material* material)
    {
        if (!mesh || !material)
        {
            return static_cast<size_t>(-1);
        }

        RenderObject obj;
        obj.mesh = mesh;
        obj.material = material;
        objects.push_back(obj);
        return objects.size() - 1;
    }

    RenderObject*
    getObject(size_t index)
    {
        if (index >= objects.size()) return nullptr;
        return &objects[index];
    }

    const RenderObject*
    getObject(size_t index) const
    {
        if (index >= objects.size()) return nullptr;
        return &objects[index];
    }

    void
    removeObject(size_t index)
    {
        if (index < objects.size())
        {
            objects.erase(objects.begin() + index);
        }
    }

    void
    clear()
    {
        objects.clear();
    }

    void
    setRenderState(uint64_t newState)
    {
        state = newState;
    }

    void
    enableWireframe(bool enable)
    {
        if (enable)
        {
            state |= BGFX_STATE_PT_LINES;
        }
        else
        {
            state &= ~BGFX_STATE_PT_LINES;
        }
    }

    void
    enableBackfaceCulling(bool enable)
    {
        if (enable)
        {
            state |= BGFX_STATE_CULL_CCW;
        }
        else
        {
            state &= ~BGFX_STATE_CULL_CCW;
        }
    }

    void
    enableDepthTest(bool enable)
    {
        if (enable)
        {
            state |= BGFX_STATE_DEPTH_TEST_LESS;
        }
        else
        {
            state &= ~BGFX_STATE_DEPTH_TEST_LESS;
        }
    }

    void
    updateViewport(uint32_t width, uint32_t height, bgfx::ViewId viewId = 0)
    {
        if (width == 0 || height == 0) return;
        
        bgfx::setViewRect(viewId, 0, 0, static_cast<uint16_t>(width), static_cast<uint16_t>(height));
        
        // Update camera aspect ratio if camera is set
        if (camera)
        {
            camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
        }
    }

    void
    render(bgfx::ViewId viewId = 0)
    {
        if (!bgfx::isValid(program)) return;
        if (!camera) return;

        camera->bind();
        
        if (lightManager)
        {
            lightManager->bindLights();
        }

        for (const RenderObject& obj : objects)
        {
            if (!obj.visible || !obj.mesh || !obj.mesh->isValid() || !obj.material) continue;

            float modelMatrix[16];
            obj.transform.getMatrix(modelMatrix);
            
            bgfx::setTransform(modelMatrix);

            obj.material->bind();
            obj.mesh->bind();

            bgfx::setState(state);
            bgfx::submit(viewId, program);
        }
    }

    size_t
    getObjectCount() const
    {
        return objects.size();
    }

    uint64_t
    getRenderState() const
    {
        return state;
    }
};

class Environment
{
private:
    float ambientColor[3] = {0.03f, 0.03f, 0.03f};
    float ambientIntensity = 1.0f;
    bgfx::UniformHandle uniformAmbient;

public:
    Environment()
    {
        uniformAmbient = bgfx::createUniform("u_ambient", bgfx::UniformType::Vec4);
    }

    ~Environment()
    {
        if (bgfx::isValid(uniformAmbient)) bgfx::destroy(uniformAmbient);
    }

    Environment(const Environment&) = delete;
    Environment& operator=(const Environment&) = delete;

    Environment(Environment&& other) noexcept
        : ambientIntensity(other.ambientIntensity)
        , uniformAmbient(other.uniformAmbient)
    {
        ambientColor[0] = other.ambientColor[0];
        ambientColor[1] = other.ambientColor[1];
        ambientColor[2] = other.ambientColor[2];
        
        other.uniformAmbient = BGFX_INVALID_HANDLE;
    }

    Environment& operator=(Environment&& other) noexcept
    {
        if (this != &other)
        {
            // Clean up existing resources
            if (bgfx::isValid(uniformAmbient)) bgfx::destroy(uniformAmbient);

            // Move resources
            ambientColor[0] = other.ambientColor[0];
            ambientColor[1] = other.ambientColor[1];
            ambientColor[2] = other.ambientColor[2];
            ambientIntensity = other.ambientIntensity;
            uniformAmbient = other.uniformAmbient;

            other.uniformAmbient = BGFX_INVALID_HANDLE;
        }
        return *this;
    }

    void
    setAmbientColor(float r, float g, float b)
    {
        ambientColor[0] = std::max(0.0f, r);
        ambientColor[1] = std::max(0.0f, g);
        ambientColor[2] = std::max(0.0f, b);
    }

    void
    setAmbientIntensity(float intensity)
    {
        ambientIntensity = std::max(0.0f, intensity);
    }

    void
    bind()
    {
        float ambient[4] = {
            ambientColor[0] * ambientIntensity,
            ambientColor[1] * ambientIntensity,
            ambientColor[2] * ambientIntensity,
            ambientIntensity
        };
        bgfx::setUniform(uniformAmbient, ambient);
    }

    const float*
    getAmbientColor() const
    {
        return ambientColor;
    }

    float
    getAmbientIntensity() const
    {
        return ambientIntensity;
    }
};

}