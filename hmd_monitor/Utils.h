#pragma once

void ConvertMatrix(const vr::HmdMatrix34_t &f_matVR, glm::mat4 &f_mat);
void AppendToString(std::string &f_string, const glm::vec3 &f_vec);
void AppendToString(std::string &f_string, const glm::quat &f_quat);
