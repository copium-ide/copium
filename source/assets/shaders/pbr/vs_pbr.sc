$input a_position, a_normal, a_tangent, a_texcoord0
$output v_worldPos, v_normal, v_tangent, v_bitangent, v_texcoord0

#include <bgfx_shader.sh>

void main()
{
    // Transform position to world space
    vec4 worldPos = mul(u_model[0], vec4(a_position, 1.0));
    v_worldPos = worldPos.xyz;
    
    // Transform to clip space
    gl_Position = mul(u_viewProj, worldPos);
    
    // Extract 3x3 rotation/scale from model matrix
    // For proper normal transformation, we should use inverse transpose,
    // but for uniform scaling and rotation-only transforms, the upper 3x3 works
    mat3 normalMatrix = mat3(
        u_model[0][0].xyz,  // First column
        u_model[0][1].xyz,  // Second column
        u_model[0][2].xyz   // Third column
    );
    
    // Transform and normalize normal
    vec3 transformedNormal = mul(normalMatrix, a_normal);
    v_normal = normalize(transformedNormal);
    
    // Transform and normalize tangent
    vec3 transformedTangent = mul(normalMatrix, a_tangent.xyz);
    v_tangent = normalize(transformedTangent);
    
    // Re-orthogonalize tangent with respect to normal using Gram-Schmidt
    v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);
    
    // Calculate bitangent with handedness
    v_bitangent = cross(v_normal, v_tangent) * a_tangent.w;
    
    // Pass through texture coordinates
    v_texcoord0 = a_texcoord0;
}