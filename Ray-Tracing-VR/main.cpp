#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <algorithm>

const GLint WIDTH = 800, HEIGHT = 600;
ovrSession session;
ovrHmdDesc desc;
ovrGraphicsLuid luid;

GLFWwindow* initGLFW();
void initVR();

int main()
{
	GLFWwindow* window = initGLFW();
	initVR();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ovrTrackingState ts = ovr_GetTrackingState(session, ovr_GetTimeInSeconds(), ovrTrue);

		if (ts.StatusFlags &(ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
		{
			ovrPosef pose = ts.HeadPose.ThePose;
			ovrVector3f position = pose.Position;
			printf("X:%f, Y:%f, Z:%f\n", position.x, position.y, position.z);
		}

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	ovr_Destroy(session);
	ovr_Shutdown();
	return EXIT_SUCCESS;
}

GLFWwindow* initGLFW() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwSwapInterval(0);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	return window;
}

void initVR() {
	ovrResult result = ovr_Initialize(nullptr);
	if (OVR_FAILURE(result))
		return;

	result = ovr_Create(&session, &luid);
	if (OVR_FAILURE(result))
	{
		ovr_Shutdown();
		return;
	}

	desc = ovr_GetHmdDesc(session);
}