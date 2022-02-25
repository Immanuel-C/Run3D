#include "Scene.h"

#include <GLFW/glfw3.h>

#include "Input.h"

void Scene::draw()
{
	Mesh3D::getVAO().bind();
	for (Mesh3D* mesh : m_meshes) {
		if (mesh->isSkyBox()) {
			glDepthMask(GL_FALSE);
			glDisable(GL_CULL_FACE);
			mesh->getShader().bind();
			mesh->getVBO().bind();
			mesh->getEBO().bind();
			if (mesh->getTexture().getID()) {
				mesh->getTexture().bind();
			}

			uint32_t projectionLoc = glGetUniformLocation(mesh->getShader().getID(), "uProjection");
			uint32_t transformLoc = glGetUniformLocation(mesh->getShader().getID(), "uTransform");
			uint32_t viewLoc = glGetUniformLocation(mesh->getShader().getID(), "uView");
			
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mesh->getTransformMat()));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_camera.getProjectionMatrix()));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_camera.getViewMatrix()));

			glDrawElements(GL_TRIANGLES, mesh->getLengthOfIndices(), GL_UNSIGNED_BYTE, nullptr);

			mesh->getEBO().unbind();
			mesh->getVBO().unbind();
			mesh->getShader().unbind();
			if (mesh->getTexture().getID()) {
				mesh->getTexture().unbind();
			}
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			glDepthMask(GL_TRUE);
		}
		else {

			for (Mesh3D* subMesh : mesh->getMeshes()) {
				subMesh->getShader().bind();
				subMesh->getVBO().bind();
				subMesh->getEBO().bind();

				glDrawElements(GL_TRIANGLES, subMesh->getLengthOfIndices(), GL_UNSIGNED_BYTE, nullptr);

				subMesh->getEBO().unbind();
				subMesh->getVBO().unbind();
				subMesh->getShader().unbind();
			}

			mesh->getShader().bind();
			mesh->getVBO().bind();
			mesh->getEBO().bind();

			if (mesh->getTexture().getID()) {
				mesh->getTexture().bind();
			}

			uint32_t projectionLoc = glGetUniformLocation(mesh->getShader().getID(), "uProjection");
			uint32_t transformLoc = glGetUniformLocation(mesh->getShader().getID(), "uTransform");
			uint32_t viewLoc = glGetUniformLocation(mesh->getShader().getID(), "uView");

			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mesh->getTransformMat()));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_camera.getProjectionMatrix()));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_camera.getViewMatrix()));

			glDrawElements(GL_TRIANGLES, mesh->getLengthOfIndices(), GL_UNSIGNED_BYTE, nullptr);

			mesh->getEBO().unbind();
			mesh->getVBO().unbind();
			mesh->getShader().unbind();
			if (mesh->getTexture().getID()) {
				mesh->getTexture().unbind();
			}
		}
	}
	Mesh3D::getVAO().unbind();
}

void Scene::destroy()
{
	Mesh3D::destroyVAO();
	for (Mesh3D* mesh : m_meshes) {
		mesh->destroy();
	}

	for (Mesh3D* h_mesh : m_heapMeshes) {
		delete h_mesh;
	}

}
