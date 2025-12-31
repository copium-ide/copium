$input v_worldPos, v_normal, v_tangent, v_bitangent, v_texcoord0

#include <bgfx_shader.sh>

// Material uniforms
uniform vec4 u_materialProps;     // metallic, roughness, ao, textureFlags
uniform vec4 u_albedoColor;
uniform vec4 u_emissiveColor;     // rgb + strength

// Camera
uniform vec4 u_cameraPos;

// Lighting (16 lights max * 4 vec4s each = 64 vec4s)
uniform vec4 u_lightData[64];
uniform vec4 u_lightCount;

// Environment
uniform vec4 u_ambient;

// Texture samplers
SAMPLER2D(s_texAlbedo, 0);
SAMPLER2D(s_texNormal, 1);
SAMPLER2D(s_texMetallic, 2);
SAMPLER2D(s_texRoughness, 3);
SAMPLER2D(s_texAO, 4);
SAMPLER2D(s_texEmissive, 5);

// Texture flags
#define HAS_ALBEDO_MAP    1u
#define HAS_NORMAL_MAP    2u
#define HAS_METALLIC_MAP  4u
#define HAS_ROUGHNESS_MAP 8u
#define HAS_AO_MAP        16u
#define HAS_EMISSIVE_MAP  32u

// Light types
#define LIGHT_DIRECTIONAL 0.0
#define LIGHT_POINT       1.0
#define LIGHT_SPOT        2.0

#define PI 3.14159265359
#define EPSILON 0.0001
#define MAX_LIGHTS 16

// Unpack material properties
#define metallicFactor  u_materialProps.x
#define roughnessFactor u_materialProps.y
#define aoFactor        u_materialProps.z
#define textureFlags    uint(u_materialProps.w)

// GGX/Trowbridge-Reitz normal distribution function
float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return nom / max(denom, EPSILON);
}

// Schlick-GGX geometry function
float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return nom / max(denom, EPSILON);
}

// Smith's method for geometry obstruction
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

// Fresnel-Schlick approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// Calculate light contribution
vec3 calculateLight(vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, 
                    vec3 F0, vec3 lightDir, vec3 radiance)
{
    vec3 L = normalize(lightDir);
    vec3 H = normalize(V + L);
    
    float NdotL = max(dot(N, L), 0.0);
    
    // Early exit if light doesn't contribute
    if (NdotL < EPSILON)
        return vec3_splat(0.0);
    
    // Cook-Torrance BRDF
    float NDF = distributionGGX(N, H, roughness);
    float G = geometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL;
    vec3 specular = numerator / max(denominator, EPSILON);
    
    // Energy conservation
    vec3 kS = F;
    vec3 kD = vec3_splat(1.0) - kS;
    kD *= 1.0 - metallic;
    
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

void main()
{
    vec2 uv = v_texcoord0;
    
    // Sample material properties
    vec3 albedo = u_albedoColor.rgb;
    if ((textureFlags & HAS_ALBEDO_MAP) != 0u)
    {
        vec4 albedoSample = texture2D(s_texAlbedo, uv);
        albedo *= albedoSample.rgb;
    }
    
    float metallic = metallicFactor;
    if ((textureFlags & HAS_METALLIC_MAP) != 0u)
        metallic *= texture2D(s_texMetallic, uv).r;
    
    float roughness = roughnessFactor;
    if ((textureFlags & HAS_ROUGHNESS_MAP) != 0u)
        roughness *= texture2D(s_texRoughness, uv).r;
    
    // Clamp roughness to avoid artifacts
    roughness = clamp(roughness, 0.04, 1.0);
    metallic = clamp(metallic, 0.0, 1.0);
    
    float ao = aoFactor;
    if ((textureFlags & HAS_AO_MAP) != 0u)
        ao *= texture2D(s_texAO, uv).r;
    
    vec3 emissive = u_emissiveColor.rgb * u_emissiveColor.a;
    if ((textureFlags & HAS_EMISSIVE_MAP) != 0u)
        emissive *= texture2D(s_texEmissive, uv).rgb;
    
    // Get normal
    vec3 N;
    if ((textureFlags & HAS_NORMAL_MAP) != 0u)
    {
        // Sample and decode normal map
        vec3 tangentNormal = texture2D(s_texNormal, uv).xyz * 2.0 - 1.0;
        
        // Construct TBN matrix
        vec3 T = normalize(v_tangent);
        vec3 B = normalize(v_bitangent);
        vec3 Nbase = normalize(v_normal);
        
        // Apply normal map
        N = normalize(T * tangentNormal.x + B * tangentNormal.y + Nbase * tangentNormal.z);
    }
    else
    {
        N = normalize(v_normal);
    }
    
    vec3 V = normalize(u_cameraPos.xyz - v_worldPos);
    
    // Calculate reflectance at normal incidence
    vec3 F0 = vec3_splat(0.04);
    F0 = mix(F0, albedo, metallic);
    
    // Lighting accumulation
    vec3 Lo = vec3_splat(0.0);
    
    int numLights = min(int(u_lightCount.x), MAX_LIGHTS);
    
    // Unrolled light loop for better performance
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        if (i >= numLights)
            break;
        
        // Light data layout: 4 vec4s per light
        int baseIdx = i * 4;
        vec4 posType = u_lightData[baseIdx + 0];
        vec4 dirRange = u_lightData[baseIdx + 1];
        vec4 colorIntensity = u_lightData[baseIdx + 2];
        vec4 spotAngles = u_lightData[baseIdx + 3];
        
        vec3 lightPos = posType.xyz;
        float lightType = posType.w;
        vec3 lightDir = dirRange.xyz;
        float lightRange = dirRange.w;
        vec3 lightColor = colorIntensity.rgb;
        float lightIntensity = colorIntensity.w;
        float innerCos = spotAngles.x;
        float outerCos = spotAngles.y;
        
        vec3 L;
        float attenuation = 1.0;
        
        // Use epsilon comparison for float light type
        if (lightType < 0.5)
        {
            // Directional light (type 0)
            L = -lightDir;
        }
        else if (lightType < 1.5)
        {
            // Point light (type 1)
            vec3 lightToFrag = v_worldPos - lightPos;
            float distance = length(lightToFrag);
            
            // Early exit if out of range
            if (distance > lightRange)
                continue;
            
            L = -lightToFrag / distance; // Normalize efficiently
            
            // Inverse square falloff with smooth range cutoff
            float distRatio = distance / lightRange;
            float distRatio4 = distRatio * distRatio * distRatio * distRatio;
            attenuation = pow(max(1.0 - distRatio4, 0.0), 2.0) / (1.0 + distance * distance);
        }
        else
        {
            // Spot light (type 2)
            vec3 lightToFrag = v_worldPos - lightPos;
            float distance = length(lightToFrag);
            
            // Early exit if out of range
            if (distance > lightRange)
                continue;
            
            vec3 L_norm = -lightToFrag / distance; // Normalize efficiently
            L = L_norm;
            
            // Distance attenuation
            float distRatio = distance / lightRange;
            float distRatio4 = distRatio * distRatio * distRatio * distRatio;
            float distAtten = pow(max(1.0 - distRatio4, 0.0), 2.0) / (1.0 + distance * distance);
            
            // Cone attenuation
            float theta = dot(L_norm, lightDir);
            float epsilon = max(innerCos - outerCos, EPSILON);
            float spotAtten = clamp((theta - outerCos) / epsilon, 0.0, 1.0);
            
            attenuation = distAtten * spotAtten;
        }
        
        // Skip lights with negligible contribution
        if (attenuation < 0.001)
            continue;
        
        vec3 radiance = lightColor * lightIntensity * attenuation;
        Lo += calculateLight(N, V, albedo, metallic, roughness, F0, L, radiance);
    }
    
    // Ambient lighting
    vec3 ambient = u_ambient.rgb * albedo * ao;
    
    // Combine lighting
    vec3 color = ambient + Lo + emissive;
    
    // Clamp to reasonable HDR range before tone mapping
    color = min(color, vec3_splat(64.0));
    
    // Tone mapping (Reinhard)
    color = color / (color + vec3_splat(1.0));
    
    // Gamma correction (sRGB)
    color = pow(color, vec3_splat(1.0 / 2.2));
    
    gl_FragColor = vec4(color, u_albedoColor.a);
}