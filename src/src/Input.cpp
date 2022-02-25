#include "Input.h"


bool Input::m_keys[];
bool Input::m_mouseButtons[];
glm::vec2 Input::m_mousePos;
GLFWwindow* Input::m_window;
bool Input::m_justPressedKeys[];

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Input* input = (Input*)glfwGetWindowUserPointer(window);
    input->m_mouseButtons[button] = action != GLFW_RELEASE;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Input* input = (Input*)glfwGetWindowUserPointer(window);
    input->m_keys[key] = action != GLFW_RELEASE;
    input->m_justPressedKeys[key] = action == GLFW_PRESS;
}

Input::Input()
{
    int len = sizeof(m_keys) / sizeof(m_keys[0]);
    for (int i = 0; i < len; i++)
    {
        m_keys[i] = false;
    }

    m_mousePos = glm::vec2();
    m_window = Window::get()->getGLFWwindow();

    glfwSetWindowUserPointer(m_window, this);

    glfwSetKeyCallback(m_window, key_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
}

glm::vec2 Input::getMousePos()
{
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);
    m_mousePos.x = (float)(xpos);
    m_mousePos.y = (float)(ypos);
    return m_mousePos;
}


bool Input::isKeyPressed(uint16_t key)
{
    if (key >= 1024)
        return false;

    return m_keys[key];
}

bool Input::isKeyJustPressed(uint16_t key)
{
    if (key >= 1024)
        return false;

    return m_justPressedKeys[key];
}

bool Input::isMouseButtonPressed(uint16_t button)
{
    if (button >= 32)
        return false;

    return m_mouseButtons[button];
}

