// 2022 Tygo Boons

#include "Renderer.h"

using namespace Tmpl8;

namespace Renderer
{
	RendererData Renderer::s_Data = RendererData();

	void Renderer::Init()
	{
		// Initialize GLAD
		gladLoadGLLoader((GLADloadproc)(sf::Context::getFunction));

		Game::Get()->GetWindow()->popGLStates();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDepthFunc(GL_LESS);
	}

	void Renderer::Renderer::BeginScene(const RefPtr<Entity::Transform> transform, const RefPtr<Entity::Camera> camera)
	{
		Game::Get()->GetWindow()->popGLStates();
		// Update view projection matrix.
		s_Data.m_ViewProjectionMatrix = camera->GetProjectionMatrix() * transform->GetTransformMatrix();
	}

	void Renderer::EndScene()
	{
		Game::Get()->GetWindow()->pushGLStates();
	}

	void Renderer::SubmitMesh(const RefPtr<Mesh> mesh, const RefPtr<Material> material)
	{
		// Bind vertex array with the data vertex layout.
		mesh->GetVertexData()->GetVertexArray()->Bind();

		// Bind shader and upload camera view projection matrix from framedata.
		material->Shader->Bind();
		material->Shader->UploadUniformMat4("uViewProjection", s_Data.m_ViewProjectionMatrix);

		sf::Texture::bind(material->Texture, sf::Texture::Normalized);

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