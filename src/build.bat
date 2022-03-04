@echo off

set cppFiles=src/*.cpp
set hFiles=-Isrc -IInclude/GLAD/headers -IInclude/GLFW/include -IInclude/GLM -IInclude/OPENAL/include -IInclude/ASSIMP/include
set libFiles=-LInclude/GLFW/lib/ -lglfw3dll -LInclude/OPENAL/lib/ -lOpenAL32 -LInclude/ASSIMP -lassimp

call clang++ -std=c++17 %cppFiles% %hFiles% %libFiles% -o Run3D.exe 
