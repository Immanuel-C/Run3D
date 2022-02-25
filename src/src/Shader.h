#pragma once

#include <cstdint>
#include <glad/glad.h>

#include "Utils.h"


class Shader
{
public:

	Shader(const std::string& vertFilePath, const std::string& fragFilePath);

	Shader() {}

	uint32_t getID();
	void bind();
	void unbind();
	void destroy();

private:
	uint32_t m_ID = 0;
};

