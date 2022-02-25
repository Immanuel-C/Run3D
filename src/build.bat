@echo off

set cppFiles=src/*.cpp
set hFiles=-Isrc -IInclude/GLAD/headers/ -IInclude/GLFW/include/ -IInclude/GLM -IInclude/ASSIMP/include
set libFiles=-LInclude/GLFW/lib/ -lglfw3dll.lib -LInclude/ASSIMP/ -lassimp.lib

call clang++ %cppFiles% %hFiles% %libFiles% -o Run3D.exe