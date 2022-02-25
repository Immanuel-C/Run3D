#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


namespace
{
    std::string readFile(const std::string& filePath) {
        std::ifstream file;

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            file.open(filePath);

            std::stringstream fileStream;

            fileStream << file.rdbuf();

            file.close();

            return fileStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cerr << "Error: Failed to load shader file: " << filePath;
            return nullptr;
        }
    }


}

