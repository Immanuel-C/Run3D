#pragma once

#include "Camera.h"
#include "Mesh3D.h"
#include "Light.h"
//#include "Model.h"
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Scene
{
public:

	Scene(Camera3D& camera) : m_camera{camera} {}
	

	inline void addMesh(Mesh3D& mesh) { m_meshes.push_back(&mesh); }
	/// Creating a temperary object and passing it in to this function is not ok in this case as
	/// this is create a dangling pointer which will cause an exception
	/// If you want to create a temperary object and pass it into this function
	/// you will have to call new and use another overload of this function
	inline void addMesh(Mesh3D&& mesh) = delete;
	/// DO NOT PASS IN A STACK ALLOCATED MESH3D OBJECTS INTO THIS FUNCTION! 
	/// This function is used for passing in temperary Mesh3D objects (new Mesh3D(...)). In the destroy function
	/// any mesh passed into this function will have delete called on it, and if it is not a heap allocated object
	/// an exception will be thrown!
	inline void addMesh(Mesh3D* mesh) { m_heapMeshes.push_back(mesh); }

	inline void addLight(Light& light) { m_lights.push_back(&light); }

	//inline void addModel(Model& model) { m_models.push_back(&model); }

	void draw();

	void destroy();
private:
	Camera3D& m_camera;
	std::vector<Mesh3D*> m_meshes;
	std::vector<Mesh3D*> m_heapMeshes;
	std::vector<Light*> m_lights;
	//std::vector<Model*> m_models;
};

