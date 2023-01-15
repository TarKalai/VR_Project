#pragma once

// #include <GLCore.h>
// #include <GLCoreUtils.h>

#include "particleSystem.h"
// #include "process.h"

class SandboxLayer
// : public GLCore::Layer //TODO
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach(); // override;
	virtual void OnDetach(); // override;
	// virtual void OnEvent(GLCore::Event& event) override;
	// virtual void OnUpdate(GLCore::Timestep ts); // override;
	// OnUpdate(glm::mat4 projection, glm::mat view, float ts)
	virtual void OnUpdate(glm::mat4 projection, glm::mat4 view, Display* window, float ts = Time::getTime()); // override;
	virtual void OnImGuiRender(); // override;
private:
	// GLCore::Utils::OrthographicCameraController m_CameraController; // TODO
	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;
	// Process process; 
	float ts; 
};