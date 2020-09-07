#include "stdafx.h"
#include "CServerDriver.h"
#include "CDriverConfig.h"
#include "CHmdDevice.h"

extern char g_modulePath[];

const char* const CServerDriver::ms_interfaces[]
{
    vr::ITrackedDeviceServerDriver_Version,
        vr::IServerTrackedDeviceProvider_Version,
        vr::IVRDisplayComponent_Version,
        nullptr
};

CServerDriver::CServerDriver()
{
    m_driverHost = nullptr;
    m_hmdDevice = nullptr;
}
CServerDriver::~CServerDriver()
{
}

vr::EVRInitError CServerDriver::Init(vr::IVRDriverContext *pDriverContext)
{
    VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
    m_driverHost = vr::VRServerDriverHost();

    m_driverConfig = new CDriverConfig();
    m_driverConfig->Load();

    m_hmdDevice = new CHmdDevice();
    m_hmdDevice->SetResolution(m_driverConfig->GetResolution());
    m_hmdDevice->SetFPS(m_driverConfig->GetFPS());
    m_hmdDevice->SetTransformation(m_driverConfig->GetPosition(), m_driverConfig->GetRotation());

    m_driverHost->TrackedDeviceAdded(m_hmdDevice->GetSerial().c_str(), vr::TrackedDeviceClass_HMD, m_hmdDevice);

    // Start utility app that closes itself on SteamVR shutdown
    std::string l_path(g_modulePath);
    l_path.erase(l_path.begin() + l_path.rfind('\\'), l_path.end());

    std::string l_appPath(l_path);
    l_appPath.append("\\hmd_monitor.exe");

    PROCESS_INFORMATION l_monitorInfo = { 0 };
    STARTUPINFOA l_infoProcess = { 0 };
    l_infoProcess.cb = sizeof(STARTUPINFOA);
    CreateProcessA(l_appPath.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, l_path.c_str(), &l_infoProcess, &l_monitorInfo);

    return vr::VRInitError_None;
}

void CServerDriver::Cleanup()
{
    glm::vec3 l_hmdPos;
    glm::quat l_hmdRot;
    m_hmdDevice->GetTransformation(l_hmdPos, l_hmdRot);
    m_driverConfig->SetPosition(l_hmdPos);
    m_driverConfig->SetRotation(l_hmdRot);
    m_driverConfig->Save();

    delete m_hmdDevice;
    m_hmdDevice = nullptr;

    delete m_driverConfig;
    m_driverConfig = nullptr;

    m_driverHost = nullptr;
    VR_CLEANUP_SERVER_DRIVER_CONTEXT();
}

const char* const* CServerDriver::GetInterfaceVersions()
{
    return ms_interfaces;
}

bool CServerDriver::ShouldBlockStandbyMode()
{
    return false;
}

void CServerDriver::EnterStandby()
{
}

void CServerDriver::LeaveStandby()
{
}

void CServerDriver::RunFrame()
{
    m_hmdDevice->RunFrame();
}