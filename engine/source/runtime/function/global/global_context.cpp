#include "runtime/function/global/global_context.h"

#include "core/log/log_system.h"

#include "runtime/engine.h"

#include "runtime/resource/asset_manager/asset_manager.h"


#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/render/window_system.h"
#include "runtime/function/render/render_system.h"


namespace Polaris
{
    RuntimeGlobalContext g_runtime_global_context;

    void RuntimeGlobalContext::startSystems(const std::string& config_file_path)
    {
        m_logger_system = std::make_shared<LogSystem>();

        m_asset_manager = std::make_shared<AssetManager>();

        m_world_manager = std::make_shared<WorldManager>();
        m_world_manager->initialize();

        m_window_system = std::make_shared<WindowSystem>();
        WindowCreateInfo window_create_info;
        m_window_system->initialize(window_create_info);

        m_render_system = std::make_shared<RenderSystem>();
        RenderSystemInitInfo render_init_info;
        render_init_info.window_system = m_window_system;
        m_render_system->initialize(render_init_info);
    }

    void RuntimeGlobalContext::shutdownSystems()
    {
        m_render_system->clear();
        m_render_system.reset();

        m_window_system.reset();

        m_world_manager->clear();
        m_world_manager.reset();

        m_asset_manager.reset();

        m_logger_system.reset();
    }
} // namespace Polaris