#include "runtime/function/framework/component/rigidbody/rigidbody_component.h"

#include "runtime/engine.h"

#include "runtime/core/base/macro.h"

#include "runtime/function/framework/object/object.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"

namespace Polaris
{
    void RigidBodyComponent::postLoadResource(std::weak_ptr<GObject> parent_object)
    {
       
    }

    RigidBodyComponent::~RigidBodyComponent()
    {
       
    }

    void RigidBodyComponent::createRigidBody(const Transform& global_transform)
    {
        
    }

    void RigidBodyComponent::removeRigidBody()
    {
       
    }

    void RigidBodyComponent::updateGlobalTransform(const Transform& transform, bool is_scale_dirty)
    {
        
    }

    void RigidBodyComponent::getShapeBoundingBoxes(std::vector<AxisAlignedBox>& out_bounding_boxes) const
    {
        
    }

} // namespace Polaris
