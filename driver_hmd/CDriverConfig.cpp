#include "stdafx.h"
#include "CDriverConfig.h"
#include "Utils.h"

extern char g_modulePath[];

const std::vector<std::string> g_settingsNames
{
    "resolution", "fps", "position", "rotation"
};
const enum SettingIndex : size_t
{
    SI_Resolution = 0U,
    SI_FramesPerSecond,
    SI_Position,
    SI_Rotation,

    SI_Count
};

CDriverConfig::CDriverConfig()
{
    m_resolution = glm::uvec2(640U, 480U);
    m_fps = 60.f;
    m_position = glm::vec3(0.f);
    m_rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
}

CDriverConfig::~CDriverConfig()
{
}

void CDriverConfig::Load()
{
    std::string l_path(g_modulePath);
    l_path.erase(l_path.begin() + l_path.rfind('\\'), l_path.end());
    l_path.append("\\..\\..\\resources\\settings.xml");

    pugi::xml_document *l_document = new pugi::xml_document();
    if(l_document->load_file(l_path.c_str()))
    {
        const pugi::xml_node l_root = l_document->child("settings");
        if(l_root)
        {
            for(pugi::xml_node l_node = l_root.child("setting"); l_node; l_node = l_node.next_sibling("setting"))
            {
                const pugi::xml_attribute l_attribName = l_node.attribute("name");
                const pugi::xml_attribute l_attribValue = l_node.attribute("value");
                if(l_attribName && l_attribValue)
                {
                    switch(ReadEnumVector(l_attribName.as_string(), g_settingsNames))
                    {
                        case SI_Resolution:
                        {
                            std::stringstream l_stream(l_attribValue.as_string("640 480"));
                            l_stream >> m_resolution.x >> m_resolution.y;
                        } break;
                        case SI_FramesPerSecond:
                            m_fps = l_attribValue.as_float(60.f);
                            break;
                        case SI_Position:
                        {
                            std::stringstream l_stream(l_attribValue.as_string("0.0 0.0 0.0"));
                            l_stream >> m_position.x >> m_position.y >> m_position.z;
                        } break;
                        case SI_Rotation:
                        {
                            std::stringstream l_stream(l_attribValue.as_string("0.0 0.0 0.0 1.0"));
                            l_stream >> m_rotation.x >> m_rotation.y >> m_rotation.z >> m_rotation.w;
                        } break;
                    }
                }
            }
        }
    }
    delete l_document;
}

void CDriverConfig::Save()
{
    std::string l_path(g_modulePath);
    l_path.erase(l_path.begin() + l_path.rfind('\\'), l_path.end());
    l_path.append("\\..\\..\\resources\\settings.xml");

    pugi::xml_document *l_document = new pugi::xml_document();
    pugi::xml_node l_root = l_document->append_child("settings");
    if(l_root)
    {
        for(size_t i = 0U; i < SI_Count; i++)
        {
            pugi::xml_node l_settingNode = l_root.append_child("setting");
            if(l_settingNode)
            {
                pugi::xml_attribute l_nameAttrib = l_settingNode.append_attribute("name");
                if(l_nameAttrib) l_nameAttrib.set_value(g_settingsNames[i].c_str());
                pugi::xml_attribute l_valueAttrib = l_settingNode.append_attribute("value");
                if(l_valueAttrib)
                {
                    switch(i)
                    {
                        case SI_Resolution:
                        {
                            std::string l_value;
                            for(int j = 0; j < 2; j++)
                            {
                                l_value.append(std::to_string(m_resolution[j]));
                                l_value.push_back(' ');
                            }
                            l_valueAttrib.set_value(l_value.c_str());
                        } break;
                        case SI_FramesPerSecond:
                            l_valueAttrib.set_value(m_fps);
                            break;
                        case SI_Position:
                        {
                            std::string l_value;
                            for(int j = 0; j < 3; j++)
                            {
                                l_value.append(std::to_string(m_position[j]));
                                l_value.push_back(' ');
                            }
                            l_valueAttrib.set_value(l_value.c_str());
                        } break;
                        case SI_Rotation:
                        {
                            std::string l_value;
                            for(int j = 0; j < 4; j++)
                            {
                                l_value.append(std::to_string(m_rotation[j]));
                                l_value.push_back(' ');
                            }
                            l_valueAttrib.set_value(l_value.c_str());
                        } break;
                    }
                }
            }
        }
    }
    l_document->save_file(l_path.c_str());
    delete l_document;
}

const glm::uvec2& CDriverConfig::GetResolution() const
{
    return m_resolution;
}

const float CDriverConfig::GetFPS() const
{
    return m_fps;
}

const glm::vec3& CDriverConfig::GetPosition() const
{
    return m_position;
}

void CDriverConfig::SetPosition(const glm::vec3 &f_pos)
{
    std::memcpy(&m_position, &f_pos, sizeof(glm::vec3));
}
    
const glm::quat& CDriverConfig::GetRotation() const
{
    return m_rotation;
}

void CDriverConfig::SetRotation(const glm::quat &f_rot)
{
    std::memcpy(&m_rotation, &f_rot, sizeof(glm::quat));
}
