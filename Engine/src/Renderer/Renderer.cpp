#include "Renderer.h"

#include "Core/Logger.hpp"
#include "Core/Time.h"

#include "Context.h"
#include "Camera.h"

#include "ECS/Component/DrawObject.h"
#include "ECS/Component/Light.h"
#include "ECS/Component/Transform.h"

#include "Renderer/Assets/Mesh.h"

#include "IO/FileSystem.h"

#include "Math/Math.h"
#include "Math/Transform.h"

#include "Platform/OpenGL/OpenGLAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace CGEngine
{
	void SetupRenderScene(int32_t width, int32_t height);

	std::shared_ptr<OpenGL::GLShader> shader = nullptr;

	std::shared_ptr<OpenGL::GLShader>	    screenShader		   = nullptr;
	std::shared_ptr<OpenGL::GLTexture>	    screenTexture		   = nullptr;
	std::shared_ptr<OpenGL::GLBuffer>       screenQuadVertexBuffer = nullptr;
	std::shared_ptr<OpenGL::GLVertexArray>  screenQuadVertexArray  = nullptr;

	std::shared_ptr<OpenGL::GLShader>       skyboxShader       = nullptr;
	std::shared_ptr<OpenGL::GLTexture>	    skyboxTexture	   = nullptr;
	std::shared_ptr<OpenGL::GLBuffer>	    skyboxVertexBuffer = nullptr;
	std::shared_ptr<OpenGL::GLVertexArray>  skyboxVertexArray  = nullptr;

	std::shared_ptr<OpenGL::GLBuffer>		shaderStorageBuffer = nullptr;
	std::shared_ptr<OpenGL::GLBuffer>       cameraUniformBuffer = nullptr;
	std::shared_ptr<OpenGL::GLBuffer>		lightUniformBuffer  = nullptr;
	std::shared_ptr<OpenGL::GLFramebuffer>  frameBuffer			= nullptr;
	std::shared_ptr<OpenGL::GLRenderbuffer> renderBuffer		= nullptr;

	GraphicsAPI Renderer::m_API = GraphicsAPI::CG_NO_API;

	constexpr uint32_t MAX_NUM_LIGHTS = 10;
	constexpr uint32_t MAX_NUM_INSTANCES = 10;

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
		// Default lit shader setup
		{
			std::string vert_src, frag_src;
			IO::ReadFile("Assets/Shaders/instance.vert", vert_src);
			IO::ReadFile("Assets/Shaders/instance.frag", frag_src);

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

			layout.add(TParamName::TEXTURE_MIN_FILTER, TParamValue::LINEAR_MIPMAP_LINEAR);
			layout.add(TParamName::TEXTURE_MAG_FILTER, TParamValue::LINEAR);

			screenTexture = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::SRGB8_ALPHA8, width, height, layout);

			Assets::Mesh mesh;

			mesh.vertices.swap(QUAD_VERTICES);

			mesh.layout.add(0, 2, DataType::FLOAT, 0)
					   .add(1, 2, DataType::FLOAT, 2 * sizeof(float));

			mesh.layout.SetStride(4 * sizeof(float));

			const BufferInfo vertexBuffer = { mesh.vertices.size() * sizeof(float), mesh.vertices.size(), 0 };

			screenQuadVertexBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::VERTEX_BUFFER, vertexBuffer.size, mesh.vertices.data());
			screenQuadVertexArray = std::make_shared<OpenGL::GLVertexArray>(screenQuadVertexBuffer->GetID(), vertexBuffer, nullptr, mesh.layout);
			screenQuadVertexArray->SetDrawType(DrawType::DRAW_ARRAYS);
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

			for (auto& face : faces)
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

			skyboxTexture = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_CUBE_MAP, 1, PixelFormat::SRGB8, layout, std::move(bitmaps));

			Assets::Mesh mesh;

			mesh.vertices.swap(SKYBOX_VERTICES);

			mesh.layout.add(0, 3, DataType::FLOAT, 0);

			mesh.layout.SetStride(3 * sizeof(float));

			const BufferInfo vertexBuffer = { mesh.vertices.size() * sizeof(float), mesh.vertices.size(), 0 };

			skyboxVertexBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::VERTEX_BUFFER, vertexBuffer.size, mesh.vertices.data());
			skyboxVertexArray = std::make_shared<OpenGL::GLVertexArray>(skyboxVertexBuffer->GetID(), vertexBuffer, nullptr, mesh.layout);
			skyboxVertexArray->SetDrawType(DrawType::DRAW_ARRAYS);
		}

		// Shader storage buffers setup
		{
			constexpr size_t instance_buffer_size = MAX_NUM_INSTANCES * sizeof(Math::Mat4);

			shaderStorageBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::SHADER_STORAGE_BUFFER, instance_buffer_size, nullptr);
			shaderStorageBuffer->BindBufferBase(0);
		}

		// Uniform buffers setup
		{
			constexpr size_t camera_buffer_size = 2 * sizeof(Math::Mat4) + sizeof(Math::Vec4);

			cameraUniformBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::UNIFORM_BUFFER, camera_buffer_size, nullptr);
			cameraUniformBuffer->BindBufferBase(1);

			constexpr size_t light_buffer_size = sizeof(uint32_t) + MAX_NUM_LIGHTS * sizeof(Component::Light);

			lightUniformBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::UNIFORM_BUFFER, light_buffer_size, nullptr);
			lightUniformBuffer->BindBufferBase(2);
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

	void Renderer::PreRender(Camera& camera)
	{
		// Various capabilities being enabled
		{
			m_backend->Enable(APICapability::DEPTH_TEST);
			m_backend->Enable(APICapability::FRAMEBUFFER_SRGB);
			m_backend->Enable(APICapability::MULTISAMPLING);
		}

		// Default camera setup
		{
			camera.aspect = GetAspectRatio(m_window.width, m_window.height);
			camera.projection = Math::Perspective(camera.fov, camera.aspect, camera.near, camera.far);
			camera.view = Math::View(camera.position, camera.direction, camera.up);
		}

		// Shader storage buffer setup
		{
			const std::vector default_matrices(MAX_NUM_INSTANCES, Math::Mat4(1.0f));
			shaderStorageBuffer->SetSubData(0, MAX_NUM_INSTANCES * sizeof(Math::Mat4), default_matrices.data());
		}

		// Uniform buffer setup
		{
			cameraUniformBuffer->SetSubData(0, sizeof(Math::Mat4), Math::ToArray(camera.projection));
			cameraUniformBuffer->SetSubData(sizeof(Math::Mat4), sizeof(Math::Mat4), Math::ToArray(camera.view));
			cameraUniformBuffer->SetSubData(2 * sizeof(Math::Mat4), sizeof(Math::Vec4), Math::ToArray(Math::Vec4(camera.position, 0.0)));

			constexpr uint32_t num_lights = 0;
			const std::vector default_lights(MAX_NUM_LIGHTS, Component::Light());

			lightUniformBuffer->SetSubData(0, MAX_NUM_LIGHTS * sizeof(Component::Light), default_lights.data());
			lightUniformBuffer->SetSubData(MAX_NUM_LIGHTS * sizeof(Component::Light), sizeof(uint32_t), &num_lights);
		}

		// Default lit shader setup
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

		// Screen quad shader setup
		{
			screenShader->Use();

			constexpr int screen_texture_sampler = 0;
			screenShader->BindUniform("screenSampler", OpenGL::UniformType::INT, &screen_texture_sampler);

			screenShader->Disable();
		}

		// Skybox shader setup
		{
			skyboxShader->Use();

			constexpr int skybox_texture_sampler = 0;
			skyboxShader->BindUniform("skyboxSampler", OpenGL::UniformType::INT, &skybox_texture_sampler);
			skyboxShader->BindUniform("SKYBOX_VIEW_MATRIX", OpenGL::UniformType::MAT4, Math::ToArray(Math::Mat4(Math::Mat3(camera.view))));

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

	void Renderer::Update(const Camera& camera, const Time& time) const
	{
		shader->Use();

		//uniformBuffer->SetSubData(sizeof(Math::Mat4), sizeof(Math::Mat4), Math::ToArray(camera.view));
	}

	void Renderer::UpdateInstance(const int32_t offset, const Component::Transform& transform) const
	{
		shaderStorageBuffer->SetSubData(offset * sizeof(Math::Mat4), sizeof(Math::Mat4), Math::ToArray(transform.model));
		shader->BindUniform("NORMAL_MATRIX", OpenGL::UniformType::MAT3, Math::ToArray(Math::Mat3(Math::Inverse(transform.model))), true);
	}

	void Renderer::UpdateLight(const int32_t offset, const Component::Light& light) const
	{
		const uint32_t num_lights = offset + 1;

		lightUniformBuffer->SetSubData(offset * sizeof(Component::Light), sizeof(Component::Light), &light);
		lightUniformBuffer->SetSubData(MAX_NUM_LIGHTS * sizeof(Component::Light), sizeof(uint32_t), &num_lights);
	}

	void Renderer::RenderPrimitive(const Component::DrawObject& primitive) const
	{
		int32_t unit = 0;

		for (const auto& texture : primitive.textures)
		{
			texture->Bind(unit);
			unit++;
		}

		for (const auto& vertexArray : primitive.vertexArrays)
			m_backend->Draw(vertexArray.get());
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

	void Renderer::PostRender() const
	{
		SwapBuffers(m_window);
	}

	void Renderer::ResizeProjection(const int32_t width, const int32_t height, Camera& camera) const
	{
		if (width == 0 || height == 0) return;

		camera.aspect	  = GetAspectRatio(width, height);
		camera.projection = Math::Perspective(camera.fov, camera.aspect, camera.near, camera.far);

		cameraUniformBuffer->SetSubData(0, sizeof(Math::Mat4), Math::ToArray(camera.projection));
	}

	void Renderer::ResizeViewport(const int32_t width, const int32_t height) const
	{
		if (width == 0 || height == 0) return;

		ResizeFramebuffer(width, height);
		m_backend->ResizeViewport(0, 0, width, height);
	}

	void Renderer::ResizeFramebuffer(const int32_t width, const int32_t height) const
	{
		TextureLayout layout;

		const auto& resizedScreenTexture = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::SRGB8_ALPHA8, width, height, layout);
		renderBuffer->ResizeBuffer(width, height);

		frameBuffer->Bind();
		frameBuffer->AttachTexture(FramebufferTextureAttachment::COLOR_ATTACHMENT, resizedScreenTexture->GetID());
		frameBuffer->AttachRenderbuffer(FramebufferTextureAttachment::DEPTH_STENCIL_ATTACHMENT, renderBuffer->GetID());

		if (!frameBuffer->CheckStatus()) CG_ERROR("Error: Framebuffer is incomplete!");

		screenTexture = resizedScreenTexture;

		frameBuffer->Unbind();
	}

	uint32_t Renderer::GetViewportID() const
	{
		return screenTexture->GetID();
	}
}
