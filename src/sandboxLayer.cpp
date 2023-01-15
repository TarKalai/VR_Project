#include "sandboxLayer.h"

// using namespace GLCore;
// using namespace GLCore::Utils;

SandboxLayer::SandboxLayer()
	//: m_CameraController(16.0f / 9.0f)
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	// EnableGLDebugging();

	glEnable(GL_BLEND); // enable blending beecause we want the particles to blend with each other
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Init here
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void SandboxLayer::OnDetach()
{
	// Shutdown here
}

// void SandboxLayer::OnEvent(Event& event)
// {
// 	// Events here
// 	// m_CameraController.OnEvent(event); // we make sure that we can resize the window

// 	if (event.GetEventType() == EventType::WindowResize)
// 	{
// 		WindowResizeEvent& e = (WindowResizeEvent&)event;
// 		glViewport(0, 0, e.GetWidth(), e.GetHeight());
// 	}
// }

// void SandboxLayer::OnUpdate(Timestep ts)
void SandboxLayer::OnUpdate(glm::mat4 projection, glm::mat4 view, Display* window, float ts)
{
	
	// m_CameraController.OnUpdate(ts); //TODO

	// Render here

	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClearColor(0,0,0, 1.0f); // clear color 
	glClear(GL_COLOR_BUFFER_BIT);

	// if (GLCore::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT)) //TODO
	// {
	// 	auto [x, y] = Input::GetMousePosition();
	// 	auto width = GLCore::Application::Get().GetWindow().GetWidth();
	// 	auto height = GLCore::Application::Get().GetWindow().GetHeight();

	// 	auto bounds = m_CameraController.GetBounds();
	// 	auto pos = m_CameraController.GetCamera().GetPosition();
	// 	x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
	// 	y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();

	// if  (glfwGetKey(window->getWindow(), GLFW_KEY_Q) == GLFW_PRESS) 
	// {
	// 	for (int i = 0; i < 5; i++){ // emit 5 particles at once
	// 		m_ParticleSystem.Emit(m_Particle);
	// 	}
	// 	printf("I AM GENERATING PARTICLES!!"); 
	// }

	m_Particle.Position = {0,0};

	for (int i = 0; i < 5; i++){ // emit 5 particles at once
			m_ParticleSystem.Emit(m_Particle);
		}

	m_ParticleSystem.OnUpdate(projection, view, ts);
	m_ParticleSystem.OnRender(projection, view);
}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Birth Color", glm::value_ptr(m_Particle.ColorBegin));
	ImGui::ColorEdit4("Death Color", glm::value_ptr(m_Particle.ColorEnd));
	ImGui::DragFloat("Life Time", &m_Particle.LifeTime, 0.1f, 0.0f, 1000.0f);
	ImGui::End();
}