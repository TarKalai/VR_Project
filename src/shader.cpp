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

    // skybox.DrawSkyBox(view, projection); 
    
    UseShader(); 

    uniformModel = GetModelLocation(); 
    uniformProjection = GetProjectionLocation(); 
    uniformView = GetViewLocation(); 
    uniformEyePosition = GetEyePositionLocation(); 
    uniformSpecularIntensity = GetSpecularIntensityLocation();
    uniformShininess = GetShininessLocation();

    
    glUniform1i(uniformskybox, 3);
    //glEnable(GL_CULL_FACE);
    

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(uniformEyePosition, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z); 

    SetDirectionalLight(mainLight);
    SetPointLights(pointLights, pointLightCount);//since it is an array we don't need to pass the address. 
    SetSpotLights(spotLights, spotLightCount); 
    SetAreaLights(areaLights, areaLightCount); 
    // --------------------------------------------------------- // 
    glm::mat4 resmainLight = mainLight->CalculateLightTransform();
    SetDirectionalLightTransform(&resmainLight); 


    mainLight->GetShadowMap()->Read(GL_TEXTURE1);// we are binding to a given textureUnit and we are binding that texture to that tewture unti. 
    // we use GL_TEXTURE1 because GL_TECTURE0 is taken for the normal texture of the objects
    // shadowMap is going to be bound to GL_TEXTURE1

    SetTexture(0); // bound to texture unit 0 
    SetDirectionalShadowMap(1); // bound to GL_TEXTURE1
    // --------------------------------------------------------- // 

    glm::vec3 lowerLight = camera.getPosition(); 
    lowerLight.y -= 0.3f; // in order to have a more realisitc flashlght we lower the real position of the camera (copy)
    // so that it creates an effect of skewness much like in reality. 
    // spotLights[0].SetFlash(lowerLight, camera.getCameraDirection()); 

    RenderScene();
}


void Shader::RenderScene() {
    for(Object* object : objectList) {
        if (object->visible) {
            //material.UseMaterial(glGetUniformLocation(ID, "material.specularIntensity"), glGetUniformLocation(ID, "material.shininess")); 
            //glUniform3f(glGetUniformLocation(shaderID, "lightColor"), object->color.x, object->color.y, object->color.z);
            glUniform3f(uniformColor, object->color.x, object->color.y, object->color.z);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(object->model)); // need to use the value_ptr because the model does not directly have the good format to work with shaders 
            object->texture->UseTexture(); // From now on anything drawn will be using the bricktexture loaded in. 

            object->material->UseMaterial(uniformSpecularIntensity, uniformShininess);  
            object->draw();
        }
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
    
    // RENDER SCENE
    RenderScene();
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
    


    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount"); 

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
    uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform"); // it will bind for vertex a AND frag because it is the same name
    uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");


    uniformAreaLightCount = glGetUniformLocation(shaderID, "areaLightCount");
    uniformLTC1 = glGetUniformLocation(shaderID, "LTC1");
    uniformLTC2 = glGetUniformLocation(shaderID, "LTC2");
    uniformMaterialDiffuse = glGetUniformLocation(shaderID, "material.diffuse");
    uniformskybox = glGetUniformLocation(shaderID, "skybox");


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


void Shader::SetPointLights(PointLight * pLight, int lightCount){

    if(lightCount > values::MAX_POINT_LIGHTS) lightCount = values::MAX_POINT_LIGHTS; 

    glUniform1i(uniformPointLightCount, lightCount); // make sure it is an int ! to go through the loop

    for(int i=0; i < lightCount; i++){

        pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, 
                           uniformPointLight[i].uniformColor, 
                           uniformPointLight[i].uniformDiffuseIntensity, 
                           uniformPointLight[i].uniformPosition,
                           uniformPointLight[i].uniformConstant, 
                           uniformPointLight[i].uniformLinear, 
                           uniformPointLight[i].uniformExponent); 

    }
}


void Shader::SetSpotLights(SpotLight * sLight, int lightCount){

     if(lightCount > values::MAX_SPOT_LIGHTS) lightCount = values::MAX_SPOT_LIGHTS; 

    glUniform1i(uniformSpotLightCount, lightCount); // make sure it is an int ! to go through the loop

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

    }
}


void Shader::SetAreaLights(AreaLight *  aLights, int lightCount){
    if(lightCount > values::MAX_AREA_LIGHTS) lightCount = values::MAX_AREA_LIGHTS; 

    glUniform1i(uniformAreaLightCount, lightCount); // make sure it is an int ! to go through the loop
    
    glUniform1i(uniformMaterialDiffuse, 0);
    glUniform1i(uniformLTC1, 1);
    glUniform1i(uniformLTC2, 2); 

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


void Shader::SetTexture(GLuint textureUnit){
    glUniform1i(uniformTexture, textureUnit);
}


void Shader::SetDirectionalShadowMap(GLuint textureUnit){
    glUniform1i(uniformDirectionalShadowMap, textureUnit);
}


void Shader::SetDirectionalLightTransform(glm::mat4* lTransform){

    glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform)); 
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


Shader::~Shader(){
    ClearShader(); 
}