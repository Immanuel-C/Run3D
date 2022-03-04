#include "Model.h"


void Model::loadMesh(const std::string& path, Shader shader) {
    Assimp::Importer importer;
    // aiProcess_Triangulate tells assimp to make sure the model is only made of triangles
    // OpenGL reads images reversed from what other image readers do
    // so we have to flip the UVs with aiProcess_FlipUVs
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp Error: " << importer.GetErrorString() << "\nFile Path: "
        << path;
    }

    processNode(scene->mRootNode, scene, shader);
}

void Model::processNode(aiNode* node, const aiScene* scene, Shader shader) {
    // process all the node's meshes if any
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene, shader));
    }

    // do the same thing for the children nodes
    for (uint32_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, shader);
    }
}

Mesh3D Model::processMesh(aiMesh* mesh, const aiScene* scene, Shader shader) {
    std::vector<float> vertices;
    vertices.reserve(mesh->mNumVertices);
    std::vector<uint32_t> indices;

    for (uint64_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        indices.reserve(face.mNumIndices);
    }

    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);

        // If model has texture coords
        if (mesh->mTextureCoords[0]) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        } else {
            // if not set texCoords to 0
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }

    for (uint64_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0 ) {

    }

    return Mesh3D{vertices, indices, {}, {1.0f, 1.0f, 1.0f}, 0.0f, {}, shader};
}