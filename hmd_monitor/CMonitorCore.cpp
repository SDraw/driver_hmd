#include "stdafx.h"
#include "CMonitorCore.h"
#include "Utils.h"

const std::chrono::microseconds g_threadDelay(16U);
const glm::vec3 g_zeroPoint3(0.f);
const glm::vec4 g_zeroPoint4(0.f, 0.f, 0.f, 1.f);
const glm::quat g_zeroRotation(1.f, 0.f, 0.f, 0.f);
const glm::vec3 g_axisX(1.f, 0.f, 0.f);
const glm::vec3 g_axisY(0.f, 1.f, 0.f);
const glm::vec3 g_axisZ(0.f, 0.f, -1.f);
const glm::vec3 g_axisXN(-1.f, 0.f, 0.f);
const glm::vec3 g_axisYN(0.f, -1.f, 0.f);
const glm::vec3 g_axisZN(0.f, 0.f, 1.f);
const float g_singleAngle = glm::pi<float>() / 180.f / 10.f;

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
        glm::vec3 l_pos = (m_triggerClicked ? (m_handPosition - m_lastHandPosition) : g_zeroPoint3);
        glm::quat l_rot = (m_triggerClicked ? g_zeroRotation : (glm::inverse(m_lastHandRotation)*m_handRotation));

        std::string l_message("transform ");
        AppendToString(l_message, l_pos);
        AppendToString(l_message, l_rot);

        char l_response[32U];
        std::memset(l_response, 0, 32U);
        vr::VRDebug()->DriverDebugRequest(vr::k_unTrackedDeviceIndex_Hmd, l_message.c_str(), l_response, 32U);

        m_lastHandPosition = m_handPosition;
        m_lastHandRotation = m_handRotation;
    }
    else
    {
        if((GetKeyState(VK_NUMLOCK) & 0xFFFF) != 0)
        {
            glm::vec3 l_pos(g_zeroPoint3);
            glm::quat l_rot(g_zeroRotation);

            glm::mat4 l_headTransform;
            ConvertMatrix(l_pose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking, l_headTransform);
            glm::quat l_headRot = glm::quat_cast(l_headTransform);

            // Forward-backward
            if(GetAsyncKeyState(VK_UP) & 0x8000) l_pos += l_headRot*(g_axisZ*0.001f);
            else if(GetAsyncKeyState(VK_DOWN) & 0x8000) l_pos += l_headRot*(g_axisZN*0.001f);

            // Left-right
            if(GetAsyncKeyState(VK_LEFT) & 0x8000) l_pos += l_headRot*(g_axisXN*0.001f);
            else if(GetAsyncKeyState(VK_RIGHT) & 0x8000) l_pos += l_headRot*(g_axisX*0.001f);

            // Up-down
            if(GetAsyncKeyState(VK_NUMPAD8) & 0x8000) l_pos += l_headRot*(g_axisY*0.001f);
            else if(GetAsyncKeyState(VK_NUMPAD2) & 0x8000) l_pos += l_headRot*(g_axisYN*0.001f);

            // Rotate localy around Y
            if(GetAsyncKeyState(VK_NUMPAD4) & 0x8000) l_rot *= glm::angleAxis(g_singleAngle, g_axisY);
            else if(GetAsyncKeyState(VK_NUMPAD6) & 0x8000) l_rot *= glm::angleAxis(-g_singleAngle, g_axisY);

            // Rotate localy around X
            if(GetAsyncKeyState(VK_NUMPAD7) & 0x8000) l_rot *= glm::angleAxis(-g_singleAngle, g_axisX);
            else if(GetAsyncKeyState(VK_NUMPAD9) & 0x8000) l_rot *= glm::angleAxis(g_singleAngle, g_axisX);

            // Rotate localy around Z
            if(GetAsyncKeyState(VK_NUMPAD1) & 0x8000) l_rot *= glm::angleAxis(-g_singleAngle, g_axisZ);
            else if(GetAsyncKeyState(VK_NUMPAD3) & 0x8000) l_rot *= glm::angleAxis(g_singleAngle, g_axisZ);

            if((l_pos != g_zeroPoint3) || (l_rot != g_zeroRotation))
            {
                std::string l_message("transform ");
                AppendToString(l_message, l_pos);
                AppendToString(l_message, l_rot);

                char l_response[32U];
                std::memset(l_response, 0, 32U);
                vr::VRDebug()->DriverDebugRequest(vr::k_unTrackedDeviceIndex_Hmd, l_message.c_str(), l_response, 32U);
            }
        }
    }

    if(m_handDevice == vr::k_unTrackedDeviceIndexInvalid) m_handDevice = m_vrSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);

    std::this_thread::sleep_for(g_threadDelay);
    return m_active;
}
