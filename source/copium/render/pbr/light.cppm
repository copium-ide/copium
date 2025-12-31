module;
#include <bgfx/bgfx.h>
#include <vector>
#include <cmath>
#include <algorithm>
export module copium.render.pbr.light;

export namespace PBR {

enum class LightType : uint32_t
{
    Directional = 0,
    Point = 1,
    Spot = 2
};

struct Light
{
    float position[3] = {0.0f, 0.0f, 0.0f};
    float direction[3] = {0.0f, -1.0f, 0.0f};
    float color[3] = {1.0f, 1.0f, 1.0f};
    float intensity = 1.0f;
    float range = 10.0f;
    float innerConeAngle = 0.785398f;  // 45 degrees
    float outerConeAngle = 1.0472f;     // 60 degrees
    LightType type = LightType::Point;

    void
    setPosition(float x, float y, float z)
    {
        position[0] = x;
        position[1] = y;
        position[2] = z;
    }

    void
    setDirection(float x, float y, float z)
    {
        float len = std::sqrt(x*x + y*y + z*z);
        if (len > 0.0f)
        {
            direction[0] = x / len;
            direction[1] = y / len;
            direction[2] = z / len;
        }
        else
        {
            // Fallback to default downward direction if zero vector provided
            direction[0] = 0.0f;
            direction[1] = -1.0f;
            direction[2] = 0.0f;
        }
    }

    void
    setColor(float r, float g, float b)
    {
        // Clamp to valid range
        color[0] = std::max(0.0f, r);
        color[1] = std::max(0.0f, g);
        color[2] = std::max(0.0f, b);
    }

    void
    setIntensity(float i)
    {
        intensity = std::max(0.0f, i);
    }

    void
    setRange(float r)
    {
        range = std::max(0.0f, r);
    }

    void
    setSpotAngles(float inner, float outer)
    {
        // Ensure valid cone angles and proper ordering
        innerConeAngle = std::clamp(inner, 0.0f, 3.14159f);
        outerConeAngle = std::clamp(outer, innerConeAngle, 3.14159f);
    }
};

class LightManager
{
private:
    std::vector<Light> lights;
    bgfx::UniformHandle uniformLightData;
    bgfx::UniformHandle uniformLightCount;
    static constexpr size_t MAX_LIGHTS = 16;
    static constexpr size_t INVALID_INDEX = static_cast<size_t>(-1);

public:
    LightManager()
    {
        uniformLightData = bgfx::createUniform("u_lightData", bgfx::UniformType::Vec4, MAX_LIGHTS * 4);
        uniformLightCount = bgfx::createUniform("u_lightCount", bgfx::UniformType::Vec4);
    }

    ~LightManager()
    {
        if (bgfx::isValid(uniformLightData)) bgfx::destroy(uniformLightData);
        if (bgfx::isValid(uniformLightCount)) bgfx::destroy(uniformLightCount);
    }

    LightManager(const LightManager&) = delete;
    LightManager& operator=(const LightManager&) = delete;

    LightManager(LightManager&& other) noexcept
        : lights(std::move(other.lights))
        , uniformLightData(other.uniformLightData)
        , uniformLightCount(other.uniformLightCount)
    {
        other.uniformLightData = BGFX_INVALID_HANDLE;
        other.uniformLightCount = BGFX_INVALID_HANDLE;
    }

    LightManager& operator=(LightManager&& other) noexcept
    {
        if (this != &other)
        {
            // Clean up existing resources
            if (bgfx::isValid(uniformLightData)) bgfx::destroy(uniformLightData);
            if (bgfx::isValid(uniformLightCount)) bgfx::destroy(uniformLightCount);

            // Move resources
            lights = std::move(other.lights);
            uniformLightData = other.uniformLightData;
            uniformLightCount = other.uniformLightCount;

            other.uniformLightData = BGFX_INVALID_HANDLE;
            other.uniformLightCount = BGFX_INVALID_HANDLE;
        }
        return *this;
    }

    size_t
    addLight(const Light& light)
    {
        if (lights.size() >= MAX_LIGHTS) return INVALID_INDEX;
        lights.push_back(light);
        return lights.size() - 1;
    }

    Light*
    getLight(size_t index)
    {
        if (index >= lights.size()) return nullptr;
        return &lights[index];
    }

    const Light*
    getLight(size_t index) const
    {
        if (index >= lights.size()) return nullptr;
        return &lights[index];
    }

    void
    removeLight(size_t index)
    {
        if (index < lights.size())
        {
            lights.erase(lights.begin() + index);
        }
    }

    void
    clear()
    {
        lights.clear();
    }

    size_t
    getLightCount() const
    {
        return lights.size();
    }

    void
    bindLights()
    {
        if (lights.empty()) return;

        // Ensure we don't exceed array bounds
        const size_t lightCount = std::min(lights.size(), MAX_LIGHTS);
        float lightData[MAX_LIGHTS * 16];

        for (size_t i = 0; i < lightCount; ++i)
        {
            const Light& light = lights[i];
            const size_t offset = i * 16;

            lightData[offset + 0] = light.position[0];
            lightData[offset + 1] = light.position[1];
            lightData[offset + 2] = light.position[2];
            lightData[offset + 3] = static_cast<float>(light.type);

            lightData[offset + 4] = light.direction[0];
            lightData[offset + 5] = light.direction[1];
            lightData[offset + 6] = light.direction[2];
            lightData[offset + 7] = light.range;

            lightData[offset + 8] = light.color[0];
            lightData[offset + 9] = light.color[1];
            lightData[offset + 10] = light.color[2];
            lightData[offset + 11] = light.intensity;

            lightData[offset + 12] = std::cos(light.innerConeAngle);
            lightData[offset + 13] = std::cos(light.outerConeAngle);
            lightData[offset + 14] = 0.0f;
            lightData[offset + 15] = 0.0f;
        }

        bgfx::setUniform(uniformLightData, lightData, static_cast<uint16_t>(lightCount * 4));

        float count[4] = {static_cast<float>(lightCount), 0.0f, 0.0f, 0.0f};
        bgfx::setUniform(uniformLightCount, count);
    }

    Light
    createDirectionalLight(float dirX, float dirY, float dirZ)
    {
        Light light;
        light.type = LightType::Directional;
        light.setDirection(dirX, dirY, dirZ);
        return light;
    }

    Light
    createPointLight(float x, float y, float z, float range)
    {
        Light light;
        light.type = LightType::Point;
        light.setPosition(x, y, z);
        light.setRange(range);
        return light;
    }

    Light
    createSpotLight(float x, float y, float z, float dirX, float dirY, float dirZ, float innerAngle, float outerAngle)
    {
        Light light;
        light.type = LightType::Spot;
        light.setPosition(x, y, z);
        light.setDirection(dirX, dirY, dirZ);
        light.setSpotAngles(innerAngle, outerAngle);
        return light;
    }
};

}