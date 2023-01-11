project("SRC2")

#Put the sources into a variable

set(SOURCES_NORMAL "normal_mapping.cpp" 
"mesh.h" "model.h" "shader.h" "camera.h")

#To use the content of a variable you need to use ${NAME_OF_YOUR_VARIABLE}
#Specify that you want to generate an executable with a certain name using a set of sources

add_executable(${PROJECT_NAME}_main ${SOURCES_NORMAL})
target_link_libraries(${PROJECT_NAME}_main PUBLIC OpenGL::GL glad glfw imgui
                      BulletDynamics BulletCollision LinearMath assimp) #assimp)