#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <future>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "TinyObjLoader.h"
#include "Timer.h"

class Mesh3D
{
public:
	template<size_t _vertSize, size_t _indicesSize>
	Mesh3D(std::array<float, _vertSize> vertices, std::array<uint32_t, _indicesSize> indices,
		glm::vec3 position, glm::vec3 scale, float rotationDeg, glm::vec3 rotationAxis, Shader shader, ITexture texture = {}, bool isSkyBox = false)
		: m_position{ position },
		m_rotation{ rotationDeg },
		m_rotationAxis{ rotationAxis },
		m_isSkyBox { isSkyBox },
		m_texture { texture },
		m_scale{ scale },
		m_shader{ shader },
		m_lenOfIndices { indices.size() },
		m_lenOfVertices { vertices.size() },
		m_doesHaveIndices { true }
	{
		create(vertices.data(), vertices.size(), indices.data(), indices.size());
	}

	Mesh3D(std::vector<float> vertices, std::vector<uint32_t> indices,
		glm::vec3 position, glm::vec3 scale, float rotationDeg, glm::vec3 rotationAxis, Shader shader, ITexture texture = {}, bool isSkyBox = false)
		: m_position{ position },
		m_rotation{ rotationDeg },
		m_rotationAxis{ rotationAxis },
		m_isSkyBox { isSkyBox },
		m_texture { texture },
		m_scale{ scale },
		m_shader{ shader },
		m_lenOfIndices { indices.size() },
		m_lenOfVertices { vertices.size() },
		m_doesHaveIndices{ true }
	{
		create(vertices.data(), vertices.size(), indices.data(), indices.size());
	}


	Mesh3D() {}


	Mesh3D(const std::string& modelPath, glm::vec3 position, glm::vec3 scale, float rotationDeg, glm::vec3 rotationAxis, Shader shader, ITexture texture = {}, bool isSkyBox = false);

	glm::vec3 getPosition();
	float getRotation();
	glm::vec3 getScale();
	glm::mat4 getTransformMat();
	VBO getVBO();
	EBO getEBO();
	Shader getShader();

	VAO getVAO();

	void setPosition(glm::vec3 position);
	void setRotation(float rotation, glm::vec3 rotationAxis);
	void setScale(glm::vec3 scale);

	void rotate(float rotation, glm::vec3 rotationAxis);

	// Linker complaing it can't find function defenition :(
	// thats why its inline

	inline void move(glm::vec3 velocity) { m_position += velocity; recalculateMatrices(); }

	inline size_t getLengthOfIndices() { return m_lenOfIndices; }
	inline size_t getLengthOfVertices() { return m_lenOfVertices; }

	inline bool isSkyBox() noexcept { return m_isSkyBox; }

	inline ITexture getTexture() { return m_texture; }

	inline bool doesHaveIndices() { return m_doesHaveIndices; }

	void destroy();

private:
	void recalculateMatrices();

	void create(float* vertices, size_t lenOfVertices, uint32_t* indices, size_t lenOfIndices);
	void createWithModel(std::string modelPath, std::vector<float>& vertices);

	glm::vec3 m_position{ 0.0f };
	float m_rotation = 0.0f;
	glm::vec3 m_rotationAxis{ 0.0f };
	glm::vec3 m_scale{ 1.0f };

	glm::mat4 m_transformMat{ 1.0f };

	size_t m_lenOfIndices;
	size_t m_lenOfVertices;

	VAO m_vao;
	Shader m_shader{};
	VBO m_vbo{};
	EBO m_ebo{};

	ITexture m_texture;

	bool m_isSkyBox = false;
	bool m_doesHaveIndices = true;

	std::vector<std::future<void>> m_futures;
};