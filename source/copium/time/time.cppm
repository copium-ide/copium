module;
#include <chrono>
export module copium.time;

export namespace Time {

class Delay
{   
    private:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::duration<float> interval;
    bool isLooping;

    public:
    /**
     * @brief Construct a new Delay object
     * 
     * @param time interval in seconds
     * @param loop true if it should loop, false if it's a one-time thing
     */
    Delay(float time, bool loop) 
        : interval(time), 
          isLooping(loop), 
          startTime(std::chrono::steady_clock::now()) 
    {
    }

    /**
     * @brief Destructor for Delay class
     * @note Ensures clean resource management
     */
    ~Delay() noexcept 
    {}

    /**
    * @brief determines if the timer is over
    * @return true if it should, false if not
    */
    bool shouldFire()
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration<float>(currentTime - startTime);

        if (elapsed >= interval) {
            if (isLooping) {
                startTime = currentTime;
            }
            return true;
        }
        return false;
    }

    /**
     * @brief Changes the looping behavior of the Delay object
     * @param loop New looping status (true for looping, false for one-time)
     */
    void setLooping(bool loop)
    {
        isLooping = loop;
    }

    /**
     * @brief Changes the duration of the Delay object
     * @param newDuration New interval in seconds
     */
    void setDuration(float newDuration)
    {
        interval = std::chrono::duration<float>(newDuration);
    }

    /**
     * @brief Resets the clock to the current time
     */
    void reset()
    {
        startTime = std::chrono::steady_clock::now();
    }
};

class FPS
{
    public:
    float frames;
    float delta;
    FPS(float value)
    {
        this->frames = value;
        this-> delta = 1/value;
    }
    ~FPS()
    {}
    void setFPS(float value)
    {
        this->frames = value;
        this-> delta = 1/value;
    }
    float getDuration()
    {
        return this->delta;
    }
    float getFPS()
    {
        return this->frames;
    }
};
}