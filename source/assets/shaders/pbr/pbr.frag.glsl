#version 450

// Fragment inputs
layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragTangent;
layout(location = 4) in vec3 fragBitangent;
layout(location = 5) in vec3 fragViewDir;

// Fragment output
layout(location = 0) out vec4 outColor;

// Texture samplers - glTF standard textures
layout(binding = 2) uniform sampler2D baseColorMap;
layout(binding = 3) uniform sampler2D normalMap;
layout(binding = 4) uniform sampler2D metallicRoughnessMap;  // B=metallic, G=roughness
layout(binding = 5) uniform sampler2D occlusionMap;          // R channel
layout(binding = 6) uniform sampler2D emissiveMap;

// Extension textures
layout(binding = 7) uniform sampler2D clearcoatMap;          // R channel
layout(binding = 8) uniform sampler2D clearcoatRoughnessMap; // G channel
layout(binding = 9) uniform sampler2D clearcoatNormalMap;
layout(binding = 10) uniform sampler2D transmissionMap;      // R channel
layout(binding = 11) uniform sampler2D thicknessMap;         // G channel for volume
layout(binding = 12) uniform sampler2D sheenColorMap;
layout(binding = 13) uniform sampler2D sheenRoughnessMap;

// Material properties buffer
layout(std140, binding = 14) readonly buffer MaterialBuffer {
    // Base PBR
    vec4 baseColorFactor;
    vec3 emissiveFactor;
    float metallicFactor;
    float roughnessFactor;
    float normalScale;
    float occlusionStrength;
    float alphaCutoff;
    
    // Material flags (using float for std140 alignment)
    float hasBaseColorMap;
    float hasNormalMap;
    float hasMetallicRoughnessMap;
    float hasOcclusionMap;
    float hasEmissiveMap;
    float alphaMode;  // 0=opaque, 1=mask, 2=blend
    float doubleSided;
    
    // Colorspace flags (0=sRGB, 1=linear)
    float baseColorSpace;
    float emissiveColorSpace;
    
    // Clearcoat extension (KHR_materials_clearcoat)
    float clearcoatFactor;
    float clearcoatRoughnessFactor;
    float hasClearcoatMap;
    float hasClearcoatRoughnessMap;
    float hasClearcoatNormalMap;
    
    // Transmission extension (KHR_materials_transmission)
    float transmissionFactor;
    float hasTransmissionMap;
    
    // Volume extension (KHR_materials_volume)
    vec3 attenuationColor;
    float attenuationDistance;
    float thicknessFactor;
    float hasThicknessMap;
    
    // IOR (KHR_materials_ior)
    float ior;
    
    // Sheen extension (KHR_materials_sheen)
    vec3 sheenColorFactor;
    float sheenRoughnessFactor;
    float hasSheenColorMap;
    float hasSheenRoughnessMap;
    
    // Specular extension (KHR_materials_specular)
    float specularFactor;
    vec3 specularColorFactor;
    
    float _padding;
} material;

// Lighting data
layout(std140, binding = 15) readonly buffer LightingData {
    vec4 lightPositions[16];  // w=0 for directional, w=1 for point/spot
    vec4 lightColors[16];     // rgb=color, a=intensity
    vec4 lightDirections[16]; // for spotlights, w=inner cone angle
    vec4 lightParams[16];     // x=outer cone, y=range, z=type (0=point,1=spot,2=directional)
    int numLights;
    vec3 ambientLight;
} lighting;

const float PI = 3.14159265359;
const float MIN_ROUGHNESS = 0.04;


// UTILITIES
vec3
sRGBToLinear(vec3 srgb)
{
    return mix(
        srgb / 12.92,
        pow((srgb + 0.055) / 1.055, vec3(2.4)),
        step(0.04045, srgb)
    );
}

vec3
linearToSRGB(vec3 linear)
{
    return mix(
        linear * 12.92,
        pow(linear, vec3(1.0/2.4)) * 1.055 - 0.055,
        step(0.0031308, linear)
    );
}

vec3
fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return num / max(denom, 0.0001);
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    
    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return num / max(denom, 0.0001);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

vec3 getNormalFromMap(sampler2D normalTex, float scale, bool hasMap) {
    if (!hasMap) {
        return normalize(fragNormal);
    }
    
    vec3 tangentNormal = texture(normalTex, fragTexCoord).xyz * 2.0 - 1.0;
    tangentNormal.xy *= scale;
    
    mat3 TBN = mat3(fragTangent, fragBitangent, fragNormal);
    return normalize(TBN * tangentNormal);
}

// ============================================================================
// Tone mapping
// ============================================================================

vec3 acesToneMap(vec3 color) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((color * (a * color + b)) / (color * (c * color + d) + e), 0.0, 1.0);
}

// ============================================================================
// PBR lighting calculation
// ============================================================================

vec3 calculatePBR(vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, vec3 F0) {
    vec3 Lo = vec3(0.0);
    
    for (int i = 0; i < lighting.numLights && i < 16; ++i) {
        vec3 L;
        float attenuation = 1.0;
        int lightType = int(lighting.lightParams[i].z);
        
        // Calculate light direction and attenuation
        if (lightType == 2) {
            // Directional light
            L = normalize(-lighting.lightDirections[i].xyz);
        } else {
            // Point or spot light
            vec3 lightVec = lighting.lightPositions[i].xyz - fragPosition;
            float distance = length(lightVec);
            L = lightVec / distance;
            
            float range = lighting.lightParams[i].y;
            attenuation = 1.0 / (distance * distance);
            if (range > 0.0) {
                attenuation *= max(0.0, 1.0 - (distance / range));
            }
            
            // Spot light cone attenuation
            if (lightType == 1) {
                vec3 spotDir = normalize(lighting.lightDirections[i].xyz);
                float theta = dot(L, -spotDir);
                float innerCone = lighting.lightDirections[i].w;
                float outerCone = lighting.lightParams[i].x;
                float epsilon = innerCone - outerCone;
                float intensity = clamp((theta - outerCone) / epsilon, 0.0, 1.0);
                attenuation *= intensity;
            }
        }
        
        vec3 H = normalize(V + L);
        vec3 radiance = lighting.lightColors[i].rgb * lighting.lightColors[i].w * attenuation;
        
        // Cook-Torrance BRDF
        float NDF = distributionGGX(N, H, roughness);
        float G = geometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;
        
        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;
        
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }
    
    return Lo;
}

// ============================================================================
// Material extensions
// ============================================================================

vec3 applyClearcoat(vec3 baseColor, vec3 N, vec3 V, float clearcoat, float clearcoatRoughness, vec3 clearcoatN) {
    if (clearcoat < 0.001) return baseColor;
    
    vec3 clearcoatF0 = vec3(0.04);
    vec3 clearcoatColor = vec3(0.0);
    
    for (int i = 0; i < lighting.numLights && i < 16; ++i) {
        vec3 L;
        float attenuation = 1.0;
        int lightType = int(lighting.lightParams[i].z);
        
        if (lightType == 2) {
            L = normalize(-lighting.lightDirections[i].xyz);
        } else {
            vec3 lightVec = lighting.lightPositions[i].xyz - fragPosition;
            float distance = length(lightVec);
            L = lightVec / distance;
            attenuation = 1.0 / (distance * distance);
            
            float range = lighting.lightParams[i].y;
            if (range > 0.0) {
                attenuation *= max(0.0, 1.0 - (distance / range));
            }
        }
        
        vec3 H = normalize(V + L);
        vec3 radiance = lighting.lightColors[i].rgb * lighting.lightColors[i].w * attenuation;
        
        float NDF = distributionGGX(clearcoatN, H, clearcoatRoughness);
        float G = geometrySmith(clearcoatN, V, L, clearcoatRoughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), clearcoatF0);
        
        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(clearcoatN, V), 0.0) * max(dot(clearcoatN, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;
        
        float NdotL = max(dot(clearcoatN, L), 0.0);
        clearcoatColor += specular * radiance * NdotL;
    }
    
    // Proper energy conservation: attenuate base layer by clearcoat fresnel
    float clearcoatFresnel = fresnelSchlick(max(dot(clearcoatN, V), 0.0), clearcoatF0).r;
    vec3 attenuatedBase = baseColor * (1.0 - clearcoat * clearcoatFresnel);
    return attenuatedBase + clearcoatColor * clearcoat;
}

vec3 applyTransmission(vec3 baseColor, vec3 N, vec3 V, float transmission, float thickness, vec3 attenuationColor, float attenuationDistance) {
    if (transmission < 0.001) return baseColor;
    
    // Calculate refraction direction
    float iorValue = material.ior > 1.0 ? material.ior : 1.5;
    float eta = 1.0 / iorValue;
    vec3 refractionDir = refract(-V, N, eta);
    
    // Use ambient as approximation for transmitted light
    // In a real implementation, this would sample from a refraction buffer
    vec3 transmittedColor = lighting.ambientLight;
    
    // Apply Beer's law attenuation (volume extension)
    if (attenuationDistance > 0.0 && thickness > 0.0) {
        vec3 attenuationCoeff = -log(attenuationColor) / attenuationDistance;
        vec3 attenuation = exp(-attenuationCoeff * thickness);
        transmittedColor *= attenuation;
    }
    
    // Calculate fresnel for transmission
    vec3 F0 = vec3(pow((1.0 - iorValue) / (1.0 + iorValue), 2.0));
    float fresnel = fresnelSchlick(max(dot(N, V), 0.0), F0).r;
    
    // Blend between transmitted and reflected based on fresnel
    float transmissionWeight = transmission * (1.0 - fresnel);
    return mix(baseColor, transmittedColor, transmissionWeight);
}

vec3 applySheen(vec3 baseColor, vec3 N, vec3 V, vec3 sheenColor, float sheenRoughness) {
    if (sheenRoughness < 0.001) return baseColor;
    
    // Simple sheen approximation using fresnel
    float sheenFresnel = pow(1.0 - max(dot(N, V), 0.0), 5.0);
    vec3 sheenContribution = sheenColor * sheenFresnel * sheenRoughness;
    
    return baseColor + sheenContribution;
}

vec3 applySpecularExtension(vec3 F0, float specularFactor, vec3 specularColorFactor) {
    // Modulate F0 by specular extension parameters
    return F0 * specularFactor * specularColorFactor;
}

// ============================================================================
// Main fragment shader
// ============================================================================

void main() {
    // Sample base color
    vec4 baseColor = material.baseColorFactor;
    if (material.hasBaseColorMap > 0.5) {
        vec4 sampledColor = texture(baseColorMap, fragTexCoord);
        // Convert to linear if texture is in sRGB space
        if (material.baseColorSpace < 0.5) {
            sampledColor.rgb = sRGBToLinear(sampledColor.rgb);
        }
        baseColor *= sampledColor;
    }
    
    // Alpha test
    if (material.alphaMode == 1.0 && baseColor.a < material.alphaCutoff) {
        discard;
    }
    
    // Albedo is now in linear space
    vec3 albedo = baseColor.rgb;
    
    // Sample metallic and roughness
    float metallic = material.metallicFactor;
    float roughness = material.roughnessFactor;
    if (material.hasMetallicRoughnessMap > 0.5) {
        vec4 mrSample = texture(metallicRoughnessMap, fragTexCoord);
        metallic *= mrSample.b;
        roughness *= mrSample.g;
    }
    roughness = max(roughness, MIN_ROUGHNESS);
    
    // Sample occlusion
    float ao = 1.0;
    if (material.hasOcclusionMap > 0.5) {
        ao = texture(occlusionMap, fragTexCoord).r;
        ao = mix(1.0, ao, material.occlusionStrength);
    }
    
    // Sample emissive
    vec3 emissive = material.emissiveFactor;
    if (material.hasEmissiveMap > 0.5) {
        vec3 sampledEmissive = texture(emissiveMap, fragTexCoord).rgb;
        // Convert to linear if texture is in sRGB space
        if (material.emissiveColorSpace < 0.5) {
            sampledEmissive = sRGBToLinear(sampledEmissive);
        }
        emissive *= sampledEmissive;
    }
    
    // Get normal
    vec3 N = getNormalFromMap(normalMap, material.normalScale, material.hasNormalMap > 0.5);
    vec3 V = normalize(fragViewDir);
    
    // Calculate F0 for dielectrics and metals
    vec3 F0 = vec3(0.04);
    
    // Apply IOR if specified (for dielectrics)
    if (material.ior > 1.0) {
        float f0 = pow((1.0 - material.ior) / (1.0 + material.ior), 2.0);
        F0 = vec3(f0);
    }
    
    // Apply specular extension
    F0 = applySpecularExtension(F0, material.specularFactor, material.specularColorFactor);
    
    // Metals use albedo as F0
    F0 = mix(F0, albedo, metallic);
    
    // Calculate base PBR lighting
    vec3 color = calculatePBR(N, V, albedo, metallic, roughness, F0);
    
    // Add ambient occlusion
    vec3 ambient = lighting.ambientLight * albedo * ao;
    color += ambient;
    
    // Apply transmission (glass/translucency effect)
    if (material.transmissionFactor > 0.0) {
        float transmission = material.transmissionFactor;
        if (material.hasTransmissionMap > 0.5) {
            transmission *= texture(transmissionMap, fragTexCoord).r;
        }
        
        float thickness = material.thicknessFactor;
        if (material.hasThicknessMap > 0.5) {
            thickness *= texture(thicknessMap, fragTexCoord).g;
        }
        
        color = applyTransmission(color, N, V, transmission, thickness, 
                                 material.attenuationColor, material.attenuationDistance);
    }
    
    // Apply clearcoat
    if (material.clearcoatFactor > 0.0) {
        float clearcoat = material.clearcoatFactor;
        if (material.hasClearcoatMap > 0.5) {
            clearcoat *= texture(clearcoatMap, fragTexCoord).r;
        }
        
        float clearcoatRoughness = material.clearcoatRoughnessFactor;
        if (material.hasClearcoatRoughnessMap > 0.5) {
            clearcoatRoughness *= texture(clearcoatRoughnessMap, fragTexCoord).g;
        }
        clearcoatRoughness = max(clearcoatRoughness, MIN_ROUGHNESS);
        
        vec3 clearcoatN = getNormalFromMap(clearcoatNormalMap, 1.0, material.hasClearcoatNormalMap > 0.5);
        color = applyClearcoat(color, N, V, clearcoat, clearcoatRoughness, clearcoatN);
    }
    
    // Apply sheen
    if (material.sheenRoughnessFactor > 0.0) {
        vec3 sheenColor = material.sheenColorFactor;
        if (material.hasSheenColorMap > 0.5) {
            sheenColor *= texture(sheenColorMap, fragTexCoord).rgb;
        }
        
        float sheenRoughness = material.sheenRoughnessFactor;
        if (material.hasSheenRoughnessMap > 0.5) {
            sheenRoughness *= texture(sheenRoughnessMap, fragTexCoord).a;
        }
        
        color = applySheen(color, N, V, sheenColor, sheenRoughness);
    }
    
    // Add emissive
    color += emissive;
    
    // Tone mapping (ACES)
    color = acesToneMap(color);
    
    // Convert from linear to sRGB for display
    color = linearToSRGB(color);
    
    outColor = vec4(color, baseColor.a);
}