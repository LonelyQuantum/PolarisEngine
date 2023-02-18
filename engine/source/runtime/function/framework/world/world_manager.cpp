#include "runtime/function/framework/world/world_manager.h"

#include "runtime/core/base/macro.h"

#include "runtime/function/framework/level/level.h"

namespace Polaris
{
	WorldManager::~WorldManager() { clear(); }

    void WorldManager::initialize()
    {
        m_is_world_loaded = false;
        m_current_world_url = "";
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