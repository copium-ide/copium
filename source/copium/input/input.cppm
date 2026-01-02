module;
#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>
export module copium.input;

export class InputManager
{
private:
    std::unordered_map<SDL_Keycode, bool> keyStates;
    std::unordered_map<SDL_Keycode, bool> keyPressedThisFrame;
    std::unordered_map<uint8_t, bool> mouseButtonStates;
    std::unordered_map<uint8_t, bool> mouseButtonPressedThisFrame;
    
    float mouseX = 0.0f;
    float mouseY = 0.0f;
    float mouseDeltaX = 0.0f;
    float mouseDeltaY = 0.0f;
    float mouseWheelDelta = 0.0f;
    
    bool quit = false;

public:
    void
    processEvent(const SDL_Event& event)
    {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                quit = true;
                break;
                
            case SDL_EVENT_KEY_DOWN:
                if (!keyStates[event.key.key]) {
                    keyPressedThisFrame[event.key.key] = true;
                }
                keyStates[event.key.key] = true;
                break;
                
            case SDL_EVENT_KEY_UP:
                keyStates[event.key.key] = false;
                break;
                
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (!mouseButtonStates[event.button.button]) {
                    mouseButtonPressedThisFrame[event.button.button] = true;
                }
                mouseButtonStates[event.button.button] = true;
                break;
                
            case SDL_EVENT_MOUSE_BUTTON_UP:
                mouseButtonStates[event.button.button] = false;
                break;
                
            case SDL_EVENT_MOUSE_MOTION:
                mouseDeltaX = event.motion.xrel;
                mouseDeltaY = event.motion.yrel;
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                break;
                
            case SDL_EVENT_MOUSE_WHEEL:
                mouseWheelDelta = event.wheel.y;
                break;
        }
    }

    void
    update()
    {
        // Clear per-frame states
        keyPressedThisFrame.clear();
        mouseButtonPressedThisFrame.clear();
        mouseDeltaX = 0.0f;
        mouseDeltaY = 0.0f;
        mouseWheelDelta = 0.0f;
    }

    bool
    isKeyDown(SDL_Keycode key) const
    {
        auto it = keyStates.find(key);
        return it != keyStates.end() && it->second;
    }

    bool
    isKeyPressed(SDL_Keycode key) const
    {
        auto it = keyPressedThisFrame.find(key);
        return it != keyPressedThisFrame.end() && it->second;
    }

    bool
    isMouseButtonDown(uint8_t button) const
    {
        auto it = mouseButtonStates.find(button);
        return it != mouseButtonStates.end() && it->second;
    }

    bool
    isMouseButtonPressed(uint8_t button) const
    {
        auto it = mouseButtonPressedThisFrame.find(button);
        return it != mouseButtonPressedThisFrame.end() && it->second;
    }

    float
    getMouseX() const
    {
        return mouseX;
    }

    float
    getMouseY() const
    {
        return mouseY;
    }

    float
    getMouseDeltaX() const
    {
        return mouseDeltaX;
    }

    float
    getMouseDeltaY() const
    {
        return mouseDeltaY;
    }

    float
    getMouseWheelDelta() const
    {
        return mouseWheelDelta;
    }

    bool
    shouldQuit() const
    {
        return quit;
    }

    void
    reset()
    {
        keyStates.clear();
        keyPressedThisFrame.clear();
        mouseButtonStates.clear();
        mouseButtonPressedThisFrame.clear();
        mouseDeltaX = 0.0f;
        mouseDeltaY = 0.0f;
        mouseWheelDelta = 0.0f;
        mouseX = 0.0f;
        mouseY = 0.0f;
        quit = false;
    }
};