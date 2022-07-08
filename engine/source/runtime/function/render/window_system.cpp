#include "runtime/function/render/window_system.h"

#include "runtime/core/base/macro.h"

namespace Polaris
{
	WindowSystem::~WindowSystem()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

    void WindowSystem::initialize(WindowCreateInfo create_info)
    {
        if (!glfwInit())
        {
            LOG_FATAL(__FUNCTION__, "failed to initialize GLFW");
            return;
        }

        m_width = create_info.width;
        m_height = create_info.height;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(create_info.width, create_info.height, create_info.title, nullptr, nullptr);
        if (!m_window)
        {
            LOG_FATAL(__FUNCTION__, "failed to create window");
            glfwTerminate();
            return;
        }

        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    }

    void WindowSystem::pollEvents() const { glfwPollEvents(); }

    bool WindowSystem::shouldClose() const { return glfwWindowShouldClose(m_window); }

    GLFWwindow* WindowSystem::getWindow() const { return m_window; }
}