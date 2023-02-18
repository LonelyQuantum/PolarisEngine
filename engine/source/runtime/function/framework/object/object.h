#pragma once

#include "runtime/function/framework/component/component.h"
#include "runtime/function/framework/object/object_id_allocator.h"

#include "runtime/resource/res_type/common/object.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace Polaris
{
    /// GObject : Game Object base class
    class GObject : public std::enable_shared_from_this<GObject>
    {
        typedef std::unordered_set<std::string> TypeNameSet;

    public:
        GObject(GObjectID id) : m_id{ id } {}
        virtual ~GObject();

        virtual void tick(float delta_time);

        bool load(const ObjectInstanceRes& object_instance_res);
        void save(ObjectInstanceRes& out_object_instance_res);

        GObjectID getID() const { return m_id; }

        void               setName(std::string name) { m_name = name; }
        const std::string& getName() const { return m_name; }

    protected:
        GObjectID   m_id{ k_invalid_gobject_id };
        std::string m_name;
        std::string m_definition_url;

        // we have to use the ReflectionPtr due to that the components need to be reflected 
        // in editor, and it's polymorphism
        std::vector<Reflection::ReflectionPtr<Component>> m_components;
    };
} // namespace Polaris