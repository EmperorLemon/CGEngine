#include "Time.h"

#include <GLFW/glfw3.h>

namespace CGEngine
{
	void Time::Start()
	{
		prevTime = glfwGetTime();
	}

	void Time::Update()
	{
		currTime = glfwGetTime();
		deltaTime = (currTime - prevTime) / TICK_RATE;
		prevTime = currTime;
	}
}