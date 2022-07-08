#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <functional>
#include <vector>

namespace Polaris
{
    struct WindowCreateInfo
    {
        int         width{ 1280 };
        int         height{ 720 };
        const char* title{ "Polaris" };
        bool        is_fullscreen{ false };
    };

    class WindowSystem
    {
    public:
        WindowSystem() = default;
        ~WindowSystem();

        void initialize(WindowCreateInfo create_info);
        void pollEvents() const;
        bool shouldClose() const;
        GLFWwindow* getWindow() const;

    private:
        GLFWwindow* m_window{ nullptr };
        int         m_width{ 0 };
        int         m_height{ 0 };
    };
} // namespace Polaris