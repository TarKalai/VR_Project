project("SRC")

#Put the sources into a variable

# set(SOURCES_EX_13 "exercises/ex13/ex13.cpp")
set(SOURCES_MAIN "main.cpp")

#To use the content of a variable you need to use ${NAME_OF_YOUR_VARIABLE}
#Specify that you want to generate an executable with a certain name using a set of sources

add_executable(${PROJECT_NAME}_main ${SOURCES_MAIN})

#Specify which libraries you want to use with your executable
target_link_libraries(${PROJECT_NAME}_main PUBLIC OpenGL::GL glfw glad
                        BulletDynamics BulletCollision LinearMath)
    

# add_executable(${PROJECT_NAME}_window ${SOURCES_WINDOW})
# target_link_libraries(${PROJECT_NAME}_window PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex03 ${SOURCES_EX_03})
# target_link_libraries(${PROJECT_NAME}_ex03 PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex04 ${SOURCES_EX_04})
# target_link_libraries(${PROJECT_NAME}_ex04 PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex05 ${SOURCES_EX_05})
# target_link_libraries(${PROJECT_NAME}_ex05 PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex06 ${SOURCES_EX_06})
# target_link_libraries(${PROJECT_NAME}_ex06 PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex07 ${SOURCES_EX_07})
# target_link_libraries(${PROJECT_NAME}_ex07 PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex08 ${SOURCES_EX_08})
# target_link_libraries(${PROJECT_NAME}_ex08 PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex09 ${SOURCES_EX_09})
# target_link_libraries(${PROJECT_NAME}_ex09 PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex10 ${SOURCES_EX_10})
# target_link_libraries(${PROJECT_NAME}_ex10 PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex11 ${SOURCES_EX_11})
# target_link_libraries(${PROJECT_NAME}_ex11 PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex12 ${SOURCES_EX_12})
# target_link_libraries(${PROJECT_NAME}_ex12 PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex13 ${SOURCES_EX_13})
# target_link_libraries(${PROJECT_NAME}_ex13 PUBLIC OpenGL::GL glfw glad)


# add_executable(${PROJECT_NAME}_ex14 ${SOURCES_EX_14})
# target_link_libraries(${PROJECT_NAME}_ex14 PUBLIC OpenGL::GL glfw glad)

# if(COMPILE_SOLUTIONS)

# set(SOURCES_EX_01_SOL "solutions/ex01/sol01.cpp")
# set(SOURCES_EX_02_SOL "solutions/ex02/sol02.cpp")
# set(SOURCES_EX_03_SOL "solutions/ex03/sol03.cpp")
# set(SOURCES_EX_04_SOL "solutions/ex04/sol04.cpp")
# set(SOURCES_EX_05_SOL "solutions/ex05/sol05.cpp")
# set(SOURCES_EX_06_SOL "solutions/ex06/sol06.cpp")
# set(SOURCES_EX_07_SOL "solutions/ex07/sol07.cpp")


# set(SOURCES_EX_08_SOL "solutions/ex08/sol08.cpp")
# set(SOURCES_EX_09_SOL "solutions/ex09/sol09.cpp")
# set(SOURCES_EX_10_SOL "solutions/ex10/sol10.cpp")
# set(SOURCES_EX_11_SOL "solutions/ex11/sol11.cpp")
# set(SOURCES_EX_12_SOL "solutions/ex12/sol12.cpp")
# set(SOURCES_EX_13_SOL "solutions/ex13/sol13.cpp")
# set(SOURCES_EX_14_SOL "solutions/ex14/sol14.cpp" "solutions/ex14/camera.h" "solutions/ex14/shader.h")



# add_executable(${PROJECT_NAME}_ex01_sol ${SOURCES_EX_01_SOL})
# target_link_libraries(${PROJECT_NAME}_ex01_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex02_sol ${SOURCES_EX_02_SOL})
# target_link_libraries(${PROJECT_NAME}_ex02_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex03_sol ${SOURCES_EX_03_SOL})
# target_link_libraries(${PROJECT_NAME}_ex03_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex04_sol ${SOURCES_EX_04_SOL})
# target_link_libraries(${PROJECT_NAME}_ex04_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex05_sol ${SOURCES_EX_05_SOL})
# target_link_libraries(${PROJECT_NAME}_ex05_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex06_sol ${SOURCES_EX_06_SOL})
# target_link_libraries(${PROJECT_NAME}_ex06_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex07_sol ${SOURCES_EX_07_SOL})
# target_link_libraries(${PROJECT_NAME}_ex07_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex08_sol ${SOURCES_EX_08_SOL})
# target_link_libraries(${PROJECT_NAME}_ex08_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex09_sol ${SOURCES_EX_09_SOL})
# target_link_libraries(${PROJECT_NAME}_ex09_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex10_sol ${SOURCES_EX_10_SOL})
# target_link_libraries(${PROJECT_NAME}_ex10_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex11_sol ${SOURCES_EX_11_SOL})
# target_link_libraries(${PROJECT_NAME}_ex11_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex12_sol ${SOURCES_EX_12_SOL})
# target_link_libraries(${PROJECT_NAME}_ex12_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex13_sol ${SOURCES_EX_13_SOL})
# target_link_libraries(${PROJECT_NAME}_ex13_sol PUBLIC OpenGL::GL glfw glad)

# add_executable(${PROJECT_NAME}_ex14_sol ${SOURCES_EX_14_SOL})
# target_link_libraries(${PROJECT_NAME}_ex14_sol PUBLIC OpenGL::GL glfw glad)

# endif()