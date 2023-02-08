#pragma once

#include <memory>
#include <Window.h>
#include "ChartEmulator.h"


extern std::unique_ptr<ap::Window> gWindow;
extern ChartEmulator* gChart;

namespace Program {
	bool Init();
	void Run();
}
