module;
#include <bgfx/bgfx.h>
#include <string>
#include <cstdint>
#include <array>
export module copium.render.pbr.material;

export namespace PBR {

struct MaterialTextures
{
    bgfx::TextureHandle albedo = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle normal = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle metallic = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle roughness = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle ao = BGFX_INVALID_HANDLE;
    bgfx::TextureHandle emissive = BGFX_INVALID_HANDLE;

    void
    release()
    {
        if (bgfx::isValid(albedo))
        {
            bgfx::destroy(albedo);
            albedo = BGFX_INVALID_HANDLE;
        }
        if (bgfx::isValid(normal))
        {
            bgfx::destroy(normal);
            normal = BGFX_INVALID_HANDLE;
        }
        if (bgfx::isValid(metallic))
        {
            bgfx::destroy(metallic);
            metallic = BGFX_INVALID_HANDLE;
        }
        if (bgfx::isValid(roughness))
        {
            bgfx::destroy(roughness);
            roughness = BGFX_INVALID_HANDLE;
        }
        if (bgfx::isValid(ao))
        {
            bgfx::destroy(ao);
            ao = BGFX_INVALID_HANDLE;
        }
        if (bgfx::isValid(emissive))
        {
            bgfx::destroy(emissive);
            emissive = BGFX_INVALID_HANDLE;
        }
    }
};

struct MaterialProperties
{
    float albedoColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float emissiveColor[3] = {0.0f, 0.0f, 0.0f};
    float metallicFactor = 1.0f;
    float roughnessFactor = 1.0f;
    float aoFactor = 1.0f;
    float emissiveStrength = 1.0f;
    float normalScale = 1.0f;
    uint32_t textureFlags = 0;

    static constexpr uint32_t HAS_ALBEDO_MAP = 1 << 0;
    static constexpr uint32_t HAS_NORMAL_MAP = 1 << 1;
    static constexpr uint32_t HAS_METALLIC_MAP = 1 << 2;
    static constexpr uint32_t HAS_ROUGHNESS_MAP = 1 << 3;
    static constexpr uint32_t HAS_AO_MAP = 1 << 4;
    static constexpr uint32_t HAS_EMISSIVE_MAP = 1 << 5;
};

class Material
{
private:
    MaterialTextures textures;
    MaterialProperties properties;
    bgfx::UniformHandle uniformProps = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle uniformAlbedoColor = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle uniformEmissiveColor = BGFX_INVALID_HANDLE;
    
    static bgfx::UniformHandle s_texAlbedo;
    static bgfx::UniformHandle s_texNormal;
    static bgfx::UniformHandle s_texMetallic;
    static bgfx::UniformHandle s_texRoughness;
    static bgfx::UniformHandle s_texAO;
    static bgfx::UniformHandle s_texEmissive;
    static bool s_samplersInitialized;
    
    bool initialized = false;

    static void
    initializeSamplers()
    {
        if (!s_samplersInitialized)
        {
            s_texAlbedo = bgfx::createUniform("s_texAlbedo", bgfx::UniformType::Sampler);
            s_texNormal = bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);
            s_texMetallic = bgfx::createUniform("s_texMetallic", bgfx::UniformType::Sampler);
            s_texRoughness = bgfx::createUniform("s_texRoughness", bgfx::UniformType::Sampler);
            s_texAO = bgfx::createUniform("s_texAO", bgfx::UniformType::Sampler);
            s_texEmissive = bgfx::createUniform("s_texEmissive", bgfx::UniformType::Sampler);
            s_samplersInitialized = true;
        }
    }

public:
    Material()
    {
        initializeSamplers();
        uniformProps = bgfx::createUniform("u_materialProps", bgfx::UniformType::Vec4);
        uniformAlbedoColor = bgfx::createUniform("u_albedoColor", bgfx::UniformType::Vec4);
        uniformEmissiveColor = bgfx::createUniform("u_emissiveColor", bgfx::UniformType::Vec4);
        initialized = true;
    }

    ~Material()
    {
        release();
    }

    Material(const Material&) = delete;
    Material& operator=(const Material&) = delete;

    Material(Material&& other) noexcept
        : textures(other.textures), properties(other.properties),
          uniformProps(other.uniformProps), uniformAlbedoColor(other.uniformAlbedoColor),
          uniformEmissiveColor(other.uniformEmissiveColor), initialized(other.initialized)
    {
        other.textures = MaterialTextures{};
        other.uniformProps = BGFX_INVALID_HANDLE;
        other.uniformAlbedoColor = BGFX_INVALID_HANDLE;
        other.uniformEmissiveColor = BGFX_INVALID_HANDLE;
        other.initialized = false;
    }

    Material&
    operator=(Material&& other) noexcept
    {
        if (this != &other)
        {
            release();
            textures = other.textures;
            properties = other.properties;
            uniformProps = other.uniformProps;
            uniformAlbedoColor = other.uniformAlbedoColor;
            uniformEmissiveColor = other.uniformEmissiveColor;
            initialized = other.initialized;
            other.textures = MaterialTextures{};
            other.uniformProps = BGFX_INVALID_HANDLE;
            other.uniformAlbedoColor = BGFX_INVALID_HANDLE;
            other.uniformEmissiveColor = BGFX_INVALID_HANDLE;
            other.initialized = false;
        }
        return *this;
    }

    void
    setAlbedoTexture(bgfx::TextureHandle texture)
    {
        if (bgfx::isValid(textures.albedo))
        {
            bgfx::destroy(textures.albedo);
        }
        textures.albedo = texture;
        if (bgfx::isValid(texture))
        {
            properties.textureFlags |= MaterialProperties::HAS_ALBEDO_MAP;
        }
        else
        {
            properties.textureFlags &= ~MaterialProperties::HAS_ALBEDO_MAP;
        }
    }

    void
    setNormalTexture(bgfx::TextureHandle texture)
    {
        if (bgfx::isValid(textures.normal))
        {
            bgfx::destroy(textures.normal);
        }
        textures.normal = texture;
        if (bgfx::isValid(texture))
        {
            properties.textureFlags |= MaterialProperties::HAS_NORMAL_MAP;
        }
        else
        {
            properties.textureFlags &= ~MaterialProperties::HAS_NORMAL_MAP;
        }
    }

    void
    setMetallicTexture(bgfx::TextureHandle texture)
    {
        if (bgfx::isValid(textures.metallic))
        {
            bgfx::destroy(textures.metallic);
        }
        textures.metallic = texture;
        if (bgfx::isValid(texture))
        {
            properties.textureFlags |= MaterialProperties::HAS_METALLIC_MAP;
        }
        else
        {
            properties.textureFlags &= ~MaterialProperties::HAS_METALLIC_MAP;
        }
    }

    void
    setRoughnessTexture(bgfx::TextureHandle texture)
    {
        if (bgfx::isValid(textures.roughness))
        {
            bgfx::destroy(textures.roughness);
        }
        textures.roughness = texture;
        if (bgfx::isValid(texture))
        {
            properties.textureFlags |= MaterialProperties::HAS_ROUGHNESS_MAP;
        }
        else
        {
            properties.textureFlags &= ~MaterialProperties::HAS_ROUGHNESS_MAP;
        }
    }

    void
    setAOTexture(bgfx::TextureHandle texture)
    {
        if (bgfx::isValid(textures.ao))
        {
            bgfx::destroy(textures.ao);
        }
        textures.ao = texture;
        if (bgfx::isValid(texture))
        {
            properties.textureFlags |= MaterialProperties::HAS_AO_MAP;
        }
        else
        {
            properties.textureFlags &= ~MaterialProperties::HAS_AO_MAP;
        }
    }

    void
    setEmissiveTexture(bgfx::TextureHandle texture)
    {
        if (bgfx::isValid(textures.emissive))
        {
            bgfx::destroy(textures.emissive);
        }
        textures.emissive = texture;
        if (bgfx::isValid(texture))
        {
            properties.textureFlags |= MaterialProperties::HAS_EMISSIVE_MAP;
        }
        else
        {
            properties.textureFlags &= ~MaterialProperties::HAS_EMISSIVE_MAP;
        }
    }

    void
    setAlbedoColor(float r, float g, float b, float a = 1.0f)
    {
        properties.albedoColor[0] = r;
        properties.albedoColor[1] = g;
        properties.albedoColor[2] = b;
        properties.albedoColor[3] = a;
    }

    void
    setEmissiveColor(float r, float g, float b)
    {
        properties.emissiveColor[0] = r;
        properties.emissiveColor[1] = g;
        properties.emissiveColor[2] = b;
    }

    void
    setMetallicFactor(float metallic)
    {
        properties.metallicFactor = metallic;
    }

    void
    setRoughnessFactor(float roughness)
    {
        properties.roughnessFactor = roughness;
    }

    void
    setAOFactor(float ao)
    {
        properties.aoFactor = ao;
    }

    void
    setEmissiveStrength(float strength)
    {
        properties.emissiveStrength = strength;
    }

    void
    setNormalScale(float scale)
    {
        properties.normalScale = scale;
    }

    void
    bind() const
    {
        if (!initialized) return;

        float props[4] = {
            properties.metallicFactor,
            properties.roughnessFactor,
            properties.aoFactor,
            static_cast<float>(properties.textureFlags)
        };
        bgfx::setUniform(uniformProps, props);
        bgfx::setUniform(uniformAlbedoColor, properties.albedoColor);
        
        float emissive[4] = {
            properties.emissiveColor[0],
            properties.emissiveColor[1],
            properties.emissiveColor[2],
            properties.emissiveStrength
        };
        bgfx::setUniform(uniformEmissiveColor, emissive);

        if (bgfx::isValid(textures.albedo))
            bgfx::setTexture(0, s_texAlbedo, textures.albedo);
        if (bgfx::isValid(textures.normal))
            bgfx::setTexture(1, s_texNormal, textures.normal);
        if (bgfx::isValid(textures.metallic))
            bgfx::setTexture(2, s_texMetallic, textures.metallic);
        if (bgfx::isValid(textures.roughness))
            bgfx::setTexture(3, s_texRoughness, textures.roughness);
        if (bgfx::isValid(textures.ao))
            bgfx::setTexture(4, s_texAO, textures.ao);
        if (bgfx::isValid(textures.emissive))
            bgfx::setTexture(5, s_texEmissive, textures.emissive);
    }

    void
    release()
    {
        if (initialized)
        {
            textures.release();
            if (bgfx::isValid(uniformProps))
            {
                bgfx::destroy(uniformProps);
                uniformProps = BGFX_INVALID_HANDLE;
            }
            if (bgfx::isValid(uniformAlbedoColor))
            {
                bgfx::destroy(uniformAlbedoColor);
                uniformAlbedoColor = BGFX_INVALID_HANDLE;
            }
            if (bgfx::isValid(uniformEmissiveColor))
            {
                bgfx::destroy(uniformEmissiveColor);
                uniformEmissiveColor = BGFX_INVALID_HANDLE;
            }
            initialized = false;
        }
    }

    const MaterialProperties&
    getProperties() const
    {
        return properties;
    }
    
    const MaterialTextures&
    getTextures() const
    {
        return textures;
    }
    
    bool
    isInitialized() const
    {
        return initialized;
    }

    // cleanup. call at shutdown
    static void
    destroySamplers()
    {
        if (s_samplersInitialized)
        {
            if (bgfx::isValid(s_texAlbedo)) bgfx::destroy(s_texAlbedo);
            if (bgfx::isValid(s_texNormal)) bgfx::destroy(s_texNormal);
            if (bgfx::isValid(s_texMetallic)) bgfx::destroy(s_texMetallic);
            if (bgfx::isValid(s_texRoughness)) bgfx::destroy(s_texRoughness);
            if (bgfx::isValid(s_texAO)) bgfx::destroy(s_texAO);
            if (bgfx::isValid(s_texEmissive)) bgfx::destroy(s_texEmissive);
            s_samplersInitialized = false;
        }
    }
};

// Static member definitions
inline bgfx::UniformHandle Material::s_texAlbedo = BGFX_INVALID_HANDLE;
inline bgfx::UniformHandle Material::s_texNormal = BGFX_INVALID_HANDLE;
inline bgfx::UniformHandle Material::s_texMetallic = BGFX_INVALID_HANDLE;
inline bgfx::UniformHandle Material::s_texRoughness = BGFX_INVALID_HANDLE;
inline bgfx::UniformHandle Material::s_texAO = BGFX_INVALID_HANDLE;
inline bgfx::UniformHandle Material::s_texEmissive = BGFX_INVALID_HANDLE;
inline bool Material::s_samplersInitialized = false;

}