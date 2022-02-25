@echo off

set cppFiles=src/*.cpp
set hFiles=-Isrc -IInclude/ASSIMP/include -IInclude/GLAD/headers -IInclude/GLFW/include -IInclude/GLM -IInclude/OPENAL/include
set libFiles=-LInclude/ASSIMP/ -lassimp -LInclude/GLFW/lib/ -lglfw3dll -LInclude/OPENAL/libs/ -lOpenAL32


call clang++ -std=c++17 %cppFiles% %hFiles% %libFiles% -o Run3D.exe 
