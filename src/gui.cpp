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
    displayFPS();
    displayTime();
    displaySpeedAnimation();
    displayDominoInfo();
    displaySaveLoad();
    dominoModify();
    displayPushPower();
    shortcutList();

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

    Point size = Point({115, 45});
    ImGui::SetNextWindowPos(ImVec2(display->getWidth()-size.x, -8));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin("time", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
    Day day = Time::getDate();
    ImGui::Text("  Time: %ih%i", day.hour, day.minute);
    ImGui::PushItemWidth(115);
    float time = Time::getTime();
    ImGui::SliderFloat("##slideTime", (&time), 0., Ttime::maxTime-1, " ",  ImGuiSliderFlags_None);
    Time::setTime(time);
    ImGui::End();
}

void GUI::displaySpeedAnimation() {
    Point size = Point({115, 100});
    ImGui::SetNextWindowPos(ImVec2(display->getWidth()-size.x, 45));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin("Animation Speed", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
    ImGui::SetCursorPos(ImVec2(0,18));
    ImGui::PushItemWidth(115);
    ImGui::SliderFloat(" ", &(process->sliderSpeedAnimation), 0., 4.);
    ImGui::End();
}


void GUI::displayDominoInfo() {
    Point size = Point({152, 130});
    ImGui::SetNextWindowPos(ImVec2(display->getWidth()-size.x, 100));
    ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
    ImGui::Begin("Domino Information", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
    ImGui::InputFloat("scale", &(process->scaleDomino));
    float color[3] = {process->colorDomino.x, process->colorDomino.y, process->colorDomino.z};
    ImGui::ColorEdit3("color", color, ImGuiColorEditFlags_NoPicker);
    process->colorDomino = glm::vec3(color[0], color[1], color[2]);

    if (ImGui::BeginMenu("Texture", !texturePicked)) {
        texturePicked = true;
        if (ImGui::Button("White")) {process->textureDomino = Textures::White();}
        else if (ImGui::Button("Brick")) {process->textureDomino = Textures::Brick();}
        else if (ImGui::Button("Dirt")) {process->textureDomino = Textures::Dirt();}
        else if (ImGui::Button("Wood")) {process->textureDomino = Textures::Wood();}
        else { texturePicked = false; }
        ImGui::EndMenu();
    } else { texturePicked = false; }
    if (ImGui::BeginMenu("Material", !materialPicked)) {
        materialPicked = true;
        if (ImGui::Button("Shiny")) {process->materialDomino = Materials::Shiny();}
        else if (ImGui::Button("Dull")) {process->materialDomino = Materials::Dull();}
        else if (ImGui::Button("Empty")) {process->materialDomino = Materials::Empty();}
        else { materialPicked = false; }
        ImGui::EndMenu();
    } else { materialPicked = false; }
    if (ImGui::BeginMenu("Geometry", !geometryPicked)) {
        geometryPicked = true;
        if (ImGui::Button("Domino")) {process->geometryDomino = geometry::domino;}
        else if (ImGui::Button("Cube")) {process->geometryDomino = geometry::cube;}
        else if (ImGui::Button("Sphere")) {process->geometryDomino = geometry::sphere;}
        else if (ImGui::Button("plane")) {process->geometryDomino = geometry::plane;}
        else { geometryPicked = false; }
        ImGui::EndMenu();
    } else { geometryPicked = false; }

    ImGui::End();
}

void GUI::dominoModify() {
    if (!display->cursor_disabled) {
        Object* domino = process->selectedDomino;
        if (domino != nullptr) {

            Point size = Point({200, 275});
            ImGui::SetNextWindowPos(ImVec2(display->getWidth()-size.x, 235));
            ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
            ImGui::Begin("Modify current domino", NULL, ImGuiWindowFlags_NoSavedSettings);

            float scale = domino->scale.x;
            ImGui::InputFloat("scale", &(scale));
            domino->scale = glm::vec3(scale);

            float color[3] = {domino->color.x, domino->color.y, domino->color.z};
            ImGui::ColorPicker3("color", color);
            domino->color = glm::vec3(color[0], color[1], color[2]);

            if (ImGui::BeginMenu("Texture", !texturePicked)) {
                texturePicked = true;
                if (ImGui::Button("White")) {domino->texture = Textures::White();}
                else if (ImGui::Button("Brick")) {domino->texture = Textures::Brick();}
                else if (ImGui::Button("Dirt")) {domino->texture = Textures::Dirt();}
                else if (ImGui::Button("Wood")) {domino->texture = Textures::Wood();}
                else { texturePicked = false; }
                ImGui::EndMenu();
            } else { texturePicked = false; }
            if (ImGui::BeginMenu("Material", !materialPicked)) {
                materialPicked = true;
                if (ImGui::Button("Shiny")) {domino->material = Materials::Shiny();}
                else if (ImGui::Button("Dull")) {domino->material = Materials::Dull();}
                else if (ImGui::Button("Empty")) {domino->material = Materials::Empty();}
                else { materialPicked = false; }
                ImGui::EndMenu();
            } else { materialPicked = false; }

            ImGui::End();
        }
    }
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
                                float posX, posY, posZ, rotX, rotY, rotZ, scaX, scaY, scaZ, colX, colY, colZ;
                                std::string tex, mat;
                                iss >> idx >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> scaX >> scaY >> scaZ >> colX >> colY >> colZ >> tex >> mat;
                                glm::vec3 pos = glm::vec3(posX, posY, posZ);
                                glm::vec3 rot = glm::vec3(rotX, rotY, rotZ);
                                glm::vec3 scale = glm::vec3(scaX, scaY, scaZ);
                                glm::vec3 color = glm::vec3(colX, colY, colZ);
                                Texture* texture = Textures::Get(tex);
                                Material* material= Materials::Get(mat);
                                Object* domino = new Object(geometry::domino, texture, material,  pos, rot, scale, color);	
                                world->addObject(domino);  
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
                    for (auto& pair : world->glObjects) {
                        int idx = pair.first;
                        Object* object = pair.second;
                        if (idx != 0) {
                            out << "d " << idx << " ";
                            out << object->position.x << " " << object->position.y << " " << object->position.z << " ";
                            out << object->rotation.x << " " << object->rotation.y << " " << object->rotation.z << " ";
                            out << object->scale.x << " " << object->scale.y << " " << object->scale.z << " ";
                            out << object->color.x << " " << object->color.y << " " << object->color.z << " ";
                            out << object->texture->name << " ";
                            out << object->material->name << std::endl;
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

void GUI::displayPushPower() {
    if (process->enterPressed) {
        Point size = Point({115, 100});
        ImGui::SetNextWindowPos(ImVec2((display->getWidth()-size.x)/2, (display->getHeight()-size.y)/2 + 40));
        ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
        ImGui::Begin("Power push", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
        ImGui::SetCursorPos(ImVec2(0,18));
        ImGui::PushItemWidth(115);
        ImGui::SliderInt(" ", &(process->enterPressed), 0, 100);
        ImGui::End();
    }

}


void GUI::shortcutList() {
    if (!display->cursor_disabled) {
        int screenHeight = display->getHeight();
        Point size = Point({600, (int) (0.7*screenHeight)});
        ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
        ImGui::SetNextWindowPos(ImVec2(display->getWidth()/2-size.x/2, display->getHeight()/2-size.y/2.5));
        ImGui::Begin("Shortcuts List", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowFontScale(1.4);
        ImGui::BeginChild("Scrolling");
        
        ImGui::Text("'Esc': Toogle menu mode");
        ImGui::Text("'Left Ctrl + Q' : Quit");
        ImGui::Text("'P' : Putting dominos (pressed)");
        ImGui::Text("'Enter' : Pushing force (proportionnal to time pressed)");

        ImGui::Text("'T' : Increase size of next domino (keep pressed)");
        ImGui::Text("'T + I' : Decrease size of next domino (keep pressed)");
        ImGui::Text("'Y' : Random color for following dominos (keep pressed)");
        ImGui::Text("'H' : Rainbow color for following dominos (keep pressed)");
        ImGui::Text("'R' : Increase red component of following dominos");
        ImGui::Text("'R + I' : Decrease red component of following dominos");
        ImGui::Text("'G' : Increase red component of following dominos");
        ImGui::Text("'G + I' : Decrease red component of following dominos");
        ImGui::Text("'B' : Increase red component of following dominos");
        ImGui::Text("'B + I' : Decrease red component of following dominos");

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