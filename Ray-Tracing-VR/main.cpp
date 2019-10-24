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

using glm::ivec3;
using glm::ivec2;
using glm::uvec2;
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::quat;
#define FAIL(X) throw std::runtime_error(X)

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
ovrSession session;
ovrGraphicsLuid luid;
// The MAIN function, from here we start the application and run the game loop

GLFWwindow* initGLFW();
void initVR();

template <typename Function>
inline void for_each_eye(Function function) {
	for (ovrEyeType eye = ovrEyeType::ovrEye_Left;
		eye < ovrEyeType::ovrEye_Count;
		eye = static_cast<ovrEyeType>(eye + 1)) {
		function(eye);
	}
}

inline mat4 toGlm(const ovrMatrix4f & om) {
	return glm::transpose(glm::make_mat4(&om.M[0][0]));
}

int main()
{
	GLFWwindow* window = initGLFW();
	initVR();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
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

	ovrHmdDesc desc = ovr_GetHmdDesc(session);
	ovrSizei resolution = desc.Resolution;

	ovr_Destroy(session);
	ovr_Shutdown();
}