#pragma once

class CHmdDevice final : public vr::ITrackedDeviceServerDriver, public vr::IVRDisplayComponent
{
    vr::TrackedDeviceIndex_t m_trackedDevice;
    vr::PropertyContainerHandle_t m_propertyContainer;
    vr::DriverPose_t m_pose;

    std::string m_serial;
    glm::uvec2 m_resolution;
    float m_fps;

    // vr::ITrackedDeviceServerDriver
    vr::EVRInitError Activate(uint32_t unObjectId);
    void Deactivate();
    void EnterStandby();
    void *GetComponent(const char *pchComponentNameAndVersion);
    void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize);
    vr::DriverPose_t GetPose();

    // vr::IVRDisplayComponent
    void GetWindowBounds(int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight);
    bool IsDisplayOnDesktop();
    bool IsDisplayRealDisplay();
    void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight);
    void GetEyeOutputViewport(vr::EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight);
    void GetProjectionRaw(vr::EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom);
    vr::DistortionCoordinates_t ComputeDistortion(vr::EVREye eEye, float fU, float fV);
public:
    const std::string& GetSerial() const;
    void GetTransformation(glm::vec3 &f_pos, glm::quat &f_rot);
protected:
    CHmdDevice();
    ~CHmdDevice();
    
    void SetResolution(const glm::uvec2 &f_res);
    void SetFPS(const float f_fps);
    void SetTransformation(const glm::vec3 &f_pos, const glm::quat &f_rot);

    void RunFrame();

    friend class CServerDriver;
};
