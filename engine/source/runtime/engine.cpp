#include "runtime/engine.h"

#include "runtime/core/base/macro.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/window_system.h"
#include "runtime/function/render/render_system.h"




namespace Polaris
{
	bool g_is_editor_mode{ false };

	void PolarisEngine::startEngine(const std::string& config_file_path)
	{
		g_runtime_global_context.startSystems(config_file_path);
		LOG_INFO("engine start");
	}

	void PolarisEngine::shutdownEngine()
	{
		LOG_INFO("engine shutdown");

		g_runtime_global_context.shutdownSystems();
	}

	void PolarisEngine::initialize()
	{

	}

	void PolarisEngine::clear()
	{

	}

	void PolarisEngine::run()
	{

	}

	float PolarisEngine::calculateDeltaTime()
	{
		static std::chrono::steady_clock::time_point last_tick_time_point{ std::chrono::steady_clock::now() };
		float delta_time;
		{
			using namespace std::chrono;

			steady_clock::time_point tick_time_point = steady_clock::now();
			duration<float> time_span = duration_cast<duration<float>>(tick_time_point - last_tick_time_point);
			delta_time = time_span.count();

			last_tick_time_point = tick_time_point;
		}
		return delta_time;
	}

	bool PolarisEngine::tickOneFrame(float delta_time)
	{
		logicalTick(delta_time);
		calculateFPS(delta_time);
		rendererTick();

		g_runtime_global_context.m_window_system->pollEvents();

		const bool should_window_close = g_runtime_global_context.m_window_system->shouldClose();;
		return !should_window_close;
	}


	void PolarisEngine::logicalTick(float delta_time)
	{

	}

	bool PolarisEngine::rendererTick()
	{
		g_runtime_global_context.m_render_system->tick();
		return true;
	}

	/*
	* Calculate the FPS using a moving average filter
	*/
	void PolarisEngine::calculateFPS(float delta_time)
	{
		static const float k_fps_alpha = 1.f / 100.0f;
		m_frame_count++;

		if (m_frame_count == 1)
		{
			m_average_duration = delta_time;
		}
		else
		{
			m_average_duration = m_average_duration * (1 - k_fps_alpha) + delta_time * k_fps_alpha;
		}

		m_fps = static_cast<int>(1.f / m_average_duration);
	}

} // namespace Polaris