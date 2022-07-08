#pragma once

namespace Polaris
{
    struct EditorGlobalContextInitInfo
    {
        class WindowSystem* window_system;
        class PolarisEngine* engine_runtime;
    };

    class EditorGlobalContext
    {
    public:
        class PolarisEngine* m_engine_runtime{ nullptr };
        class WindowSystem* m_window_system{ nullptr };

    public:
        void initialize(const EditorGlobalContextInitInfo& init_info);
        void clear();
    };

    extern EditorGlobalContext g_editor_global_context;
} // namespace Polaris