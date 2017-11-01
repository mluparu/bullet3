INCLUDE_DIRECTORIES(
		.
		../../src
		../gtest-1.7.0/include
        ${BULLET_PHYSICS_SOURCE_DIR}/src
        ${BULLET_PHYSICS_SOURCE_DIR}/btgui
        ${BULLET_PHYSICS_SOURCE_DIR}/examples
        ${BULLET_PHYSICS_SOURCE_DIR}/examples/ThirdPartyLibs/Glew
)


SET(AppBasicExampleGui_SRCS
	main.cpp
	../../examples/BasicDemo/BasicExample.cpp
        ${BULLET_PHYSICS_SOURCE_DIR}/build3/bullet.rc
	../../examples/StandaloneMain/main_opengl_single_example.cpp
        ../../examples/ExampleBrowser/OpenGLGuiHelper.cpp
        ../../examples/ExampleBrowser/GL_ShapeDrawer.cpp
        ../../examples/ExampleBrowser/CollisionShape2TriangleMesh.cpp
		../../examples/Utils/b3Clock.cpp
)

#this define maps StandaloneExampleCreateFunc to the right 'CreateFunc'
ADD_DEFINITIONS(-DB3_USE_STANDALONE_EXAMPLE)
ADD_DEFINITIONS("-DIN_GTEST")

LINK_LIBRARIES(
        BulletDynamics BulletCollision LinearMath OpenGLWindow Bullet3Common gtest
)

#some code to support OpenGL and Glew cross platform
IF (WIN32)
        INCLUDE_DIRECTORIES(
             ${BULLET_PHYSICS_SOURCE_DIR}/btgui/OpenGLWindow/GlewWindows
        )
        ADD_DEFINITIONS(-DGLEW_STATIC)
	LINK_LIBRARIES( ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} )
ELSE(WIN32)
        IF(APPLE)
                find_library(COCOA NAMES Cocoa)
                MESSAGE(${COCOA})
                link_libraries(${COCOA} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})

        ELSE(APPLE)
                INCLUDE_DIRECTORIES(
                        ${BULLET_PHYSICS_SOURCE_DIR}/btgui/OpenGLWindow/GlewWindows
                )
                ADD_DEFINITIONS("-DGLEW_INIT_OPENGL11_FUNCTIONS=1")
                ADD_DEFINITIONS("-DGLEW_STATIC")
                ADD_DEFINITIONS("-DGLEW_DYNAMIC_LOAD_ALL_GLX_FUNCTIONS=1")

                LINK_LIBRARIES(  pthread ${DL} )
        ENDIF(APPLE)
ENDIF(WIN32)


ADD_EXECUTABLE(Test_AppBasicTestGui
                ${AppBasicExampleGui_SRCS}
)

ADD_TEST(Test_AppBasicTestGui_PASS Test_AppBasicTestGui)

IF (INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)
                        SET_TARGET_PROPERTIES(Test_AppBasicTestGui PROPERTIES  DEBUG_POSTFIX "_Debug")
                        SET_TARGET_PROPERTIES(Test_AppBasicTestGui PROPERTIES  MINSIZEREL_POSTFIX "_MinsizeRel")
                        SET_TARGET_PROPERTIES(Test_AppBasicTestGui PROPERTIES  RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo")
ENDIF(INTERNAL_ADD_POSTFIX_EXECUTABLE_NAMES)

