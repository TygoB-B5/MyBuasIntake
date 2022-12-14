// 2022 Tygo Boons

#include "mypch.h"
#include "Renderer.h"

namespace Renderer
{
	RendererData Renderer::s_Data = RendererData();

	void Renderer::Init()
	{
		// Initialize GLAD
		gladLoadGLLoader((GLADloadproc)(sf::Context::getFunction));

		// Set opengl states for rendering.

		// Enable depth testing so object don't render in front of each other.
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Enable backface culling.
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		
		// Enable transparency.
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::Renderer::BeginScene(const RefPtr<Entity::Camera> camera)
	{
		// Update view projection matrix.
		if (camera)
		{
			s_Data.m_ViewProjectionMatrix = camera->GetViewProjectionMatrix();
		}
	}

	void Renderer::EndScene()
	{
		// Reset opengl buffer, texture and shader states.
		// Required for sfml rendering.
		// https://en.sfml-dev.org/forums/index.php?topic=24978.0 

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Renderer::SubmitMesh(const RefPtr<Entity::Transform>& transform, const RefPtr<Mesh> mesh, const RefPtr<Material> material)
	{
		// Bind vertex array with the data vertex layout.
		mesh->GetVertexData()->GetVertexArray()->Bind();

		// Bind shader and upload required matrices.
		material->Shader->Bind();
		material->Shader->UploadUniformMat4("uModel", transform->GetTransformMatrix());
		material->Shader->UploadUniformMat4("uViewProjection", s_Data.m_ViewProjectionMatrix);

		// Bind used texture.
		material->Texture->Bind();

		material->Shader->UploadUniformTextureSlot("uTexture", 0);

		// Get element count.
		uint32_t count = mesh->GetVertexData()->GetIndexBuffer()->GetElementCount();

		// Gl call to draw elements.
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::Clear(const glm::vec3& color)
	{
		// Clear depth and stencil buffer.
		glClearColor(color.r, color.g, color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}