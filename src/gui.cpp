#include "gui.h"

GUI::GUI(Display* displayArg, PhysicalWorld* worldArg) {
    display = displayArg;
    world = worldArg;
}

void GUI::update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    displayFPS();
    //displaySaveLoad();
    displaySpeedAnimation();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

float* GUI::getFPS() {
	double now = glfwGetTime();
    double deltaTime = now - prev;
    deltaFrame++;
    if (deltaTime > 0.5) {
        prev = now;
        fps = (float)deltaFrame / deltaTime;
        deltaFrame = 0;
    }
	return &fps;
}

void GUI::displayFPS() {
    Point size = Point({80, 20});
    ImGui::SetNextWindowPos(ImVec2(display->getWidth()-size.x, display->getHeight()-size.y));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin("fps", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetCursorPos(ImVec2(0,0));
    ImGui::InputFloat("fps", getFPS(), 0, 0, "%.1f", ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsNoBlank);
    ImGui::End();
}

void GUI::displaySaveLoad() {
    if (display->cursor_disabled) {
        ImGui::Begin("My name is window", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
		ImGui::Text("Hello world");
		bool check;
		ImGui::Checkbox("CHECK", &check);
		std::cout << "check:" << check << std::endl;
		float slider;
		ImGui::SliderFloat("SLIDER", &slider, 0., 1.);
		std::cout << "slider:" << slider << std::endl;
		//float testcolor[3] = {1.,1.,0.};
		//ImGui::ColorEdit3("COLOR", testcolor);
		ImGui::End();
    }
    else {
    }
}

void GUI::displaySpeedAnimation() {
    Point size = Point({80, 20});
    ImGui::SetNextWindowPos(ImVec2(display->getWidth()-size.x, 0));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin("Animation Speed", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
    //ImGui::SliderFloat("spped", &world->speedAnimation, 0., 1.);
    ImGui::End();
}

GUI::~GUI(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}