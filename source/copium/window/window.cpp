module;
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <numeric>
export module copium.window;

export namespace window {

using windowFlag = uint64_t;

struct WindowControls {
    // Globals
    inline static SDL_Event event;

    // Window functionality
    static constexpr windowFlag RESIZABLE           = SDL_WINDOW_RESIZABLE;
    static constexpr windowFlag MINIMIZED           = SDL_WINDOW_MINIMIZED;
    static constexpr windowFlag MAXIMIZED           = SDL_WINDOW_MAXIMIZED;
    static constexpr windowFlag NOT_FOCUSABLE       = SDL_WINDOW_NOT_FOCUSABLE;

    // Window appearance
    static constexpr windowFlag FULLSCREEN          = SDL_WINDOW_FULLSCREEN;
    static constexpr windowFlag BORDERLESS          = SDL_WINDOW_BORDERLESS;
    static constexpr windowFlag TRANSPARENT         = SDL_WINDOW_TRANSPARENT;
    static constexpr windowFlag OCCLUDED            = SDL_WINDOW_OCCLUDED; ///< Is the window covered by other windows? (minimize also triggers this)
    static constexpr windowFlag HIDDEN              = SDL_WINDOW_HIDDEN;   ///< Window is not shown at all. Can be toggled using Window.show() or Window.hide()
    static constexpr windowFlag ALWAYS_ON_TOP       = SDL_WINDOW_ALWAYS_ON_TOP; ///< Window appears at the top of the screen regardless of its focus.
    static constexpr windowFlag UTILITY             = SDL_WINDOW_UTILITY;  ///< System tray

    // Comfort
    static constexpr windowFlag HIGH_PIXEL_DENSITY  = SDL_WINDOW_HIGH_PIXEL_DENSITY;

    // Input focus
    static constexpr windowFlag INPUT_FOCUS         = SDL_WINDOW_INPUT_FOCUS;         ///< Is the window focused?
    static constexpr windowFlag MOUSE_FOCUS         = SDL_WINDOW_MOUSE_FOCUS;         ///< Is the user hovering over the window?
    static constexpr windowFlag MOUSE_CAPTURE       = SDL_WINDOW_MOUSE_CAPTURE;       ///< Receives mouse input regardless of whether it is focused on the window.
    static constexpr windowFlag MOUSE_RELATIVE_MODE = SDL_WINDOW_MOUSE_RELATIVE_MODE; ///< Useful for first-person shooters. Keeps the mouse in the middle.
    static constexpr windowFlag MOUSE_GRABBED       = SDL_WINDOW_MOUSE_GRABBED;       ///< Constrains the mouse to the window.
    static constexpr windowFlag KEYBOARD_GRABBED    = SDL_WINDOW_KEYBOARD_GRABBED;    ///< Constrains keyboard input to the window. **This will disable some system shortcuts.**

    // Menus
    static constexpr windowFlag TOOLTIP             = SDL_WINDOW_TOOLTIP;    ///< This is for stuff like tooltips. **Use only within another window.**
    static constexpr windowFlag POPUP_MENU          = SDL_WINDOW_POPUP_MENU; ///< This is for stuff like right click menus. **Use only within another window.**
    static constexpr windowFlag MODAL               = SDL_WINDOW_MODAL;      ///< Disables input to the main window until the modal closes.

    // Rendering backend
    static constexpr windowFlag OPENGL              = SDL_WINDOW_OPENGL; ///< Vulkan is recommended for performance. **Cannot be changed.**
    static constexpr windowFlag VULKAN              = SDL_WINDOW_VULKAN; ///< Recommended rendering backend. **Cannot be changed.**
    static constexpr windowFlag METAL               = SDL_WINDOW_METAL;  ///< Use for apple devices. **Cannot be changed.**

    // Misc
    static constexpr windowFlag EXTERNAL            = SDL_WINDOW_EXTERNAL; ///< Window was not created with SDL or the standard Copium window library (basically sdl)

    static void pollEvents() {
        SDL_PollEvent(&event);
    }
};

// Global instance to allow "windowControls.pollEvents()" and flag access
inline WindowControls windowControls;

class Window {
private:
    SDL_Window* window = nullptr;
    

public:
    bool closed = false;
    Window(const std::string& title, int x, int y, int w, int h, const std::vector<windowFlag>& flags) {
        if (SDL_WasInit(0) == 0) {
            SDL_Init(0);
        }
        if (SDL_WasInit(SDL_INIT_VIDEO) == 0) {
            SDL_InitSubSystem(SDL_INIT_VIDEO);
        }

        // Combine flags using bitwise OR (equivalent to D's reduce!"|")
        windowFlag combinedFlags = std::accumulate(flags.begin(), flags.end(), 0ULL, std::bit_or<windowFlag>());

        this->window = SDL_CreateWindow(title.c_str(), w, h, combinedFlags);
        if (this->window) {
            SDL_SetWindowPosition(this->window, (float)x, (float)y);
        }
    }

    ~Window() {
        close();
    }

    // RAII: Prevent copying to avoid multiple objects managing one pointer
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    SDL_Window* getWindowContext() { return this->window; }

    bool shouldClose() {
        bool closeRequested = false;
        if (windowControls.event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            if (windowControls.event.window.windowID == SDL_GetWindowID(this->window)) {
                closeRequested = true;
            }
        }
        return closeRequested;
    }

    void close() {
        if (!closed && this->window != nullptr) {
            SDL_DestroyWindow(this->window);
            this->window = nullptr;
            this->closed = true;
        }
    }
};

}