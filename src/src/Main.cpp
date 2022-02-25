#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <string>
#include <iostream>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"
#include "Window.h"
#include "Input.h"
#include "Mesh3D.h"
#include "Scene.h"
#include "Camera.h"
#include "R3DTime.h"
#include "Texture.h"
#include "Sound.h"


int main()
{
    Window* window = Window::get();
    window->setSize(1280, 720);
    window->setTitle("Learning OpenGL 3D");
    window->useVsync(false);
    window->setColor(0.0f, 0.0f, 0.0f, 1.0f);

    Input input{};

    
    Shader shader{ "Assets/Shaders/BasicShader.vert", "Assets/Shaders/BasicShader.frag" };
    Shader basicTextureShader{ "Assets/Shaders/BasicShader.vert", "Assets/Shaders/BasicTextureShader.frag" };
    Shader skyBoxShader{ "Assets/Shaders/BasicShader.vert", "Assets/Shaders/SkyBoxShader.frag" };

    TextureEffects textureEffects;

    textureEffects.filtering.bilinear = R3D_USE;
    textureEffects.wrapping.repeat = R3D_USE;

    Texture basicTexture{"Assets/Textures/wall.jpg", textureEffects};

    std::array<std::string, 6> faces = 
    {
        "Assets/Textures/SkyBoxes/RedSpace/right.png",
        "Assets/Textures/SkyBoxes/RedSpace/left.png",
        "Assets/Textures/SkyBoxes/RedSpace/top.png",
        "Assets/Textures/SkyBoxes/RedSpace/bottom.png",
        "Assets/Textures/SkyBoxes/RedSpace/front.png",
        "Assets/Textures/SkyBoxes/RedSpace/back.png"
    };

    TextureEffects skyBoxEffects;

    skyBoxEffects.wrapping.clampToEdge = R3D_USE;
    skyBoxEffects.filtering.bilinear = R3D_USE;

    CubeMap cubeMap{faces, skyBoxEffects};

    std::array<float, 64> vertices = {
        // front
        -0.5, -0.5,  0.5,   1.0, 0.0, 0.0,  0.0f, 0.0f,   // FBL
         0.5, -0.5,  0.5,   0.0, 1.0, 0.0,  1.0f, 0.0f,   // FBR
         0.5,  0.5,  0.5,   0.0, 0.0, 1.0,  1.0f, 1.0f,   // FTR
        -0.5,  0.5,  0.5,   1.0, 1.0, 1.0,  0.0f, 1.0f,   // FTL
        // back      
        -0.5, -0.5, -0.5,   1.0, 0.0, 0.0,  0.0f, 0.0f,   // BBL
         0.5, -0.5, -0.5,   0.0, 1.0, 0.0,  1.0f, 0.0f,   // BBR
         0.5,  0.5, -0.5,   0.0, 0.0, 1.0,  1.0f, 1.0f,   // BTR
        -0.5,  0.5, -0.5,   1.0, 1.0, 1.0,  0.0f, 1.0f    // BTL
    };

    std::array<uint8_t, 36> indices = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };

    Camera3D camera{90.0f, (float)window->getWidth() / (float)window->getHeight(), 0.1f, 100000.0f};
    camera.setPosition({ 0.0f, 0.0f, -3.0f });

    Scene scene{ camera };

    Mesh3D mesh0{ vertices, indices, {5.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 0.0f, {}, shader };
    Mesh3D mesh1{ vertices, indices, {-5.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 0.0f, {}, shader };
    Mesh3D texturedMesh{ vertices, indices, {}, {1.0f, 1.0f, 1.0f}, 0.0f, {}, basicTextureShader, basicTexture };
    //Mesh3D mesh2{ "Assets/Models/EMP_Handgun.fbx", {}, {10.0f, 10.0f, 10.0f,}, 0.0f, {}, shader };

    Mesh3D skybox{ vertices, indices, {}, {50000.0f, 50000.0f, 50000.0f}, 0.0f, {}, skyBoxShader, cubeMap, true };

    scene.addMesh(mesh0);
    scene.addMesh(mesh1);
    scene.addMesh(texturedMesh);
    //scene.addMesh(&mesh2);

    scene.addMesh(skybox);

    double highestFPS = 0;
    uint32_t FPScounter = 0;

    float constexpr SPEED = 10.f;
    float constexpr ROTATION_SPEED = 5.f;
    float constexpr GRAVITY_SPEED = -0.005f;

    float mouseLastX = Input::getMousePos().x;
    float mouseLastY = Input::getMousePos().y;

    float yaw = 90.0f;
    float pitch = 0.0f;

    bool isCursorHidden = false;

    Sound sound{"Assets/Audio/getout.ogg"};
    sound.play(false);

    while (!window->shouldClose()) {
        window->clear();

        double dt = Time::getDeltaTime();
        int FPS = Time::getFPS(dt);

        FPScounter++;

        if (FPS > highestFPS || FPScounter >= 1000) {
            highestFPS = FPS;
            FPScounter = 0;
        }

        if (dt >= 1.0 / 1000.0) {
            std::string newWindowTitle = "Learning OpenGL 3D - FPS: ";
            newWindowTitle.append(std::to_string(FPS).c_str());
            newWindowTitle.append(" - Highest FPS: " + std::to_string(highestFPS));
            window->setTitle(newWindowTitle);
        }
            

        if (Input::isKeyJustPressed(GLFW_KEY_ESCAPE)) {
            isCursorHidden = !isCursorHidden;
        }

        if (isCursorHidden) {
            glfwSetInputMode(window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            glfwSetInputMode(window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }


        if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
            camera.move({0.0f, (SPEED * (float)dt), 0.0f});
        }
        if (Input::isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
            camera.move({ 0.0f, -(SPEED * (float)dt), 0.0f });
        }
        if (Input::isKeyPressed(GLFW_KEY_W)) {
            camera.move(((camera.getFront() * SPEED) * (float)dt));
        }
        if (Input::isKeyPressed(GLFW_KEY_S)) {
            camera.move(-((camera.getFront() * SPEED) * (float)dt));
        }
        if (Input::isKeyPressed(GLFW_KEY_D)) {
            camera.move(((glm::cross(camera.getFront(), camera.getUp()) * SPEED * 2.0f) * (float)dt));
        }
        if (Input::isKeyPressed(GLFW_KEY_A)) {
            camera.move(-((glm::cross(camera.getFront(), camera.getUp()) * SPEED * 2.0f) * (float)dt));
        }

        if (Input::getMousePos().x != mouseLastX || Input::getMousePos().y != mouseLastY) {
            float xoffset = Input::getMousePos().x - mouseLastX;
            float yoffset = mouseLastY - Input::getMousePos().y;
            mouseLastX = Input::getMousePos().x;
            mouseLastY = Input::getMousePos().y;

            float sensitivity = 0.1f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw += xoffset;
            pitch += yoffset;

            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            camera.setFront(glm::normalize(direction));
        }

        scene.draw();

        window->doBackEndStuff();
    }

    scene.destroy();

    window->destroy();

    return EXIT_SUCCESS;
}
