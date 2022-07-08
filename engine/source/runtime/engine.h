#pragma once

#include <string>
#include <chrono>

namespace Polaris
{
	extern bool g_is_editor_mode;

	class PolarisEngine
	{
		friend class PolarisEditor;

	public:
		void startEngine(const std::string& config_file_path);
		void shutdownEngine();

		void initialize();
		void clear();

		inline bool isQuit() const { return m_is_quit; }
		void run();
		bool tickOneFrame(float delta_time);

		inline int getFPS() const { return m_fps; }

	protected:
		void logicalTick(float delta_time);
		bool rendererTick();

		void calculateFPS(float delta_time);

		/**
		 *  Each frame can only be called once
		 */
		float calculateDeltaTime();

	protected:
		bool m_is_quit{ 0 };

		std::chrono::steady_clock::time_point m_last_tick_time_point{ std::chrono::steady_clock::now() };

		float m_average_duration{ 0.f };
		int   m_frame_count{ 0 };
		int   m_fps{ 0 };
	};

} // namespace Polaris