#pragma once

#include "runtime/function/framework/object/object_id_allocator.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace Polaris
{
	class Character;
	class GObject;
	class ObjectInstanceRes;

	using LevelObjectsMap = std::unordered_map<GObjectID, std::shared_ptr<GObject>>;

	class Level
	{
	public:
		virtual ~Level() {};

		bool load(const std::string& level_res_url);
		void unload();

		bool save();

		void tick(float delta_time);

		GObjectID createObject(const ObjectInstanceRes& object_instance_res);
		void      deleteGObjectByID(GObjectID go_id);

	protected:
		void clear();

		bool        m_is_loaded{ false };
		std::string m_level_res_url;

		// all game objects in this level, key: object id, value: object instance
		LevelObjectsMap m_gobjects;

		std::shared_ptr<Character> m_current_active_character;
	};

} // namespace Polaris