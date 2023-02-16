#include "runtime/function/framework/world/world_manager.h"

#include "runtime/core/base/macro.h"

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
        // clear world
        m_current_world_resource.reset();
        m_current_world_url.clear();
        m_is_world_loaded = false;
    }

} // namespace Polaris