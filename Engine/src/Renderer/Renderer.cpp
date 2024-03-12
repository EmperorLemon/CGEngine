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

	std::shared_ptr<OpenGL::GLShader>	   screenShader = nullptr;
	std::shared_ptr<OpenGL::GLTexture>	   screenTexture = nullptr;
	std::shared_ptr<OpenGL::GLBuffer>      screenQuadVertexBuffer = nullptr;
	std::shared_ptr<OpenGL::GLVertexArray> screenQuadVertexArray = nullptr;

	std::shared_ptr<OpenGL::GLShader>      skyboxShader = nullptr;
	std::shared_ptr<OpenGL::GLTexture>	   skyboxTexture = nullptr;
	std::shared_ptr<OpenGL::GLBuffer>	   skyboxVertexBuffer = nullptr;
	std::shared_ptr<OpenGL::GLVertexArray> skyboxVertexArray = nullptr;

	std::shared_ptr<OpenGL::GLBuffer>       uniformBuffer = nullptr;
	std::shared_ptr<OpenGL::GLFramebuffer>  frameBuffer = nullptr;
	std::shared_ptr<OpenGL::GLRenderbuffer> renderBuffer = nullptr;

	Assets::Light light;

	GraphicsAPI Renderer::m_API = GraphicsAPI::CG_NO_API;

	std::vector QUAD_VERTICES =
	{
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	std::vector SKYBOX_VERTICES =
	{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
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

		// Light setup
		{
			light.direction = Math::Vector4(0.0f);
			light.type = Assets::LightType::DIRECTIONAL_LIGHT;
			light.cutOff = Math::Cos(Math::DegToRad(light.cutOff));
			light.outerCutOff = Math::Cos(Math::DegToRad(light.outerCutOff));
		}

		// Default lit shader setup
		{
			std::string vert_src, frag_src;
			IO::ReadFile("Assets/Shaders/lit.vert", vert_src);
			IO::ReadFile("Assets/Shaders/lit.frag", frag_src);

			ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
			shader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));
		}

		// Screen quad setup
		{
			std::string vert_src, frag_src;
			IO::ReadFile("Assets/Shaders/screen.vert", vert_src);
			IO::ReadFile("Assets/Shaders/screen.frag", frag_src);

			ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
			screenShader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));

			TextureLayout layout;

			layout.add(TParamName::TEXTURE_MIN_FILTER, TParamValue::NEAREST);
			layout.add(TParamName::TEXTURE_MAG_FILTER, TParamValue::NEAREST);

			screenTexture = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::RGB8, width, height, layout);

			Assets::Mesh mesh;

			mesh.vertices.swap(QUAD_VERTICES);

			mesh.layout.add(0, 2, DataType::FLOAT, 0)
				.add(1, 2, DataType::FLOAT, 2 * sizeof(float));

			mesh.layout.SetStride(4 * sizeof(float));

			const BufferInfo vertexBuffer = { mesh.vertices.size() * sizeof(float), mesh.vertices.size(), 0 };

			screenQuadVertexBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::VERTEX_BUFFER, vertexBuffer.size, mesh.vertices.data());
			screenQuadVertexArray = std::make_shared<OpenGL::GLVertexArray>(screenQuadVertexBuffer->GetID(), vertexBuffer, nullptr, mesh.layout);
		}

		// Skybox setup
		{
			std::string vert_src, frag_src;
			IO::ReadFile("Assets/Shaders/skybox.vert", vert_src);
			IO::ReadFile("Assets/Shaders/skybox.frag", frag_src);

			ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
			skyboxShader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));

			std::vector<std::string_view> faces = { "Assets/Textures/Skybox/right.jpg", "Assets/Textures/Skybox/left.jpg", "Assets/Textures/Skybox/top.jpg",
													"Assets/Textures/Skybox/bottom.jpg","Assets/Textures/Skybox/front.jpg","Assets/Textures/Skybox/back.jpg" };
			std::vector<Image> bitmaps;
			bitmaps.reserve(faces.size());

			for (auto face : faces)
			{
				Image image = {};
				IO::LoadImageFile(face, image.width, image.height, image.channels, image.pixels);
				bitmaps.emplace_back(std::move(image));
			}

			TextureLayout layout;

			layout.add(TParamName::TEXTURE_WRAP_S, TParamValue::CLAMP_TO_EDGE);
			layout.add(TParamName::TEXTURE_WRAP_T, TParamValue::CLAMP_TO_EDGE);
			layout.add(TParamName::TEXTURE_WRAP_R, TParamValue::CLAMP_TO_EDGE);

			layout.add(TParamName::TEXTURE_MIN_FILTER, TParamValue::LINEAR);
			layout.add(TParamName::TEXTURE_MAG_FILTER, TParamValue::LINEAR);

			skyboxTexture = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_CUBE_MAP, 1, TextureFormat::RGBA, PixelFormat::RGBA8, layout, std::move(bitmaps));

			Assets::Mesh mesh;

			mesh.vertices.swap(SKYBOX_VERTICES);

			mesh.layout.add(0, 3, DataType::FLOAT, 0);

			mesh.layout.SetStride(3 * sizeof(float));

			const BufferInfo vertexBuffer = { mesh.vertices.size() * sizeof(float), mesh.vertices.size(), 0 };

			skyboxVertexBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::VERTEX_BUFFER, vertexBuffer.size, mesh.vertices.data());
			skyboxVertexArray = std::make_shared<OpenGL::GLVertexArray>(skyboxVertexBuffer->GetID(), vertexBuffer, nullptr, mesh.layout);
		}

		// Uniform buffer setup
		{
			uniformBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::UNIFORM_BUFFER, 2 * sizeof(Math::Mat4) + sizeof(Assets::Light) + sizeof(Math::Vector3), nullptr);
			uniformBuffer->BindBufferRange(0, 0, 2 * sizeof(Math::Mat4));
			uniformBuffer->BindBufferRange(1, 2 * sizeof(Math::Mat4), sizeof(Assets::Light));
			uniformBuffer->BindBufferRange(2, 2 * sizeof(Math::Mat4) + sizeof(Assets::Light), sizeof(Math::Vector3));
		}

		// Framebuffer setup
		{
			frameBuffer = std::make_shared<OpenGL::GLFramebuffer>(BufferTarget::FRAMEBUFFER);
			frameBuffer->AttachTexture(FramebufferTextureAttachment::COLOR_ATTACHMENT, screenTexture->GetID());

			renderBuffer = std::make_shared<OpenGL::GLRenderbuffer>(BufferTarget::RENDERBUFFER, FramebufferTextureAttachmentFormat::DEPTH24_STENCIL8, width, height);
			frameBuffer->AttachRenderbuffer(FramebufferTextureAttachment::DEPTH_STENCIL_ATTACHMENT, renderBuffer->GetID());

			if (!frameBuffer->CheckStatus()) CG_ERROR("Error: Framebuffer is incomplete!");
		}
	}

	void Renderer::PreRender(const Camera& camera)
	{
		m_backend->Enable(APICapability::DEPTH_TEST);
		m_backend->Enable(APICapability::FRAMEBUFFER_SRGB);

		const auto& PROJECTION_MATRIX = Math::Perspective(camera.fov, GetAspectRatio(m_window.width, m_window.height), camera.near, camera.far);
		const auto& VIEW_MATRIX       = Math::View(camera.position, camera.direction, camera.up);

		uniformBuffer->SetSubData(0, sizeof(Math::Mat4), Math::value_ptr(PROJECTION_MATRIX));
		uniformBuffer->SetSubData(1 * sizeof(Math::Mat4), sizeof(Math::Mat4), Math::value_ptr(VIEW_MATRIX));
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

		{
			skyboxShader->Use();

			constexpr int skybox_texture_sampler = 0;
			skyboxShader->BindUniform("skyboxSampler", OpenGL::UniformType::INT, &skybox_texture_sampler);
			skyboxShader->BindUniform("SKYBOX_VIEW_MATRIX", OpenGL::UniformType::MAT4, Math::value_ptr(Math::Mat4(Math::Mat3(VIEW_MATRIX))));

			skyboxShader->Disable();
		}
	}

	void Renderer::FirstPass() const
	{
		frameBuffer->Bind();

		constexpr float clearColor[4] = { 0.2f, 0.45f, 0.55f, 1.0f };
		constexpr float clearDepth = 1.0f;
		frameBuffer->Clear(BufferType::COLOR, 0, clearColor);
		frameBuffer->Clear(BufferType::DEPTH, 0, &clearDepth);

		m_backend->Enable(APICapability::DEPTH_TEST);
	}

	void Renderer::SecondPass() const
	{
		// Draw skybox after drawn objects, but before second pass.
		{
			m_backend->SetDepthFunc(DepthFunc::LEQUAL);
			skyboxShader->Use();
			{
				skyboxTexture->Bind(0);
				m_backend->Draw(skyboxVertexArray.get());
			}
			m_backend->SetDepthFunc(DepthFunc::LESS);
		}

		// Second pass
		{
			frameBuffer->Unbind();

			m_backend->Clear(BufferMask::COLOR_BUFFER_BIT);
			m_backend->Disable(APICapability::DEPTH_TEST);

			screenShader->Use();
			{
				screenTexture->Bind(0);
				m_backend->Draw(screenQuadVertexArray.get());
			}
		}
	}


	void Renderer::Render(const Time& time)
	{
		// First pass
		{
			shader->Use();

			for (const auto& object : objects)
			{
				auto model = Math::Mat4(1.0f);

				model = Math::Translate(model, Math::Vector3(0.0f, 0.0f, 4.0f));

				model = Math::Rotate(model, 0.0f, Math::X_AXIS);
				model = Math::Rotate(model, -static_cast<float>(time.currTime), Math::Y_AXIS);
				model = Math::Rotate(model, 0.0f, Math::Z_AXIS);

				shader->BindUniform("MODEL_MATRIX",  OpenGL::UniformType::MAT4, Math::value_ptr(model));
				shader->BindUniform("NORMAL_MATRIX", OpenGL::UniformType::MAT3, Math::value_ptr(Math::Mat3(Math::Transpose(Math::Inverse(model)))));

				int unit = 0;

				for (const auto& texture : object->textures)
				{
					texture.Bind(unit);
					unit++;
				}

				//for (const auto& material : object->GetMaterials())
				//{
				//	//shader->BindUniform("material.albedo", OpenGL::UniformType::VEC4, Math::value_ptr(material.albedo));

				//	//shader->BindUniform("material.metallicFactor", OpenGL::UniformType::FLOAT, &material.metallicFactor);
				//	//shader->BindUniform("material.roughnessFactor", OpenGL::UniformType::FLOAT, &material.roughnessFactor);
				//}

				for (const auto& vertexArray : object->vertexArrays)
				{
					m_backend->Draw(&vertexArray);
				}
			}
		}
	}

	void Renderer::PostRender()
	{
		SwapBuffers(m_window);
	}

	void Renderer::ResizeViewport(const int32_t width, const int32_t height) const
	{
		m_backend->ResizeViewport(0, 0, width, height);
	}

	void Renderer::ResizeProjection(const Camera& camera) const
	{
		uniformBuffer->SetSubData(0, sizeof(Math::Mat4), Math::value_ptr(Math::Perspective(camera.fov, GetAspectRatio(m_window.width, m_window.height), camera.near, camera.far)));
		//uniformBuffer->SetSubData(1 * sizeof(Math::Mat4), sizeof(Math::Mat4), Math::value_ptr(Math::View(camera.position, camera.direction, camera.up)));
	}

	GraphicsAPI Renderer::GetAPI()
	{
		return m_API;
	}
}
