#include "gui.h"



GUI::GUI(Process* processArg, Display* displayArg, PhysicalWorld* worldArg, Shader* shaderArg, Shader* shadowArg) {
    process = processArg;
    display = displayArg;
    world = worldArg;
    shader = shaderArg;
    shadow = shadowArg;
}

void GUI::update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    menuTitle();
    reflectionRefraction();
    shortcutList();
    displayFPS();
    displayTime();
    displaySaveLoad();
    displaySpeedAnimation();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::menuTitle() {
    if (!display->cursor_disabled) {
        ImGui::SetNextWindowPos(ImVec2(display->getWidth()/2-40, 50));
        ImGui::Begin("menu", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
        auto wPos = ImGui::GetWindowPos();
        auto wSize = ImGui::GetWindowSize();
        ImGui::GetWindowDrawList()->AddRect(wPos, { wPos.x + wSize.x, wPos.y + wSize.y }, ImColor(1.f, 1.f, 1.f, 1.f), 200.f);
        ImGui::SetWindowFontScale(2.);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,255,255,255));
        ImGui::Text("MENU");
        ImGui::PopStyleColor();
        ImGui::End();
    }
}

void GUI::reflectionRefraction() {
    if (!display->cursor_disabled) {
        Point size = Point({200, 100});
        ImGui::SetNextWindowPos(ImVec2(0, (display->getHeight()-size.y)/2));
        ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
        ImGui::Begin("Reflection & Refraction", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        float reflec = Optic::getReflectivity();
        float refrac = Optic::getRefractivity();
        float coefRefrac = Optic::getCoefRefractivity();
        ImGui::SliderFloat("Reflec.", &(reflec), 0., 1.);
        ImGui::SliderFloat("Refrac.", &(refrac), 0., 1.);
        ImGui::SliderFloat("Coef. ", &(coefRefrac), 1., 10.);
        Optic::setReflectivity(reflec);
        Optic::setRefractivity(refrac);
        Optic::setCoefRefractivity(coefRefrac);
        ImGui::End();
    }
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
    Point size = Point({55, 20});
    ImGui::SetNextWindowPos(ImVec2(display->getWidth()-size.x, display->getHeight()-size.y));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin("fps", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
    ImGui::SetCursorPos(ImVec2(0,0));
    ImGui::InputFloat("##fps", getFPS(), 0, 0, "%.1f", ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsNoBlank);
    ImGui::End();
}

void GUI::displayTime() {

    Point size = Point({115, 10});
    ImGui::SetNextWindowPos(ImVec2(display->getWidth()-size.x, -8));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin("time", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);
    Day day = Time::getDate();
    ImGui::Text("  Time: %ih%i", day.hour, day.minute);
    ImGui::End();
}

void GUI::displaySaveLoad() {
    if (!display->cursor_disabled) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("save load window", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Load"))
            {
                char file[256] = "";
                #ifdef _WIN32
                if (ImGui::InputText("##loadName", file, sizeof(file), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    strcat(file, ".save");
                }
                #elif __linux__
                DIR* dir = opendir("../../save");
                if (dir == nullptr) {std::cerr << "Error opening save directory" << std::endl;}
                dirent* entry;
                while ((entry = readdir(dir)) != nullptr) {
                    if (entry->d_name[0] != '.') {
                        if (ImGui::MenuItem(entry->d_name)) { strcat(file, entry->d_name); }
                    }
                }
                #endif
                if (file != "") {
                    std::ifstream in("../../save/"+std::string(file));
                    // Check if the file was successfully opened
                    if (in.fail()) { std::cout << "Error opening file" << std::endl;}
                    else {
                        std::string line;
                        while (std::getline(in, line)) {
                            std::istringstream iss(line); // Like a "split"
                            std::string indice;
                            iss >> indice; // Go the next elem of the split (reach elem 1 by 1 separated by " ")
                            if (indice == "d") {
                                int idx;
                                float posX, posY, posZ, rotX, rotY, rotZ, scaX, scaY, scaZ;
                                iss >> idx >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> scaX >> scaY >> scaZ;
                                glm::vec3 pos = glm::vec3(posX, posY, posZ);
                                glm::vec3 rot = glm::vec3(rotX, rotY, rotZ);
                                glm::vec3 scale = glm::vec3(scaX, scaY, scaZ);

                                Object* domino = new Object(geometry::domino,Textures::White(), Materials::Dull(),  pos, rot, scale);	
                                world->addDomino(domino);  
                                shader->addObject(domino);
                                shadow->addObject(domino);
                            }
                        }
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Save"))
            {
                char name[64] = "";
                if (ImGui::InputText("##saveName", name, sizeof(name), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    strcat(name, ".save");
                    std::cout << "Save: " << name << std::endl;
                    // Open a file for writing
                    std::ofstream out("../../save/"+std::string(name));
                    // Write to the file
                    for (auto& [idx, object] : world->glObjects) {
                        if (idx != 0) {
                            out << "d " << idx << " ";
                            out << object->position.x << " " << object->position.y << " " << object->position.z << " ";
                            out << object->rotation.x << " " << object->rotation.y << " " << object->rotation.z << " ";
                            out << object->scale.x << " " << object->scale.y << " " << object->scale.z << " ";
                            out << "textureToDo" << std::endl;
                        }
                    }  
                    // Close the file
                    out.close();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
}

void GUI::displaySpeedAnimation() {
    Point size = Point({115, 100});
    ImGui::SetNextWindowPos(ImVec2(display->getWidth()-size.x, 15));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin("Animation Speed", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
    ImGui::SetCursorPos(ImVec2(0,18));
    ImGui::PushItemWidth(115);
    ImGui::SliderFloat(" ", &(process->sliderSpeedAnimation), 0., 4.);
    ImGui::End();
}

void GUI::shortcutList() {
    if (!display->cursor_disabled) {
        int screenHeight = display->getHeight();
        Point size = Point({600, (float) 0.7*screenHeight});
        ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
        ImGui::SetNextWindowPos(ImVec2(display->getWidth()/2-size.x/2, display->getHeight()/2-size.y/2.5));
        ImGui::Begin("Shortcuts List", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowFontScale(1.4);
        ImGui::BeginChild("Scrolling");
        
        ImGui::Text("'Esc': Toogle menu mode");
        ImGui::Text("'Left Ctrl + Q' : Quit");
        ImGui::Text("'P' : Putting dominos (pressed)");
        ImGui::Text("'Enter' : Pushing force (proportionnal to time pressed)");
        ImGui::Text("'Q | Left': Left direction");
        ImGui::Text("'D | Right': Right direction");
        ImGui::Text("'Z | Up': Forward direction");
        ImGui::Text("'S | Down': Backward direction");
        ImGui::Text("'Space | Right Shift': Up direction");
        ImGui::Text("'Left Shift | Right Ctrl': Down direction");
        ImGui::Text("Keypad 0 : Stop time");
        ImGui::Text("keypad 1 : Slow motion");
        ImGui::Text("keypad 2 : Normal time");
        ImGui::Text("keypad 3 : Fast motion");
        ImGui::Text("F10 : Decrease screen size");
        ImGui::Text("F11 : Toogle fullscreen");
        ImGui::Text("F12 : Increase screen size");
            
        ImGui::EndChild();
        ImGui::End();
    }
}

void GUI::clear(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}