#include "shader.h"

Shader::Shader(){
    shaderID = 0; 
    uniformModel=0; 
    uniformProjection=0; 

    pointLightCount = 0;
    spotLightCount = 0;  
    areaLightCount = 0; 
}


void Shader::addObject(Object *obj) {
    objectList.push_back(obj);
}


void Shader::addObjects(std::vector<Object*> objects){
    for (Object *object: objects){
        addObject(object);
    }
}

void Shader::RenderParalax(Camera camera, glm::mat4 projection, glm::mat4 view, 
                         DirectionalLight* mainLight,
                         PointLight* pointLights, 
                         int pointLightCount, 
                         SpotLight* spotLights, 
                         int spotLightCount, 
                         AreaLight* areaLights, 
                         int areaLightCount){
    UseShader(); 

    uniformModel = GetModelLocation(); 
    uniformProjection = GetProjectionLocation(); 
    uniformView = GetViewLocation(); 
    uniformEyePosition = GetEyePositionLocation(); 

    //fog
    SetSkyColor(0.5, 0.5, 0.5); 

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(uniformEyePosition, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z); 

    SetDirectionalLight(mainLight);
    SetPointLights(pointLights, pointLightCount, 9, 0);//since it is an array we don't need to pass the address. 
    SetSpotLights(spotLights, spotLightCount, 9 + pointLightCount, pointLightCount); 
    SetAreaLights(areaLights, areaLightCount); 

    glm::mat4 resmainLight = mainLight->CalculateLightTransform();
    SetDirectionalLightTransform(&resmainLight); 


    mainLight->GetShadowMap()->Read(GL_TEXTURE4);


    SetTexture(1);
    SetNormalMap(2);
    SetDepthMap(3);
    SetDirectionalShadowMap(4);
    SetLTC1(5);
    SetLTC2(6);

    spotLights[0].SetFlash(camera.getPosition(), camera.getDirection()); 


    RenderScene();
}

void Shader::RenderBump(Camera camera, glm::mat4 projection, glm::mat4 view, 
                         DirectionalLight* mainLight,
                         PointLight* pointLights, 
                         int pointLightCount, 
                         SpotLight* spotLights, 
                         int spotLightCount, 
                         AreaLight* areaLights, 
                         int areaLightCount){
    UseShader(); 

    uniformModel = GetModelLocation(); 
    uniformProjection = GetProjectionLocation(); 
    uniformView = GetViewLocation(); 
    uniformEyePosition = GetEyePositionLocation(); 

    //fog
    SetSkyColor(0.5, 0.5, 0.5); 

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(uniformEyePosition, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z); 

    SetDirectionalLight(mainLight);
    SetPointLights(pointLights, pointLightCount, 9, 0);//since it is an array we don't need to pass the address. 
    SetSpotLights(spotLights, spotLightCount, 9 + pointLightCount, pointLightCount); 
    SetAreaLights(areaLights, areaLightCount); 

    glm::mat4 resmainLight = mainLight->CalculateLightTransform();
    SetDirectionalLightTransform(&resmainLight); 


    mainLight->GetShadowMap()->Read(GL_TEXTURE4);


    SetTexture(1);
    SetNormalMap(2);
    SetDirectionalShadowMap(4);
    SetLTC1(5);
    SetLTC2(6);

    spotLights[0].SetFlash(camera.getPosition(), camera.getDirection()); 


    RenderScene();
}

void Shader::RenderPass(Camera camera, glm::mat4 projection, glm::mat4 view,  
                         DirectionalLight* mainLight,
                         PointLight* pointLights, 
                         int pointLightCount, 
                         SpotLight* spotLights, 
                         int spotLightCount, 
                         AreaLight* areaLights, 
                         int areaLightCount) {

    // glViewport(0, 0, 960, 540); 
    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Clear all the frame so that you will be able to draw another frame (can chose the color of the clear)
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // A pixel does not only have color as data, it also has depth and other things. We are specifying here that we want to clear the color. 
    // //glClear is also clearing the depth buffer bit.
    
    UseShader(); 

    uniformModel = GetModelLocation(); 
    uniformProjection = GetProjectionLocation(); 
    uniformView = GetViewLocation(); 
    uniformEyePosition = GetEyePositionLocation(); 
    uniformSpecularIntensity = GetSpecularIntensityLocation();
    uniformShininess = GetShininessLocation();

    
    glUniform1i(uniformSkyboxDay, 7); 
    glUniform1i(uniformSkyboxNight, 8); 
    SetBlendFactor();
    SetTime();
    glUniform1f(GetReflectivityLocation(), Optic::getReflectivity());
    glUniform1f(GetRefractivityLocation(), Optic::getRefractivity());
    glUniform1f(GetCoefRefractionLocation(), Optic::getCoefRefractivity());
    
    

    //fog
    SetSkyColor(0.5, 0.5, 0.5); 

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(uniformEyePosition, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z); 

    
    SetDirectionalLight(mainLight);
    SetPointLights(pointLights, pointLightCount, 9, 0);//since it is an array we don't need to pass the address. 
    SetSpotLights(spotLights, spotLightCount, 9 + pointLightCount, pointLightCount); 
    SetAreaLights(areaLights, areaLightCount); 
   
    // --------------------------------------------------------- // 
    glm::mat4 resmainLight = mainLight->CalculateLightTransform();
    SetDirectionalLightTransform(&resmainLight); 


    mainLight->GetShadowMap()->Read(GL_TEXTURE4);// we are binding to a given textureUnit and we are binding that texture to that tewture unti. 
    // we use GL_TEXTURE1 because GL_TECTURE0 is taken for the normal texture of the objects
    // shadowMap is going to be bound to GL_TEXTURE1

    SetTexture(1); // bound to texture unit 0 
    SetDirectionalShadowMap(4); // bound to GL_TEXTURE1
    SetLTC1(5);
    SetLTC2(6);

    // --------------------------------------------------------- // 
    spotLights[0].SetFlash(camera.getPosition(), camera.getDirection()); 

    Validate(); 

    RenderScene();
}


void Shader::RenderScene() {
    for(Object* object : objectList) {
        glUniform3f(uniformColor, object->color.x, object->color.y, object->color.z);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(object->model)); // need to use the value_ptr because the model does not directly have the good format to work with shaders 
        object->texture->UseTexture(); // From now on anything drawn will be using the bricktexture loaded in. 

        object->material->UseMaterial(uniformSpecularIntensity, uniformShininess);  
        object->draw();
        
    }
}


void Shader::DirectionalShadowMapPass(DirectionalLight* light) {
    UseShader(); // doesn't have colors, it is the depth map. 

    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight()); // we need to make sure the buffer we are drawing to is the same size as the viewport

    light->GetShadowMap()->Write();

    glClear(GL_DEPTH_BUFFER_BIT); // if there iis already some depth infor. in the buffer we clear it

    uniformModel = GetModelLocation();

    glm::mat4 resLight = light->CalculateLightTransform(); 
    SetDirectionalLightTransform(&resLight);

    Validate(); 

    RenderScene(); 

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // attach the default buffer
}


void Shader::DrawLightObjects(glm::mat4 projection, glm::mat4 view) {
    UseShader(); 

    uniformModel = GetModelLocation(); 
    uniformProjection = GetProjectionLocation(); 
    uniformView = GetViewLocation(); 

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));

    SetTexture(1); // bound to texture unit 1

    
    // RENDER SCENE
    RenderScene();
}

void Shader::remove(int objID) {
    if (objID != -1) {
        for (int i = 0; i < objectList.size(); i++) {
            if (objectList[i]->id==objID) {
                objectList.erase(objectList.begin() + i);
                break;
            }
        }
    }
}



void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode){
    CompileShader(vertexCode, fragmentCode); 
}


void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation){
    std::string vertexString = ReadFile(vertexLocation); 
    std::string fragmentString = ReadFile(fragmentLocation); 

    const char* vertexCode = vertexString.c_str(); //c_str : convert to c string format which is const char array. 
    const char* fragmentCode = fragmentString.c_str(); 

    CompileShader(vertexCode, fragmentCode); 
}


void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation); 
    std::string geometryString = ReadFile(geometryLocation); 
    std::string fragmentString = ReadFile(fragmentLocation); 

    const char* vertexCode = vertexString.c_str(); //c_str : convert to c string format which is const char array. 
    const char* geometryCode = geometryString.c_str(); 
    const char* fragmentCode = fragmentString.c_str(); 

    CompileShader(vertexCode, geometryCode, fragmentCode); 
}


std::string Shader::ReadFile(const char* fileLocation){
    std::string content; 
    std::ifstream fileStream(fileLocation, std::ios::in); 

    if(!fileStream.is_open()){
        printf("Failed to read %s, file doe snot exist!!! ", fileLocation); 
        return ""; 
    }

    std::string line = ""; 
    while(!fileStream.eof())
    {   
        std::getline(fileStream, line); 
        content.append(line + "\n");
    }

    fileStream.close(); 
    return content; 

}

void Shader::Validate()
{
    // Validate Program

    GLint result = 0; 
    GLchar eLog[1024] = { 0 };

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);//2scd arg: what kind of info we want, 3rd arg: where do we put the info 

    if(!result){
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog); 
        printf("Error validating program : %s\n", eLog);
        return; 
    }

}

void Shader::CompileProgram()
{
    GLint result = 0; 
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID); // create the executables on the graphics card

    //Make sure the program is linked properly: 

    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);//2scd arg: what kind of info we want, 3rd arg: where do we put the info 

    if(!result){
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog); 
        printf("Error linking program : %s\n", eLog);
        return; 
    }

    uniformProjection = glGetUniformLocation(shaderID, "projection");  // UniformProjection will be the location of the uniformProjection
    uniformModel = glGetUniformLocation(shaderID, "model");  // UniformModel will be the location of the uniformMatrix
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color"); 
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction"); 
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity"); 
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity"); 
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess"); 
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
    uniformColor = glGetUniformLocation(shaderID, "objectColor");
    
    uniformSkyColor = glGetUniformLocation(shaderID, "skyColor"); // fog
    uniformSkyboxDay = glGetUniformLocation(shaderID, "skyboxDay"); 
    uniformSkyboxNight = glGetUniformLocation(shaderID, "skyboxNight"); 
    uniformBlendFactor = glGetUniformLocation(shaderID, "blendFactor"); 
    uniformSunLightColor = glGetUniformLocation(shaderID, "sunLightColor"); 
    uniformSinTime = glGetUniformLocation(shaderID, "sinTime"); 
    uniformCosTime = glGetUniformLocation(shaderID, "cosTime"); 

    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount"); 
    uniformMaxPointLight = glGetUniformLocation(shaderID, "MAX_POINT_LIGHTS");


    for (int i = 0; i < values::MAX_POINT_LIGHTS; i ++){

        char locBuff[100] = {'\0'}; //setting all values to \0 which is EOS (End Of String)

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i); // we are going to print to a buffer
        uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i); // we are going to print to a buffer
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i); // we are going to print to a buffer
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i); // we are going to print to a buffer
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i); // we are going to print to a buffer
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i); // we are going to print to a buffer
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i); // we are going to print to a buffer
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff); 
    }

    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount"); 
    uniformMaxSpotLight = glGetUniformLocation(shaderID, "MAX_SPOT_LIGHTS");


    for (int i = 0; i < values::MAX_SPOT_LIGHTS; i ++){

        char locBuff[100] = {'\0'}; //setting all values to \0 which is EOS (End Of String)

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i); // we are going to print to a buffer 
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff); 

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i); // we are going to print to a buffer
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff); 
    }

    uniformTexture = glGetUniformLocation(shaderID, "theTexture");
    uniformNormalMap = glGetUniformLocation(shaderID, "normalMap");
    uniformDepthMap = glGetUniformLocation(shaderID, "depthMap");
    uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform"); // it will bind for vertex a AND frag because it is the same name
    uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");


    uniformAreaLightCount = glGetUniformLocation(shaderID, "areaLightCount");
    uniformMaxAreaLight = glGetUniformLocation(shaderID, "MAX_AREA_LIGHTS");
    uniformLTC1 = glGetUniformLocation(shaderID, "LTC1");
    uniformLTC2 = glGetUniformLocation(shaderID, "LTC2");
    uniformAlbedoRoughness = glGetUniformLocation(shaderID, "material.albedoRoughness");
    
    uniformReflectivity = glGetUniformLocation(shaderID, "Reflectivity");
    uniformRefractivity = glGetUniformLocation(shaderID, "Refractivity");
    uniformCoefRefraction = glGetUniformLocation(shaderID, "CoefRefraction");


    for (int i = 0; i < values::MAX_AREA_LIGHTS; i++){

        std::string str_points = "areaLights[" + std::to_string(i) + "].points";
		std::string str_color = "areaLights[" + std::to_string(i) + "].base.color";
		std::string str_intensity = "areaLights[" + std::to_string(i) + "].base.diffuseIntensity";
		std::string str_twoSided = "areaLights[" + std::to_string(i) + "].twoSided";

        uniformAreaLight[i].uniformPoint0 = glGetUniformLocation(shaderID, (str_points + "[0]").c_str());
        uniformAreaLight[i].uniformPoint1 = glGetUniformLocation(shaderID, (str_points + "[1]").c_str());
        uniformAreaLight[i].uniformPoint2 = glGetUniformLocation(shaderID, (str_points + "[2]").c_str());
        uniformAreaLight[i].uniformPoint3 = glGetUniformLocation(shaderID, (str_points + "[3]").c_str());

        uniformAreaLight[i].uniformColor = glGetUniformLocation(shaderID, str_color.c_str());
		uniformAreaLight[i].uniformIntensity = glGetUniformLocation(shaderID, str_intensity.c_str());
        uniformAreaLight[i].uniformTwoSided = glGetUniformLocation(shaderID, str_twoSided.c_str());

    }

    uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos"); 
    uniformFarPlane = glGetUniformLocation(shaderID, "farPlane"); 

    for (size_t i =0; i < 6; i++)
    {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i); 
        uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);  
    }

    for (size_t i =0; i < values::MAX_POINT_LIGHTS + values::MAX_SPOT_LIGHTS ; i++)
    {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i); 
        uniformOmniShadowMap[i].shadowMap = glGetUniformLocation(shaderID, locBuff);  

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i); 
        uniformOmniShadowMap[i].farPlane = glGetUniformLocation(shaderID, locBuff);
    } 
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode){
    shaderID = glCreateProgram(); // Outputs the Ids of the shaders. 

    if (!shaderID){
        printf("Error Creating shader program");
        return; 
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER); //GL_VERTEX_SHADER is the type of shader, it needs to know what type the shaders are. 
    AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    CompileProgram(); 

}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode){
    shaderID = glCreateProgram(); // Outputs the Ids of the shaders. 

    if (!shaderID){
        printf("Error Creating shader program");
        return; 
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER); //GL_VERTEX_SHADER is the type of shader, it needs to know what type the shaders are. 
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    CompileProgram(); 
}

void Shader::SetDirectionalLight(DirectionalLight * dLight){
    dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor, 
    uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}


void Shader::SetPointLights(PointLight * pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset){

    if(lightCount > values::MAX_POINT_LIGHTS) lightCount = values::MAX_POINT_LIGHTS; 

    glUniform1i(uniformPointLightCount, lightCount); // make sure it is an int ! to go through the loop
    glUniform1i(uniformMaxPointLight, values::MAX_POINT_LIGHTS);

    for(int i=0; i < lightCount; i++){
        pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, 
                           uniformPointLight[i].uniformColor, 
                           uniformPointLight[i].uniformDiffuseIntensity, 
                           uniformPointLight[i].uniformPosition,
                           uniformPointLight[i].uniformConstant, 
                           uniformPointLight[i].uniformLinear, 
                           uniformPointLight[i].uniformExponent); 
        pLight[i].GetShadowMap()->Read(GL_TEXTURE0+ textureUnit + i); // need a new GL_TEXTURE for each pointlight
        // the textureUnit is just the starting point: for instance if it is 0 then we will have GL_TEXTURE0 then GL_TEXTURE1, ...
        glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i); // for pointlights the offset will be 0 IF WE SET THEM FIRST before the spotlights
        glUniform1f(uniformOmniShadowMap[i + offset].farPlane, pLight[i].GetFarPlane()); 
        
    }
}


void Shader::SetSpotLights(SpotLight * sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset){

     if(lightCount > values::MAX_SPOT_LIGHTS) lightCount = values::MAX_SPOT_LIGHTS; 

    glUniform1i(uniformSpotLightCount, lightCount); // make sure it is an int ! to go through the loop
    glUniform1i(uniformMaxSpotLight, values::MAX_SPOT_LIGHTS);

    for(int i=0; i < lightCount; i++){

        sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, 
                           uniformSpotLight[i].uniformColor, 
                           uniformSpotLight[i].uniformDiffuseIntensity, 
                           uniformSpotLight[i].uniformPosition,
                           uniformSpotLight[i].uniformDirection, 
                           uniformSpotLight[i].uniformConstant, 
                           uniformSpotLight[i].uniformLinear, 
                           uniformSpotLight[i].uniformExponent,
                           uniformSpotLight[i].uniformEdge); 

        sLight[i].GetShadowMap()->Read(GL_TEXTURE0+ textureUnit + i); 
        // need a new GL_TEXTURE for each pointlight
        // the textureUnit is just the starting point: for instance if it is 0 then we will have GL_TEXTURE0 then GL_TEXTURE1, ...
        glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i); 
        glUniform1f(uniformOmniShadowMap[i + offset].farPlane, sLight[i].GetFarPlane()); 

    }
}


void Shader::SetAreaLights(AreaLight *  aLights, int lightCount){
    if(lightCount > values::MAX_AREA_LIGHTS) lightCount = values::MAX_AREA_LIGHTS; 

    glUniform1i(uniformAreaLightCount, lightCount);
    glUniform1i(uniformMaxAreaLight, values::MAX_AREA_LIGHTS);
    glUniform1f(uniformAlbedoRoughness, 0.5);

    for(int i=0; i < lightCount; i++){

        aLights[i].UseLight(uniformAreaLight[i].uniformPoint0,
                            uniformAreaLight[i].uniformPoint1,
                            uniformAreaLight[i].uniformPoint2,
                            uniformAreaLight[i].uniformPoint3,
                            uniformAreaLight[i].uniformColor,
                            uniformAreaLight[i].uniformIntensity,
                            uniformAreaLight[i].uniformTwoSided);

    }
}

void Shader::SetLTC1(GLuint textureUnit){
    glUniform1i(uniformLTC1, textureUnit);
}

void Shader::SetLTC2(GLuint textureUnit){
    glUniform1i(uniformLTC2, textureUnit);
}

void Shader::SetBlendFactor()
{
    float blend = pow(cos(3.14*Time::getTime()/Ttime::maxTime),4); 
    glUniform1f(uniformBlendFactor, blend); 
}

void Shader::SetSunLightColor() {
    float base = 1.5;
    float x = glm::sin(2*3.14*Time::getTime()/Ttime::maxTime);
    glm::vec3 color = glm::normalize(glm::vec3(base+color::OrangeRed.x*pow(x,20), base+color::OrangeRed.y*pow(x,20), base+color::OrangeRed.z*pow(x,20)));
    glUniform3f(uniformSunLightColor, color.x, color.y, color.z);
}

void Shader::SetTime()
{
    float sinTime = -sin(2*3.14*Time::getTime()/Ttime::maxTime); 
    glUniform1f(uniformSinTime, sinTime);
    float cosTime = -cos(2*3.14*Time::getTime()/Ttime::maxTime); 
    glUniform1f(uniformCosTime, cosTime);  
}

void Shader::ConnectSkyboxes()
{
    glUniform1i(uniformSkyboxDay, 7);
    glUniform1i(uniformSkyboxNight, 8); 
}

void Shader::SetSkyColor(float r, float g, float b)
{
    glUniform3f(uniformSkyColor, r, g, b); 
}

void Shader::SetTexture(GLuint textureUnit){
    glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetNormalMap(GLuint textureUnit){
    glUniform1i(uniformNormalMap, textureUnit);
}

void Shader::SetDepthMap(GLuint textureUnit){
    glUniform1i(uniformDepthMap, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit){
    glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* lTransform){
    glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform)); 
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices)
{
    for(size_t i = 0; i < 6; i ++){

        glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i])); 
        
    }
}

void Shader::UseShader(){
    glUseProgram(shaderID); 
}



void Shader::ClearShader(){
    if(shaderID != 0){
        glDeleteProgram(shaderID);  // delete the program of the graphics card to make more space. 
        shaderID=0; 
    }

    uniformModel = 0; 
    uniformProjection = 0; 
}

void Shader::AddShader(GLuint program, const char* shader_code, GLenum shader_type){
    GLuint the_shader = glCreateShader(shader_type); // create empty shader and pass the id into "the_shader"

    const GLchar* the_code[1];//Pointer to the 1st elem of an array

    the_code[0] = shader_code;

    GLint code_length[1];
    code_length[0] = strlen(shader_code); //strlen comes from the string.h library. 

    glShaderSource(the_shader, 1, the_code, code_length);
    glCompileShader(the_shader);

    GLint result = 0; 
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(the_shader, GL_COMPILE_STATUS, &result);//2scd arg: what kind of info we want, 3rd arg: where do we put the info 

    if(!result){
        glGetShaderInfoLog(the_shader, sizeof(eLog), NULL, eLog); 
        printf("Error compiling the %d shader : %s\n", shader_type, eLog);
        return; 
    }
    glAttachShader(program, the_shader);
}

GLuint Shader::GetProjectionLocation(){
    return uniformProjection; 
}

GLuint Shader::GetModelLocation(){
    return uniformModel; 
}

GLuint Shader::GetUniformSkyboxDay(){
    return uniformSkyboxDay; 
}

GLuint Shader::GetUniformSkyboxNight(){
    return uniformSkyboxNight; 
}

GLuint Shader::GetUniformBlendFactor(){
    return uniformBlendFactor; 
}

GLuint Shader::GetUniformSunLightColor(){
    return uniformSunLightColor; 
}

GLuint Shader::GetUniformSinTime(){
    return uniformSinTime; 
}

GLuint Shader::GetUniformCosTime(){
    return uniformCosTime; 
}

GLuint Shader::GetViewLocation(){
    return uniformView; 
}

GLuint Shader::GetAmbientColorLocation(){
    return uniformDirectionalLight.uniformColor; 
}

GLuint Shader::GetAmbientIntensityLocation(){
    return uniformDirectionalLight.uniformAmbientIntensity;     
}

GLuint Shader::GetDiffuseIntensityLocation(){
    return uniformDirectionalLight.uniformDiffuseIntensity; 
}

GLuint Shader::GetDirectionLocation(){
    return uniformDirectionalLight.uniformDirection; 
}

GLuint Shader::GetSpecularIntensityLocation(){
    return uniformSpecularIntensity; 
}

GLuint Shader::GetShininessLocation(){
    return uniformShininess; 
}

GLuint Shader::GetEyePositionLocation(){
    return uniformEyePosition; 
}

GLuint Shader::GetReflectivityLocation() {
    return uniformReflectivity;
} 

GLuint Shader::GetRefractivityLocation() {
    return uniformRefractivity;
} 

GLuint Shader::GetCoefRefractionLocation() {
    return uniformCoefRefraction;
}

GLuint Shader::GetOmniLightPosLocation(){
    return uniformOmniLightPos; 
}

GLuint Shader::GetFarPlaneLocation(){
    return uniformFarPlane; 
}

Shader::~Shader(){
    ClearShader(); 
}