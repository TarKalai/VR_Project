project("SRC")

#Put the sources into a variable

set(SOURCES_MAIN "main.cpp" "physics.cpp" "physics.h" "camera.h" "shader.h" "object.h") 

#To use the content of a variable you need to use ${NAME_OF_YOUR_VARIABLE}
#Specify that you want to generate an executable with a certain name using a set of sources

add_executable(${PROJECT_NAME}_main ${SOURCES_MAIN})

#Specify which libraries you want to use with your executable
target_link_libraries(${PROJECT_NAME}_main PUBLIC OpenGL::GL glfw glad
                        BulletDynamics BulletCollision LinearMath)