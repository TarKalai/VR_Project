project("SRC")

#Put the sources into a variable

set(SOURCES_REFACTOR "main.cpp" 
"object.cpp" "object.h"
"shader.cpp" "shader.h"
"camera.cpp" "camera.h"
"texture.cpp" "texture.h"
"light.cpp" "light.h"
"material.cpp" "material.h"
"directionalLight.cpp" "directionalLight.h"
"pointLight.cpp" "pointLight.h"
"spotLight.cpp" "spotLight.h"
"shadowMap.cpp" "shadowMap.h"
"areaLight.cpp" "areaLight.h"
"light_constructor.cpp" "light_constructor.h"
"object_constructor.cpp" "object_constructor.h"
"process.cpp" "process.h"
"display.cpp" "display.h"
"physics.cpp" "physics.h"
"gui.cpp" "gui.h"
"shader2D.cpp" "shader2D.h"
"debug.h"
"constant.h")

#To use the content of a variable you need to use ${NAME_OF_YOUR_VARIABLE}
#Specify that you want to generate an executable with a certain name using a set of sources

add_executable(${PROJECT_NAME}_main ${SOURCES_REFACTOR})
target_link_libraries(${PROJECT_NAME}_main PUBLIC OpenGL::GL glad glfw imgui
                      BulletDynamics BulletCollision LinearMath) #assimp)