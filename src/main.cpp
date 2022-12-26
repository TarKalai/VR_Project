#define STB_IMAGE_IMPLEMENTATION


#include<iostream>
#include<cstdlib>



//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_inverse.hpp>


#include "camera.h"
#include "shader.h"
#include "object.h"
#include "physics.h"
// #include "debug.h"

#include "display.h"
#include "process.h"


Display mainWindow; 

char fileVert[128] = "../../src/Shaders/vertSrc.txt";
char fileFrag[128] = "../../src/Shaders/fragSrc.txt";

char groundVertex[128] = "../../src/Shaders/vertGround.txt";
char groundFrag[128] = "../../src/Shaders/fragGround.txt";
char groundImage[128] = "../../image/container.jpg";


char terrainVertex[128] = "../../src/Shaders/vertTerrain.txt";
char terrainFrag[128] = "../../src/Shaders/fragTerrain.txt";
char terrainImage[128] = "../../image/iceland_heightmap.png";

Object object; 


Camera camera(glm::vec3(0.0, 15.0, -25.0), glm::vec3(0.0, 1.0, 0.0), 90.0, -30.);


float getRandom(float from=-4, float to=4) {
	int mod = (to - from)*100;
	return float(rand()%mod + 100*from)/100;
}




int main(int argc, char* argv[]){
	std::cout << "Project is running... " << std::endl;

	mainWindow = Display(); 
	mainWindow.Initialise(); 

	Shader shader(NULL, fileVert, fileFrag, false, true);
	Shader groundShader(groundImage, groundVertex, groundFrag, true, false);
	Shader terrainShader(terrainImage, terrainVertex, terrainFrag, true, false);

	stbi_set_flip_vertically_on_load(true);
	// int width, height, nrChannels;
    // unsigned char *data = stbi_load("resources/heightmaps/iceland_heightmap.png", &width, &height, &nrChannels, 0);
	unsigned char *data = object.data; 
	int height = object.height; 
	int width = object.width; 
    if (data)
    {
        std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

	// set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<float> vertices;
    float yScale = 64.0f / 256.0f, yShift = 16.0f;
    // int rez = 1;
	int rez = object.rez; 
    unsigned bytePerPixel = object.nrChannels;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
            unsigned char y = pixelOffset[0];

            // vertex
            vertices.push_back( -height/2.0f + height*i/(float)height );   // vx
            vertices.push_back( (int) y * yScale - yShift);   // vy
            vertices.push_back( -width/2.0f + width*j/(float)width );   // vz
        }
    }
    std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;
    stbi_image_free(data);

    std::vector<unsigned> indices;
    for(unsigned i = 0; i < height-1; i += rez)
    {
        for(unsigned j = 0; j < width; j += rez)
        {
            for(unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + width * (i + k*rez));
            }
        }
    }
    std::cout << "Loaded " << indices.size() << " indices" << std::endl;

    // const int numStrips = (height-1)/rez;
    // const int numTrisPerStrip = (width/rez)*2-2;

	const int numStrips = object.numStrips; 
	const int numTrisPerStrip = object.numTrisPerStrip; 
	


    std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
    std::cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << std::endl;

    // first, configure the cube's VAO (and terrainVBO + terrainIBO)
    unsigned int terrainVAO, terrainVBO, terrainIBO;
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &terrainIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	

	char sphereGeometry[] = "../../objects/sphere.obj";
	char cubeGeometry[] = "../../objects/cube.obj";
	char groundGeometry[] = "../../objects/ground.obj";
	// char groundGeometry[] = "../../objects/iceland_heightmap.obj";

	Object ground_obj = Object(groundGeometry, glm::vec3(0.), glm::vec3(0.), glm::vec3(1.), 0);
    PhysicalWorld world = PhysicalWorld(&ground_obj); // BULLET3
	groundShader.addObject(&ground_obj);

	Object sphere;
	for (int i=0; i<5; i++) { // went from 40 to 5
		glm::vec3 pos = glm::vec3(getRandom(), 2.+5*i, getRandom());
		glm::vec3 rot = glm::vec3(getRandom(0.,3.14), getRandom(0.,3.14), getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(getRandom(0.5,2.));
		Object* sphere = new Object(sphereGeometry, pos, rot, scale, world.glObjects.size());	
		world.addSphere(sphere);  
		shader.addObject(sphere);
	}
	
	for (int i=0; i<5; i++) { // went from 10 to 5
		glm::vec3 pos = glm::vec3(getRandom(), 2.+5*i, getRandom());
		glm::vec3 rot = glm::vec3(getRandom(0.,3.14), getRandom(0.,3.14), getRandom(0.,3.14));
		glm::vec3 scale = glm::vec3(getRandom(0.5,2.), getRandom(0.5,2.), getRandom(0.5,2.));
		Object* cube = new Object(cubeGeometry, pos, rot, scale, world.glObjects.size());	
		world.addCube(cube);  
		shader.addObject(cube);
	}




	// Object sphere = Object(sphereGeometry, glm::vec3(0., 1., 0.), glm::vec3(0., 0, 0), glm::vec3(1.), world.glObjects.size());	
	// world.addSphere(&sphere);  
	// shader.addObject(&sphere);
	// Object cube = Object(cubeGeometry, glm::vec3(-3.5, 4, 0.), glm::vec3(0., 0, 0), glm::vec3(1., 2, 5), world.glObjects.size());	
	// world.addCube(&cube);
	// shader.addObject(&cube);

	//2. Choose a position for the light
	const glm::vec3 light_pos = glm::vec3(0.5, 2.5, -0.7);


	double prev = 0;
	int deltaFrame = 0;
	//fps function
	auto fps = [&](double now) {
		double deltaTime = now - prev;
		deltaFrame++;
		if (deltaTime > 0.5) {
			prev = now;
			const double fpsCount = (double)deltaFrame / deltaTime;
			deltaFrame = 0;
			std::cout << "\r FPS: " << fpsCount;
		}
	};



	// glm::mat4 view = camera.getViewMatrix();
	// // printf("camera value: %f", camera.ZOOM); 
	// glm::mat4 perspective = camera.getProjectionMatrix(glm::radians(camera.ZOOM), mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), 0.01, 100.0);



	//Rendering
	glfwSwapInterval(1);
	Process process = Process();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // only show the vertexes

    glfwSetWindowUserPointer(mainWindow.getWindow(), reinterpret_cast<void *>(&camera));


	while (!mainWindow.getShouldClose()){

		// BULLET3
		world.animate();

		process.processInput(mainWindow.getWindow(), camera);
		glm::mat4 view = camera.getViewMatrix();
		// printf("camera value: %f", camera.ZOOM); 
		glm::mat4 perspective = camera.getProjectionMatrix(glm::radians(camera.ZOOM), mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), 0.01, 100.0);
		view = camera.getViewMatrix();
		glfwPollEvents();
		double now = glfwGetTime();
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// draw mesh
	

		//2. Use the shader Class to send the relevant uniform
		shader.DrawObjects(view, perspective, light_pos);
		groundShader.DrawObjects(view, perspective, light_pos); // Create DrawGround ? 
		terrainShader.use(); 
		terrainShader.setMatrix4("view",view);
        terrainShader.setMatrix4("projection", perspective);
        terrainShader.setMatrix4("model", object.model);


		glBindVertexArray(terrainVAO);
		// render the mesh triangle strip by triangle strip - each row at a time
		for(unsigned int strip = 0; strip < numStrips; ++strip)
		{
			glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
							numTrisPerStrip+2, // number of indices to render
							GL_UNSIGNED_INT,     // index data type
							(void*)(sizeof(unsigned) * (numTrisPerStrip+2) * strip)); // offset to starting index
		}

		
		fps(now);
		mainWindow.swapBuffers(); 

		

	}

	glDeleteVertexArrays(1, &terrainVAO);
	glDeleteBuffers(1, &terrainVBO);
	glDeleteBuffers(1, &terrainIBO);

	// BULLET3
	world.clear();

	return 0;
}
