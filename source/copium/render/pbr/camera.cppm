module;
#include <bgfx/bgfx.h>
#include <cmath>
#include <cstring>
export module copium.render.pbr.camera;

export namespace PBR {

constexpr float PI = 3.14159265359f;
constexpr float DEG_TO_RAD = PI / 180.0f;

class Camera
{
private:
    float position[3] = {0.0f, 0.0f, 5.0f};
    float target[3] = {0.0f, 0.0f, 0.0f};
    float up[3] = {0.0f, 1.0f, 0.0f};
    
    float fov = 60.0f;
    float aspectRatio = 16.0f / 9.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    
    float viewMatrix[16];
    float projMatrix[16];
    
    bgfx::UniformHandle uniformCameraPos = BGFX_INVALID_HANDLE;
    bgfx::ViewId currentViewId = 0;
    bool initialized = false;

    void
    cross(float* result, const float* a, const float* b) const
    {
        result[0] = a[1] * b[2] - a[2] * b[1];
        result[1] = a[2] * b[0] - a[0] * b[2];
        result[2] = a[0] * b[1] - a[1] * b[0];
    }

    float
    dot(const float* a, const float* b) const
    {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }

    void
    normalize(float* v) const
    {
        float len = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        if (len > 1e-6f)
        {
            float invLen = 1.0f / len;
            v[0] *= invLen;
            v[1] *= invLen;
            v[2] *= invLen;
        }
    }

    void
    updateMatrices()
    {
        float forward[3] = {
            target[0] - position[0],
            target[1] - position[1],
            target[2] - position[2]
        };
        normalize(forward);
        
        float right[3];
        cross(right, forward, up);
        normalize(right);
        
        float realUp[3];
        cross(realUp, right, forward);
        normalize(realUp);

        viewMatrix[0] = right[0];
        viewMatrix[1] = realUp[0];
        viewMatrix[2] = -forward[0];
        viewMatrix[3] = 0.0f;
        
        viewMatrix[4] = right[1];
        viewMatrix[5] = realUp[1];
        viewMatrix[6] = -forward[1];
        viewMatrix[7] = 0.0f;
        
        viewMatrix[8] = right[2];
        viewMatrix[9] = realUp[2];
        viewMatrix[10] = -forward[2];
        viewMatrix[11] = 0.0f;
        
        viewMatrix[12] = -dot(right, position);
        viewMatrix[13] = -dot(realUp, position);
        viewMatrix[14] = dot(forward, position);
        viewMatrix[15] = 1.0f;

        // projection matrix
        float fovRad = fov * DEG_TO_RAD;
        float tanHalfFov = std::tan(fovRad * 0.5f);
        
        std::memset(projMatrix, 0, sizeof(projMatrix));

        // Standard perspective projection for bgfx
        projMatrix[0] = 1.0f / (aspectRatio * tanHalfFov);
        projMatrix[5] = 1.0f / tanHalfFov;
        projMatrix[10] = farPlane / (nearPlane - farPlane);
        projMatrix[11] = -1.0f;
        projMatrix[14] = (nearPlane * farPlane) / (nearPlane - farPlane);
        projMatrix[15] = 0.0f;
    }

public:
    Camera()
    {
        std::memset(viewMatrix, 0, sizeof(viewMatrix));
        std::memset(projMatrix, 0, sizeof(projMatrix));
        
        viewMatrix[0] = viewMatrix[5] = viewMatrix[10] = viewMatrix[15] = 1.0f;
        projMatrix[0] = projMatrix[5] = projMatrix[10] = projMatrix[15] = 1.0f;
    }

    ~Camera()
    {
        release();
    }

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    Camera(Camera&& other) noexcept
        : fov(other.fov), aspectRatio(other.aspectRatio),
          nearPlane(other.nearPlane), farPlane(other.farPlane),
          uniformCameraPos(other.uniformCameraPos), currentViewId(other.currentViewId),
          initialized(other.initialized)
    {
        std::memcpy(position, other.position, sizeof(position));
        std::memcpy(target, other.target, sizeof(target));
        std::memcpy(up, other.up, sizeof(up));
        std::memcpy(viewMatrix, other.viewMatrix, sizeof(viewMatrix));
        std::memcpy(projMatrix, other.projMatrix, sizeof(projMatrix));
        
        other.uniformCameraPos = BGFX_INVALID_HANDLE;
        other.initialized = false;
    }

    Camera&
    operator=(Camera&& other) noexcept
    {
        if (this != &other)
        {
            release();
            
            std::memcpy(position, other.position, sizeof(position));
            std::memcpy(target, other.target, sizeof(target));
            std::memcpy(up, other.up, sizeof(up));
            std::memcpy(viewMatrix, other.viewMatrix, sizeof(viewMatrix));
            std::memcpy(projMatrix, other.projMatrix, sizeof(projMatrix));
            
            fov = other.fov;
            aspectRatio = other.aspectRatio;
            nearPlane = other.nearPlane;
            farPlane = other.farPlane;
            uniformCameraPos = other.uniformCameraPos;
            currentViewId = other.currentViewId;
            initialized = other.initialized;
            
            other.uniformCameraPos = BGFX_INVALID_HANDLE;
            other.initialized = false;
        }
        return *this;
    }

    void
    initialize(bgfx::ViewId viewId = 0)
    {
        if (!initialized)
        {
            uniformCameraPos = bgfx::createUniform("u_cameraPos", bgfx::UniformType::Vec4);
            currentViewId = viewId;
            initialized = true;
            updateMatrices();
        }
    }

    void
    release()
    {
        if (initialized)
        {
            if (bgfx::isValid(uniformCameraPos))
            {
                bgfx::destroy(uniformCameraPos);
                uniformCameraPos = BGFX_INVALID_HANDLE;
            }
            initialized = false;
        }
    }

    void
    bind(bgfx::ViewId viewId) const
    {
        if (!initialized) return;

        bgfx::setViewTransform(viewId, viewMatrix, projMatrix);

        // Set camera position uniform
        float camPos[4] = {position[0], position[1], position[2], 1.0f};
        bgfx::setUniform(uniformCameraPos, camPos);
    }

    void
    bind() const
    {
        bind(currentViewId);
    }

    void
    setViewId(bgfx::ViewId viewId)
    {
        currentViewId = viewId;
    }

    void
    setPosition(float x, float y, float z)
    {
        position[0] = x;
        position[1] = y;
        position[2] = z;
        updateMatrices();
    }

    void
    setTarget(float x, float y, float z)
    {
        target[0] = x;
        target[1] = y;
        target[2] = z;
        updateMatrices();
    }

    void
    setUp(float x, float y, float z)
    {
        up[0] = x;
        up[1] = y;
        up[2] = z;
        normalize(up);
        updateMatrices();
    }

    void
    setFOV(float degrees)
    {
        fov = degrees;
        updateMatrices();
    }

    void
    setAspectRatio(float ratio)
    {
        aspectRatio = ratio;
        updateMatrices();
    }

    void
    setAspectRatio(uint32_t width, uint32_t height)
    {
        if (height > 0)
        {
            aspectRatio = static_cast<float>(width) / static_cast<float>(height);
            updateMatrices();
        }
    }

    void
    setClipPlanes(float near, float far)
    {
        if (near > 0.0f && far > near)
        {
            nearPlane = near;
            farPlane = far;
            updateMatrices();
        }
    }

    void
    lookAt(float eyeX, float eyeY, float eyeZ, float targetX, float targetY, float targetZ)
    {
        position[0] = eyeX;
        position[1] = eyeY;
        position[2] = eyeZ;
        target[0] = targetX;
        target[1] = targetY;
        target[2] = targetZ;
        updateMatrices();
    }

    void
    orbit(float yaw, float pitch, float distance)
    {
        if (distance <= 0.0f) return;

        float yawRad = yaw * DEG_TO_RAD;
        float pitchRad = pitch * DEG_TO_RAD;

        // Clamp pitch to avoid gimbal lock
        const float maxPitch = 89.0f * DEG_TO_RAD;
        if (pitchRad > maxPitch) pitchRad = maxPitch;
        if (pitchRad < -maxPitch) pitchRad = -maxPitch;

        position[0] = target[0] + distance * std::cos(pitchRad) * std::sin(yawRad);
        position[1] = target[1] + distance * std::sin(pitchRad);
        position[2] = target[2] + distance * std::cos(pitchRad) * std::cos(yawRad);
        
        updateMatrices();
    }

    void
    translate(float dx, float dy, float dz)
    {
        position[0] += dx;
        position[1] += dy;
        position[2] += dz;
        target[0] += dx;
        target[1] += dy;
        target[2] += dz;
        updateMatrices();
    }

    const float*
    getPosition() const
    {
        return position;
    }

    const float*
    getTarget() const
    {
        return target;
    }

    const float*
    getUp() const
    {
        return up;
    }

    const float*
    getViewMatrix() const
    {
        return viewMatrix;
    }

    const float*
    getProjectionMatrix() const
    {
        return projMatrix;
    }

    float
    getFOV() const
    {
        return fov;
    }

    float
    getAspectRatio() const
    {
        return aspectRatio;
    }

    float
    getNearPlane() const
    {
        return nearPlane;
    }

    float
    getFarPlane() const
    {
        return farPlane;
    }

    bool
    isInitialized() const
    {
        return initialized;
    }
};

}