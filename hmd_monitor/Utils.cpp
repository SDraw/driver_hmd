#include "stdafx.h"

#include "Utils.h"

void ConvertMatrix(const vr::HmdMatrix34_t &f_matVR, glm::mat4 &f_mat)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 3; j++) f_mat[i][j] = f_matVR.m[j][i];
    }
    for(int i = 0; i < 3; i++) f_mat[i][3] = 0.f;
    f_mat[3][3] = 1.f;
}

void AppendToString(std::string &f_string, const glm::vec3 &f_vec)
{
    for(int i = 0; i < 3; i++)
    {
        f_string.append(std::to_string(f_vec[i]));
        f_string.push_back(' ');
    }
}

void AppendToString(std::string &f_string, const glm::quat &f_quat)
{
    for(int i = 0; i < 4; i++)
    {
        f_string.append(std::to_string(f_quat[i]));
        f_string.push_back(' ');
    }
}
