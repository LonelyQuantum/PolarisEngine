#pragma once

#include "runtime/resource/res_type/common/world.h"

#include <filesystem>
#include <string>

namespace Polaris
{
	class Level;
	class PhysicsScene;

	/// Manage all game worlds, it should be support multiple worlds, including game world and editor world.
	/// Currently, the implement just supports one active world and one active level
	class WorldManager
	{
	public:
		virtual ~WorldManager();

		void initialize();
		void clear();

		void tick(float delta_time);

	private:
		bool						m_is_world_loaded{ false };
		std::string					m_current_world_url;
		std::shared_ptr<WorldRes>	m_current_world_resource;
	};
} // namespace Polaris