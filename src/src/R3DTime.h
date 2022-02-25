#pragma once

#include <GLFW/glfw3.h>

class Time
{
public:
	//Only call this at the top of the main loop
	static double getDeltaTime();
	static int getFPS(double dt);
private:
	static double m_currTime;
	static double m_prevTime;
	static double m_dt;
};

