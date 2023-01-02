project("SRC2")

#Put the sources into a variable

set(SOURCES_MAIN2 "area_lights.cpp" "shader.h" "camera.h" "mesh.h" "model.h" "colors.hpp" "ltc_matrix.hpp") 

#To use the content of a variable you need to use ${NAME_OF_YOUR_VARIABLE}
#Specify that you want to generate an executable with a certain name using a set of sources

add_executable(${PROJECT_NAME}_main ${SOURCES_MAIN2})

#Specify which libraries you want to use with your executable
target_link_libraries(${PROJECT_NAME}_main PUBLIC OpenGL::GL glfw glad assimp
                        BulletDynamics BulletCollision LinearMath)