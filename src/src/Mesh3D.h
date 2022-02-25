#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

struct _Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh3D
{
public:
	template<size_t _vertSize, size_t _indicesSize>
	Mesh3D(std::array<float, _vertSize> vertices, std::array<uint8_t, _indicesSize> indices,
		glm::vec3 position, glm::vec3 scale, float rotationDeg, glm::vec3 rotationAxis, Shader shader,  ITexture texture = {}, bool isSkyBox = false)
		: m_position{ position },
		m_rotation{ rotationDeg },
		m_rotationAxis{ rotationAxis },
		m_isSkyBox { isSkyBox },
		m_texture { texture },
		m_scale{ scale },
		m_shader{ shader }
	{
		if (!m_vao) {
			m_vao = {};
		}

		m_vao.bind();
		m_shader.bind();

		m_vbo = { vertices };
		m_ebo = { indices };


		m_lenOfIndices = _indicesSize;

		m_vbo.bind();
		m_ebo.bind();

		m_vao.unbind();
		m_shader.unbind();
		m_vbo.unbind();
		m_ebo.unbind();

		recalculateMatrices();
	}

	Mesh3D(std::vector<float> vertices, std::vector<uint8_t> indices,
		glm::vec3 position, glm::vec3 scale, float rotationDeg, glm::vec3 rotationAxis, Shader shader, ITexture texture = {}, bool isSkyBox = false)
		: m_position{ position },
		m_rotation{ rotationDeg },
		m_rotationAxis{ rotationAxis },
		m_isSkyBox { isSkyBox },
		m_texture { texture },
		m_scale{ scale },
		m_shader{ shader }
	{
		if (!m_vao) {
			m_vao = {};
		}

		m_vao.bind();
		m_shader.bind();

		m_vbo = { vertices };
		m_ebo = { indices };


		m_lenOfIndices = indices.size();

		m_vbo.bind();
		m_ebo.bind();

		m_vao.unbind();
		m_shader.unbind();
		m_vbo.unbind();
		m_ebo.unbind();

		recalculateMatrices();
	}


	Mesh3D() {}


	Mesh3D(std::string modelPath, glm::vec3 position, glm::vec3 scale, float rotationDeg, glm::vec3 rotationAxis, Shader shader);

	static void destroyVAO() { m_vao.destroy(); }

	glm::vec3 getPosition();
	float getRotation();
	glm::vec3 getScale();
	glm::mat4 getTransformMat();
	VBO getVBO();
	EBO getEBO();
	Shader getShader();

	static VAO getVAO();

	void setPosition(glm::vec3 position);
	void setRotation(float rotation, glm::vec3 rotationAxis);
	void setScale(glm::vec3 scale);

	// Linker complaing it can't find function defenition :(
	// thats why is inline

	inline void move(glm::vec3 velocity) { m_position += velocity; recalculateMatrices(); }

	inline size_t getLengthOfIndices() { return m_lenOfIndices; }

	inline std::vector<Mesh3D*> getMeshes() { return m_meshes; };

	inline bool isSkyBox() { return m_isSkyBox; }

	inline ITexture getTexture() { return m_texture; }

	void destroy();

private:
	void createMesh(std::vector<_Vertex> vertices, std::vector<uint8_t> indices /*std::vector<Texture>*/);

	void recalculateMatrices();

	void processNode(aiNode* node, const aiScene* scene);

	void processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<_Vertex> vertices;
	std::vector<uint8_t> indices;
	// TODO: Make texture class
	// std::vector<Texture> textures

	std::string m_directory;

	glm::vec3 m_position{ 0.0f };
	float m_rotation = 0.0f;
	glm::vec3 m_rotationAxis{ 0.0f };
	glm::vec3 m_scale{ 1.0f };

	glm::mat4 m_transformMat{ 1.0f };

	size_t m_lenOfIndices;

	static VAO m_vao;
	Shader m_shader{};
	VBO m_vbo{};
	EBO m_ebo{};

	ITexture m_texture;

	std::vector<Mesh3D*> m_meshes;

	bool m_isSkyBox = false;
};
