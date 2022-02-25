#include "R3DTime.h"

double Time::m_currTime = glfwGetTime();
double Time::m_prevTime = 0.0;
double Time::m_dt = 0.0;

double Time::getDeltaTime()
{
    m_currTime = glfwGetTime();
    m_dt = m_currTime - m_prevTime;
    m_prevTime = m_currTime;

    return m_dt;
}

int Time::getFPS(double dt)
{
    return (int)(1.0 / dt);
}
