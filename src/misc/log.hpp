#pragma once

#include "../graphics/framebuffer.hpp"

#define LOG(s, ...) graphics::framebuffer::printf("[%s@%s:%d]: " s "\n", __func__, __FILE__, __LINE__, ##__VA_ARGS__)