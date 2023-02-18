#pragma once

#include "runtime/core/math/transform.h"

#include "runtime/function/framework/object/object.h"

#include <vector>

namespace Polaris
{
	class Character
	{
	public:
		Character(std::shared_ptr<GObject> character_object);

		GObjectID getObjectID() const;
		void      setObject(std::shared_ptr<GObject> gobject);

		void tick(float delta_time) {};

	private:
		std::shared_ptr<GObject> m_character_object;
	};
}