#include "gui.h"

GUI::GUI(Process* processArg, Display* displayArg, PhysicalWorld* worldArg, Shader* shaderArg, Shader* shadowArg, Shader* omniShadowShader, Shader* bumpMapShader, Shader* paralaxMapShader, Shader* objectLightShader, LightConstructor* _lightConstructor) {
    process = processArg;
    display = displayArg;
    world = worldArg;
    shader = shaderArg;
    shadow = shadowArg;
    omniShadow = omniShadowShader;
    bumpmap = bumpMapShader;
    parallax = paralaxMapShader;
    objectLight = objectLightShader;
    lightConstructor = _lightConstructor;
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
        else if (ImGui::Button("Brick2")) {process->textureDomino = Textures::Brick2();}
        else if (ImGui::Button("Cobblestone")) {process->textureDomino = Textures::Brickwall();}
        else if (ImGui::Button("Dirt")) {process->textureDomino = Textures::Dirt();}
        else if (ImGui::Button("Wood")) {process->textureDomino = Textures::Wood();}
        else if (ImGui::Button("Concrete")) {process->textureDomino = Textures::Concrete();}
        else if (ImGui::Button("Container")) {process->textureDomino = Textures::Container();}
        //else if (ImGui::Button("Damier")) {process->textureDomino = Textures::Damier();}
        else if (ImGui::Button("Anubis")) {process->textureDomino = Textures::Anubis();}
        else if (ImGui::Button("Point1")) {process->textureDomino = Textures::Point1();}
        else if (ImGui::Button("Point2")) {process->textureDomino = Textures::Point2();}
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
        if (ImGui::Button("Domino")) {
            process->geometryDomino = geometry::domino;
            process->dimDomino = glm::vec3(dominoDim::width, dominoDim::height, dominoDim::thick);
        }
        else if (ImGui::Button("Cube")) {
            process->geometryDomino = geometry::cube;
            process->dimDomino = glm::vec3(2); 
        }
        else if (ImGui::Button("Sphere")) {
            process->geometryDomino = geometry::sphere; 
            process->dimDomino = glm::vec3(2); // rayon of 1 = diameter (size) of 2
            }
        else if (ImGui::Button("plane")) {
            process->geometryDomino = geometry::plane;
            process->dimDomino = glm::vec3(2,0,2);
            }
        else if (ImGui::Button("bunny")) {
            process->geometryDomino = geometry::bunny;
            process->dimDomino = glm::vec3(bunnyDim::width, bunnyDim::height, bunnyDim::thick);
            }
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
                else if (ImGui::Button("Brick2")) {domino->texture = Textures::Brick2();}
                else if (ImGui::Button("Cobblestone")) {domino->texture = Textures::Brickwall();}
                else if (ImGui::Button("Dirt")) {domino->texture = Textures::Dirt();}
                else if (ImGui::Button("Wood")) {domino->texture = Textures::Wood();}
                else if (ImGui::Button("Concrete")) {domino->texture = Textures::Concrete();}
                else if (ImGui::Button("Container")) {domino->texture = Textures::Container();}
                //else if (ImGui::Button("Damier")) {domino->texture = Textures::Damier();}
                else if (ImGui::Button("Anubis")) {domino->texture = Textures::Anubis();}
                else if (ImGui::Button("Point1")) {domino->texture = Textures::Point1();}
                else if (ImGui::Button("Point2")) {domino->texture = Textures::Point2();}
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

                std::string fileStr(file);
                if (fileStr.length() >= 5) {
                    std::ifstream in("../../save/"+fileStr);
                    // Check if the file was successfully opened
                    if (in.fail()) { std::cout << "Error opening file" << std::endl;}
                    else {
                        // Delete World Lights
                        objectLight->objectList.clear();
                        lightConstructor->reset();
                        // Delete World Objects
                        shader->objectList.clear();
                        shadow->objectList.clear();
                        omniShadow->objectList.clear();
                        bumpmap->objectList.clear();
                        parallax->objectList.clear();
                        
                        world->reset();

                        std::string line;
                        while (std::getline(in, line)) {
                            std::istringstream iss(line); // Like a "split"
                            std::string indice;

                            int idx, shaderType, physicType, type;
                            float posX, posY, posZ, rotX, rotY, rotZ, scaX, scaY, scaZ, colX, colY, colZ;
                            std::string tex, mat;
                            iss >> idx >> shaderType >> physicType >> type >> posX >> posY >> posZ >> rotX >> rotY >> rotZ >> scaX >> scaY >> scaZ >> colX >> colY >> colZ >> tex >> mat;
                            glm::vec3 pos = glm::vec3(posX, posY, posZ);
                            glm::vec3 rot = glm::vec3(rotX, rotY, rotZ);
                            glm::vec3 scale = glm::vec3(scaX, scaY, scaZ);
                            glm::vec3 color = glm::vec3(colX, colY, colZ);
                            Texture* texture = Textures::Get(tex);
                            Material* material= Materials::Get(mat);

                            Object* obj = new Object(type, shaderType, texture, material, physicType,  pos, rot, scale, color);	
                            addToShaders(obj);
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
                    std::ofstream out("../../save/"+std::string(name));
                    // Lights
                    for (Object *object: objectLight->objectList){
                        out << object->id << " " << object->shaderType << " " << object->physicType << " " << object->type << " ";
                        out << object->position.x << " " << object->position.y << " " << object->position.z << " ";
                        out << object->rotation.x << " " << object->rotation.y << " " << object->rotation.z << " ";
                        out << object->scale.x << " " << object->scale.y << " " << object->scale.z << " ";
                        out << object->color.x << " " << object->color.y << " " << object->color.z << " ";
                        out << object->texture->name << " ";
                        out << object->material->name << std::endl;
                    } 
                    // Objects
                    for (auto& pair : world->glObjects) {
                        int idx = pair.first;
                        Object* object = pair.second;

                        out << idx << " " << object->shaderType << " " << object->physicType << " " << object->type << " ";
                        out << object->position.x << " " << object->position.y << " " << object->position.z << " ";
                        out << object->rotation.x << " " << object->rotation.y << " " << object->rotation.z << " ";
                        out << object->scale.x << " " << object->scale.y << " " << object->scale.z << " ";
                        out << object->color.x << " " << object->color.y << " " << object->color.z << " ";
                        out << object->texture->name << " ";
                        out << object->material->name << std::endl;
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
        ImGui::Text("'X' : Deleting dominos (pressed)");
        ImGui::Text("'Enter' : Pushing force (proportionnal to time pressed)");

        ImGui::Text("Keypad 0 : Stop time");
        ImGui::Text("keypad 1 : Slow motion");
        ImGui::Text("keypad 2 : Normal time");
        ImGui::Text("keypad 3 : Fast motion");
        ImGui::Text("F10 : Decrease screen size");
        //ImGui::Text("F11 : Toogle fullscreen");
        ImGui::Text("F12 : Increase screen size");
        // Movement
        ImGui::Text("'TAB': Sprint (movement speed x5)");
        ImGui::Text("'TAB + I': Precision mode (movement speed x0.2)");
        ImGui::Text("'Q | Left': Left direction");
        ImGui::Text("'D | Right': Right direction");
        ImGui::Text("'Z | Up': Forward direction");
        ImGui::Text("'S | Down': Backward direction");
        ImGui::Text("'Space | Right Shift': Up direction");
        ImGui::Text("'Left Shift | Right Ctrl': Down direction");
        // Paramatrizing domino
        ImGui::Text("'T' : Increase size of next domino (keep pressed)");
        ImGui::Text("'T + I' : Decrease size of next domino (keep pressed)");
        ImGui::Text("'Y' : Random color for following dominos (keep pressed)");
        ImGui::Text("'H' : Rainbow color for following dominos (keep pressed)");
        ImGui::Text("'R' : Increase red component of following dominos");
        ImGui::Text("'R + I' : Decrease red component of following dominos");
        ImGui::Text("'G' : Increase green component of following dominos");
        ImGui::Text("'G + I' : Decrease green component of following dominos");
        ImGui::Text("'B' : Increase blue component of following dominos");
        ImGui::Text("'B + I' : Decrease blue component of following dominos");
        ImGui::Text("'M + Esc' : Modifying the appearance of the targeted domino");
        // Flashlight
        ImGui::Text("'V' : Flashlight on (keep pressed)");
        ImGui::Text("'V + T' : Increase angle of the flashlight");
        ImGui::Text("'V + T + I' : Decrease angle of the flashlight");
        ImGui::Text("'V + C' : Increase intensity of the flashlight");
        ImGui::Text("'V + C + I' : Decrease intensity of the flashlight");
        ImGui::Text("'V + Y' : Random color for the flashlight");
        ImGui::Text("'V + H' : Rainbow color for the flashlight");
        ImGui::Text("'V + R' : Increase red component of the flashlight");
        ImGui::Text("'V + R + I' : Decrease red component of the flashlight");
        ImGui::Text("'V + G' : Increase green component of the flashlight");
        ImGui::Text("'V + G + I' : Decrease green component of the flashlight");
        ImGui::Text("'V + B' : Increase blue component of the flashlight");
        ImGui::Text("'V + B + I' : Decrease blue component of the flashlight");      
            
        ImGui::EndChild();
        ImGui::End();
    }
}

void GUI::clear(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::addToShaders(Object* obj) {
    if (obj->shaderType == ShaderType::POINTLIGHT) {
        objectLight->addObject(obj);
        lightConstructor->addPointLight(obj);
    }
    else if (obj->shaderType == ShaderType::SPOTLIGHT) {
        objectLight->addObject(obj);
        lightConstructor->addSpotLight(obj);
    }
    else if (obj->shaderType == ShaderType::AREALIGHT) {
        objectLight->addObject(obj);
        lightConstructor->addAreaLight(obj);
    }
    else {
        world->addObject(obj);
        shadow->addObject(obj); 
        omniShadow->addObject(obj);
        if (obj->shaderType == ShaderType::OBJECT)
            shader->addObject(obj);
        if (obj->shaderType == ShaderType::BUMPMAP)
            bumpmap->addObject(obj);
        else if (obj->shaderType == ShaderType::PARALLAX)
            parallax->addObject(obj);
    }
}