module;
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <SDL3/SDL.h>
export module copium.render.vulkan;

import copium.window;

export class VulkanRender
{
public:
    bgfx::Init init;
    Window *window;
    uint32_t renderFlags;

    VulkanRender(Window *window)
    {
        this->window = window;
        this->renderFlags = BGFX_RESET_VSYNC; // vsync ons

        SDL_Window* sdl_win = (SDL_Window*)window->getWindowContext();
        SDL_PropertiesID props = SDL_GetWindowProperties(sdl_win);
        const char* driver = SDL_GetCurrentVideoDriver();

        // what am i using?
        if (SDL_strcmp(driver, "wayland") == 0) {
            this->init.platformData.ndt  = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
            this->init.platformData.nwh  = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
            this->init.platformData.type = bgfx::NativeWindowHandleType::Wayland;
        } 
        else if (SDL_strcmp(driver, "x11") == 0) {
            this->init.platformData.ndt  = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
            this->init.platformData.nwh  = (void*)(uintptr_t)SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        } 
        else if (SDL_strcmp(driver, "windows") == 0) {
            this->init.platformData.nwh  = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
        }

        this->init.type = bgfx::RendererType::Vulkan;
        this->init.resolution.width = window->w;
        this->init.resolution.height = window->h;
        this->init.resolution.reset = this->renderFlags;

        bgfx::init(this->init);
    }

    void setVSync(bool enabled) {
        if (enabled) {
            this->renderFlags |= BGFX_RESET_VSYNC;
        } else {
            this->renderFlags &= ~BGFX_RESET_VSYNC;
        }

        // apply changes
        bgfx::reset(this->window->w, this->window->h, this->renderFlags);
    }

    void renderBlank(unsigned long long color) {
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, color);
        bgfx::setViewRect(0, 0, 0, this->window->w, this->window->h);
        bgfx::touch(0);
        bgfx::frame();
    }
};
