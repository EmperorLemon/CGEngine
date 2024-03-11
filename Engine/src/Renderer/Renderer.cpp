#include "Renderer.h"

#include "Core/Logger.hpp"
#include "Core/Time.h"

#include "Context.h"
#include "Camera.h"

#include "Renderer/Assets/Model.h"
#include "IO/FileSystem.h"

#include "Math/Math.h"
#include "Math/Transform.h"
#include "Math/Vector3.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLDrawObject.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace CGEngine
{
	void SetupRenderScene(int32_t width, int32_t height);

	std::vector<std::shared_ptr<OpenGL::GLDrawObject>> objects;
	std::shared_ptr<OpenGL::GLShader> shader = nullptr;

	std::shared_ptr<OpenGL::GLShader> screenShader = nullptr;
	std::shared_ptr<OpenGL::GLTexture> screenTexture = nullptr;

	std::shared_ptr<OpenGL::GLBuffer> uniformBuffer = nullptr;
	std::shared_ptr<OpenGL::GLFramebuffer> frameBuffer = nullptr;
	std::shared_ptr<OpenGL::GLRenderbuffer> renderBuffer = nullptr;

	Assets::Light light;

	GraphicsAPI Renderer::m_API = GraphicsAPI::CG_NO_API;

	constexpr float QUAD_VERTICES[] =
	{
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	Renderer::Renderer(const RendererCreateInfo& rendererInfo) : m_window(rendererInfo.window)
	{
		DeviceProperties deviceProperties = {};
		const ContextCreateInfo context = { m_window, deviceProperties };

		CreateContext(context);
		m_backend = std::make_shared<OpenGL::OpenGLAPI>();

		SetupRenderScene(m_window.width, m_window.height);
	}

	void SetupRenderScene(const int32_t width, const int32_t height)
	{
		Assets::Model model;
		IO::LoadModelFile("Assets/Models/Cube/cube.gltf", model);

		objects.emplace_back(std::make_shared<OpenGL::GLDrawObject>(std::move(model)));
		objects.at(0)->position = Math::Vector3(0.0f, 0.0f, 4.0f);

		light.direction = Math::Vector4(0.0f);
		light.type = Assets::LightType::DIRECTIONAL_LIGHT;
		light.cutOff = Math::Cos(Math::DegToRad(light.cutOff));
		light.outerCutOff = Math::Cos(Math::DegToRad(light.outerCutOff));

		{
			std::string vert_src, frag_src;
			IO::ReadFile("Assets/Shaders/lit.vert", vert_src);
			IO::ReadFile("Assets/Shaders/lit.frag", frag_src);

			ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
			shader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));
		}

		{
			std::string vert_src, frag_src;
			IO::ReadFile("Assets/Shaders/screen.vert", vert_src);
			IO::ReadFile("Assets/Shaders/screen.frag", frag_src);

			ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
			screenShader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));
		}

		uniformBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::UNIFORM_BUFFER, 2 * sizeof(Math::Mat4) + sizeof(Assets::Light) + sizeof(Math::Vector3), nullptr);
		uniformBuffer->BindBufferRange(0, 0, 2 * sizeof(Math::Mat4));
		uniformBuffer->BindBufferRange(1, 2 * sizeof(Math::Mat4), sizeof(Assets::Light));
		uniformBuffer->BindBufferRange(2, 2 * sizeof(Math::Mat4) + sizeof(Assets::Light), sizeof(Math::Vector3));

		screenTexture = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, PixelFormat::RGB8, width, height);

		frameBuffer = std::make_shared<OpenGL::GLFramebuffer>(BufferTarget::FRAMEBUFFER);
		frameBuffer->AttachTexture(FramebufferTextureAttachment::COLOR_ATTACHMENT, screenTexture->GetID());

		renderBuffer = std::make_shared<OpenGL::GLRenderbuffer>(BufferTarget::RENDERBUFFER, FramebufferTextureAttachmentFormat::DEPTH24_STENCIL8, width, height);
		frameBuffer->AttachRenderbuffer(FramebufferTextureAttachment::DEPTH_STENCIL_ATTACHMENT, renderBuffer->GetID());

		if (!frameBuffer->CheckStatus())
		{
			CG_ERROR("Error: Framebuffer is incomplete!");
		}
	}

	void Renderer::PreRender(const Camera& camera)
	{
		m_backend->Enable(APICapability::DEPTH_TEST);
		m_backend->Enable(APICapability::FRAMEBUFFER_SRGB);

		uniformBuffer->SetSubData(0, sizeof(Math::Mat4), Math::value_ptr(camera.projection));
		uniformBuffer->SetSubData(1 * sizeof(Math::Mat4), sizeof(Math::Mat4), Math::value_ptr(camera.view));
		uniformBuffer->SetSubData(2 * sizeof(Math::Mat4), sizeof(Assets::Light), &light);
		uniformBuffer->SetSubData(2 * sizeof(Math::Mat4) + sizeof(Assets::Light), sizeof(Math::Vector3), &camera.position);

		{
			shader->Use();

			constexpr int albedo_texture_sampler = 0;
			constexpr int normal_texture_sampler = 1;
			constexpr int occlusion_texture_sampler = 2;
			shader->BindUniform("material.baseAlbedoSampler", OpenGL::UniformType::INT, &albedo_texture_sampler);
			//shader->BindUniform("material.baseNormalSampler",	  OpenGL::UniformType::INT, &normal_texture_sampler);
			//shader->BindUniform("material.baseOcclusionSampler", OpenGL::UniformType::INT, &occlusion_texture_sampler);

			shader->Disable();
		}

		{
			screenShader->Use();

			constexpr int screen_texture_sampler = 0;
			screenShader->BindUniform("screenSampler", OpenGL::UniformType::INT, &screen_texture_sampler);

			screenShader->Disable();
		}
	}

	void Renderer::Render(const Time& time)
	{
		frameBuffer->Bind();

		constexpr float clearColor[4] = { 0.2f, 0.45f, 0.55f, 1.0f };
		constexpr float clearDepth    = 1.0f;
		frameBuffer->Clear(BufferType::COLOR, 0, clearColor);
		frameBuffer->Clear(BufferType::DEPTH, 0, &clearDepth);

		shader->Use();

		for (const auto& object : objects)
		{
			auto model = Math::Mat4(1.0f);

			model = Math::Translate(model, object->position);

			model = Math::Rotate(model, 0.0f, Math::X_AXIS);
			model = Math::Rotate(model, -static_cast<float>(time.currTime), Math::Y_AXIS);
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
				//shader->BindUniform("material.albedo", OpenGL::UniformType::VEC4, Math::value_ptr(material.albedo));

				//shader->BindUniform("material.metallicFactor", OpenGL::UniformType::FLOAT, &material.metallicFactor);
				//shader->BindUniform("material.roughnessFactor", OpenGL::UniformType::FLOAT, &material.roughnessFactor);
			}

			m_backend->Draw(object.get());
		}

		frameBuffer->Unbind();
		frameBuffer->Blit(Math::IVector4(0, 0, 800, 800), Math::IVector4(0, 0, 800, 800), BufferMask::COLOR_BUFFER_BIT, TextureFilter::NEAREST);

		SwapBuffers(m_window);
	}

	void Renderer::PostRender()
	{
		shader->Disable();
		screenShader->Disable();
	}

	GraphicsAPI Renderer::GetAPI()
	{
		return m_API;
	}
}
