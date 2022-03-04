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
#include "Light.h"

int main()
{
    Window* window = Window::get();
    window->setSize(1280, 720);
    window->setTitle("Run3D");
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
        "Assets/Textures/SkyBoxes/Mountains/right.jpg",
        "Assets/Textures/SkyBoxes/Mountains/left.jpg",
        "Assets/Textures/SkyBoxes/Mountains/top.jpg",
        "Assets/Textures/SkyBoxes/Mountains/bottom.jpg",
        "Assets/Textures/SkyBoxes/Mountains/front.jpg",
        "Assets/Textures/SkyBoxes/Mountains/back.jpg"
    };

    TextureEffects skyBoxEffects;

    skyBoxEffects.wrapping.clampToEdge = R3D_USE;
    skyBoxEffects.filtering.bilinear = R3D_USE;

    CubeMap cubeMap{faces, skyBoxEffects};


    std::array<float, 65> oldVerticesCube = {
        // front
        -0.5, -0.5,  0.5,   1.0, 0.0, 0.0,  0.0f, 0.0f,   // FBL
         0.5, -0.5,  0.5,   0.0, 1.0, 0.0,  1.0f, 0.0f,   // FBR
         0.5,  0.5,  0.5,   0.0, 0.0, 1.0,  1.0f, 1.0f,   // FTR
        -0.5,  0.5,  0.5,   1.0, 1.0, 1.0,  0.0f, 1.0f,   // FTL
        // back      
        -0.5, -0.5, -0.5,   1.0, 0.0, 0.0,  0.0f, 0.0f,   // BBL
         0.5, -0.5, -0.5,   0.0, 1.0, 0.0,  1.0f, 0.0f,   // BBR
         0.5,  0.5, -0.5,   0.0, 0.0, 1.0,  1.0f, 1.0f,   // BTR
        -0.5,  0.5, -0.5,   1.0, 1.0, 1.0,  0.0f, 1.0f,   // BTL
    };


    std::array<float, 288> verticesCube = {
         // BACK

         // Positions      |         Normals           |       Tex Coords
        -0.5f, -0.5f, -0.5f,         0.0f,  0.0f, -1.0f,       0.0f, 0.0f, // 0
         0.5f, -0.5f, -0.5f,         0.0f,  0.0f, -1.0f,       1.0f, 0.0f, // 1
         0.5f,  0.5f, -0.5f,         0.0f,  0.0f, -1.0f,       1.0f, 1.0f, // 2

         0.5f,  0.5f, -0.5f,         0.0f,  0.0f, -1.0f,       1.0f, 1.0f, // 3
        -0.5f,  0.5f, -0.5f,         0.0f,  0.0f, -1.0f,       0.0f, 1.0f, // 4
        -0.5f, -0.5f, -0.5f,         0.0f,  0.0f, -1.0f,       0.0f, 0.0f, // 5

         // FRONT
        -0.5f, -0.5f,  0.5f,         0.0f,  0.0f,  1.0f,       0.0f, 0.0f, // 6
         0.5f, -0.5f,  0.5f,         0.0f,  0.0f,  1.0f,       1.0f, 0.0f, // 7
         0.5f,  0.5f,  0.5f,         0.0f,  0.0f,  1.0f,       1.0f, 1.0f, // 8

         0.5f,  0.5f,  0.5f,         0.0f,  0.0f,  1.0f,       1.0f, 1.0f, // 9
        -0.5f,  0.5f,  0.5f,         0.0f,  0.0f,  1.0f,       0.0f, 1.0f, // 10
        -0.5f, -0.5f,  0.5f,         0.0f,  0.0f,  1.0f,       0.0f, 0.0f, // 11

         // LEFT
        -0.5f,  0.5f,  0.5f,        -1.0f,  0.0f,  0.0f,       1.0f, 1.0f, // 12
        -0.5f,  0.5f, -0.5f,        -1.0f,  0.0f,  0.0f,       1.0f, 0.0f, // 13
        -0.5f, -0.5f, -0.5f,        -1.0f,  0.0f,  0.0f,       0.0f, 0.0f, // 14

        -0.5f, -0.5f, -0.5f,        -1.0f,  0.0f,  0.0f,       0.0f, 0.0f, // 15
        -0.5f, -0.5f,  0.5f,        -1.0f,  0.0f,  0.0f,       0.0f, 1.0f, // 16
        -0.5f,  0.5f,  0.5f,        -1.0f,  0.0f,  0.0f,       1.0f, 1.0f, // 17

         // RIGHT
         0.5f,  0.5f,  0.5f,         1.0f,  0.0f,  0.0f,       1.0f, 1.0f, // 18
         0.5f,  0.5f, -0.5f,         1.0f,  0.0f,  0.0f,       1.0f, 0.0f, // 19
         0.5f, -0.5f, -0.5f,         1.0f,  0.0f,  0.0f,       0.0f, 0.0f, // 20

         0.5f, -0.5f, -0.5f,         1.0f,  0.0f,  0.0f,       0.0f, 0.0f, // 21
         0.5f, -0.5f,  0.5f,         1.0f,  0.0f,  0.0f,       0.0f, 1.0f, // 22
         0.5f,  0.5f,  0.5f,         1.0f,  0.0f,  0.0f,       1.0f, 1.0f, // 23

         // BOTTOM
        -0.5f, -0.5f, -0.5f,         0.0f, -1.0f,  0.0f,       0.0f, 0.0f, // 24
         0.5f, -0.5f, -0.5f,         0.0f, -1.0f,  0.0f,       1.0f, 0.0f, // 25
         0.5f, -0.5f,  0.5f,         0.0f, -1.0f,  0.0f,       1.0f, 1.0f, // 26

         0.5f, -0.5f,  0.5f,         0.0f, -1.0f,  0.0f,       1.0f, 1.0f, // 27
        -0.5f, -0.5f,  0.5f,         0.0f, -1.0f,  0.0f,       0.0f, 1.0f, // 28
        -0.5f, -0.5f, -0.5f,         0.0f, -1.0f,  0.0f,       0.0f, 0.0f, // 29

         // TOP
        -0.5f,  0.5f, -0.5f,         0.0f,  1.0f,  0.0f,       0.0f, 0.0f, // 30
         0.5f,  0.5f, -0.5f,         0.0f,  1.0f,  0.0f,       1.0f, 0.0f, // 31
         0.5f,  0.5f,  0.5f,         0.0f,  1.0f,  0.0f,       1.0f, 1.0f, // 32

         0.5f,  0.5f,  0.5f,         0.0f,  1.0f,  0.0f,       1.0f, 1.0f, // 33
        -0.5f,  0.5f,  0.5f,         0.0f,  1.0f,  0.0f,       0.0f, 1.0f, // 34
        -0.5f,  0.5f, -0.5f,         0.0f,  1.0f,  0.0f,       0.0f, 0.0f, // 35
    };

    std::array<uint32_t, 36> indicesCube = {
        0,  1,  2,
        3,  4,  5,
        6,  7,  8,
        9,  10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35
    };

    std::array<uint32_t, 36> oldIndicesCube = {
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

    Scene scene{camera};



    Mesh3D mesh0{ verticesCube, indicesCube, {5.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 0.0f, {}, shader };
    Mesh3D mesh1{ verticesCube, indicesCube, {-5.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 0.0f, {}, shader };
    Mesh3D texturedMesh{ verticesCube, indicesCube, {-5.0f, -5.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 0.0f, {}, basicTextureShader, basicTexture };
    Mesh3D lightMesh{ verticesCube, indicesCube, {0.0f, 5.0f, -10.0f}, {0.3f, 0.3f, 0.3f}, 0.0f, {}, shader };

    Light light{};
    light.position = lightMesh.getPosition();
    light.colour = {1.0f, 1.0f, 1.0f};
    light.strength = 2.0f;

    Mesh3D skybox{ verticesCube, indicesCube, {}, {50000.0f, 50000.0f, 50000.0f}, 0.0f, {}, skyBoxShader, cubeMap, true };


    Mesh3D deccerCubes{"Assets/Models/deccer_cubes.obj", {0.0f, -16.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 0.0f, {}, shader};
    Mesh3D tree{"Assets/Models/tree.obj", {0.0f, 7.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, 0.0f, {}, shader};
    //Mesh3D backPack{"Assets/Models/backPack.obj", {0.0f, 0.0f, 0.0f}, {0.05f, 0.05f, 0.05f}, -90.0f, {1.0f, 0.0f, 0.0f}, shader};
    Mesh3D iphone{"Assets/Models/iphone.obj", {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, -90.0f, {1.0f, 0.0f, 0.0f}, shader};


    scene.addMesh(mesh0);
    scene.addMesh(mesh1);
//    scene.addMesh(texturedMesh);
    scene.addMesh(tree);
    scene.addMesh(deccerCubes);
    //scene.addMesh(backPack);
    scene.addMesh(iphone);
    scene.addMesh(skybox);
    scene.addMesh(lightMesh);

    scene.addLight(light);
    
    double highestFPS = 0;
    uint16_t FPScounter = 0;


    float constexpr SPEED = 5.0f;
    float constexpr ROTATION_SPEED = 5.0f;
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
            std::string newWindowTitle = "Run3D - FPS: ";
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

        //backPack.rotate(0.5f * dt, {0.0f, 1.0f, 1.0f});

        scene.draw();

        window->doBackEndStuff();
    }

    scene.destroy();

    window->destroy();

    return EXIT_SUCCESS;
}
