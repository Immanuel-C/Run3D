#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "Mesh3D.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

class Model {
public:
    Model(const std::string& path, Shader shader) { loadMesh(path, shader); }

    std::vector<Mesh3D>& getMeshses() { return m_meshes; }

private:
    std::vector<Mesh3D> m_meshes;

    void loadMesh(const std::string& path, Shader shader);
    void processNode(aiNode* node, const aiScene* scene, Shader shader);
    Mesh3D processMesh(aiMesh* mesh, const aiScene* scene, Shader shader);
};