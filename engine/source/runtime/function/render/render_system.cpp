#include "runtime/function/render/render_system.h"

#include "runtime/function/render/rhi.h"
#include "runtime/function/render/rhi/vulkan/vulkan_rhi.h"




namespace Polaris
{
	RenderSystem::~RenderSystem() {}

	void RenderSystem::initialize(RenderSystemInitInfo init_info)
	{
		// render context initialize
		RHIInitInfo rhi_init_info;
		rhi_init_info.window_system = init_info.window_system;

		m_rhi = std::make_shared<VulkanRHI>();
		m_rhi->initialize(rhi_init_info);
	}

	void RenderSystem::tick()
	{
		// prepare render command context
		m_rhi->tick();
	}
}