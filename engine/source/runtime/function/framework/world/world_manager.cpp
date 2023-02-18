#include "runtime/function/framework/world/world_manager.h"

#include "runtime/core/base/macro.h"

#include "runtime/resource/asset_manager/asset_manager.h"
#include "runtime/resource/config_manager/config_manager.h"

#include "runtime/function/framework/level/level.h"

namespace Polaris
{
	WorldManager::~WorldManager() { clear(); }

    void WorldManager::initialize()
    {
        m_is_world_loaded = false;
        m_current_world_url = g_runtime_global_context.m_config_manager->getDefaultWorldUrl();
    }
    
    void WorldManager::clear()
    {
        // unload all loaded levels
        for (auto level_pair : m_loaded_levels)
        {
            level_pair.second->unload();
        }
        m_loaded_levels.clear();

        m_current_active_level.reset();

        // clear world
        m_current_world_resource.reset();
        m_current_world_url.clear();
        m_is_world_loaded = false;
    }

    void WorldManager::tick(float delta_time)
    {
        if (!m_is_world_loaded)
        {
            loadWorld(m_current_world_url);
        }

        // tick the active level
        std::shared_ptr<Level> active_level = m_current_active_level.lock();
        if (active_level)
        {
            active_level->tick(delta_time);
        }
    }

    bool WorldManager::loadWorld(const std::string& world_url)
    {
        LOG_INFO("loading world: {}", world_url);
        WorldRes   world_res;
        const bool is_world_load_success = g_runtime_global_context.m_asset_manager->loadAsset(world_url, world_res);
        if (!is_world_load_success)
        {
            return false;
        }

        m_current_world_resource = std::make_shared<WorldRes>(world_res);

        const bool is_level_load_success = loadLevel(world_res.m_default_level_url);
        if (!is_level_load_success)
        {
            return false;
        }

        // set the default level to be active level
        auto iter = m_loaded_levels.find(world_res.m_default_level_url);
        ASSERT(iter != m_loaded_levels.end());

        m_current_active_level = iter->second;

        m_is_world_loaded = true;

        LOG_INFO("world load succeed!");
        return true;
    }

    bool WorldManager::loadLevel(const std::string& level_url)
    {
        std::shared_ptr<Level> level = std::make_shared<Level>();
        // set current level temporary
        m_current_active_level = level;

        const bool is_level_load_success = level->load(level_url);
        if (is_level_load_success == false)
        {
            return false;
        }

        m_loaded_levels.emplace(level_url, level);

        return true;
    }

} // namespace Polaris