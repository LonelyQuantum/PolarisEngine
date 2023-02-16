#pragma once

#include <memory>
#include <string>

namespace Polaris
{
    class LogSystem;
    class WorldManager;
    class WindowSystem;
    class RenderSystem;

    struct EngineInitParams;

    /// Manage the lifetime and creation/destruction order of all global system
    class RuntimeGlobalContext
    {
    public:
        // create all global systems and initialize these systems
        void startSystems(const std::string& config_file_path);
        // destroy all global systems
        void shutdownSystems();

    public:
        std::shared_ptr<LogSystem>      m_logger_system;
        std::shared_ptr<WorldManager>   m_world_manager;
        std::shared_ptr<WindowSystem>   m_window_system;
        std::shared_ptr<RenderSystem>   m_render_system;

 
    };

    extern RuntimeGlobalContext g_runtime_global_context;
} // namespace Polaris