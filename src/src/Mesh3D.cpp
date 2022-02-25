#include "Mesh3D.h"

VAO Mesh3D::m_vao;

Mesh3D::Mesh3D(std::string modelPath, glm::vec3 position, glm::vec3 scale, float rotationDeg, glm::vec3 rotationAxis, Shader shader)
	: m_position{ position },
	  m_rotation{ rotationDeg },
	  m_rotationAxis{ rotationAxis },
	  m_scale{ scale },
	  m_shader{ shader }
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath,
		aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE 
		|| !scene->mRootNode) {
		std::cerr << "Failed to load file: " << modelPath << "\nAssimp Error: " << importer.GetErrorString();
		return;
	}

	m_directory = modelPath.substr(0, modelPath.find_last_of('/'));


	processNode(scene->mRootNode, scene);
}

glm::vec3 Mesh3D::getPosition()
{
	return m_position;
}

float Mesh3D::getRotation()
{
	return m_rotation;
}

glm::vec3 Mesh3D::getScale()
{
	return m_scale;
}

glm::mat4 Mesh3D::getTransformMat()
{
	return m_transformMat;
}

VBO Mesh3D::getVBO()
{
	return m_vbo;
}

EBO Mesh3D::getEBO()
{
	return m_ebo;
}

Shader Mesh3D::getShader()
{
	return m_shader;
}

VAO Mesh3D::getVAO()
{
	return m_vao;
}

void Mesh3D::setPosition(glm::vec3 position)
{
	m_position = position;
	recalculateMatrices();
}

void Mesh3D::setRotation(float rotation, glm::vec3 rotationAxis)
{
	m_rotation = rotation;
	m_rotationAxis = rotationAxis;
	recalculateMatrices();
}

void Mesh3D::setScale(glm::vec3 scale)
{
	m_scale = scale;
	recalculateMatrices();
}


void Mesh3D::destroy()
{
	m_shader.destroy();
	m_vbo.destroy();
	m_ebo.destroy();
}

void Mesh3D::createMesh(std::vector<_Vertex> vertices, std::vector<uint8_t> indices)
{
	std::vector<float> convertedVertices(vertices.size());

	// Convert vertices into a way that the vbo class can understand
	for (_Vertex& vertex : vertices) {
		convertedVertices.push_back(vertex.position.x);
		convertedVertices.push_back(vertex.position.y);
		convertedVertices.push_back(vertex.position.z);
		
		convertedVertices.push_back(1.0f);
		convertedVertices.push_back(1.0f);
		convertedVertices.push_back(1.0f);
		
		convertedVertices.push_back(vertex.texCoords.x);
		convertedVertices.push_back(vertex.texCoords.y);
	}

	m_meshes.emplace_back(new Mesh3D(convertedVertices, indices, m_position, m_scale, m_rotation, m_rotationAxis, m_shader));
}
void Mesh3D::recalculateMatrices()
{
	if (m_rotationAxis != glm::vec3{ 0.0f, 0.0f, 0.0f }) {
		m_transformMat = glm::scale(glm::mat4{ 1.0f }, m_scale) *
			glm::rotate(glm::mat4{ 1.0f }, m_rotation, m_rotationAxis) *
			glm::translate(glm::mat4{ 1.0f }, m_position);
	}
	else {
		m_transformMat = glm::scale(glm::mat4{ 1.0f }, m_scale) *
			glm::translate(glm::mat4{ 1.0f }, m_position);
	}
}

void Mesh3D::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any) 
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}
	// then do the same for each of its children
	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void Mesh3D::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<_Vertex> vertices;
	std::vector<uint8_t> indices;

	// Process vertices
	for (int i = 0; i < mesh->mNumVertices; i++) {
		_Vertex vertex;

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		/*
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		*/

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertex.position = vector;

		vertices.push_back(vertex);
	}

	// process indices
	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}


	// process textures
	if (mesh->mMaterialIndex >= 0) {

	}


	createMesh(vertices, indices);
}
