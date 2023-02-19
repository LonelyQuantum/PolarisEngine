#include "runtime/resource/res_type/components/rigid_body.h"

#include "runtime/core/base/macro.h"

namespace Polaris
{
    RigidBodyShape::RigidBodyShape(const RigidBodyShape& res) :
        m_local_transform(res.m_local_transform)
    {
        if (res.m_geometry.getTypeName() == "Box")
        {
            m_type     = RigidBodyShapeType::box;
            m_geometry = POLARIS_REFLECTION_NEW(Box);
            POLARIS_REFLECTION_DEEP_COPY(Box, m_geometry, res.m_geometry);
        }
        else
        {
            LOG_ERROR("Not supported shape type!");
        }
    }

    RigidBodyShape::~RigidBodyShape()
    {
        POLARIS_REFLECTION_DELETE(m_geometry);
    }
} // namespace Polaris