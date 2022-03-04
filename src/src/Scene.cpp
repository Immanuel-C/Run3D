#include "Scene.h"

#include <GLFW/glfw3.h>

#include "Input.h"

void Scene::draw()
{
	for (Mesh3D* mesh : m_meshes) {
		mesh->getVAO().bind();
		if (mesh->isSkyBox()) {
			glDepthMask(GL_FALSE);
			mesh->getShader().bind();
			mesh->getVBO().bind();
			mesh->getEBO().bind();
			if (mesh->getTexture().getID()) 
				mesh->getTexture().bind();

			uint32_t projectionLoc = glGetUniformLocation(mesh->getShader().getID(), "uProjection");
			uint32_t transformLoc = glGetUniformLocation(mesh->getShader().getID(), "uTransform");
			uint32_t viewLoc = glGetUniformLocation(mesh->getShader().getID(), "uView");
			
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mesh->getTransformMat()));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_camera.getProjectionMatrix()));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_camera.getViewMatrix()));

			if (mesh->doesHaveIndices()) 
				glDrawElements(GL_TRIANGLES, mesh->getLengthOfIndices(), GL_UNSIGNED_INT, nullptr);
			else
				glDrawArrays(GL_TRIANGLES, 0, mesh->getLengthOfVertices());
				
			mesh->getEBO().unbind();
			mesh->getVBO().unbind();
			mesh->getShader().unbind();
			if (mesh->getTexture().getID())
				mesh->getTexture().unbind();

			mesh->getVAO().unbind();

			glDepthMask(GL_TRUE);
		}
		else {
			glDepthMask(GL_TRUE);
			mesh->getVAO().bind();
			mesh->getShader().bind();
			mesh->getVBO().bind();

			if (mesh->getTexture().getID()) 
				mesh->getTexture().bind();

			uint32_t projectionLoc = glGetUniformLocation(mesh->getShader().getID(), "uProjection");
			uint32_t transformLoc = glGetUniformLocation(mesh->getShader().getID(), "uTransform");
			uint32_t viewLoc = glGetUniformLocation(mesh->getShader().getID(), "uView");

			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mesh->getTransformMat()));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_camera.getProjectionMatrix()));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_camera.getViewMatrix()));

			glUniform3f(glGetUniformLocation(mesh->getShader().getID(), "lightPos"), m_lights[0]->position.x, m_lights[0]->position.y, m_lights[0]->position.z);
			glUniform3f(glGetUniformLocation(mesh->getShader().getID(), "lightColour"), m_lights[0]->colour.x, m_lights[0]->colour.y, m_lights[0]->colour.z);
			glUniform1f(glGetUniformLocation(mesh->getShader().getID(), "lightStrength"), m_lights[0]->strength);
			glUniform3f(glGetUniformLocation(mesh->getShader().getID(), "camPos"), m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z);

			if (mesh->doesHaveIndices()) {
				mesh->getEBO().bind();
				glDrawElements(GL_TRIANGLES, mesh->getLengthOfIndices(), GL_UNSIGNED_INT, nullptr);
				mesh->getEBO().unbind();
			}
			else
				glDrawArrays(GL_TRIANGLES, 0, mesh->getLengthOfVertices());
				
			mesh->getVBO().unbind();
			mesh->getShader().unbind();
			
			if (mesh->getTexture().getID())
				mesh->getTexture().unbind();

			mesh->getVAO().unbind();
		}
	}
}

void Scene::destroy()
{
	for (Mesh3D* mesh : m_meshes) {
		mesh->destroy();
	}

	for (Mesh3D* h_mesh : m_heapMeshes) {
		delete h_mesh;
	}

}
