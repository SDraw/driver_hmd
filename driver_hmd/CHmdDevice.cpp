#include "stdafx.h"
#include "CHmdDevice.h"

CHmdDevice::CHmdDevice()
{
    m_trackedDevice = vr::k_unTrackedDeviceIndexInvalid;
    m_propertyContainer = vr::k_ulInvalidPropertyContainer;
    m_pose = { 0 };

    m_pose.poseTimeOffset = -0.016;
    m_pose.qWorldFromDriverRotation.w = 1.0;
    m_pose.qWorldFromDriverRotation.x = .0;
    m_pose.qWorldFromDriverRotation.y = .0;
    m_pose.qWorldFromDriverRotation.z = .0;
    m_pose.qDriverFromHeadRotation.w = 1.0;
    m_pose.qDriverFromHeadRotation.x = .0;
    m_pose.qDriverFromHeadRotation.y = .0;
    m_pose.qDriverFromHeadRotation.z = .0;
    m_pose.vecDriverFromHeadTranslation[0U] = .0;
    m_pose.vecDriverFromHeadTranslation[1U] = .0;
    m_pose.vecDriverFromHeadTranslation[2U] = .0;
    m_pose.qRotation.w = 1.f;
    m_pose.qRotation.x = 0.f;
    m_pose.qRotation.y = 0.f;
    m_pose.qRotation.z = 0.f;
    m_pose.vecPosition[0U] = 0.f;
    m_pose.vecPosition[1U] = 0.f;
    m_pose.vecPosition[2U] = 0.f;
    m_pose.poseIsValid = false;
    m_pose.willDriftInYaw = false;
    m_pose.shouldApplyHeadModel = false;
    m_pose.result = vr::TrackingResult_Uninitialized;
    m_pose.deviceIsConnected = false;

    m_serial.assign("vr_hmd_0001");
    m_resolution = glm::uvec2(640U, 480U);
    m_fps = 60.f;
}
CHmdDevice::~CHmdDevice()
{
}

const std::string& CHmdDevice::GetSerial() const
{
    return m_serial;
}

void CHmdDevice::SetResolution(const glm::uvec2 &f_res)
{
    std::memcpy(&m_resolution, &f_res, sizeof(glm::uvec2));
}

void CHmdDevice::SetFPS(const float f_fps)
{

}

void CHmdDevice::GetTransformation(glm::vec3 &f_pos, glm::quat &f_rot)
{
    f_pos.x = m_pose.vecPosition[0U];
    f_pos.y = m_pose.vecPosition[1U];
    f_pos.z = m_pose.vecPosition[2U];
    f_rot.x = m_pose.qRotation.x;
    f_rot.y = m_pose.qRotation.y;
    f_rot.z = m_pose.qRotation.z;
    f_rot.w = m_pose.qRotation.w;
}

void CHmdDevice::SetTransformation(const glm::vec3 &f_pos, const glm::quat &f_rot)
{
    m_pose.vecPosition[0U] = f_pos.x;
    m_pose.vecPosition[1U] = f_pos.y;
    m_pose.vecPosition[2U] = f_pos.z;
    m_pose.qRotation.x = f_rot.x;
    m_pose.qRotation.y = f_rot.y;
    m_pose.qRotation.z = f_rot.z;
    m_pose.qRotation.w = f_rot.w;
}

vr::EVRInitError CHmdDevice::Activate(uint32_t unObjectId)
{
    m_trackedDevice = unObjectId;
    m_propertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_trackedDevice);

    vr::VRProperties()->SetInt32Property(m_propertyContainer, vr::Prop_DeviceClass_Int32, vr::TrackedDeviceClass_HMD);
    vr::VRProperties()->SetStringProperty(m_propertyContainer, vr::Prop_ModelNumber_String, m_serial.c_str());
    vr::VRProperties()->SetStringProperty(m_propertyContainer, vr::Prop_RenderModelName_String, m_serial.c_str());
    vr::VRProperties()->SetFloatProperty(m_propertyContainer, vr::Prop_UserIpdMeters_Float, 0.f);
    vr::VRProperties()->SetFloatProperty(m_propertyContainer, vr::Prop_UserHeadToEyeDepthMeters_Float, 0.f);
    vr::VRProperties()->SetFloatProperty(m_propertyContainer, vr::Prop_DisplayFrequency_Float, m_fps);
    vr::VRProperties()->SetFloatProperty(m_propertyContainer, vr::Prop_SecondsFromVsyncToPhotons_Float, 1.f / m_fps);
    vr::VRProperties()->SetUint64Property(m_propertyContainer, vr::Prop_CurrentUniverseId_Uint64, 2);
    vr::VRProperties()->SetBoolProperty(m_propertyContainer, vr::Prop_IsOnDesktop_Bool, false);
    vr::VRProperties()->SetBoolProperty(m_propertyContainer, vr::Prop_HasDisplayComponent_Bool, true);
    vr::VRProperties()->SetBoolProperty(m_propertyContainer, vr::Prop_ContainsProximitySensor_Bool, false);
    vr::VRProperties()->SetStringProperty(m_propertyContainer, vr::Prop_NamedIconPathDeviceReady_String, "{hmd}/icons/headset_status_ready.png");
    vr::VRProperties()->SetStringProperty(m_propertyContainer, vr::Prop_NamedIconPathDeviceOff_String, "{hmd}/icons/headset_status_off.png");
    vr::VRProperties()->SetStringProperty(m_propertyContainer, vr::Prop_NamedIconPathDeviceSearching_String, "{hmd}/icons/headset_status_searching.png");
    vr::VRProperties()->SetStringProperty(m_propertyContainer, vr::Prop_NamedIconPathDeviceStandby_String, "{hmd}/icons/headset_status_standby.png");

    m_pose.deviceIsConnected = true;
    m_pose.poseIsValid = true;
    m_pose.result = vr::TrackingResult_Running_OK;

    return vr::VRInitError_None;
}

void CHmdDevice::Deactivate()
{
    m_trackedDevice = vr::k_unTrackedDeviceIndexInvalid;
    m_propertyContainer = vr::k_ulInvalidPropertyContainer;
}

void CHmdDevice::EnterStandby()
{
}

void* CHmdDevice::GetComponent(const char *pchComponentNameAndVersion)
{
    void *l_result = nullptr;
    if(!strcmp(pchComponentNameAndVersion, vr::ITrackedDeviceServerDriver_Version)) l_result = dynamic_cast<vr::ITrackedDeviceServerDriver*>(this);
    if(!strcmp(pchComponentNameAndVersion, vr::IVRDisplayComponent_Version)) l_result = dynamic_cast<vr::IVRDisplayComponent*>(this);
    return l_result;
}

void CHmdDevice::DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize)
{
    std::stringstream l_message(pchRequest);
    std::string l_messageType;
    l_message >> l_messageType;
    if(!l_messageType.compare("transform"))
    {
        glm::vec3 l_diffPos;
        for(int i = 0; i < 3; i++) l_message >> l_diffPos[i];
        m_pose.vecPosition[0U] += l_diffPos.x;
        m_pose.vecPosition[1U] += l_diffPos.y;
        m_pose.vecPosition[2U] += l_diffPos.z;

        glm::quat l_diffRot;
        for(int i = 0; i < 4; i++) l_message >> l_diffRot[i];

        glm::quat l_deviceRot(m_pose.qRotation.w, m_pose.qRotation.x, m_pose.qRotation.y, m_pose.qRotation.z);
        glm::quat l_newRot = l_deviceRot*l_diffRot;

        m_pose.qRotation.w = l_newRot.w;
        m_pose.qRotation.x = l_newRot.x;
        m_pose.qRotation.y = l_newRot.y;
        m_pose.qRotation.z = l_newRot.z;
    }
}

vr::DriverPose_t CHmdDevice::GetPose()
{
    return m_pose;
}

void CHmdDevice::GetWindowBounds(int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight)
{
    *pnX = 0;
    *pnY = 0;
    *pnWidth = m_resolution.x;
    *pnHeight = m_resolution.y;
}
bool CHmdDevice::IsDisplayOnDesktop()
{
    return true;
}
bool CHmdDevice::IsDisplayRealDisplay()
{
    return false;
}
void CHmdDevice::GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight)
{
    *pnWidth = m_resolution.x;
    *pnHeight = m_resolution.y;
}
void CHmdDevice::GetEyeOutputViewport(vr::EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight)
{
    // Ignore right eye, all eyes are at same place due to zero IPD
    *pnX = 0;
    *pnY = 0;
    *pnWidth = (eEye == vr::Eye_Left) ? m_resolution.x : 0;
    *pnHeight = (eEye == vr::Eye_Left) ? m_resolution.y : 0;
}
void CHmdDevice::GetProjectionRaw(vr::EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom)
{
    *pfLeft = -1.0;
    *pfRight = 1.0;
    *pfTop = -1.0;
    *pfBottom = 1.0;
}
vr::DistortionCoordinates_t CHmdDevice::ComputeDistortion(vr::EVREye eEye, float fU, float fV)
{
    vr::DistortionCoordinates_t l_coordinates;
    l_coordinates.rfBlue[0] = fU;
    l_coordinates.rfBlue[1] = fV;
    l_coordinates.rfGreen[0] = fU;
    l_coordinates.rfGreen[1] = fV;
    l_coordinates.rfRed[0] = fU;
    l_coordinates.rfRed[1] = fV;
    return l_coordinates;
}

void CHmdDevice::RunFrame()
{
    if(m_trackedDevice != vr::k_unTrackedDeviceIndexInvalid)
    {
        vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_trackedDevice, m_pose, sizeof(vr::DriverPose_t));
    }
}
