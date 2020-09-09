#pragma once

class CMonitorCore
{
    bool m_active;

    vr::IVRSystem *m_vrSystem;
    vr::TrackedDeviceIndex_t m_handDevice;
    vr::VREvent_t m_event;

    bool m_trasnformActive;
    bool m_triggerClicked;
    unsigned long long m_lastGripTick;
    glm::vec3 m_handPosition;
    glm::quat m_handRotation;
    glm::vec3 m_lastHandPosition;
    glm::quat m_lastHandRotation;
public:
    CMonitorCore();
    ~CMonitorCore();

    bool Initialize();
    void Terminate();
    bool DoPulse();
};
