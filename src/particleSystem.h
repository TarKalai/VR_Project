#pragma once

// #include <GLCore.h>
// #include <GLCoreUtils.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //use type_ptr to pass the values to the shaders
#include "glm/gtx/string_cast.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "utils.h"
#include "shader.h"
#include "camera.h"

struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void OnUpdate(glm::mat4 projection, glm::mat4 view, float ts= Time::getTime()); // will be called every frame.
	// void OnRender(GLCore::Utils::OrthographicCamera& camera); // TODO get the orthographic of the camera...
	// void Render(Camera camera); 
	void OnRender(glm::mat4 projection, glm::mat4 view); 

	void Emit(const ParticleProps& particleProps); // Emit 1 particle 
    // uses a struct so that we don't have too many arg. in the function head.
private:
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin = 1.0f; 
		float SizeEnd = 0.0;

		float LifeTime = 100.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};
	std::vector<Particle> m_ParticlePool; // we want to create a pull of particles, we don't want to create a new particle every time we need one
    // otherwise it will slow down a lot the code. We want to pre-allocate the pool of particles and then re-use the array that way there is no memory
    // allocation
	uint32_t m_PoolIndex = 999; // index of the next active particle
    // we start at the maximum size, we do not start at 0 we do it in the reverse order 

	GLuint m_QuadVA = 0;
    Camera camera; 
	Shader m_ParticleShader;
	// std::unique_ptr<GLCore::Utils::Shader> m_ParticleShader;
	GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;
};