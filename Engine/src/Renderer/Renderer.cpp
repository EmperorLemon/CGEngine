#include "Renderer.h"

#include "Core/Logger.hpp"

#include "Context.h"
#include "Camera.h"

#include "Renderer/Assets/Model.h"
#include "IO/FileSystem.h"

#include "Math/Math.h"
#include "Math/Transform.h"
#include "Math/Vector3.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLDrawObject.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace CGEngine
{
	void SetupRenderScene();

	std::vector<std::shared_ptr<OpenGL::GLDrawObject>> objects;

	std::shared_ptr<OpenGL::GLShader> shader = nullptr;
	std::shared_ptr<OpenGL::GLBuffer> uniformBuffer = nullptr;

	Assets::Light light;

	GraphicsAPI Renderer::m_API = GraphicsAPI::CG_NO_API;

	Renderer::Renderer(const RendererCreateInfo& rendererInfo) : m_window(rendererInfo.window)
	{
		DeviceProperties deviceProperties = {};
		const ContextCreateInfo context = { m_window, deviceProperties };

		CreateContext(context);
		m_backend = std::make_shared<OpenGL::OpenGLAPI>();

		SetupRenderScene();
	}

	void SetupRenderScene()
	{
		Assets::Model model;
		IO::LoadModelFile("Assets/Models/Cube/cube.gltf", model);

		objects.emplace_back(std::make_shared<OpenGL::GLDrawObject>(std::move(model)));
		objects.at(0)->position = Math::Vector3(0.0f, 0.0f, 4.0f);

		light.position = Math::Vector3(2.0f, 0.0f, 10.0f);

		std::string vert_src, frag_src;
		IO::ReadFile("Assets/Shaders/lit.vert", vert_src);
		IO::ReadFile("Assets/Shaders/lit.frag", frag_src);

		ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
		shader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));

		uniformBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::UNIFORM_BUFFER, 2 * sizeof(Math::Mat4) + sizeof(Assets::Light), nullptr);
		uniformBuffer->BindBufferRange(0, 0, 2 * sizeof(Math::Mat4));
		uniformBuffer->BindBufferRange(1, 2 * sizeof(Math::Mat4), sizeof(Assets::Light));
	}

	void Renderer::PreRender(const Camera& camera)
	{
		m_backend->Enable(static_cast<uint32_t>(APICapability::CG_DEPTH_TEST));

		uniformBuffer->SetSubData(0, sizeof(Math::Mat4), Math::value_ptr(camera.projection));
		uniformBuffer->SetSubData(sizeof(Math::Mat4), sizeof(Math::Mat4), Math::value_ptr(camera.view));
		uniformBuffer->SetSubData(2 * sizeof(Math::Mat4), sizeof(Assets::Light), &light);

		shader->Use();

		constexpr int albedo_texture_sampler	= 0;
		constexpr int normal_texture_sampler	= 1;
		constexpr int occlusion_texture_sampler = 2;

		shader->BindUniform("material.baseAlbedoSampler",	  OpenGL::UniformType::INT, &albedo_texture_sampler);
		shader->BindUniform("material.baseNormalSampler",	  OpenGL::UniformType::INT, &normal_texture_sampler);
		shader->BindUniform("material.baseOcclusionSampler", OpenGL::UniformType::INT, &occlusion_texture_sampler);

		shader->Disable();
	}

	void Renderer::Render()
	{
		float RGBA[4] = { 0.2f, 0.45f, 0.55f, 1.0f };

		m_backend->Clear(static_cast<uint32_t>(ClearMask::CG_COLOR_DEPTH_BUFFER_BIT));
		m_backend->ClearColor(RGBA);

		shader->Use();
		for (const auto& object : objects)
		{
			auto model = Math::Mat4(1.0f);

			model = Math::Translate(model, object->position);

			model = Math::Rotate(model, 0.0f, Math::X_AXIS);
			model = Math::Rotate(model, 45.0f, Math::Y_AXIS);
			model = Math::Rotate(model, 0.0f, Math::Z_AXIS);

			shader->BindUniform("MODEL_MATRIX", OpenGL::UniformType::MAT4, Math::value_ptr(model));
			shader->BindUniform("NORMAL_MATRIX", OpenGL::UniformType::MAT3, Math::value_ptr(Math::Mat3(Math::Transpose(Math::Inverse(model)))));

			int unit = 0;

			for (const auto& texture : object->GetTextures())
			{
				texture.Bind(unit);
				unit++;
			}

			for (const auto& material : object->GetMaterials())
			{
				shader->BindUniform("material.albedo", OpenGL::UniformType::VEC4, Math::value_ptr(material.albedo));

				shader->BindUniform("material.metallicFactor", OpenGL::UniformType::FLOAT, &material.metallicFactor);
				shader->BindUniform("material.roughnessFactor", OpenGL::UniformType::FLOAT, &material.roughnessFactor);
			}

			m_backend->Draw(object.get());
		}

		SwapBuffers(m_window);
	}

	void Renderer::PostRender()
	{
		shader->Disable();
	}

	GraphicsAPI Renderer::GetAPI()
	{
		return m_API;
	}
}
