#pragma once

class CDriverConfig;
class CHmdDevice;

class CServerDriver final : public vr::IServerTrackedDeviceProvider
{
    vr::IVRServerDriverHost *m_driverHost;
    static const char* const ms_interfaces[];

    CDriverConfig *m_driverConfig;
    CHmdDevice *m_hmdDevice;

    // vr::IServerTrackedDeviceProvider
    vr::EVRInitError Init(vr::IVRDriverContext *pDriverContext);
    void Cleanup();
    const char * const *GetInterfaceVersions();
    void RunFrame();
    bool ShouldBlockStandbyMode();
    void EnterStandby();
    void LeaveStandby();
public:
    CServerDriver();
    ~CServerDriver();
};
