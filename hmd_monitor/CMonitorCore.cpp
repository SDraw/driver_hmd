#include "stdafx.h"
#include "CMonitorCore.h"
#include "Utils.h"

const std::chrono::microseconds g_threadDelay(16U);
const glm::vec3 g_zeroPoint3(0.f);
const glm::vec4 g_zeroPoint4(0.f, 0.f, 0.f, 1.f);
const glm::quat g_zeroRotation(1.f, 0.f, 0.f, 0.f);

CMonitorCore::CMonitorCore()
{
    m_active = false;

    m_vrSystem = nullptr;
    m_handDevice = vr::k_unTrackedDeviceIndexInvalid;
    m_event = { 0 };

    m_trasnformActive = false;
    m_triggerClicked = false;
    m_lastGripTick = 0U;
    m_handPosition = g_zeroPoint3;
    m_handRotation = g_zeroRotation;
    m_lastHandPosition = g_zeroPoint3;
    m_lastHandRotation = g_zeroRotation;
}
CMonitorCore::~CMonitorCore()
{
}

bool CMonitorCore::Initialize()
{
    if(!m_active)
    {
        vr::EVRInitError l_error;
        m_vrSystem = vr::VR_Init(&l_error, vr::VRApplication_Utility);
        if(l_error == vr::VRInitError_None)
        {
            m_handDevice = m_vrSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);

            m_active = true;
        }
    }

    return m_active;
}

void CMonitorCore::Terminate()
{
    if(m_active)
    {
        vr::VR_Shutdown();
        m_vrSystem = nullptr;

        m_active = false;
    }
}

bool CMonitorCore::DoPulse()
{
    vr::TrackedDevicePose_t l_pose[vr::k_unMaxTrackedDeviceCount];
    m_vrSystem->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseRawAndUncalibrated, 0.f, l_pose, vr::k_unMaxTrackedDeviceCount);

    if(m_handDevice != vr::k_unTrackedDeviceIndexInvalid)
    {
        glm::mat4 l_transform;
        ConvertMatrix(l_pose[m_handDevice].mDeviceToAbsoluteTracking, l_transform);
        m_handPosition = l_transform*g_zeroPoint4;
        m_handRotation = glm::quat_cast(l_transform);
    }

    while(m_vrSystem->PollNextEvent(&m_event, sizeof(vr::VREvent_t)))
    {
        switch(m_event.eventType)
        {
            case vr::VREvent_Quit:
                m_active = false;
                break;
            case vr::VREvent_TrackedDeviceDeactivated:
            {
                if(m_handDevice == m_event.trackedDeviceIndex)
                {
                    m_handDevice = vr::k_unTrackedDeviceIndexInvalid;
                    m_trasnformActive = false;
                    m_triggerClicked = false;
                }
            } break;
            case vr::VREvent_TrackedDeviceActivated:
            {
                if(m_handDevice == vr::k_unTrackedDeviceIndexInvalid)
                {
                    if(m_vrSystem->GetControllerRoleForTrackedDeviceIndex(m_event.trackedDeviceIndex) == vr::TrackedControllerRole_LeftHand)
                    {
                        m_handDevice = m_event.trackedDeviceIndex;
                    }
                }
            } break;
            case vr::VREvent_TrackedDeviceRoleChanged:
            {
                if(m_handDevice == vr::k_unTrackedDeviceIndexInvalid)
                {
                    m_handDevice = m_vrSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
                }
            } break;

            case vr::VREvent_ButtonPress:
            {
                if(m_event.trackedDeviceIndex == m_handDevice)
                {
                    switch(m_event.data.controller.button)
                    {
                        case vr::k_EButton_Grip:
                        {
                            unsigned long long l_tick = GetTickCount64();
                            m_trasnformActive = ((l_tick - m_lastGripTick) < 500U);
                            if(m_trasnformActive)
                            {
                                m_lastHandPosition = m_handPosition;
                                m_lastHandRotation = m_handRotation;
                            }
                            m_lastGripTick = l_tick;
                        } break;
                        case vr::k_EButton_SteamVR_Trigger:
                            m_triggerClicked = true;
                            break;
                    }
                }
            } break;
            case vr::VREvent_ButtonUnpress:
            {
                if(m_event.trackedDeviceIndex == m_handDevice)
                {
                    switch(m_event.data.controller.button)
                    {
                        case  vr::k_EButton_Grip:
                        {
                            if(m_trasnformActive) m_trasnformActive = false;
                        } break;
                        case vr::k_EButton_SteamVR_Trigger:
                            m_triggerClicked = false;
                            break;
                    }
                }
            }
        }
    }

    if(m_trasnformActive)
    {
        glm::vec3 l_posDiff = (m_triggerClicked ? (m_handPosition - m_lastHandPosition) : g_zeroPoint3);
        glm::quat l_rotDiff = (m_triggerClicked ? g_zeroRotation : (glm::inverse(m_lastHandRotation)*m_handRotation));

        std::string l_debugMessage("transform ");
        for(int i = 0; i < 3; i++)
        {
            l_debugMessage.append(std::to_string(l_posDiff[i]));
            l_debugMessage.push_back(' ');
        }
        for(int i = 0; i < 4; i++)
        {
            l_debugMessage.append(std::to_string(l_rotDiff[i]));
            l_debugMessage.push_back(' ');
        }

        char l_response[32U];
        std::memset(l_response, 0, 32U);
        vr::VRDebug()->DriverDebugRequest(vr::k_unTrackedDeviceIndex_Hmd, l_debugMessage.c_str(), l_response, 32U);

        m_lastHandPosition = m_handPosition;
        m_lastHandRotation = m_handRotation;
    }

    std::this_thread::sleep_for(g_threadDelay);
    return m_active;
}
