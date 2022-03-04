#include "Mesh3D.h"


void Mesh3D::create(float* vertices, size_t lenOfVertices, uint32_t* indices, size_t lenOfIndices) {

	if (!m_vao.getID()) {
		m_vao = {};
	}

	m_vao.bind();
	m_shader.bind();

	m_vbo = { vertices, m_lenOfVertices };
	m_ebo = { indices, m_lenOfIndices };

	m_vao.unbind();
	m_shader.unbind();
	m_vbo.unbind();
	m_ebo.unbind();
	
	recalculateMatrices();
}

Mesh3D::Mesh3D(const std::string& modelPath, glm::vec3 position, glm::vec3 scale, float rotationDeg, glm::vec3 rotationAxis, Shader shader, ITexture texture, bool isSkyBox)
		: m_position{ position },
		m_rotation{ rotationDeg },
		m_rotationAxis{ rotationAxis },
		m_isSkyBox { isSkyBox },
		m_texture { texture },
		m_scale{ scale },
		m_shader{ shader },
		m_lenOfIndices { 0 },
		m_doesHaveIndices { false }
{
	std::vector<float> vertices;

	if (!m_vao.getID()) {
		m_vao = {};
	}

	m_vao.bind();
	m_shader.bind();


	Timer timer;
	timer.start();

	// attrib contains the vertex arrays of the file
	tinyobj::attrib_t attrib;
	// shapes contain the info for each separate object in the file
	std::vector<tinyobj::shape_t> shapes;
	// materials contain the info about the material of each shape
	std::vector<tinyobj::material_t> materials;

	// err and warning output from the LoadObj function
	std::string warn;
	std::string err;

    //load the OBJ file
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str(), nullptr, true)) {
		std::cerr << "Failed to load model: " << modelPath;
	}

	if (!warn.empty()) {
		std::cerr << "Model Loading Warning: " << warn << '\n';
	} 
	
	if (!err.empty()) {
		std::cerr << "Model Loading Error: " << err << '\n';
		return;
	}

	uint16_t fv = 3;
	for (auto& shape : shapes) {
		int indexOffset = 0;
		for (int f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
			for (int v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];

				// Vertex positions
				vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
				vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
				vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

				// Vertex normals
				vertices.push_back(attrib.normals[3 * idx.normal_index + 0]);
				vertices.push_back(attrib.normals[3 * idx.normal_index + 1]);
				vertices.push_back(attrib.normals[3 * idx.normal_index + 2]);

				// Vertex texcoords
				vertices.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
				vertices.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
			}
			indexOffset += fv;
		}
	}

	timer.end();

	std::cout << "Loading Model: " << modelPath.substr(modelPath.find_last_of('/') + 1) << " Took " << timer.seconds << "s and " << timer.miliseconds << "ms\n";

	m_lenOfVertices = vertices.size();

	m_vbo = { (std::vector<float>)vertices };

	m_vao.unbind();
	m_shader.unbind();
	m_vbo.unbind();
	
	recalculateMatrices();
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

void Mesh3D::rotate(float rotation, glm::vec3 rotationAxis) {
	m_rotation += rotation;
	m_rotationAxis = rotationAxis;
	recalculateMatrices();
}


void Mesh3D::destroy()
{
	m_shader.destroy();
	m_vbo.destroy();
	m_ebo.destroy();
	m_vao.destroy();
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