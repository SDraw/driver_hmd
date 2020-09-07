#pragma once

#define WIN32_LEAN_AND_MEAN
#define NO_MIN_MAX

#include <Windows.h>

#ifdef _DEBUG
#include <iostream>
#endif
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "openvr.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
