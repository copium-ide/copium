#version 450

// Vertex inputs
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec4 inTangent;
layout(location = 4) in vec4 inJoints;    // Joint indices for skinning
layout(location = 5) in vec4 inWeights;   // Joint weights for skinning

// Vertex outputs
layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 fragTangent;
layout(location = 4) out vec3 fragBitangent;
layout(location = 5) out vec3 fragViewDir;

// Push constants - per draw call
layout(push_constant) uniform PushConstants {
    mat4 model;
    mat4 normalMatrix;  // Pre-computed transpose(inverse(model))
    uint materialIdx;
    uint hasAnimation;  // 0 = static, 1 = animated
} push;

// Scene data - updated per frame
layout(std140, binding = 0) readonly buffer SceneData {
    mat4 view;
    mat4 projection;
    vec3 cameraPos;
    float time;
} scene;

// Animation data - joint matrices for skinning
layout(std140, binding = 1) readonly buffer AnimationData {
    mat4 jointMatrices[256];  // Support up to 256 bones
} animation;

void main() {
    vec4 localPosition = vec4(inPosition, 1.0);
    vec3 localNormal = inNormal;
    vec3 localTangent = inTangent.xyz;
    
    // Apply skinning if animated
    if (push.hasAnimation > 0) {
        // Blend up to 4 joints per vertex
        mat4 skinMatrix = 
            inWeights.x * animation.jointMatrices[int(inJoints.x)] +
            inWeights.y * animation.jointMatrices[int(inJoints.y)] +
            inWeights.z * animation.jointMatrices[int(inJoints.z)] +
            inWeights.w * animation.jointMatrices[int(inJoints.w)];
        
        localPosition = skinMatrix * localPosition;
        localNormal = mat3(skinMatrix) * localNormal;
        localTangent = mat3(skinMatrix) * localTangent;
    }
    
    // Transform to world space
    vec4 worldPos = push.model * localPosition;
    gl_Position = scene.projection * scene.view * worldPos;
    
    fragPosition = worldPos.xyz;
    fragTexCoord = inTexCoord;
    
    // Calculate TBN matrix for normal mapping
    mat3 normalMat = mat3(push.normalMatrix);
    vec3 T = normalize(normalMat * localTangent);
    vec3 N = normalize(normalMat * localNormal);
    
    // Re-orthogonalize T against N (Gram-Schmidt)
    T = normalize(T - dot(T, N) * N);
    
    // Compute bitangent with handedness from inTangent.w
    vec3 B = cross(N, T) * inTangent.w;
    
    fragTangent = T;
    fragBitangent = B;
    fragNormal = N;
    fragViewDir = scene.cameraPos - worldPos.xyz;
}