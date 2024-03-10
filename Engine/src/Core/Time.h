#pragma once

namespace CGEngine
{
	constexpr double TICK_RATE = 1.0 / 60.0;

	struct Time
	{
		double timer = 0.0;

		double prevTime = 0.0;
		double currTime = 0.0;

		double deltaTime = 0.0;

		int frames = 0;
		int ticks  = 0;
	};
}