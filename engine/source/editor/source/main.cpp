#include <filesystem>
#include <iostream>

#include "runtime/engine.h"

#include "editor/include/editor.h"

int main(int argc, char** argv)
{
	std::filesystem::path executable_path(argv[0]);
	std::filesystem::path config_file_path = executable_path.parent_path() / "PolarisEditor.ini";

	Polaris::PolarisEngine* engine = new Polaris::PolarisEngine();
	engine->startEngine(config_file_path.generic_string());

	Polaris::PolarisEditor* editor = new Polaris::PolarisEditor();
	editor->initialize(engine);

	editor->run();

	editor->clear();
	engine->clear();
	engine->shutdownEngine();
	

	return 0;
}