#include "particleSystem.h"

#include "random.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem()
{
	m_ParticlePool.resize(1000); 
	// we resize the particle pool to be 1000 particles
	// resize will instantiate 1000 particles
}

void ParticleSystem::OnUpdate(glm::mat4 projection, glm::mat4 view, float ts)// TODO time in seconds //Time::getTime() ts
{
	for (auto& particle : m_ParticlePool) // got hrought the entire particle pool
	{
		if (!particle.Active) 
			continue; // can be replaced by break, because if one particle is not active, it is likely that all the further ones will also be
			// inactive BUT this requires us to know the starting particle index

		if (particle.LifeRemaining <= 0.0f) // particle is dead
		{
			particle.Active = false;
			// printf("I AM DEAD"); 
			continue;
		}

		// printf("I AM NOT DEAD"); 

		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts; // increment the position by the celocity 
		particle.Rotation += 0.01f * ts;
	}
}

// void ParticleSystem::OnRender(GLCore::Utils::OrthographicCamera& camera)
void ParticleSystem::OnRender(glm::mat4 projection, glm::mat4 view)
{
	if (!m_QuadVA) // if not means we have not created the vertex array yet
	{
		// float vertices[] = {
		// 	 -0.5f, -0.5f, 0.0f,
		// 	  0.5f, -0.5f, 0.0f,
		// 	  0.5f,  0.5f, 0.0f,
		// 	 -0.5f,  0.5f, 0.0f
		// };

		float vertices[] = {
			 -1.f, -1.f, 0.0f,
			  1.f, -1.f, 0.0f,
			  1.f,  1.f, 0.0f,
			 -1.f,  1.f, 0.0f
		};

		glCreateVertexArrays(1, &m_QuadVA);
		glBindVertexArray(m_QuadVA);

		GLuint quadVB, quadIB;
		glCreateBuffers(1, &quadVB);
		glBindBuffer(GL_ARRAY_BUFFER, quadVB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexArrayAttrib(quadVB, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		uint32_t indices[] = {
			0, 1, 2, 2, 3, 0
		};

		glCreateBuffers(1, &quadIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// m_ParticleShader = std::unique_ptr<GLCore::Utils::Shader>(GLCore::Utils::Shader::FromGLSLTextFiles("../../Shaders/particle.vs", "assets/shader.glsl.frag"));
		m_ParticleShader.CreateFromFiles("../../src/Shaders/particle.vs", "../../src/Shaders/particle.fs"); 
		m_ParticleShaderViewProj = m_ParticleShader.GetParticleViewProjectionLocation(); 
		m_ParticleShaderTransform = m_ParticleShader.GetTransformLocation(); 
		m_ParticleShaderColor = m_ParticleShader.GetParticleColorLocation(); 

		// m_ParticleShaderViewProj = glGetUniformLocation(m_ParticleShader->GetRendererID(), "u_ViewProj");
		// m_ParticleShaderTransform = glGetUniformLocation(m_ParticleShader->GetRendererID(), "u_Transform");
		// m_ParticleShaderColor = glGetUniformLocation(m_ParticleShader->GetRendererID(), "u_Color");
	}

	// glUseProgram(m_ParticleShader->GetRendererID());
	m_ParticleShader.UseShader(); 
	glUniformMatrix4fv(m_ParticleShaderViewProj, 1, GL_FALSE, glm::value_ptr(projection * view)); //TODO

	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		// Fade away particles
		float life = particle.LifeRemaining / particle.LifeTime;
		printf("the life is : %f\n", life); 
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life); // linear interpollation : based on the remaining life what the color should be
		color.a = color.a * life; // life is a val. between 0 and 1
		// make it so it will fade out by the end of its life (alpha = 0)
		printf("the color is : %f\n", color); 

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
		// TODO maybe add the fact that the particle gets smaller with its remaining life ?
		printf("the size is : %f\n", size); 

		
		// Render
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
		glUniformMatrix4fv(m_ParticleShaderTransform, 1, GL_FALSE, glm::value_ptr(transform));
		glUniform4fv(m_ParticleShaderColor, 1, glm::value_ptr(color));
		glBindVertexArray(m_QuadVA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	/**
	 * @brief Create a particle, as the particle already exist (with the pool), it just need to re-use
	 *  It querries the actual poolindex : "where are we in this particle pool ? What is the next index we can use?"
	 */
	Particle& particle = m_ParticlePool[m_PoolIndex]; // retrieve a reference to a particle
	particle.Active = true; // make the particle active so that it will get rendered and updated
	particle.Position = particleProps.Position; 
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);
	// VelocityVariation : controls how big we can vary a velocity, we start with the base velocity and then we add on to that a number 
	// that will be between -0.5 and 0.5, we can simulate gravity.

	// Color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	particle.LifeTime = particleProps.LifeTime; // lifetime is th total lifetime
	particle.LifeRemaining = particleProps.LifeTime; // keep track of life remaining to know the percentage of life that past
	// we need that so that we can figure out any kind of looping.So anything that needs to be interpollated over life such as the color, 
	// alpha, the size, needs to know the liferemaining and xhat percentage of life is remaining.
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	// we decrememnt pool index and we take the modulo by the size so that it wraps around 
	// the purpose of this is to set up the poolindex for the next particles, when we emit the next particles, we know the index
	// in memory where we can write particle data to.

}