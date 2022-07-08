#pragma once

#include <memory>

namespace Polaris
{
    class PolarisEngine;

	class PolarisEditor
	{
    public:
        PolarisEditor();
        virtual ~PolarisEditor();

        void initialize(PolarisEngine* engine_runtime);
        void clear();

        void run();

    protected:
        PolarisEngine* m_engine_runtime{ nullptr };
	};

} //namespace Polaris