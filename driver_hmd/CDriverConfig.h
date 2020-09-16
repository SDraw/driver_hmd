#pragma once

class CDriverConfig final
{
    glm::uvec2 m_resolution;
    float m_fps;
    glm::vec3 m_position;
    glm::quat m_rotation;
public:
    CDriverConfig();
    ~CDriverConfig();

    void Load();
    void Save();

    const glm::uvec2& GetResolution() const;
    const float GetFPS() const;

    const glm::vec3& GetPosition() const;
    void SetPosition(const glm::vec3 &f_pos);
    
    const glm::quat& GetRotation() const;
    void SetRotation(const glm::quat &f_rot);
};
