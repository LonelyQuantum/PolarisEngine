#include <assert.h>

#include "editor//include/editor.h"

#include "runtime/engine.h"
#include "runtime/function/global/global_context.h"

#include "editor/include/editor_global_context.h"



namespace Polaris
{

    PolarisEditor::PolarisEditor() {};

    PolarisEditor::~PolarisEditor() {};

    void PolarisEditor::initialize(PolarisEngine* engine_runtime)
    {
        assert(engine_runtime);

        g_is_editor_mode = true;
        m_engine_runtime = engine_runtime;

        EditorGlobalContextInitInfo init_info = { g_runtime_global_context.m_window_system.get(), 
                                                  engine_runtime };
        g_editor_global_context.initialize(init_info);
    }

    void PolarisEditor::clear() { g_editor_global_context.clear(); }

    void PolarisEditor::run()
    {
        assert(m_engine_runtime);
        bool run_flag = true;
        while (run_flag)
        {
            float delta_time = m_engine_runtime->calculateDeltaTime();
            run_flag = m_engine_runtime->tickOneFrame(delta_time);
        }
    }

} // namespace Polaris