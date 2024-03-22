#include "Renderer.h"

#include <array>

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

	std::shared_ptr<OpenGL::GLShader> defaultShader	     = nullptr;
	std::shared_ptr<OpenGL::GLShader> depthShader        = nullptr;
	std::shared_ptr<OpenGL::GLShader> HDRShader          = nullptr;
	std::shared_ptr<OpenGL::GLShader> emissiveShader     = nullptr;
	std::shared_ptr<OpenGL::GLShader> blurShader         = nullptr;
	std::shared_ptr<OpenGL::GLShader> skyboxShader       = nullptr;

	std::shared_ptr<OpenGL::GLTexture> depthTexture      = nullptr;
	std::shared_ptr<OpenGL::GLTexture> HDRColorTexture   = nullptr;
	std::shared_ptr<OpenGL::GLTexture> HDRBloomTexture   = nullptr;
	std::array<std::shared_ptr<OpenGL::GLTexture>, 2> blurTextures;
	std::shared_ptr<OpenGL::GLTexture> skyboxTexture     = nullptr;
	std::shared_ptr<OpenGL::GLTexture> sceneTexture   = nullptr;

	std::shared_ptr<OpenGL::GLFramebuffer>  depthFramebuffer     = nullptr;
	std::shared_ptr<OpenGL::GLFramebuffer>  HDRColorFramebuffer  = nullptr;
	std::array<std::shared_ptr<OpenGL::GLFramebuffer>, 2> blurFramebuffers;
	std::shared_ptr<OpenGL::GLRenderbuffer> HDRColorRenderbuffer = nullptr;

	std::shared_ptr<OpenGL::GLBuffer> screenQuadVertexBuffer    = nullptr;
	std::shared_ptr<OpenGL::GLBuffer> skyboxVertexBuffer        = nullptr;

	std::shared_ptr<OpenGL::GLVertexArray>  quadVertexArray     = nullptr;
	std::shared_ptr<OpenGL::GLVertexArray>  skyboxVertexArray   = nullptr;

	std::shared_ptr<OpenGL::GLBuffer>		shaderStorageBuffer = nullptr;
	std::shared_ptr<OpenGL::GLBuffer>       cameraUniformBuffer = nullptr;
	std::shared_ptr<OpenGL::GLBuffer>		lightUniformBuffer  = nullptr;
	std::shared_ptr<OpenGL::GLBuffer>		shadowUniformBuffer = nullptr;

	GraphicsAPI Renderer::m_API = GraphicsAPI::CG_NO_API;

	constexpr float CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	constexpr float CLEAR_DEPTH    = 1.0f;

	constexpr uint32_t MAX_NUM_LIGHTS = 8;
	constexpr uint32_t MAX_NUM_INSTANCES = 1000;

	constexpr uint32_t SHADOW_WIDTH  = 4096;
	constexpr uint32_t SHADOW_HEIGHT = 4096;

	constexpr uint32_t BLUR_PASSES = 10;

	uint32_t NUM_LIGHTS = 1;

	const Math::Mat4 LIGHT_PROJECTION = Math::Orthographic(-25.0f, 25.0f, -25.0f, 25.0f, 0.5f, 15.0f);

	TextureLayout GLOBAL_TEXTURE_LAYOUT;

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
		GLOBAL_TEXTURE_LAYOUT.add(TParamName::TEXTURE_MIN_FILTER, TParamValue::LINEAR_MIPMAP_LINEAR);
		GLOBAL_TEXTURE_LAYOUT.add(TParamName::TEXTURE_MAG_FILTER, TParamValue::LINEAR);
		GLOBAL_TEXTURE_LAYOUT.add(TParamName::TEXTURE_WRAP_S, TParamValue::CLAMP_TO_EDGE);
		GLOBAL_TEXTURE_LAYOUT.add(TParamName::TEXTURE_WRAP_T, TParamValue::CLAMP_TO_EDGE);

		// Default lit shader setup
		{
			std::string vert_src, frag_src;
			IO::ReadFile("Assets/Shaders/instance.vert", vert_src);
			IO::ReadFile("Assets/Shaders/instance.frag", frag_src);

			ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
			defaultShader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));
		}

		// Shadow map setup
		{
			std::string vert_src, frag_src;
			IO::ReadFile("Assets/Shaders/depth.vert", vert_src);
			IO::ReadFile("Assets/Shaders/depth.frag", frag_src);

			ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
			depthShader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));

			TextureLayout layout;

			layout.add(TParamName::TEXTURE_MIN_FILTER, TParamValue::NEAREST);
			layout.add(TParamName::TEXTURE_MAG_FILTER, TParamValue::NEAREST);
			layout.add(TParamName::TEXTURE_WRAP_S, TParamValue::CLAMP_TO_BORDER);
			layout.add(TParamName::TEXTURE_WRAP_T, TParamValue::CLAMP_TO_BORDER);

			depthTexture = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::DEPTH32F, SHADOW_WIDTH, SHADOW_HEIGHT, layout);

			constexpr float borderColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			depthTexture->AddTextureParameter(TParamName::TEXTURE_BORDER_COLOR, borderColor);

			depthFramebuffer = std::make_shared<OpenGL::GLFramebuffer>();
			depthFramebuffer->AttachTexture(FramebufferTextureAttachment::DEPTH_ATTACHMENT, depthTexture->GetID());
			depthFramebuffer->DrawBuffer();
			depthFramebuffer->ReadBuffer();

			if (!depthFramebuffer->CheckStatus()) CG_ERROR("Error: Framebuffer is incomplete!");
		}

		// HDR color shader setup
		{
			std::string vert_src, frag_src;
			IO::ReadFile("Assets/Shaders/HDR.vert", vert_src);
			IO::ReadFile("Assets/Shaders/HDR.frag", frag_src);

			ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
			HDRShader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));
		}

		// Screen quad setup
		{
			Assets::Mesh mesh;

			mesh.vertices.swap(QUAD_VERTICES);

			mesh.layout.add(0, 2, DataType::FLOAT, 0)
					   .add(1, 2, DataType::FLOAT, 2 * sizeof(float));

			mesh.layout.SetStride(4 * sizeof(float));

			const BufferInfo vertexBuffer = { mesh.vertices.size() * sizeof(float), mesh.vertices.size(), 0 };
			screenQuadVertexBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::VERTEX_BUFFER, vertexBuffer.size, mesh.vertices.data());
			quadVertexArray = std::make_shared<OpenGL::GLVertexArray>(screenQuadVertexBuffer->GetID(), vertexBuffer, nullptr, mesh.layout);
			quadVertexArray->SetDrawType(DrawType::DRAW_ARRAYS);
		}
		
		// HDR + Glow (Bloom) texture setup
		{
			TextureLayout layout;

			layout.add(TParamName::TEXTURE_MIN_FILTER, TParamValue::LINEAR_MIPMAP_LINEAR);
			layout.add(TParamName::TEXTURE_MAG_FILTER, TParamValue::LINEAR);
			layout.add(TParamName::TEXTURE_WRAP_S, TParamValue::CLAMP_TO_EDGE);
			layout.add(TParamName::TEXTURE_WRAP_T, TParamValue::CLAMP_TO_EDGE);

			HDRColorTexture = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::RGB16F, width, height, layout);
			HDRBloomTexture = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::RGB16F, width, height, layout);
		}

		// Screen framebuffer + renderbuffer setup
		{
			HDRColorFramebuffer = std::make_shared<OpenGL::GLFramebuffer>();
			HDRColorFramebuffer->AttachTexture(FramebufferTextureAttachment::COLOR_ATTACHMENT, HDRColorTexture->GetID());
			HDRColorFramebuffer->AttachTexture(FramebufferTextureAttachment::COLOR_ATTACHMENT, HDRBloomTexture->GetID(), 1);
			HDRColorFramebuffer->DrawBuffers();

			HDRColorRenderbuffer = std::make_shared<OpenGL::GLRenderbuffer>(FramebufferTextureAttachmentFormat::DEPTH24, width, height);
			HDRColorFramebuffer->AttachRenderbuffer(FramebufferTextureAttachment::DEPTH_ATTACHMENT, HDRColorRenderbuffer->GetID());

			if (!HDRColorFramebuffer->CheckStatus()) CG_ERROR("Error: Framebuffer is incomplete!");
		}

		// Emissive shader setup
		{
			std::string vert_src, frag_src;
			IO::ReadFile("Assets/Shaders/emissive.vert", vert_src);
			IO::ReadFile("Assets/Shaders/emissive.frag", frag_src);

			ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
			emissiveShader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));
		}

		// Blur (for Bloom) shader setup
		{
			std::string vert_src, frag_src;
			IO::ReadFile("Assets/Shaders/blur.vert", vert_src);
			IO::ReadFile("Assets/Shaders/blur.frag", frag_src);

			ShaderModule modules[] = { {vert_src.data(), ShaderType::VERTEX} , {frag_src.data(), ShaderType::FRAGMENT} };
			blurShader = std::make_shared<OpenGL::GLShader>(modules, std::size(modules));
		}

		// Blur (for Bloom) framebuffers + textures setup 
		{
			TextureLayout layout;

			layout.add(TParamName::TEXTURE_MIN_FILTER, TParamValue::LINEAR_MIPMAP_LINEAR);
			layout.add(TParamName::TEXTURE_MAG_FILTER, TParamValue::LINEAR);
			layout.add(TParamName::TEXTURE_WRAP_S, TParamValue::CLAMP_TO_EDGE);
			layout.add(TParamName::TEXTURE_WRAP_T, TParamValue::CLAMP_TO_EDGE);

			for (int i = 0; i < 2; ++i)
			{
				blurTextures.at(i) = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::RGB16F, width, height, layout);
				blurFramebuffers.at(i) = std::make_shared<OpenGL::GLFramebuffer>();
				blurFramebuffers.at(i)->AttachTexture(FramebufferTextureAttachment::COLOR_ATTACHMENT, blurTextures.at(i)->GetID());

				if (!blurFramebuffers.at(i)->CheckStatus()) CG_ERROR("Error: Framebuffer is incomplete!");
			}
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


		// HDR Scene texture setup
		{
			TextureLayout layout;

			layout.add(TParamName::TEXTURE_MIN_FILTER, TParamValue::LINEAR_MIPMAP_LINEAR);
			layout.add(TParamName::TEXTURE_MAG_FILTER, TParamValue::LINEAR);
			layout.add(TParamName::TEXTURE_WRAP_S, TParamValue::CLAMP_TO_EDGE);
			layout.add(TParamName::TEXTURE_WRAP_T, TParamValue::CLAMP_TO_EDGE);

			sceneTexture = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::SRGB8_ALPHA8, width, height, layout);
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

			constexpr size_t shadow_buffer_size = sizeof(Math::Mat4);

			shadowUniformBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::UNIFORM_BUFFER, shadow_buffer_size, nullptr);
			shadowUniformBuffer->BindBufferBase(2);

			constexpr size_t light_buffer_size = sizeof(uint32_t) + MAX_NUM_LIGHTS * sizeof(Component::Light);

			lightUniformBuffer = std::make_shared<OpenGL::GLBuffer>(BufferTarget::UNIFORM_BUFFER, light_buffer_size, nullptr);
			lightUniformBuffer->BindBufferBase(3);
		}
	}

	void Renderer::PreRender(Camera& camera)
	{
		// Various capabilities being enabled
		{
			m_backend->Enable(APICapability::CULL_FACE);
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

			const auto& light_view = Math::View(Math::Vec3(0.0f), Math::Vec3(0.0f), Math::Vec3(0.0f, 1.0f, 0.0f));

			shadowUniformBuffer->SetSubData(0, sizeof(Math::Mat4), Math::ToArray(LIGHT_PROJECTION * light_view));

			const std::vector default_lights(MAX_NUM_LIGHTS, Component::Light(Math::Vec4(0.0f, 4.0f, -2.0f, 0.0f)));

			lightUniformBuffer->SetSubData(0, MAX_NUM_LIGHTS * sizeof(Component::Light), default_lights.data());
		}

		// Default lit shader setup
		{
			defaultShader->Use();

			constexpr int albedo_texture_sampler = 0;
			constexpr int normal_texture_sampler = 1;
			constexpr int occlusion_texture_sampler = 2;
			constexpr int shadow_map_sampler = 3;
			defaultShader->BindUniform("baseAlbedoSampler", OpenGL::UniformType::INT, &albedo_texture_sampler);
			defaultShader->BindUniform("baseNormalSampler", OpenGL::UniformType::INT, &normal_texture_sampler);
			//defaultShader->BindUniform("baseOcclusionSampler", OpenGL::UniformType::INT, &occlusion_texture_sampler);
			defaultShader->BindUniform("shadowMapSampler", OpenGL::UniformType::INT, &shadow_map_sampler);

			defaultShader->BindUniform("material.uv_scale",  OpenGL::UniformType::VEC2, Math::ToArray(Math::Vec2(1.0f)));
			defaultShader->BindUniform("material.uv_offset", OpenGL::UniformType::VEC2, Math::ToArray(Math::Vec2(0.0f)));

			defaultShader->Disable();
		}

		// Shadow shader setup
		{
			depthShader->Use();

			depthShader->Disable();
		}

		// Screen quad shader setup
		{
			HDRShader->Use();

			constexpr int HDR_color_texture_sampler = 0;
			constexpr int HDR_bloom_texture_sampler = 1;
			HDRShader->BindUniform("HDRColorSampler", OpenGL::UniformType::INT, &HDR_color_texture_sampler);
			HDRShader->BindUniform("HDRBloomSampler", OpenGL::UniformType::INT, &HDR_bloom_texture_sampler);

			HDRShader->Disable();
		}

		// Blur (for Bloom) shader setup
		{
			blurShader->Use();

			constexpr int image_sampler = 0;
			blurShader->BindUniform("imageSampler", OpenGL::UniformType::INT, &image_sampler);

			blurShader->Disable();
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

	void Renderer::EnableShader(const ShaderEnable enable) const
	{
		switch (enable)
		{
		case ShaderEnable::NONE:
			break;
		case ShaderEnable::DEFAULT_SHADER:
			defaultShader->Use();
			break;
		case ShaderEnable::DEPTH_SHADER:
			depthShader->Use();
			break;
		case ShaderEnable::LIGHT_SHADER:
			emissiveShader->Use();
			break;
		}
	}

	void Renderer::UpdateUniforms(Camera& camera, const Time& time) const
	{
		camera.view = Math::View(camera.position, camera.direction, camera.up);
		cameraUniformBuffer->SetSubData(sizeof(Math::Mat4), sizeof(Math::Mat4), Math::ToArray(camera.view));
		cameraUniformBuffer->SetSubData(2 * sizeof(Math::Mat4), sizeof(Math::Vec4), Math::ToArray(Math::Vec4(camera.position, 0.0f)));
	}

	void Renderer::UpdateInstance(const int32_t offset, const Component::Transform& transform) const
	{
		const auto model = GetModelMatrix(transform.position, transform.rotation, transform.scale);

		shaderStorageBuffer->SetSubData(offset * sizeof(Math::Mat4), sizeof(Math::Mat4), Math::ToArray(model));
	}

	void Renderer::UpdateLight(const int32_t offset, const uint32_t count, const Component::Light& light) const
	{
		NUM_LIGHTS = count;

		lightUniformBuffer->SetSubData(offset * sizeof(Component::Light), sizeof(Component::Light), &light);

		const auto& light_view = Math::View(Math::Vec3(light.direction.x, light.direction.y, light.direction.z), Math::Vec3(0.0f), Math::Vec3(0.0f, 1.0f, 0.0f));
		shadowUniformBuffer->SetSubData(0, sizeof(Math::Mat4), Math::ToArray(LIGHT_PROJECTION * light_view));
	}

	void Renderer::DrawLight(const Component::Transform& transform, const Component::Light& light) const
	{
		emissiveShader->BindUniform("MODEL_MATRIX", OpenGL::UniformType::MAT4, Math::ToArray(GetModelMatrix(transform.position, transform.rotation, Math::Vec3(0.25f))));
		emissiveShader->BindUniform("LIGHT_COLOR", OpenGL::UniformType::VEC4, Math::ToArray(light.diffuseColor));

		m_backend->Draw(skyboxVertexArray.get());
	}

	void Renderer::RenderPrimitive(const Component::DrawObject& primitive) const
	{
		int32_t unit = 0;

		for (const auto& texture : primitive.textures)
		{
			texture->Bind(unit);
			unit++;
		}

		depthTexture->Bind(3);

		for (const auto& vertexArray : primitive.vertexArrays)
			m_backend->Draw(vertexArray.get());
	}

	void Renderer::FirstPass() const
	{
		// Resize viewport to the dimensions of the shadow texture 
		m_backend->ResizeViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

		// 1. Render depth of scene from light perspective to texture
		depthFramebuffer->Bind();
		depthFramebuffer->Clear(BufferType::DEPTH, 0, &CLEAR_DEPTH);
		m_backend->CullFace(CullFace::FRONT);
	}

	void Renderer::SecondPass() const
	{
		m_backend->CullFace(CullFace::BACK);
		// Unbind depth framebuffer back to default framebuffer
		depthFramebuffer->Unbind();

		// Reset viewport back to screen size
		m_backend->ResizeViewport(0, 0, m_window.width, m_window.height);
		m_backend->Clear(BufferMask::COLOR_DEPTH_BUFFER_BIT);

		// 2. Render scene to HDR floating point framebuffer
		HDRColorFramebuffer->Bind();
		HDRColorFramebuffer->Clear(BufferType::COLOR, 0, CLEAR_COLOR);
		HDRColorFramebuffer->Clear(BufferType::COLOR, 1, CLEAR_COLOR);
		HDRColorFramebuffer->Clear(BufferType::DEPTH, 0, &CLEAR_DEPTH);

		defaultShader->BindUniform("NUM_LIGHTS", OpenGL::UniformType::UNSIGNED_INT, &NUM_LIGHTS);
	}

	void Renderer::ThirdPass() const
	{
		// Unbind HDR floating point framebuffer back to default framebuffer
		HDRColorFramebuffer->Unbind();

		// 3. Blur bright fragments with two-pass Gaussian Blur
		bool horizontal = true, first_iteration = true;

		blurShader->Use();
		for (uint32_t i = 0; i < BLUR_PASSES; ++i)
		{
			blurFramebuffers.at(horizontal)->Bind();
			blurShader->BindUniform("horizontal", OpenGL::UniformType::BOOL, &horizontal);
			first_iteration ? HDRBloomTexture->Bind() : blurTextures.at(!horizontal)->Bind();
			m_backend->Draw(quadVertexArray.get());
			horizontal = !horizontal;
			first_iteration = false;
		}

		m_backend->ResetFramebuffer();

		HDRColorFramebuffer->Bind();
		// 4. Render skybox
		{
			m_backend->SetDepthFunc(DepthFunc::LEQUAL);

			skyboxShader->Use();
			skyboxTexture->Bind();
			m_backend->Draw(skyboxVertexArray.get());

			m_backend->SetDepthFunc(DepthFunc::LESS);
		}
		HDRColorFramebuffer->Unbind();

		// 5. Render HDR floating point framebuffer to 2D quad
		m_backend->Clear(BufferMask::COLOR_DEPTH_BUFFER_BIT);
		HDRShader->Use();
		HDRColorTexture->Bind();
		blurTextures.at(!horizontal)->Bind(1);
		m_backend->Draw(quadVertexArray.get());

		sceneTexture->CopySubImage(0, m_window.width, m_window.height);
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
		const auto& resizedHDRColorTexture   = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::RGB16F, width, height, GLOBAL_TEXTURE_LAYOUT);
		const auto& resizedHDRBloomTexture   = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::RGB16F, width, height, GLOBAL_TEXTURE_LAYOUT);
		const std::array resizedBlurTextures = { std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::RGB16F, width, height, GLOBAL_TEXTURE_LAYOUT),
			                                     std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::RGB16F, width, height, GLOBAL_TEXTURE_LAYOUT) };
		const auto& resizedSceneTexture      = std::make_shared<OpenGL::GLTexture>(TextureTarget::TEXTURE_2D, 1, PixelFormat::SRGB8_ALPHA8, width, height, GLOBAL_TEXTURE_LAYOUT);

		{
			HDRColorRenderbuffer->ResizeBuffer(width, height);

			HDRColorFramebuffer->Bind();
			HDRColorFramebuffer->AttachTexture(FramebufferTextureAttachment::COLOR_ATTACHMENT, resizedHDRColorTexture->GetID());
			HDRColorFramebuffer->AttachTexture(FramebufferTextureAttachment::COLOR_ATTACHMENT, resizedHDRBloomTexture->GetID(), 1);
			HDRColorFramebuffer->AttachRenderbuffer(FramebufferTextureAttachment::DEPTH_ATTACHMENT, HDRColorRenderbuffer->GetID());

			if (!HDRColorFramebuffer->CheckStatus()) CG_ERROR("Error: Framebuffer is incomplete!");

			HDRColorTexture = resizedHDRColorTexture;
			HDRBloomTexture = resizedHDRBloomTexture;

			HDRColorFramebuffer->Unbind();
		}

		{
			for (int i = 0; i < 2; ++i)
			{
				blurFramebuffers.at(i)->Bind();
				blurFramebuffers.at(i)->AttachTexture(FramebufferTextureAttachment::COLOR_ATTACHMENT, resizedBlurTextures.at(i)->GetID());

				if (!blurFramebuffers.at(i)->CheckStatus()) CG_ERROR("Error: Framebuffer is incomplete!");

				blurTextures.at(i) = resizedBlurTextures.at(i);
			}

			m_backend->ResetFramebuffer();
		}

		sceneTexture = resizedSceneTexture;
	}

	uint32_t Renderer::GetSceneTextureID() const
	{
		return sceneTexture->GetID();
	}

	uint32_t Renderer::GetDepthTextureID() const
	{
		return depthTexture->GetID();
	}

	uint32_t Renderer::GetGlowTextureID() const
	{
		return HDRBloomTexture->GetID();
	}
}
