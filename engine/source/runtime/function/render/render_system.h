#include <array>
#include <memory>
#include <optional>

namespace Polaris
{
	class WindowSystem;
	class RHI;

	struct RenderSystemInitInfo
	{
		std::shared_ptr<WindowSystem> window_system;
	};

    class RenderSystem
    {
    public:
        RenderSystem() = default;
        ~RenderSystem();

        void initialize(RenderSystemInitInfo init_info);
        void tick();
        void clear();

    private:
        std::shared_ptr<RHI> m_rhi;

    };
} // namespace Polaris