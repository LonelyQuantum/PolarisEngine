#include "runtime/function/character/character.h"

namespace Polaris
{
    Character::Character(std::shared_ptr<GObject> character_object) { setObject(character_object); }

    GObjectID Character::getObjectID() const
    {
        if (m_character_object)
        {
            return m_character_object->getID();
        }

        return k_invalid_gobject_id;
    }

    void Character::setObject(std::shared_ptr<GObject> gobject)
    {
        m_character_object = gobject;
    }
} // namespace Polaris