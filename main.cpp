// Std. Includes
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Image.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>


using namespace std;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
bool Collision(Camera &camera);



// The MAIN function, from here we start our application and run our Game loop
int main()
{
	// Properties
	GLuint screenWidth = 800, screenHeight = 600;
	
	// Camera
	Camera camera(glm::vec3(0.0f, 0.4f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH);
	bool keys[1024];
	GLfloat lastX = 400, lastY = 300;
	bool firstMouse = true;	
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Austin3D", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	Shader ourShader("cubes.vs", "cubes.frag");

	// Set up our vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		

		// elevator double doors exit
		-26.66f, -3.0f, -77.33f,  0.0f, 0.0f, // bottom left point
		-20.33f,  -3.0f, -77.33f,  1.0f, 0.0f, // bottom right point
		-20.33f,   3.0f, -77.33f,  1.0f, 1.0f, // top right point (shared)
		-20.33f,   3.0f, -77.33f,  1.0f, 1.0f, // top right point (shared)
		-26.66f,  3.0f, -77.33f,  0.0f, 1.0f, // top left point
		-26.66f, -3.0f, -77.33f,  0.0f, 0.0f, // bottom left point


		// offices top of door
		-0.6f, 2.0f, -74.33f,  0.0f, 0.0f, // bottom left point
		1.33f, 2.0f, -74.33f,  1.0f, 0.0f, // bottom right point
		1.33f,  3.0f, -74.33f,  1.0f, 1.0f, // top right point (shared)
		1.33f,  3.0f, -74.33f,  1.0f, 1.0f, // top right point (shared)
		-0.6f,  3.0f, -74.33f,  0.0f, 1.0f, // top left point
		-0.6f, 2.0f, -74.33f,  0.0f, 0.0f, // bottom left point

		//left wall top of 213
		-3.0f,  3.0f, -39.34f,  1.0f, 1.0f, // top point closest to me
		-3.0f,  3.0f, -41.34f,  0.0f, 1.0f, // top point furthest from me
		-3.0f,  2.0f, -41.34f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f,  2.0f, -41.34f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f,  2.0f, -39.34f,   1.0f, 0.0f, // bottom point closest to me
		-3.0f,  3.0f, -39.34f,  1.0f, 1.0f, // top point closest to me

		// right wall door top 206
		3.0f, 3.0f, -24.96f, 1.0f, 1.0f, // top point closest to me
		3.0f, 3.0f, -26.96f, 0.0f, 1.0f, // top point furthest from me
		3.0f, 2.0f, -26.96f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, 2.0f, -26.96f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, 2.0f, -24.96f, 1.0f, 0.0f, // bottom point closest to me
		3.0f, 3.0f, -24.96f, 1.0f, 1.0f, // top point closest to me

		// right wall top door 208
		3.0f, 3.0f, -40.44f, 1.0f, 1.0f, // top point closest to me
		3.0f, 3.0f, -42.44f, 0.0f, 1.0f, // top point furthest from me
		3.0f, 2.0f, -42.44f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, 2.0f, -42.44f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, 2.0f, -40.44f, 1.0f, 0.0f, // bottom point closest to me
		3.0f, 3.0f, -40.44f, 1.0f, 1.0f, // top point closest to me

		// right wall top door 210
		3.0f, 3.0f, -55.87f, 1.0f, 1.0f, // top point closest to me
		3.0f, 3.0f, -57.87f, 0.0f, 1.0f, // top point furthest from me
		3.0f, 2.0f, -57.87f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, 2.0f, -57.87f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, 2.0f, -55.87f, 1.0f, 0.0f, // bottom point closest to me
		3.0f, 3.0f, -55.87f, 1.0f, 1.0f, // top point closest to me

		// offices right wall begin
		1.37f,  3.0f,  -74.33f,  1.0f, 1.0f, // top point closest to me
		1.37f,  3.0f, -79.0f,  0.0f, 1.0f, // top point furthest from me
		1.37f, -3.0f, -79.0f,  0.0f, 0.0f, // bottom point furthest from me
		1.37f, -3.0f, -79.00f,  0.0f, 0.0f, // bottom point furthest from me
		1.37f, -3.0f,  -74.33f,  1.0f, 0.0f, // bottom point closest to me
		1.37f,  3.0f,  -74.33f,  1.0f, 1.0f, // top point closest to me

		// offices jones wall
		1.37f,  3.0f,  -81.0f,  1.0f, 1.0f, // top point closest to me
		1.37f,  3.0f, -82.0f,  0.0f, 1.0f, // top point furthest from me
		1.37f, -3.0f, -82.0f,  0.0f, 0.0f, // bottom point furthest from me
		1.37f, -3.0f, -82.0f,  0.0f, 0.0f, // bottom point furthest from me
		1.37f, -3.0f,  -81.0f,  1.0f, 0.0f, // bottom point closest to me
		1.37f,  3.0f,  -81.0f,  1.0f, 1.0f, // top point closest to me

		// offices right wall to end
		1.37f,  3.0f,  -84.0f,  1.0f, 1.0f, // top point closest to me
		1.37f,  3.0f, -88.33f,  0.0f, 1.0f, // top point furthest from me
		1.37f, -3.0f, -88.33f,  0.0f, 0.0f, // bottom point furthest from me
		1.37f, -3.0f, -88.33f,  0.0f, 0.0f, // bottom point furthest from me
		1.37f, -3.0f,  -84.0f,  1.0f, 0.0f, // bottom point closest to me
		1.37f,  3.0f,  -84.0f,  1.0f, 1.0f, // top point closest to me
		// offices barrier right wall furthest
		-1.296f,  3.0f,  -87.496f,  1.0f, 1.0f, // top point closest to me
		-1.296f,  3.0f, -88.33f,  0.0f, 1.0f, // top point furthest from me
		-1.296f, -3.0f, -88.33f,  0.0f, 0.0f, // bottom point furthest from me
		-1.296f, -3.0f, -88.33f,  0.0f, 0.0f, // bottom point furthest from me
		-1.296f, -3.0f,  -87.496f,  1.0f, 0.0f, // bottom point closest to me
		-1.296f,  3.0f,  -87.496f,  1.0f, 1.0f, // top point closest to me
		// offices barrier right wall closest
		-1.296f,  3.0f,  -84.66f,  1.0f, 1.0f, // top point closest to me
		-1.296f,  3.0f, -85.49f,  0.0f, 1.0f, // top point furthest from me
		-1.296f, -3.0f, -85.49f,  0.0f, 0.0f, // bottom point furthest from me
		-1.296f, -3.0f, -85.49f,  0.0f, 0.0f, // bottom point furthest from me
		-1.296f, -3.0f,  -84.66f,  1.0f, 0.0f, // bottom point closest to me
		-1.296f,  3.0f,  -84.66f,  1.0f, 1.0f, // top point closest to me
		// offices barrier left wall
		-8.96f,  3.0f,  -84.66f,  1.0f, 1.0f, // top point closest to me
		-8.96f,  3.0f, -88.33f,  0.0f, 1.0f, // top point furthest from me
		-8.96f, -3.0f, -88.33f,  0.0f, 0.0f, // bottom point furthest from me
		-8.96f, -3.0f, -88.33f,  0.0f, 0.0f, // bottom point furthest from me
		-8.96f, -3.0f,  -84.66f,  1.0f, 0.0f, // bottom point closest to me
		-8.96f,  3.0f,  -84.66f,  1.0f, 1.0f, // top point closest to me
		// offices barrier wall
		-8.96f, -3.0f, -84.66f, 0.0f, 0.0f,//bottom left point
		-1.296f, -3.0f, -84.66f, 1.0f, 0.0f,// bottom right point
		-1.296f,  3.0f, -84.66f, 1.0f, 1.0f,// top right point
		-1.296f,  3.0f, -84.66f, 1.0f, 1.0f,// top right point
		-8.96f,  3.0f, -84.66f, 0.0f, 1.0f,// top left point
		-8.96f, -3.0f, -84.66f, 0.0f, 0.0f,//bottom left point
		// offices left wall at desk back wall
		-8.96f,  3.0f,  -74.33f,  1.0f, 1.0f, // top point closest to me
		-8.96f,  3.0f, -75.0f,  0.0f, 1.0f, // top point furthest from me
		-8.96f, -3.0f, -75.0f,  0.0f, 0.0f, // bottom point furthest from me
		-8.96f, -3.0f, -75.00f,  0.0f, 0.0f, // bottom point furthest from me
		-8.96f, -3.0f,  -74.33f,  1.0f, 0.0f, // bottom point closest to me
		-8.96f,  3.0f,  -74.33f,  1.0f, 1.0f, // top point closest to me
		// offices left wall start from neff
		-11.63f,  3.0f,  -75.0f,  1.0f, 1.0f, // top point closest to me
		-11.63f,  3.0f, -79.0f,  0.0f, 1.0f, // top point furthest from me
		-11.63f, -3.0f, -79.0f,  0.0f, 0.0f, // bottom point furthest from me
		-11.63f, -3.0f, -79.00f,  0.0f, 0.0f, // bottom point furthest from me
		-11.63f, -3.0f,  -75.0f,  1.0f, 0.0f, // bottom point closest to me
		-11.63f,  3.0f,  -75.0f,  1.0f, 1.0f, // top point closest to me
		// offices left wall in between doors
		-11.63f,  3.0f,  -81.0f,  1.0f, 1.0f, // top point closest to me
		-11.63f,  3.0f, -82.0f,  0.0f, 1.0f, // top point furthest from me
		-11.63f, -3.0f, -82.0f,  0.0f, 0.0f, // bottom point furthest from me
		-11.63f, -3.0f, -82.0f,  0.0f, 0.0f, // bottom point furthest from me
		-11.63f, -3.0f,  -81.0f,  1.0f, 0.0f, // bottom point closest to me
		-11.63f,  3.0f,  -81.0f,  1.0f, 1.0f, // top point closest to me
		// offices left wall to end
		-11.63f,  3.0f,  -84.0f,  1.0f, 1.0f, // top point closest to me
		-11.63f,  3.0f, -88.33f,  0.0f, 1.0f, // top point furthest from me
		-11.63f, -3.0f, -88.33f,  0.0f, 0.0f, // bottom point furthest from me
		-11.63f, -3.0f, -88.33f,  0.0f, 0.0f, // bottom point furthest from me
		-11.63f, -3.0f,  -84.0f,  1.0f, 0.0f, // bottom point closest to me
		-11.63f,  3.0f,  -84.0f,  1.0f, 1.0f, // top point closest to me
		

		//left wall begin
		-3.0f,  3.0f, -11.67f,  1.0f, 1.0f, // top point closest to me
		-3.0f,  3.0f, -13.67f,  0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -13.67f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -13.67f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -11.67f,   1.0f, 0.0f, // bottom point closest to me
		-3.0f,  3.0f, -11.67f,  1.0f, 1.0f, // top point closest to me

		//left wall between 209 and 211
		-3.0f,  3.0f, -15.67f,  1.0f, 1.0f, // top point closest to me
		-3.0f,  3.0f, -20.34f,  0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -20.34f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -20.34f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -15.67f,   1.0f, 0.0f, // bottom point closest to me
		-3.0f,  3.0f, -15.67f,  1.0f, 1.0f, // top point closest to me

		//left wall between 211 and 213
		-3.0f,  3.0f, -22.34f,  1.0f, 1.0f, // top point closest to me
		-3.0f,  3.0f, -39.34f,  0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -39.34f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -39.34f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -22.34f,   1.0f, 0.0f, // bottom point closest to me
		-3.0f,  3.0f, -22.34f,  1.0f, 1.0f, // top point closest to me

		//left wall between 213 and 215
		-3.0f,  3.0f, -41.34f,  1.0f, 1.0f, // top point closest to me
		-3.0f,  3.0f, -44.67f,  0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -44.67f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -44.67f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -41.34f,   1.0f, 0.0f, // bottom point closest to me
		-3.0f,  3.0f, -41.34f,  1.0f, 1.0f, // top point closest to me

		//left wall between 215 and 217
		-3.0f,  3.0f, -46.67,  1.0f, 1.0f, // top point closest to me
		-3.0f,  3.0f, -47.67f,  0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -47.67f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -47.67f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -46.67f,   1.0f, 0.0f, // bottom point closest to me
		-3.0f,  3.0f, -46.67f,  1.0f, 1.0f, // top point closest to me

		//left wall between 217 and 219
		-3.0f,  3.0f, -49.67f,  1.0f, 1.0f, // top point closest to me
		-3.0f,  3.0f, -59.00f,  0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -59.00f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -59.00f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -49.67f,   1.0f, 0.0f, // bottom point closest to me
		-3.0f,  3.0f, -49.67f,  1.0f, 1.0f, // top point closest to me

		//left wall 219 to end
		-3.0f,  3.0f, -61.00f,  1.0f, 1.0f, // top point closest to me
		-3.0f,  3.0f, -64.66f,  0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -64.66f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -64.66f,  0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -61.00f,   1.0f, 0.0f, // bottom point closest to me
		-3.0f,  3.0f, -61.00f,  1.0f, 1.0f, // top point closest to me


		// downstairs hall wall
		-20.66f, -3.0f, -7.0f, 0.0f, 0.0f,//bottom left point
		-3.0f, -3.0f, -7.0f, 1.0f, 0.0f,// bottom right point
		-3.0f,  3.0f, -7.0f, 1.0f, 1.0f,// top right point
		-3.0f,  3.0f, -7.0f, 1.0f, 1.0f,// top right point
		-20.66f,  3.0f, -7.0f, 0.0f, 1.0f,// top left point
		-20.66f, -3.0f, -7.0f, 0.0f, 0.0f,//bottom left point

		// bathroom wall
		-13.49f, -3.0f, -11.67f, 0.0f, 0.0f,//bottom left point
		-3.0f, -3.0f, -11.67f, 1.0f, 0.0f,// bottom right point
		-3.0f,  3.0f, -11.67f, 1.0f, 1.0f,// top right point
		-3.0f,  3.0f, -11.67f, 1.0f, 1.0f,// top right point
		-13.49f,  3.0f, -11.67f, 0.0f, 1.0f,// top left point
		-13.49f, -3.0f, -11.67f, 0.0f, 0.0f,//bottom left point
		
		// bathroom wall middle
		-20.32f, -3.0f, -11.67f, 0.0f, 0.0f,//bottom left point
		-15.99f, -3.0f, -11.67f, 1.0f, 0.0f,// bottom right point
		-15.99f, 3.0f, -11.67f, 1.0f, 1.0f,// top right point
		-15.99f, 3.0f, -11.67f, 1.0f, 1.0f,// top right point
		-20.32f, 3.0f, -11.67f, 0.0f, 1.0f,// top left point
		-20.32f, -3.0f, -11.67f, 0.0f, 0.0f,//bottom left point

		// bathroom wall to end
		-26.66f, -3.0f, -11.67f, 0.0f, 0.0f,//bottom left point
		-22.82f, -3.0f, -11.67f, 1.0f, 0.0f,// bottom right point
		-22.82f, 3.0f, -11.67f, 1.0f, 1.0f,// top right point
		-22.82f, 3.0f, -11.67f, 1.0f, 1.0f,// top right point
		-26.66f, 3.0f, -11.67f, 0.0f, 1.0f,// top left point
		-26.66f, -3.0f, -11.67f, 0.0f, 0.0f,//bottom left point
		// windows lab backdoor wall
		-23.88f, -3.0f, -64.66f, 0.0f, 0.0f,//bottom left point
		-3.0f, -3.0f, -64.66f, 1.0f, 0.0f,// bottom right point
		-3.0f,  3.0f, -64.66f, 1.0f, 1.0f,// top right point
		-3.0f,  3.0f, -64.66f, 1.0f, 1.0f,// top right point
		-23.88f,  3.0f, -64.66f, 0.0f, 1.0f,// top left point
		-23.88f, -3.0f, -64.66f, 0.0f, 0.0f,//bottom left point
		// windows wall backdoor wall to end
		-26.66f, -3.0f, -64.66f, 0.0f, 0.0f,//bottom left point
		-25.88f, -3.0f, -64.66f, 1.0f, 0.0f,// bottom right point
		-25.88f, 3.0f, -64.66f, 1.0f, 1.0f,// top right point
		-25.88f, 3.0f, -64.66f, 1.0f, 1.0f,// top right point
		-26.66f, 3.0f, -64.66f, 0.0f, 1.0f,// top left point
		-26.66f, -3.0f, -64.66f, 0.0f, 0.0f,//bottom left point

		// offices/microwave shared wall
		-9.0f, -3.0f, -74.33f, 0.0f, 0.0f,//bottom left point
		-0.6f, -3.0f, -74.33f, 1.0f, 0.0f,// bottom right point
		-0.6f,  3.0f, -74.33f, 1.0f, 1.0f,// top right point
		-0.6f,  3.0f, -74.33f, 1.0f, 1.0f,// top right point
		-9.0f,  3.0f, -74.33f, 0.0f, 1.0f,// top left point
		-9.0f, -3.0f, -74.33f, 0.0f, 0.0f,//bottom left point

		//  microwave wall
		-9.0f,  3.0f, -69.66f,  1.0f, 1.0f, // top point closest to me
		-9.0f,  3.0f, -74.33f,  0.0f, 1.0f, // top point furthest from me
		-9.0f, -3.0f, -74.33f,  0.0f, 0.0f, // bottom point furthest from me
		-9.0f, -3.0f, -74.33f,  0.0f, 0.0f, // bottom point furthest from me
		-9.0f, -3.0f, -69.66f,   1.0f, 0.0f, // bottom point closest to me
		-9.0f,  3.0f, -69.66f,  1.0f, 1.0f, // top point closest to me

		// windows lab backdoor wall parallel
		-20.66f, -3.0f, -69.66f, 0.0f, 0.0f,//bottom left point
		-9.0f, -3.0f, -69.66f, 1.0f, 0.0f,// bottom right point
		-9.0f,  3.0f, -69.66f, 1.0f, 1.0f,// top right point
		-9.0f,  3.0f, -69.66f, 1.0f, 1.0f,// top right point
		-20.66f,  3.0f, -69.66f, 0.0f, 1.0f,// top left point
		-20.66f, -3.0f, -69.66f, 0.0f, 0.0f,//bottom left point

		// elevator wall
		-26.66f,  3.0f, -64.66f,  1.0f, 1.0f, // top point closest to me
		-26.66f,  3.0f, -68.0f,  0.0f, 1.0f, // top point furthest from me
		-26.66f, -3.0f, -68.0f,  0.0f, 0.0f, // bottom point furthest from me
		-26.66f, -3.0f, -68.0f,  0.0f, 0.0f, // bottom point furthest from me
		-26.66f, -3.0f, -64.66f,  1.0f, 0.0f, // bottom point closest to me
		-26.66f,  3.0f, -64.66f,  1.0f, 1.0f, // top point closest to me
		
		// elevator wall to end
		-26.66f, 3.0f, -72.66f, 1.0f, 1.0f, // top point closest to me
		-26.66f, 3.0f, -77.33f, 0.0f, 1.0f, // top point furthest from me
		-26.66f, -3.0f, -77.33f, 0.0f, 0.0f, // bottom point furthest from me
		-26.66f, -3.0f, -77.33f, 0.0f, 0.0f, // bottom point furthest from me
		-26.66f, -3.0f, -72.66f, 1.0f, 0.0f, // bottom point closest to me
		-26.66f, 3.0f, -72.66f, 1.0f, 1.0f, // top point closest to me

		// wall parallel to microwave wall
		-20.66f,  3.0f, -69.66f,  1.0f, 1.0f, // top point closest to me
		-20.66f,  3.0f, -77.33f,  0.0f, 1.0f, // top point furthest from me
		-20.66f, -3.0f, -77.33f,  0.0f, 0.0f, // bottom point furthest from me
		-20.66f, -3.0f, -77.33f,  0.0f, 0.0f, // bottom point furthest from me
		-20.66f, -3.0f, -69.66f,   1.0f, 0.0f, // bottom point closest to me
		-20.66f,  3.0f, -69.66f,  1.0f, 1.0f, // top point closest to me



		//right wall begin
		3.0f,  3.0f,  -7.0f,  1.0f, 1.0f, // top point closest to me
		3.0f,  3.0f, -22.3f,  0.0f, 1.0f, // top point furthest from me
		3.0f, -3.0f, -22.3f,  0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, -22.3f,  0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f,  -7.0f,  1.0f, 0.0f, // bottom point closest to me
		3.0f,  3.0f,  -7.0f,  1.0f, 1.0f, // top point closest to me

		// right sliver between 206 and 204
		3.0f, 3.0f, -24.3f, 1.0f, 1.0f, // top point closest to me
		3.0f, 3.0f, -24.96f, 0.0f, 1.0f, // top point furthest from me
		3.0f, -3.0f, -24.96f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, -24.96f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, -24.3f, 1.0f, 0.0f, // bottom point closest to me
		3.0f, 3.0f, -24.3f, 1.0f, 1.0f, // top point closest to me


		//right wall continue 206
		3.0f, 3.0f, -26.96, 1.0f, 1.0f, // top point closest to me
		3.0f, 3.0f, -40.44f, 0.0f, 1.0f, // top point furthest from me
		3.0f, -3.0f, -40.44f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, -40.44f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, -26.96f, 1.0f, 0.0f, // bottom point closest to me
		3.0f, 3.0f, -26.96f, 1.0f, 1.0f, // top point closest to me


		//right wall continue 208
		3.0f, 3.0f, -42.44, 1.0f, 1.0f, // top point closest to me
		3.0f, 3.0f, -55.87f, 0.0f, 1.0f, // top point furthest from me
		3.0f, -3.0f, -55.87f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, -55.87f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, -42.44f, 1.0f, 0.0f, // bottom point closest to me
		3.0f, 3.0f, -42.44f, 1.0f, 1.0f, // top point closest to me


		//right wall continue 210
		3.0f, 3.0f, -57.87, 1.0f, 1.0f, // top point closest to me
		3.0f, 3.0f, -71.66f, 0.0f, 1.0f, // top point furthest from me
		3.0f, -3.0f, -71.66f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, -71.66f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, -57.87f, 1.0f, 0.0f, // bottom point closest to me
		3.0f, 3.0f, -57.87f, 1.0f, 1.0f, // top point closest to me



		

		// hallway to 214, right wall
		3.0f, -3.0f, -71.66f, 0.0f, 0.0f,//bottom left point
		9.0f, -3.0f, -71.66f, 1.0f, 0.0f,// bottom right point
		9.0f,  3.0f, -71.66f, 1.0f, 1.0f,// top right point
		9.0f,  3.0f, -71.66f, 1.0f, 1.0f,// top right point
		3.0f,  3.0f, -71.66f, 0.0f, 1.0f,// top left point3
		3.0f, -3.0f, -71.66f, 0.0f, 0.0f,//bottom left point
		
		// hallway to 214, left wall
		1.33f, -3.0f, -74.33f, 0.0f, 0.0f,//bottom left point
		9.0f, -3.0f, -74.33f, 1.0f, 0.0f,// bottom right point
		9.0f,  3.0f, -74.33f, 1.0f, 1.0f,// top right point
		9.0f,  3.0f, -74.33f, 1.0f, 1.0f,// top right point
		1.33f,  3.0f, -74.33f, 0.0f, 1.0f,// top left point3
		1.33f, -3.0f, -74.33f, 0.0f, 0.0f,//bottom left point



		// floor main hallway
		3.0f, -3.0f, 0.5f, 0.0f, 0.0f,// closest right point
		-3.0f, -3.0f, 0.5f, 0.0f, 4.0f,// closest left point
		-3.0f, -3.0f, -74.33f, 55.75f, 4.0f,// furthest left point
		-3.0f, -3.0f, -74.33f, 55.75f, 4.0f,// furthest left point
		3.0f, -3.0f, -74.33f, 55.75f, 0.0f,// furthest right point
		3.0f, -3.0f, 0.5f, 0.0f, 0.0f,// closest right point

		// bathroom floor
		-3.0f, -3.0f, -7.0f, 0.0f, 0.0f,// closest left point		
		-3.0f, -3.0f, -11.67f, 0.0f, 3.75f,// closest right point
		-26.66f, -3.0f, -11.67f, 13.75f, 3.75f,// furthest right point
		-26.66f, -3.0f, -11.67f, 13.75f, 3.75f,// furthest right point
		-26.66f, -3.0f, -7.0f, 13.75f, 0.0f,// furthest left point
		-3.0f, -3.0f, -7.0f, 0.0f, 0.0f,// closest left point	
	
		// start floor 204 hallway
		3.0f, -3.0f, -74.33f,  0.0f, 0.0f,// furthest left point
		9.0f, -3.0f, -74.33f,  4.5f, 0.0f,// furthest right point
		9.0f, -3.0f, -71.66f,  4.5f, 2.0f,// closest right point
		9.0f, -3.0f, -71.66f,  4.5f, 2.0f,// closest right point
		3.0f, -3.0f, -71.66f,  0.0f, 2.0f,// closest left point
		3.0f, -3.0f, -74.33f,  0.0f, 0.0f,// furthest left point

		// elevator alley floor
		-26.66f,-3.0f, -77.33f, 0.0f, 0.0f, // furthest left point
		-20.33f, -3.0f, -77.33f, 0.0f, 4.5f, // furthest right point
		-20.33f, -3.0f, -64.66f, 9.5f, 4.5f, // closest right point
		-20.33f, -3.0f, -64.66f, 9.5f, 4.5f, // closest right point
		-26.66f,-3.0f, -64.66f, 9.5f, 0.0f, // closest left point
		-26.66f,-3.0f, -77.33f, 0.0f, 0.0f, //furthest left point


		// windows lab backdoor floor
		-9.0f, -3.0f, -64.66f, 0.0f, 0.0f,// closest left point		
		-9.0f, -3.0f, -69.66f, 0.0f, 3.75f,// closest right point
		-20.33f, -3.0f, -69.66f, 8.5f, 3.75f,// furthest right point
		-20.33f, -3.0f, -69.66f, 8.5f, 3.75f,// furthest right point
		-20.33f, -3.0f, -64.66f, 8.5f, 0.0f,// furthest left point
		-9.0f, -3.0f, -64.66f, 0.0f, 0.0f,// closest left point		

		// microwave floor
		-3.0f, -3.0f, -64.66f, 0.0f, 0.0f,// closest left point		
		-3.0f, -3.0f, -74.33f, 0.0f, 7.25f,// closest right point
		-9.0f, -3.0f, -74.33f, 3.5f, 7.25f,// furthest right point
		-9.0f, -3.0f, -74.33f, 3.5f, 7.25f,// furthest right point
		-9.0f, -3.0f, -64.66f, 3.5f, 0.0f,// furthest left point
		-3.0f, -3.0f, -64.66f, 0.0f, 0.0f,// closest left point	



		// ceiling main hallway
		-3.0f, 3.0f, -74.33f,  0.0f, 0.0f, // furthest left point
		3.0f,  3.0f, -74.33f,  0.0f, 1.0f, // furthest right point
		3.0f,  3.0f,   0.5f,  20.0f, 1.0f, // closest right point
		3.0f,  3.0f,   0.5f,  20.0f, 1.0f, // closest right point
		-3.0f, 3.0f,   0.5f,  20.0f, 0.0f, // closest left point
		-3.0f, 3.0f, -74.33f,  0.0f, 0.0f, // furthest left point

		// ceiling bathroom
		-3.0f,   3.0f, -7.0f, 0.0f, 0.0f,// closest left point		
		-3.0f,   3.0f, -11.67f, 0.0f, 1.0f,// closest right point
		-26.66f, 3.0f, -11.67f, 6.36f, 1.0f,// furthest right point
		-26.66f, 3.0f, -11.67f, 6.36f, 1.0f,// furthest right point
		-26.66f, 3.0f, -7.0f, 6.36f, 0.0f,// furthest left point
		-3.0f,   3.0f, -7.0f, 0.0f, 0.0f,// closest left point	

		//start ceiling 204 hallway
		3.0f,  3.0f, -74.33f,  0.0f, 0.0f,// furthest left point
		9.0f,  3.0f, -74.33f,  3.0f, 0.0f,// furthest right point
		9.0f,  3.0f, -71.66f,  3.0f, 1.0f,// closest right point
		9.0f,  3.0f, -71.66f,  3.0f, 1.0f,// closest right point
		3.0f,  3.0f, -71.66f,  0.0f, 1.0f,// closest left point
		3.0f,  3.0f, -74.33f,  0.0f, 0.0f,// furthest left point

		//microwave ceiling
		-3.0f, 3.0f, -69.66f, 0.0f, 0.0f,// closest left point	
		-3.0f, 3.0f, -74.33f, 0.0f, 1.0f,// closest right point
		-9.0f, 3.0f, -74.33f, 1.61f, 1.0f,// furthest right point
		-9.0f, 3.0f, -74.33f, 1.61f, 1.0f,// furthest right point
		-9.0f, 3.0f, -69.66f, 1.61f, 0.0f,// furthest left point
		-3.0f, 3.0f, -69.66f, 0.0f, 0.0f,// closest left point	


		// windows lab backdoor ceiling
		-3.0f,   3.0f, -64.66f, 0.0f, 0.0f,// closest left point		
		-3.0f,   3.0f, -69.66f, 0.0f, 1.0f,// closest right point
		-26.66f, 3.0f, -69.66f, 6.36f, 1.0f,// furthest right point
		-26.66f, 3.0f, -69.66f, 6.36f, 1.0f,// furthest right point
		-26.66f, 3.0f, -64.66f, 6.36f, 0.0f,// furthest left point
		-3.0f,   3.0f, -64.66f, 0.0f, 0.0f,// closest left point	

		// elevator ceiling
		-26.66f,  3.0f, -77.33f, 0.0f, 0.0f, // furthest left point
		-20.33f,  3.0f, -77.33f, 0.0f, 1.0f, // furthest right point
		-20.33f,  3.0f, -69.66f, 2.06f, 1.0f, // closest right point
		-20.33f,  3.0f, -69.66f, 2.06f, 1.0f, // closest right point
		-26.66f,  3.0f, -69.66f, 2.06f, 0.0f, // closest left point
		-26.66f,  3.0f, -77.33f, 0.0f, 0.0f, //furthest left point


		// single door 206
		3.0f, 3.0f, -22.3f, 1.0f, 1.0f, // top point closest to me
		3.0f, 3.0f, -24.3f, 0.0f, 1.0f, // top point furthest from me
		3.0f, -3.0f, -24.3f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, -24.3f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, -22.3f, 1.0f, 0.0f, // bottom point closest to me
		3.0f, 3.0f, -22.3f, 1.0f, 1.0f, // top point closest to me


		// single door 217
		-3.0f, 3.0f, -47.67, 1.0f, 1.0f, // top point closest to me
		-3.0f, 3.0f, -49.67f, 0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -49.67f, 0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -49.67f, 0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -47.67f, 1.0f, 0.0f, // bottom point closest to me
		-3.0f, 3.0f, -47.67f, 1.0f, 1.0f, // top point closest to me

		// single door linux lab								 
		-13.67f, 3.0f, -41.74f, 1.0f, 1.0f, // top point closest to me
		-15.67f, 3.0f, -41.74f, 0.0f, 1.0f, // top point furthest from me
		-15.67f, -3.0f, -41.74f, 0.0f, 0.0f, // bottom point furthest from me
		-15.67f, -3.0f, -41.74f, 0.0f, 0.0f, // bottom point furthest from me
		-13.67f, -3.0f, -41.74f, 1.0f, 0.0f, // bottom point closest to me
		-13.67f, 3.0f, -41.74f, 1.0f, 1.0f, // top point closest to me
		


		//carpet 214
		23.67f, -3.0f, -71.26f, 0.0f, 0.0f,// closest right point
		9.0f, -3.0f, -71.26f, 0.0f, 5.5f,// closest left point
		9.0f, -3.0f, -94.53f, 8.88f, 5.5f,// furthest left point
		9.0f, -3.0f, -94.53f, 8.88f, 5.5f,// furthest left point
		23.67f, -3.0f, -94.53f, 8.88f, 0.0f,// furthest right point
		23.67f, -3.0f, -71.26f, 0.0f, 0.0f,// closest right point

		//carpet 210
		24.0f, -3.0f, -55.47f, 0.0f, 0.0f,// closest right point
		3.0f, -3.0f, -55.47f, 0.0f, 7.88f,// closest left point
		3.0f, -3.0f, -70.14f, 5.5f, 7.88f,// furthest left point
		3.0f, -3.0f, -70.14f, 5.5f, 7.88f,// furthest left point
		24.0f, -3.0f, -70.14f, 5.5f, 0.0f,// furthest right point
		24.0f, -3.0f, -55.47f, 0.0f, 0.0f,// closest right point

		//carpet 208
		24.0f, -3.0f, -40.04f, 0.0f, 0.0f,// closest right point
		3.0f, -3.0f, -40.04f, 0.0f, 7.88f,// closest left point
		3.0f, -3.0f, -54.71f, 5.5f, 7.88f,// furthest left point
		3.0f, -3.0f, -54.71f, 5.5f, 7.88f,// furthest left point
		24.0f, -3.0f, -54.71f, 5.5f, 0.0f,// furthest right point
		24.0f, -3.0f, -40.04f, 0.0f, 0.0f,// closest right point

		//carpet 206
		24.0f, -3.0f, -24.56f, 0.0f, 0.0f,// closest right point
		3.0f, -3.0f, -24.56f, 0.0f, 7.88f,// closest left point
		3.0f, -3.0f, -39.23f, 5.5f, 7.88f,// furthest left point
		3.0f, -3.0f, -39.23f, 5.5f, 7.88f,// furthest left point
		24.0f, -3.0f, -39.23f, 5.5f, 0.0f,// furthest right point
		24.0f, -3.0f, -24.56f, 0.0f, 0.0f,// closest right point

	    //carpet Linux Lab
		-3.0f, -3.0f, -23.07f, 0.0f, 0.0f,// closest right point
		-26.36f, -3.0f, -23.07f, 0.0f, 8.75f,// closest left point
		-26.36f, -3.0f, -41.74f, 7.0f, 8.75f,// furthest left point
		-26.36f, -3.0f, -41.74f, 7.0f, 8.75f,// furthest left point
		-3.0f, -3.0f, -41.74f, 7.0f, 0.0f,// furthest right point
		-3.0f, -3.0f, -23.07f, 0.0f, 0.0f,// closest right point
	
		// carpet offices
		1.37f, -3.0f, -74.33f, 0.0f, 0.0f,// closest right point
		-11.63f, -3.0f, -74.33f, 0.0f, 4.8f,// closest left point
		-11.63f, -3.0f, -88.33f, 5.17f, 4.8f,// furthest left point
		-11.63f, -3.0f, -88.33f, 5.17f, 4.8f,// furthest left point
		1.37f, -3.0f, -88.33f, 5.17f, 0.0f,// furthest right point
		1.37f, -3.0f, -74.33f, 0.0f, 0.0f,// closest right point

		//206 clockwall
		3.03f, 3.0f, -26.96f, 1.0f, 1.0f, // top point closest to me
		3.03f, 3.0f, -39.23f, 0.0f, 1.0f, // top point furthest from me
		3.03f, -3.0f, -39.23f, 0.0f, 0.0f, // bottom point furthest from me
		3.03f, -3.0f, -39.23f, 0.0f, 0.0f, // bottom point furthest from me
		3.03f, -3.0f, -26.96f, 1.0f, 0.0f, // bottom point closest to me
		3.03f, 3.0f, -26.96f, 1.0f, 1.0f, // top point closest to me

		//208 clockwall
		3.03f, 3.0f, -42.44f, 1.0f, 1.0f, // top point closest to me
		3.03f, 3.0f, -54.71f, 0.0f, 1.0f, // top point furthest from me
		3.03f, -3.0f, -54.71f, 0.0f, 0.0f, // bottom point furthest from me
		3.03f, -3.0f, -54.71f, 0.0f, 0.0f, // bottom point furthest from me
		3.03f, -3.0f, -42.44f, 1.0f, 0.0f, // bottom point closest to me
		3.03f, 3.0f, -42.44f, 1.0f, 1.0f, // top point closest to me

		//210 clockwall
		3.03f, 3.0f, -57.87f, 1.0f, 1.0f, // top point closest to me
		3.03f, 3.0f, -70.14f, 0.0f, 1.0f, // top point furthest from me
		3.03f, -3.0f, -70.14f, 0.0f, 0.0f, // bottom point furthest from me
		3.03f, -3.0f, -70.14f, 0.0f, 0.0f, // bottom point furthest from me
		3.03f, -3.0f, -57.87f, 1.0f, 0.0f, // bottom point closest to me
		3.03f, 3.0f, -57.87f, 1.0f, 1.0f, // top point closest to me

		// 210 backwall whiteboards
		12.66f, -3.0f, -55.47f, 0.0f, 0.0f,//bottom left point
		24.0f, -3.0f, -55.47f, 1.0f, 0.0f,// bottom right point
		24.0f, 3.0f, -55.47f, 1.0f, 1.0f,// top right point
		24.0f, 3.0f, -55.47f, 1.0f, 1.0f,// top right point
		12.66f, 3.0f, -55.47f, 0.0f, 1.0f,// top left point
		12.66f, -3.0f, -55.47f, 0.0f, 0.0f,//bottom left point

		// 208 backwall whiteboards
		12.66f, -3.0f, -40.04f, 0.0f, 0.0f,//bottom left point
		24.0f, -3.0f, -40.04f, 1.0f, 0.0f,// bottom right point
		24.0f, 3.0f, -40.04f, 1.0f, 1.0f,// top right point
		24.0f, 3.0f, -40.04f, 1.0f, 1.0f,// top right point
		12.66f, 3.0f, -40.04f, 0.0f, 1.0f,// top left point
		12.66f, -3.0f, -40.04f, 0.0f, 0.0f,//bottom left point

		// 206 backwall whiteboards
		12.66f, -3.0f, -24.56f, 0.0f, 0.0f,//bottom left point
		24.0f, -3.0f, -24.56f, 1.0f, 0.0f,// bottom right point
		24.0f, 3.0f, -24.56f, 1.0f, 1.0f,// top right point
		24.0f, 3.0f, -24.56f, 1.0f, 1.0f,// top right point
		12.66f, 3.0f, -24.56f, 0.0f, 1.0f,// top left point
		12.66f, -3.0f, -24.56f, 0.0f, 0.0f,//bottom left point

		// 206 frontwall 1
		3.03f, -3.0f, -39.23f, 0.0f, 0.0f,//bottom left point
		17.0f, -3.0f, -39.23f, 1.0f, 0.0f,// bottom right point
		17.0f, 3.0f, -39.23f, 1.0f, 1.0f,// top right point
		17.0f, 3.0f, -39.23f, 1.0f, 1.0f,// top right point
		3.03f, 3.0f, -39.23f, 0.0f, 1.0f,// top left point
		3.03f, -3.0f, -39.23f, 0.0f, 0.0f,//bottom left point

		// 208 frontwall 1
		3.03f, -3.0f, -54.71f, 0.0f, 0.0f,//bottom left point
		17.0f, -3.0f, -54.71f, 1.0f, 0.0f,// bottom right point
		17.0f, 3.0f, -54.71f, 1.0f, 1.0f,// top right point
		17.0f, 3.0f, -54.71f, 1.0f, 1.0f,// top right point
		3.03f, 3.0f, -54.71f, 0.0f, 1.0f,// top left point
		3.03f, -3.0f, -54.71f, 0.0f, 0.0f,//bottom left point

		// 210 frontwall 1
		3.03f, -3.0f, -70.14f, 0.0f, 0.0f,//bottom left point
		17.0f, -3.0f, -70.14f, 1.0f, 0.0f,// bottom right point
		17.0f, 3.0f, -70.14f, 1.0f, 1.0f,// top right point
		17.0f, 3.0f, -70.14f, 1.0f, 1.0f,// top right point
		3.03f, 3.0f, -70.14f, 0.0f, 1.0f,// top left point
		3.03f, -3.0f, -70.14f, 0.0f, 0.0f,//bottom left point

		// 214 leftside wall
		9.03f, -3.0f, -71.26f, 0.0f, 0.0f,//bottom left point
		23.67f, -3.0f, -71.26f, 1.0f, 0.0f,// bottom right point
		23.67f, 3.0f, -71.26f, 1.0f, 1.0f,// top right point
		23.67f, 3.0f, -71.26f, 1.0f, 1.0f,// top right point
		9.03f, 3.0f, -71.26f, 0.0f, 1.0f,// top left point
		9.03f, -3.0f, -71.26f, 0.0f, 0.0f,//bottom left point


		// 214 rightside wall
		9.03f, -3.0f, -94.53f, 0.0f, 0.0f,//bottom left point
		23.67f, -3.0f, -94.53f, 1.0f, 0.0f,// bottom right point
		23.67f, 3.0f, -94.53f, 1.0f, 1.0f,// top right point
		23.67f, 3.0f, -94.53f, 1.0f, 1.0f,// top right point
		9.03f, 3.0f, -94.53f, 0.0f, 1.0f,// top left point
		9.03f, -3.0f, -94.53f, 0.0f, 0.0f,//bottom left point

		// 214 front wall 1
		9.03f, 3.0f, -74.33f, 1.0f, 1.0f, // top point closest to me
		9.03f, 3.0f, -88.33f, 0.0f, 1.0f, // top point furthest from me
		9.03f, -3.0f, -88.33f, 0.0f, 0.0f, // bottom point furthest from me
		9.03f, -3.0f, -88.33f, 0.0f, 0.0f, // bottom point furthest from me
		9.03f, -3.0f, -74.33f, 1.0f, 0.0f, // bottom point closest to me
		9.03f, 3.0f, -74.33f, 1.0f, 1.0f, // top point closest to me

		// 214 frontwall 1 repeated
		9.03f, 3.0f, -88.33f, 0.0f, 1.0f, // top point closest to me
		9.03f, 3.0f, -94.53f, 0.50f, 1.0f, // top point furthest from me
		9.03f, -3.0f, -94.53f, 0.50f, 0.0f, // bottom point furthest from me
		9.03f, -3.0f, -94.53f, 0.50f, 0.0f, // bottom point furthest from me
		9.03f, -3.0f, -88.33f, 0.0f, 0.0f, // bottom point closest to me
		9.03f, 3.0f, -88.33f, 0.0f, 1.0f, // top point closest to me
			
		//Linux lab frontwall 1
		-3.0f, 3.0f, -23.07f, 0.0f, 1.0f, // top point closest to me
		-18.0f, 3.0f, -23.07f, 1.0f, 1.0f, // top point furthest from me
		-18.0f, -3.0f, -23.07f, 1.0f, 0.0f, // bottom point furthest from me
		-18.0f, -3.0f, -23.07f, 1.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -23.07f, 0.0f, 0.0f, // bottom point closest to me
		-3.0f, 3.0f, -23.07f, 0.0f, 1.0f, // top point closest to me



		// 206 backwall cabinets
		3.0f, -3.0f, -24.56f, 0.0f, 0.0f,//bottom left point
		12.66f, -3.0f, -24.56f, 1.0f, 0.0f,// bottom right point
		12.66, 3.0f, -24.56f, 1.0f, 1.0f,// top right point
		12.66f, 3.0f, -24.56f, 1.0f, 1.0f,// top right point
		3.0f, 3.0f, -24.56f, 0.0f, 1.0f,// top left point
		3.0f, -3.0f, -24.56f, 0.0f, 0.0f,//bottom left point

		// 208 backwall cabinets
		3.0f, -3.0f, -40.04f, 0.0f, 0.0f,//bottom left point
		12.66f, -3.0f, -40.04f, 1.0f, 0.0f,// bottom right point
		12.66, 3.0f, -40.04f, 1.0f, 1.0f,// top right point
		12.66f, 3.0f, -40.04f, 1.0f, 1.0f,// top right point
		3.0f, 3.0f, -40.04f, 0.0f, 1.0f,// top left point
		3.0f, -3.0f, -40.04f, 0.0f, 0.0f,//bottom left point

		// 210 backwall cabinets
		3.0f, -3.0f, -55.47f, 0.0f, 0.0f,//bottom left point
		12.66f, -3.0f, -55.47f, 1.0f, 0.0f,// bottom right point
		12.66, 3.0f, -55.47f, 1.0f, 1.0f,// top right point
		12.66f, 3.0f, -55.47f, 1.0f, 1.0f,// top right point
		3.0f, 3.0f, -55.47f, 0.0f, 1.0f,// top left point
		3.0f, -3.0f, -55.47f, 0.0f, 0.0f,//bottom left point

		//Linux lab cabinets backwall 1
		-3.0f, 3.0f, -41.74f, 0.0f, 1.0f, // top point closest to me
		-13.67f, 3.0f, -41.74f, 0.51f, 1.0f, // top point furthest from me
		-13.67f, -3.0f, -41.74f, 0.51f, 0.0f, // bottom point furthest from me
		-13.67f, -3.0f, -41.74f, 0.51f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -41.74f, 0.0f, 0.0f, // bottom point closest to me
		-3.0f, 3.0f, -41.74f, 0.0f, 1.0f, // top point closest to me

		//Linux lab cabinets backwall 2
		-15.67f, 3.0f, -41.74f, 0.0f, 1.0f, // top point closest to me
		-26.36f, 3.0f, -41.74f, 0.51f, 1.0f, // top point furthest from me
		-26.36f, -3.0f, -41.74f, 0.51f, 0.0f, // bottom point furthest from me
		-26.36f, -3.0f, -41.74f, 0.51f, 0.0f, // bottom point furthest from me
		-15.67f, -3.0f, -41.74f, 0.0f, 0.0f, // bottom point closest to me
		-15.67f, 3.0f, -41.74f, 0.0f, 1.0f, // top point closest to me





		// 206 frontwall 2
		17.0f, -3.0f, -39.23f, 0.0f, 0.0f,//bottom left point
		24.0f, -3.0f, -39.23f, 0.51f, 0.0f,// bottom right point
		24.0f, 3.0f, -39.23f, 0.51f, 1.0f,// top right point
		24.0f, 3.0f, -39.23f, 0.51f, 1.0f,// top right point
		17.0f, 3.0f, -39.23f, 0.0f, 1.0f,// top left point
		17.0f, -3.0f, -39.23f, 0.0f, 0.0f,//bottom left point

		// 208 frontwall 2
		17.0f, -3.0f, -54.71f, 0.0f, 0.0f,//bottom left point
		24.0f, -3.0f, -54.71f, 0.51f, 0.0f,// bottom right point
		24.0f, 3.0f, -54.71f, 0.51f, 1.0f,// top right point
		24.0f, 3.0f, -54.71f, 0.51f, 1.0f,// top right point
		17.0f, 3.0f, -54.71f, 0.0f, 1.0f,// top left point
		17.0f, -3.0f, -54.71f, 0.0f, 0.0f,//bottom left point

		// 210 frontwall 2
		17.0f, -3.0f, -70.14f, 0.0f, 0.0f,//bottom left point
		24.0f, -3.0f, -70.14f, 0.51f, 0.0f,// bottom right point
		24.0f,  3.0f, -70.14f, 0.51f, 1.0f,// top right point
		24.0f,  3.0f, -70.14f, 0.51f, 1.0f,// top right point
		17.0f,  3.0f, -70.14f, 0.0f, 1.0f,// top left point
		17.0f, -3.0f, -70.14f, 0.0f, 0.0f,//bottom left point

		//Linux lab frontwall repeated
		-18.0f, 3.0f, -23.07f, 0.0f, 1.0f, // top point closest to me
		-26.36f, 3.0f, -23.07f, 0.51f, 1.0f, // top point furthest from me
		-26.36f, -3.0f, -23.07f, 0.51f, 0.0f, // bottom point furthest from me
		-26.36f, -3.0f, -23.07f, 0.51f, 0.0f, // bottom point furthest from me
		-18.0f, -3.0f, -23.07f, 0.0f, 0.0f, // bottom point closest to me
		-18.0f, 3.0f, -23.07f, 0.0f, 1.0f, // top point closest to me

		// 214 concrete wall 1
		23.67f,  3.0f, -78.93f, 3.0f, 3.0f, // top point closest to me
		23.67f,  3.0f, -94.53f, 0.0f, 3.0f, // top point furthest from me
		23.67f, -3.0f, -94.53f, 0.0f, 0.0f, // bottom point furthest from me
		23.67f, -3.0f, -94.53f, 0.0f, 0.0f, // bottom point furthest from me
		23.67f, -3.0f, -78.93f, 3.0f, 0.0f, // bottom point closest to me
		23.67f,  3.0f, -78.93f, 3.0f, 3.0f, // top point closest to me

		// 214 concrete wall 2
		23.67f, -1.04f, -71.26f, 1.0f, 1.0f, // top point closest to me
		23.67f, -1.04f, -78.93f, 0.0f, 1.0f, // top point furthest from me
		23.67f, -3.0f, -78.93f, 0.0f, 0.0f, // bottom point furthest from me
		23.67f, -3.0f, -78.93f, 0.0f, 0.0f, // bottom point furthest from me
		23.67f, -3.0f, -71.26f, 1.0f, 0.0f, // bottom point closest to me
		23.67f, -1.04f, -71.26f, 1.0f, 1.0f, // top point closest to me



		//210 concrete wall 1
		24.0f, 3.0f, -55.47f, 0.5f, 3.0f, // top point closest to me
		24.0f, 3.0f, -58.14f, 0.0f, 3.0f, // top point furthest from me
		24.0f, -3.0f, -58.14f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -3.0f, -58.14f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -3.0f, -55.47f, 0.5f, 0.0f, // bottom point closest to me
		24.0f, 3.0f, -55.47f, 0.5f, 3.0f, // top point closest to me

		//210 concrete wall 2
		24.0f, -0.95f, -58.14f, 2.5f, 1.0f, // top point closest to me
		24.0f, -0.95f, -70.14f, 0.0f, 1.0f, // top point furthest from me
		24.0f, -3.0f, -70.14f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -3.0f, -70.14f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -3.0f, -58.14f, 2.5f, 0.0f, // bottom point closest to me
		24.0f, -0.95f, -58.14f, 2.5f, 1.0f, // top point closest to me

		//208 concrete wall 1
		24.0f, 3.0f, -40.04f, 0.5f, 3.0f, // top point closest to me
		24.0f, 3.0f, -42.71f, 0.0f, 3.0f, // top point furthest from me
		24.0f, -3.0f, -42.71f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -3.0f, -42.71f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -3.0f, -40.04f, 0.5f, 0.0f, // bottom point closest to me
		24.0f, 3.0f, -40.04f, 0.5f, 3.0f, // top point closest to me

		//208 concrete wall 2
		24.0f, -0.95f, -42.71f, 2.5f, 1.0f, // top point closest to me
		24.0f, -0.95f, -54.71f, 0.0f, 1.0f, // top point furthest from me
		24.0f, -3.0f, -54.71f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -3.0f, -54.71f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -3.0f, -42.71f, 2.5f, 0.0f, // bottom point closest to me
		24.0f, -0.95f, -42.71f, 2.5f, 1.0f, // top point closest to me

		//206 concrete wall 1
		24.0f, 3.0f, -24.56f, 0.5f, 3.0f, // top point closest to me
		24.0f, 3.0f, -27.23f, 0.0f, 3.0f, // top point furthest from me
		24.0f, -3.0f, -27.23f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -3.0f, -27.23f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -3.0f, -24.56f, 0.5f, 0.0f, // bottom point closest to me
		24.0f, 3.0f, -24.56f, 0.5f, 3.0f, // top point closest to me

		//206 concrete wall 2
		24.0f, -0.95f, -27.23f, 2.5f, 1.0f, // top point closest to me
		24.0f, -0.95f, -39.23f, 0.0f, 1.0f, // top point furthest from me
		24.0f, -3.0f, -39.23f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -3.0f, -39.23f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -3.0f, -27.23f, 2.5f, 0.0f, // bottom point closest to me
		24.0f, -0.95f, -27.23f, 2.5f, 1.0f, // top point closest to me

		//206 windows
		24.0f, 3.0f, -27.23f, 1.0f, 1.0f, // top point closest to me
		24.0f, 3.0f, -39.23f, 0.0f, 1.0f, // top point furthest from me
		24.0f, -0.95f, -39.23f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -0.95f, -39.23f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -0.95f, -27.23f, 1.0f, 0.0f, // bottom point closest to me
		24.0f, 3.0f, -27.23f, 1.0f, 1.0f, // top point closest to me

		//208 windows
		24.0f, 3.0f, -42.71f, 1.0f, 1.0f, // top point closest to me
		24.0f, 3.0f, -54.71f, 0.0f, 1.0f, // top point furthest from me
		24.0f, -0.95f, -54.71f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -0.95f, -54.71f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -0.95f, -42.71f, 1.0f, 0.0f, // bottom point closest to me
		24.0f, 3.0f, -42.71f, 1.0f, 1.0f, // top point closest to me

		//210 windows
		24.0f,  3.0f, -58.14f, 1.0f, 1.0f, // top point closest to me
		24.0f,  3.0f, -70.14f, 0.0f, 1.0f, // top point furthest from me
		24.0f, -0.95f, -70.14f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -0.95f, -70.14f, 0.0f, 0.0f, // bottom point furthest from me
		24.0f, -0.95f, -58.14f, 1.0f, 0.0f, // bottom point closest to me
		24.0f,  3.0f, -58.14f, 1.0f, 1.0f, // top point closest to me

		// ceiling 214
		23.67f,  3.0f, -71.26f, 0.0f, 0.0f,// closest right point
		9.0f,    3.0f, -71.26f, 0.0f, 5.5f,// closest left point
		9.0f,    3.0f, -94.53f, 8.88f, 5.5f,// furthest left point
		9.0f,    3.0f, -94.53f, 8.88f, 5.5f,// furthest left point
		23.67f,  3.0f, -94.53f, 8.88f, 0.0f,// furthest right point
		23.67f,  3.0f, -71.26f, 0.0f, 0.0f,// closest right point

		//ceiling 210
		24.0f, 3.0f, -55.47f, 0.0f, 0.0f,// closest right point
		3.0f, 3.0f, -55.47f, 0.0f, 7.88f,// closest left point
		3.0f, 3.0f, -70.14f, 5.5f, 7.88f,// furthest left point
		3.0f, 3.0f, -70.14f, 5.5f, 7.88f,// furthest left point
		24.0f, 3.0f, -70.14f, 5.5f, 0.0f,// furthest right point
		24.0f, 3.0f, -55.47f, 0.0f, 0.0f,// closest right point

		//ceiling 208
		24.0f, 3.0f, -40.04f, 0.0f, 0.0f,// closest right point
		3.0f,  3.0f, -40.04f, 0.0f, 7.88f,// closest left point
		3.0f,  3.0f, -54.71f, 5.5f, 7.88f,// furthest left point
		3.0f,  3.0f, -54.71f, 5.5f, 7.88f,// furthest left point
		24.0f, 3.0f, -54.71f, 5.5f, 0.0f,// furthest right point
		24.0f, 3.0f, -40.04f, 0.0f, 0.0f,// closest right point

		//ceiling 206
		24.0f, 3.0f, -24.56f, 0.0f, 0.0f,// closest right point
		3.0f,  3.0f, -24.56f, 0.0f, 7.88f,// closest left point
		3.0f,  3.0f, -39.23f, 5.5f, 7.88f,// furthest left point
		3.0f,  3.0f, -39.23f, 5.5f, 7.88f,// furthest left point
		24.0f, 3.0f, -39.23f, 5.5f, 0.0f,// furthest right point
		24.0f, 3.0f, -24.56f, 0.0f, 0.0f,// closest right point
		
	
		//ceiling linux lab
		-3.0f,    3.0f, -23.07f, 0.0f, 0.0f,// closest right point
		-26.36f,  3.0f, -23.07f, 0.0f, 8.75f,// closest left point
		-26.36f,  3.0f, -41.74f, 7.0f, 8.75f,// furthest left point
		-26.36f,  3.0f, -41.74f, 7.0f, 8.75f,// furthest left point
		-3.0f,    3.0f, -41.74f, 7.0f, 0.0f,// furthest right point
		-3.0f,    3.0f, -23.07f, 0.0f, 0.0f,// closest right point
		// ceiling offices
		  1.37f,    3.0f, -74.33f, 0.0f, 0.0f,// closest right point
		-11.63f,    3.0f, -74.33f, 0.0f, 4.8f,// closest left point
		-11.63f,    3.0f, -88.33f, 5.17f, 4.8f,// furthest left point
		-11.63f,    3.0f, -88.33f, 5.17f, 4.8f,// furthest left point
		  1.37f,    3.0f, -88.33f, 5.17f, 0.0f,// furthest right point
		  1.37f,    3.0f, -74.33f, 0.0f, 0.0f,// closest right point

		// colored wall
		-26.66f, 3.0f, -7.0f, 1.0f, 1.0f, // top point closest to me
		-26.66f, 3.0f, -11.67f, 0.0f, 1.0f, // top point furthest from me
		-26.66f, -3.0f, -11.67f, 0.0f, 0.0f, // bottom point furthest from me
		-26.66f, -3.0f, -11.67f, 0.0f, 0.0f, // bottom point furthest from me
		-26.66f, -3.0f, -7.0f, 1.0f, 0.0f, // bottom point closest to me
		-26.66f, 3.0f, -7.0f, 1.0f, 1.0f, // top point closest to me

		//start back wall 204
		-3.0f, -3.0f, 0.5f, 0.0f, 0.0f, // bottom left point
		3.0f, -3.0f, 0.5f, 1.0f, 0.0f, // bottom right point
		3.0f, 3.0f, 0.5f, 1.0f, 1.0f, // top right point
		3.0f, 3.0f, 0.5f, 1.0f, 1.0f, // top right point
		-3.0f, 3.0f, 0.5f, 0.0f, 1.0f, // top left point
		-3.0f, -3.0f, 0.5f, 0.0f, 0.0f, // bottom left point

		//left wall begin 202 doors
		-3.0f, 3.0f, 0.5f, 1.0f, 1.0f, // top point closest to me
		-3.0f, 3.0f, -7.0f, 0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -7.0f, 0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -7.0f, 0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, 0.5f, 1.0f, 0.0f, // bottom point closest to me
		-3.0f, 3.0f, 0.5f, 1.0f, 1.0f, // top point closest to me

			// double doors right wall 206
		3.0f, 3.0f, 0.5f, 1.0f, 1.0f, // top point closest to me
		3.0f, 3.0f, -7.0f, 0.0f, 1.0f, // top point furthest from me
		3.0f, -3.0f, -7.0f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, -7.0f, 0.0f, 0.0f, // bottom point furthest from me
		3.0f, -3.0f, 0.5f, 1.0f, 0.0f, // bottom point closest to me
		3.0f, 3.0f, 0.5f, 1.0f, 1.0f, // top point closest to me

		// single door 209
		-3.0f, 3.0f, -13.67f, 1.0f, 1.0f, // top point closest to me
		-3.0f, 3.0f, -15.67f, 0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -15.67f, 0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -15.67f, 0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -13.67f, 1.0f, 0.0f, // bottom point closest to me
		-3.0f, 3.0f, -13.67f, 1.0f, 1.0f, // top point closest to me

		// single door 211
		-3.0f, 3.0f, -20.34f, 1.0f, 1.0f, // top point closest to me
		-3.0f, 3.0f, -22.34f, 0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -22.34f, 0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -22.34f, 0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -20.34f, 1.0f, 0.0f, // bottom point closest to me
		-3.0f, 3.0f, -20.34f, 1.0f, 1.0f, // top point closest to me

		// single door 215
		- 3.0f, 3.0f, -44.67f, 1.0f, 1.0f, // top point closest to me
		-3.0f, 3.0f, -46.67f, 0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -46.67f, 0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -46.67f, 0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -44.67f, 1.0f, 0.0f, // bottom point closest to me
		-3.0f, 3.0f, -44.67f, 1.0f, 1.0f, // top point closest to me


		// single door 219								 
		-3.0f, 3.0f, -59.00f, 1.0f, 1.0f, // top point closest to me
		-3.0f, 3.0f, -61.00f, 0.0f, 1.0f, // top point furthest from me
		-3.0f, -3.0f, -61.00f, 0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -61.00f, 0.0f, 0.0f, // bottom point furthest from me
		-3.0f, -3.0f, -59.00f, 1.0f, 0.0f, // bottom point closest to me
		-3.0f, 3.0f, -59.00f, 1.0f, 1.0f, // top point closest to me
		// single door windows lab backway
		-25.88f, -3.0f, -64.66f, 0.0f, 0.0f,//bottom left point
		-23.88f, -3.0f, -64.66f, 1.0f, 0.0f,// bottom right point
		-23.88f, 3.0f, -64.66f, 1.0f, 1.0f,// top right point
		-23.88f, 3.0f, -64.66f, 1.0f, 1.0f,// top right point
		-25.88f, 3.0f, -64.66f, 0.0f, 1.0f,// top left point
		-25.88f, -3.0f, -64.66f, 0.0f, 0.0f,//bottom left point
		// single door offices jones
		1.37f, 3.0f, -79.0f, 1.0f, 1.0f, // top point closest to me
		1.37f, 3.0f, -81.0f, 0.0f, 1.0f, // top point furthest from me
		1.37f, -3.0f, -81.0f, 0.0f, 0.0f, // bottom point furthest from me
		1.37f, -3.0f, -81.0f, 0.0f, 0.0f, // bottom point furthest from me
		1.37f, -3.0f, -79.0f, 1.0f, 0.0f, // bottom point closest to me
		1.37f, 3.0f, -79.0f, 1.0f, 1.0f, // top point closest to me
		// single door office after Jones
		1.37f, 3.0f, -82.0f, 1.0f, 1.0f, // top point closest to me
		1.37f, 3.0f, -84.0f, 0.0f, 1.0f, // top point furthest from me
		1.37f, -3.0f, -84.0f, 0.0f, 0.0f, // bottom point furthest from me
		1.37f, -3.0f, -84.0f, 0.0f, 0.0f, // bottom point furthest from me
		1.37f, -3.0f, -82.0f, 1.0f, 0.0f, // bottom point closest to me
		1.37f, 3.0f, -82.0f, 1.0f, 1.0f, // top point closest to me		
		// single door furthest right wing
		-1.296f, -3.0f, -88.33f, 0.0f, 0.0f,//bottom left point
		1.37f, -3.0f, -88.33f, 1.0f, 0.0f,// bottom right point
		1.37f, 3.0f, -88.33f, 1.0f, 1.0f,// top right point
		1.37f, 3.0f, -88.33f, 1.0f, 1.0f,// top right point
		-1.296f, 3.0f, -88.33f, 0.0f, 1.0f,// top left point
		-1.296f, -3.0f, -88.33f, 0.0f, 0.0f,//bottom left point	
		// single door barrier right side
		-1.296f, 3.0f, -85.49f, 1.0f, 1.0f, // top point closest to me
		-1.296f, 3.0f, -87.496f, 0.0f, 1.0f, // top point furthest from me
		-1.296f, -3.0f, -87.496f, 0.0f, 0.0f, // bottom point furthest from me
		-1.296f, -3.0f, -87.496f, 0.0f, 0.0f, // bottom point furthest from me
		-1.296f, -3.0f, -85.49f, 1.0f, 0.0f, // bottom point closest to me
		-1.296f, 3.0f, -85.49f, 1.0f, 1.0f, // top point closest to me	
		// single door Neff
		-11.63f, -3.0f, -75.0f, 0.0f, 0.0f,//bottom left point
		-8.96f, -3.0f, -75.0f, 1.0f, 0.0f,// bottom right point
		-8.96f, 3.0f, -75.0f, 1.0f, 1.0f,// top right point
		-8.96f, 3.0f, -75.0f, 1.0f, 1.0f,// top right point
		-11.63f, 3.0f, -75.0f, 0.0f, 1.0f,// top left point
		-11.63f, -3.0f, -75.0f, 0.0f, 0.0f,//bottom left point		
		// single door copy room
		-11.63f, 3.0f, -79.0f, 1.0f, 1.0f, // top point closest to me
		-11.63f, 3.0f, -81.0f, 0.0f, 1.0f, // top point furthest from me
		-11.63f, -3.0f, -81.0f, 0.0f, 0.0f, // bottom point furthest from me
		-11.63f, -3.0f, -81.0f, 0.0f, 0.0f, // bottom point furthest from me
		-11.63f, -3.0f, -79.0f, 1.0f, 0.0f, // bottom point closest to me
		-11.63f, 3.0f, -79.0f, 1.0f, 1.0f, // top point closest to me

		// single door Burton
		-11.63f, 3.0f, -82.0f, 1.0f, 1.0f, // top point closest to me
		-11.63f, 3.0f, -84.0f, 0.0f, 1.0f, // top point furthest from me
		-11.63f, -3.0f, -84.0f, 0.0f, 0.0f, // bottom point furthest from me
		-11.63f, -3.0f, -84.0f, 0.0f, 0.0f, // bottom point furthest from me
		-11.63f, -3.0f, -82.0f, 1.0f, 0.0f, // bottom point closest to me
		-11.63f, 3.0f, -82.0f, 1.0f, 1.0f, // top point closest to me	

		// single door furthest left wing
		-11.63f, -3.0f, -88.33f, 0.0f, 0.0f,//bottom left point
		-8.96f,  -3.0f, -88.33f, 1.0f, 0.0f,// bottom right point
		-8.96f,   3.0f, -88.33f, 1.0f, 1.0f,// top right point
		-8.96f,   3.0f, -88.33f, 1.0f, 1.0f,// top right point
		-11.63f,  3.0f, -88.33f, 0.0f, 1.0f,// top left point
		-11.63f, -3.0f, -88.33f, 0.0f, 0.0f,//bottom left point	

		//214 windows
		23.67f, 3.0f, -71.26f, 1.0f, 1.0f, // top point closest to me
		23.67f, 3.0f, -78.93f, 0.0f, 1.0f, // top point furthest from me
		23.67f, -1.04f, -78.93f, 0.0f, 0.0f, // bottom point furthest from me
		23.67f, -1.04f, -78.93f, 0.0f, 0.0f, // bottom point furthest from me
		23.67f, -1.04f, -71.26f, 1.0f, 0.0f, // bottom point closest to me
		23.67f, 3.0f, -71.26f, 1.0f, 1.0f, // top point closest to me


		// Linux Lab clockwall 
		-3.03f, 3.0f, -23.07f, 1.0f, 1.0f, // top point closest to me
		-3.03f, 3.0f, -39.335f, 0.0f, 1.0f, // top point furthest from me
		-3.03f, -3.0f, -39.335f, 0.0f, 0.0f, // bottom point furthest from me
		-3.03f, -3.0f, -39.335f, 0.0f, 0.0f, // bottom point furthest from me
		-3.03f, -3.0f, -23.07f, 1.0f, 0.0f, // bottom point closest to me
		-3.03f, 3.0f, -23.07f, 1.0f, 1.0f, // top point closest to me

		// Linux Lab right wall
		-26.36f, 3.0f, -23.07f, 1.0f, 1.0f, // top point closest to me
		-26.36f, 3.0f, -41.74f, 0.0f, 1.0f, // top point furthest from me
		-26.36f, -3.0f, -41.74f, 0.0f, 0.0f, // bottom point furthest from me
		-26.36f, -3.0f, -41.74f, 0.0f, 0.0f, // bottom point furthest from me
		-26.36f, -3.0f, -23.07f, 1.0f, 0.0f, // bottom point closest to me
		-26.36f, 3.0f, -23.07f, 1.0f, 1.0f, // top point closest to me

		// elevator 
		-26.66f, 3.0f, -68.0f, 1.0f, 1.0f, // top point closest to me
		-26.66f, 3.0f, -72.66f, 0.0f, 1.0f, // top point furthest from me
		-26.66f, -3.0f, -72.66f, 0.0f, 0.0f, // bottom point furthest from me
		-26.66f, -3.0f, -72.66f, 0.0f, 0.0f, // bottom point furthest from me
		-26.66f, -3.0f, -68.0f, 1.0f, 0.0f, // bottom point closest to me
		-26.66f, 3.0f, -68.0f, 1.0f, 1.0f, // top point closest to me

		// mens
		-22.82f, -3.0f, -11.67f, 0.0f, 0.0f,//bottom left point
		-20.32f, -3.0f, -11.67f, 1.0f, 0.0f,// bottom right point
		-20.32f, 3.0f, -11.67f, 1.0f, 1.0f,// top right point
		-20.32f, 3.0f, -11.67f, 1.0f, 1.0f,// top right point
		-22.82f, 3.0f, -11.67f, 0.0f, 1.0f,// top left point
		-22.82f, -3.0f, -11.67f, 0.0f, 0.0f,//bottom left point
		// womens
		-15.99f, -3.0f, -11.67f, 0.0f, 0.0f,//bottom left point
		-13.49f, -3.0f, -11.67f, 1.0f, 0.0f,// bottom right point
		-13.49f, 3.0f, -11.67f, 1.0f, 1.0f,// top right point
		-13.49f, 3.0f, -11.67f, 1.0f, 1.0f,// top right point
		-15.99f, 3.0f, -11.67f, 0.0f, 1.0f,// top left point
		-15.99f, -3.0f, -11.67f, 0.0f, 0.0f,//bottom left point
	};
	
			
	GLfloat cabinets[] =
	{

		// bottom cabinet front face
		6.2f,   -3.0f, -25.89f, 0.0f, 0.0f,//bottom left point
		12.53f, -3.0f, -25.89f, 1.0f, 0.0f,// bottom right point
		12.53,  -1.0f, -25.89f, 1.0f, 1.0f,// top right point
		12.53f, -1.0f, -25.89f, 1.0f, 1.0f,// top right point
		6.2f,   -1.0f, -25.89f, 0.0f, 1.0f,// top left point
		6.2f,   -3.0f, -25.89f, 0.0f, 0.0f,//bottom left point

		// bottom cabinet right side
		6.2f, -1.0f, -24.56f, 1.0f, 1.0f, // top point closest to me
		6.2f, -1.0f, -25.89f, 0.0f, 1.0f, // top point furthest from me
		6.2f, -3.0f, -25.89f, 0.0f, 0.0f, // bottom point furthest from me
		6.2f, -3.0f, -25.89f, 0.0f, 0.0f, // bottom point furthest from me
		6.2f, -3.0f, -24.56f, 1.0f, 0.0f, // bottom point closest to me
		6.2f, -1.0f, -24.56f, 1.0f, 1.0f, // top point closest to me

		// bottom cabinet left side
		12.53f, -1.0f, -24.56f, 1.0f, 1.0f, // top point closest to me
		12.53f, -1.0f, -25.89f, 0.0f, 1.0f, // top point furthest from me
		12.53f, -3.0f, -25.89f, 0.0f, 0.0f, // bottom point furthest from me
		12.53f, -3.0f, -25.89f, 0.0f, 0.0f, // bottom point furthest from me
		12.53f, -3.0f, -24.56f, 1.0f, 0.0f, // bottom point closest to me
		12.53f, -1.0f, -24.56f, 1.0f, 1.0f, // top point closest to me

		// top cabinet right side
		6.2f,  1.92f, -24.56f, 1.0f, 1.0f, // top point closest to me
		6.2f,  1.92f, -25.36f, 0.0f, 1.0f, // top point furthest from me
		6.2f,  0.25f, -25.36f, 0.0f, 0.0f, // bottom point furthest from me
		6.2f,  0.25f, -25.36f, 0.0f, 0.0f, // bottom point furthest from me
		6.2f,  0.25f, -24.56f, 1.0f, 0.0f, // bottom point closest to me
		6.2f,  1.92f, -24.56f, 1.0f, 1.0f, // top point closest to me

		 // top cabinet left side
		12.53f, 1.92f, -24.56f, 1.0f, 1.0f, // top point closest to me
		12.53f, 1.92f, -25.36f, 0.0f, 1.0f, // top point furthest from me
		12.53f,  0.25f, -25.36f, 0.0f, 0.0f, // bottom point furthest from me
		12.53f,  0.25f, -25.36f, 0.0f, 0.0f, // bottom point furthest from me
		12.53f,  0.25f, -24.56f, 1.0f, 0.0f, // bottom point closest to me
		12.53f, 1.92f, -24.56f, 1.0f, 1.0f, // top point closest to me

		// top cabinets top side
		12.53f, 1.92f, -24.56f, 0.0f, 0.0f,// closest right point
		6.2f,   1.92f, -24.56f, 0.0f, 1.0f,// closest left point
		6.2f,   1.92f, -25.36f, 1.0f, 1.0f,// furthest left point
		6.2f,   1.92f, -25.36f, 1.0f, 1.0f,// furthest left point
		12.53f, 1.92f, -25.36f, 1.0f, 0.0f,// furthest right point
		12.53f, 1.92f, -24.56f, 0.0f, 0.0f,// closest right point

		// top cabinets bottom side
		12.53f, 0.25f, -24.56f, 0.0f, 0.0f,// closest right point
		6.2f,   0.25f, -24.56f, 0.0f, 1.0f,// closest left point
		6.2f,   0.25f, -25.36f, 1.0f, 1.0f,// furthest left point
		6.2f,   0.25f, -25.36f, 1.0f, 1.0f,// furthest left point
		12.53f, 0.25f, -25.36f, 1.0f, 0.0f,// furthest right point
		12.53f, 0.25f, -24.56f, 0.0f, 0.0f,// closest right point
		
		// bottom cabinets top
		12.53f, -1.0f, -24.56f, 0.0f, 1.0f,// closest right point
		6.2f,   -1.0f, -24.56f, 1.0f, 1.0f,// closest left point
		6.2f,   -1.0f, -25.89f, 1.0f, 0.0f,// furthest left point
		6.2f,   -1.0f, -25.89f, 1.0f, 0.0f,// furthest left point
		12.53f, -1.0f, -25.89f, 0.0f, 0.0f,// furthest right point
		12.53f, -1.0f, -24.56f, 0.0f, 1.0f,// closest right point

		// top cabinet front face
		6.2f,    0.25f, -25.36f, 0.0f, 0.0f,//bottom left point
		12.53f,  0.25f, -25.36f, 1.0f, 0.0f,// bottom right point
		12.53,   1.92f, -25.36f, 1.0f, 1.0f,// top right point
		12.53f,  1.92f, -25.36f, 1.0f, 1.0f,// top right point
		6.2f,    1.92f, -25.36f, 0.0f, 1.0f,// top left point
		6.2f,    0.25f, -25.36f, 0.0f, 0.0f//bottom left point

	};

	glm::vec3 cabinetPositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -15.48f),
		glm::vec3(0.0f, 0.0f, -15.43f)
	};


	GLfloat llTable[] = 
	{
		// ll tabletop top
		-12.83f, -1.33f, -36.0f, 1.0f, 0.0f,// furthest left point
		-3.1f, -1.33f, -36.0f, 0.0f, 0.0f,// furthest right point
		-3.1f, -1.33f, -34.2f, 0.0f, 1.0f,// closest right point
		-3.1f, -1.33f, -34.2f, 0.0f, 1.0f,// closest right point
		-12.83f, -1.33f, -34.2f, 1.0f, 1.0f,// closest left point
		-12.83f, -1.33f, -36.0f, 1.0f, 0.0f,// furthest left point

		// ll tabletop bottom
		-12.83f, -1.38f, -36.0f, 1.0f, 0.0f,// furthest left point
		-3.1f, -1.38f, -36.0f, 0.0f, 0.0f,// furthest right point
		-3.1f, -1.38f, -34.2f, 0.0f, 1.0f,// closest right point
		-3.1f, -1.38f, -34.2f, 0.0f, 1.0f,// closest right point
		-12.83f, -1.38f, -34.2f, 1.0f, 1.0f,// closest left point
		-12.83f, -1.38f, -36.0f, 1.0f, 0.0f,// furthest left point

		// ll table backside 1
		-3.1f, -1.0f, -34.2f, 0.0f, 1.0f, // top point closest to me
		-12.83f, -1.0f, -34.2f, 1.0f, 1.0f, // top point furthest from me
		-12.83f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point furthest from me
		-12.83f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point furthest from me
		-3.1f,  -3.0f, -34.2f, 0.0f, 0.0f, // bottom point closest to me
		-3.1f,  -1.0f, -34.2f, 0.0f, 1.0f, // top point closest to me

		// ll table backside 2
		-3.1f, -1.0f, -34.28f, 0.0f, 1.0f, // top point closest to me
		-12.83f, -1.0f, -34.28f, 1.0f, 1.0f, // top point furthest from me
		-12.83f, -3.0f, -34.28f, 1.0f, 0.0f, // bottom point furthest from me
		-12.83f, -3.0f, -34.28f, 1.0f, 0.0f, // bottom point furthest from me
		-3.1f, -3.0f, -34.28f, 0.0f, 0.0f, // bottom point closest to me
		-3.1f, -1.0f, -34.28f, 0.0f, 1.0f, // top point closest to me

		// ll table backside 1 top
		-12.83f, -1.0f, -34.28f, 1.0f, 0.0f,// furthest left point
		-3.1f, -1.0f, -34.28f, 0.0f, 0.0f,// furthest right point
		-3.1f, -1.0f, -34.2f, 0.0f, 1.0f,// closest right point
		-3.1f, -1.0f, -34.2f, 0.0f, 1.0f,// closest right point
		-12.83f, -1.0f, -34.2f, 1.0f, 1.0f,// closest left point
		-12.83f, -1.0f, -34.28f, 1.0f, 0.0f,// furthest left point

		//ll table backside right
		-12.83f, -1.0f,  -34.2f, 1.0f, 1.0f, // top point closest to me
		-12.83f, -1.0f,  -34.28f, 0.0f, 1.0f, // top point furthest from me
		-12.83f, -3.0f, -34.28f, 0.0f, 0.0f, // bottom point furthest from me
		-12.83f, -3.0f, -34.28f, 0.0f, 0.0f, // bottom point furthest from me
		-12.83f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-12.83f, -1.0f,  -34.2f, 1.0f, 1.0f, // top point closest to me

		//ll table backside left
		-3.1f, -1.0f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-3.1f, -1.0f, -34.28f, 0.0f, 1.0f, // top point furthest from me
		-3.1f, -3.0f, -34.28f, 0.0f, 0.0f, // bottom point furthest from me
		-3.1f, -3.0f, -34.28f, 0.0f, 0.0f, // bottom point furthest from me
		-3.1f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-3.1f, -1.0f, -34.2f, 1.0f, 1.0f, // top point closest to me

		 //ll table left left
		-3.15f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-3.15f, -1.38f, -35.8f, 0.0f, 1.0f, // top point furthest from me
		-3.15f, -3.0f, -35.8f, 0.0f, 0.0f, // bottom point furthest from me
		-3.15f, -3.0f, -35.8f, 0.0f, 0.0f, // bottom point furthest from me
		-3.15f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-3.15f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me

		//ll table left right
		-3.2f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-3.2f, -1.38f, -35.9f, 0.0f, 1.0f, // top point furthest from me
		-3.2f, -3.0f, -35.9f, 0.0f, 0.0f, // bottom point furthest from me
		-3.2f, -3.0f, -35.9f, 0.0f, 0.0f, // bottom point furthest from me
		-3.2f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-3.2f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me

		//ll table left front panel
		-3.15f, -1.38f, -35.9f, 0.0f, 1.0f, // top point closest to me
		-3.2f, -1.38f, -35.9f, 1.0f, 1.0f, // top point furthest from me
		-3.2f, -3.0f, -35.9f, 1.0f, 0.0f, // bottom point furthest from me
		-3.2f, -3.0f, -35.9f, 1.0f, 0.0f, // bottom point furthest from me
		-3.15f, -3.0f, -35.9f, 0.0f, 0.0f, // bottom point closest to me
		-3.15f, -1.38f, -35.9f, 0.0f, 1.0f, // top point closest to me

		//ll table right right
		-12.77f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-12.77f, -1.38f, -35.9f, 0.0f, 1.0f, // top point furthest from me
		-12.77f, -3.0f, -35.9f, 0.0f, 0.0f, // bottom point furthest from me
		-12.77f, -3.0f, -35.9f, 0.0f, 0.0f, // bottom point furthest from me
		-12.77f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-12.77f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me

		//ll table right left
		-12.72f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-12.72f, -1.38f, -35.9f, 0.0f, 1.0f, // top point furthest from me
		-12.72f, -3.0f, -35.9f, 0.0f, 0.0f, // bottom point furthest from me
		-12.72f, -3.0f, -35.9f, 0.0f, 0.0f, // bottom point furthest from me
		-12.72f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-12.72f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me

		//ll table right front panel
		-12.72f, -1.38f, -35.9f, 0.0f, 1.0f, // top point closest to me
		-12.77f, -1.38f, -35.9f, 1.0f, 1.0f, // top point furthest from me
		-12.77f, -3.0f, -35.9f, 1.0f, 0.0f, // bottom point furthest from me
		-12.77f, -3.0f, -35.9f, 1.0f, 0.0f, // bottom point furthest from me
		-12.72f, -3.0f, -35.9f, 0.0f, 0.0f, // bottom point closest to me
		-12.72f, -1.38f, -35.9f, 0.0f, 1.0f, // top point closest to me

		 //ll table slit left left
		-5.06f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-5.06f, -1.38f, -35.9f, 0.0f, 1.0f, // top point furthest from me
		-5.06f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-5.06f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-5.06f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-5.06f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me

		//ll table slit left right
		-5.11f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-5.11f, -1.38f, -35.9f, 0.0f, 1.0f, // top point furthest from me
		-5.11f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-5.11f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-5.11f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-5.11f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me

		// ll table slit left front panel
		-5.06f, -1.38f, -35.9f, 0.0f, 1.0f, // top point closest to me
		-5.11f, -1.38f, -35.9f, 1.0f, 1.0f, // top point furthest from me
		-5.11f, -3.0f, -35.2f, 1.0f, 0.0f, // bottom point furthest from me
		-5.11f, -3.0f, -35.2f, 1.0f, 0.0f, // bottom point furthest from me
		-5.06f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point closest to me
		-5.06f, -1.38f, -35.9f, 0.0f, 1.0f, // top point closest to me

		//ll table slit mid left
		-6.97f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-6.97f, -1.38f, -35.9f, 0.0f, 1.0f, // top point furthest from me
		-6.97f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-6.97f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-6.97f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-6.97f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me

		//ll table slit mid right
		-7.02f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-7.02f, -1.38f, -35.9f, 0.0f, 1.0f, // top point furthest from me
		-7.02f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-7.02f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-7.02f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-7.02f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me

		// ll table slit mid front panel
		-6.97f, -1.38f, -35.9f, 0.0f, 1.0f, // top point closest to me
		-7.02f, -1.38f, -35.9f, 1.0f, 1.0f, // top point furthest from me
		-7.02f, -3.0f, -35.2f, 1.0f, 0.0f, // bottom point furthest from me
		-7.02f, -3.0f, -35.2f, 1.0f, 0.0f, // bottom point furthest from me
		-6.97f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point closest to me
		-6.97f, -1.38f, -35.9f, 0.0f, 1.0f, // top point closest to me

		//ll table slit right left
		-8.88f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-8.88f, -1.38f, -35.9f, 0.0f, 1.0f, // top point furthest from me
		-8.88f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-8.88f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-8.88f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-8.88f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me

		//ll table slit right left
		-8.93f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-8.93f, -1.38f, -35.9f, 0.0f, 1.0f, // top point furthest from me
		-8.93f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-8.93f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-8.93f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-8.93f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me

		// ll table slit right front panel
		-8.88f, -1.38f, -35.9f, 0.0f, 1.0f, // top point closest to me
		-8.93f, -1.38f, -35.9f, 1.0f, 1.0f, // top point furthest from me
		-8.93f, -3.0f, -35.2f, 1.0f, 0.0f, // bottom point furthest from me
		-8.93f, -3.0f, -35.2f, 1.0f, 0.0f, // bottom point furthest from me
		-8.88f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point closest to me
		-8.88f, -1.38f, -35.9f, 0.0f, 1.0f, // top point closest to me

		//ll table slit last left
		-10.79f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-10.79f, -1.38f, -35.9f, 0.0f, 1.0f, // top point furthest from me
		-10.79f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-10.79f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-10.79f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-10.79f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me

		//ll table slit last left
		-10.84f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me
		-10.84f, -1.38f, -35.9f, 0.0f, 1.0f, // top point furthest from me
		-10.84f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-10.84f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-10.84f, -3.0f, -34.2f, 1.0f, 0.0f, // bottom point closest to me
		-10.84f, -1.38f, -34.2f, 1.0f, 1.0f, // top point closest to me

		//ll table slit last front panel
		-10.79f, -1.38f, -35.9f, 0.0f, 1.0f, // top point closest to me
		-10.84f, -1.38f, -35.9f, 1.0f, 1.0f, // top point furthest from me
		-10.84f, -3.0f, -35.2f, 1.0f, 0.0f, // bottom point furthest from me
		-10.84, -3.0f, -35.2f, 1.0f, 0.0f, // bottom point furthest from me
		-10.79f, -3.0f, -35.2f, 0.0f, 0.0f, // bottom point closest to me
		-10.79, -1.38f, -35.9f, 0.0f, 1.0f, // top point closest to me

		// ll table wood lining left
		-3.15f, -1.33f, -34.28f, 1.0f, 1.0f, // top point closest to me
		-3.15f, -1.33f, -36.0f, 0.0f, 1.0f, // top point furthest from me
		-3.15f, -1.38f, -36.0f, 0.0f, 0.0f, // bottom point furthest from me
		-3.15f, -1.38f, -36.0f, 0.0f, 0.0f, // bottom point furthest from me
		-3.15f, -1.38f, -34.28f, 1.0f, 0.0f, // bottom point closest to me
		-3.15f, -1.33f, -34.28f, 1.0f, 1.0f, // top point closest to me

		// ll table wood lining right
		-12.83f, -1.33f, -34.28f, 1.0f, 1.0f, // top point closest to me
		-12.83f, -1.33f, -36.0f, 0.0f, 1.0f, // top point furthest from me
		-12.83f, -1.38f, -36.0f, 0.0f, 0.0f, // bottom point furthest from me
		-12.83f, -1.38f, -36.0f, 0.0f, 0.0f, // bottom point furthest from me
		-12.83f, -1.38f, -34.28f, 1.0f, 0.0f, // bottom point closest to me
		-12.83f, -1.33f, -34.28f, 1.0f, 1.0f, // top point closest to me

		// ll table wood lining front
		-3.1f,   -1.33f, -36.0f, 0.0f, 1.0f, // top point closest to me
		-12.83f, -1.33f, -36.0f, 1.0f, 1.0f, // top point furthest from me
		-12.83f, -1.38f, -36.0f, 1.0f, 0.0f, // bottom point furthest from me
		-12.83f, -1.38f, -36.0f, 1.0f, 0.0f, // bottom point furthest from me
		-3.1f,   -1.38f, -36.0f, 0.0f, 0.0f, // bottom point closest to me
		-3.1f,   -1.33f, -36.0f, 0.0f, 1.0f // top point closest to me
	};

	glm::vec3 llTablePositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 3.6f),
		glm::vec3(0.0f, 0.0f, 3.6f),
		glm::vec3(-13.3f, 0.0f, 0.21f),
		glm::vec3(0.0f, 0.0f, -3.6f),
		glm::vec3(0.0f, 0.0f, -3.6f),
		glm::vec3(0.0f, 0.0f, -3.9f)
	};
		
	GLfloat chair[] =
	{
		// back support front 1.8 length x 1.6 tall x .1 wide
		-3.5f, -0.95f, -36.5f, 0.0f, 1.0f, // top point closest to me
		-4.7f, -0.95f, -36.5f, 1.0f, 1.0f, // top point furthest from me
		-4.55f, -2.0f, -36.5f, 1.0f, 0.0f, // bottom point furthest from me
		-4.55f, -2.0f, -36.5f, 1.0f, 0.0f, // bottom point furthest from me
		-3.65f, -2.0f, -36.5f, 0.0f, 0.0f, // bottom point closest to me
		-3.5f,  -0.95f, -36.5f, 0.f, 1.0f, // top point closest to me
		
		// back support back
		-3.5f, -0.95f, -36.45f, 0.0f, 1.0f, // top point closest to me
		-4.7f, -0.95f, -36.45f, 1.0f, 1.0f, // top point furthest from me
		-4.55f, -2.0f, -36.45f, 1.0f, 0.0f, // bottom point furthest from me
		-4.55f, -2.0f, -36.45f, 1.0f, 0.0f, // bottom point furthest from me
		-3.65f, -2.0f, -36.45f, 0.0f, 0.0f, // bottom point closest to me
		-3.5f,  -0.95f, -36.45f, 0.f, 1.0f, // top point closest to me
		
		// butt support top
		-4.55f, -1.95f, -36.45f, 1.0f, 0.0f,// furthest left point
		-3.65f, -1.95f, -36.45f, 0.0f, 0.0f,// furthest right point
		-3.5f, -1.95f, -35.6f, 0.0f, 1.0f,// closest right point
		-3.5f, -1.95f, -35.6f, 0.0f, 1.0f,// closest right point
		-4.7f, -1.95f, -35.6f, 1.0f, 1.0f,// closest left point
		-4.55f, -1.95f, -36.45f, 1.0f, 0.0f,// furthest left point
		
		// butt support bottom
		-4.55f, -2.0f, -36.45f, 1.0f, 0.0f,// furthest left point
		-3.65f, -2.0f, -36.45f, 0.0f, 0.0f,// furthest right point
		-3.5f,  -2.0f, -35.6f, 0.0f, 1.0f,// closest right point
		-3.5f,  -2.0f, -35.6f, 0.0f, 1.0f,// closest right point
		-4.7f,  -2.0f, -35.6f, 1.0f, 1.0f,// closest left point
		-4.55f, -2.0f, -36.45f, 1.0f, 0.0f,// furthest left point



		// back support top
		-4.7f, -0.95f, -36.5f, 1.0f, 0.0f,// furthest left point
		-3.5f, -0.95f, -36.5f, 0.0f, 0.0f,// furthest right point
		-3.5f, -0.95f, -36.45f, 0.0f, 1.0f,// closest right point
		-3.5f, -0.95f, -36.45f, 0.0f, 1.0f,// closest right point
		-4.7f, -0.95f, -36.45f, 1.0f, 1.0f,// closest left point
		-4.7f, -0.95f, -36.5f, 1.0f, 0.0f,// furthest left point
		
		// back support bottom
		-4.55f, -2.0f, -36.5f, 1.0f, 0.0f,// furthest left point
		-3.65f, -2.0f, -36.5f, 0.0f, 0.0f,// furthest right point
		-3.65f, -2.0f, -36.45f, 0.0f, 1.0f,// closest right point
		-3.65f, -2.0f, -36.45f, 0.0f, 1.0f,// closest right point
		-4.55f, -2.0f, -36.45f, 1.0f, 1.0f,// closest left point
		-4.55f, -2.0f, -36.5f, 1.0f, 0.0f,// furthest left point

		// back support left
		-3.5f, -0.95f, -36.45f, 1.0f, 1.0f, // top point closest to me
		-3.5f, -0.95f, -36.5f, 0.0f, 1.0f, // top point furthest from me
		-3.65f, -2.0f, -36.5f, 0.0f, 0.0f, // bottom point furthest from me
		-3.65f, -2.0f, -36.5f, 0.0f, 0.0f, // bottom point furthest from me
		-3.65, -2.0f, -36.45f, 1.0f, 0.0f, // bottom point closest to me
		-3.5f, -0.95f, -36.45f, 1.0f, 1.0f, // top point closest to me
		
		// back support right
		-4.7f, -0.95f, -36.45f, 1.0f, 1.0f, // top point closest to me
		-4.7f, -0.95f, -36.5f, 0.0f, 1.0f, // top point furthest from me
		-4.55f, -2.0f, -36.5f, 0.0f, 0.0f, // bottom point furthest from me
		-4.55f, -2.0f, -36.5f, 0.0f, 0.0f, // bottom point furthest from me
		-4.55, -2.0f, -36.45f, 1.0f, 0.0f, // bottom point closest to me
		-4.7f, -0.95f, -36.45f, 1.0f, 1.0f, // top point closest to me

		// butt support front panel
		-3.5f,   -1.95f, -35.6f, 0.0f, 1.0f, // top point closest to me
		-4.7f, -1.95f, -35.6f, 1.0f, 1.0f, // top point furthest from me
		-4.7f, -2.0f, -35.6f, 1.0f, 0.0f, // bottom point furthest from me
		-4.7f, -2.0f, -35.6f, 1.0f, 0.0f, // bottom point furthest from me
		-3.5f,   -2.0f, -35.6f, 0.0f, 0.0f, // bottom point closest to me
		-3.5f,   -1.95f, -35.6f, 0.0f, 1.0f, // top point closest to me

		// butt support left
		-3.5f, -1.95f, -35.6f, 1.0f, 1.0f, // top point closest to me
		-3.65f, -1.95f, -36.45f, 0.0f, 1.0f, // top point furthest from me
		-3.65f, -2.0f, -36.45f, 0.0f, 0.0f, // bottom point furthest from me
		-3.65f, -2.0f, -36.45f, 0.0f, 0.0f, // bottom point furthest from me
		-3.5f, -2.0f, -35.6f, 1.0f, 0.0f, // bottom point closest to me
		-3.5f, -1.95f, -35.6f, 1.0f, 1.0f, // top point closest to me

		// butt support right
		-4.7f, -1.95f, -35.6f, 1.0f, 1.0f, // top point closest to me
		-4.55f, -1.95f, -36.45f, 0.0f, 1.0f, // top point furthest from me
		-4.55f, -2.0f, -36.45f, 0.0f, 0.0f, // bottom point furthest from me
		-4.55f, -2.0f, -36.45f, 0.0f, 0.0f, // bottom point furthest from me
		-4.7f, -2.0f, -35.6f, 1.0f, 0.0f, // bottom point closest to me
		-4.7f, -1.95f, -35.6f, 1.0f, 1.0f, // top point closest to me


		// rail leg bottom right, front side
		-4.5f, -2.0f, -36.45f, 0.0f, 1.0f, // top point closest to me
		-4.55f, -2.0f, -36.45f, 1.0f, 1.0f, // top point furthest from me
		-4.65f, -3.0f, -36.55f, 1.0f, 0.0f, // bottom point furthest from me
		-4.65f, -3.0f, -36.55f, 1.0f, 0.0f, // bottom point furthest from me
		-4.6f, -3.0f, -36.55f, 0.0f, 0.0f, // bottom point closest to me
		-4.5f,  -2.0f, -36.45f, 0.f, 1.0f, // top point closest to me

		// rail leg bottom right, back side
		-4.5f, -2.0f, -36.48f, 0.0f, 1.0f, // top point closest to me
		-4.55f, -2.0f, -36.48f, 1.0f, 1.0f, // top point furthest from me
		-4.65f, -3.0f, -36.58f, 1.0f, 0.0f, // bottom point furthest from me
		-4.65f, -3.0f, -36.58f, 1.0f, 0.0f, // bottom point furthest from me
		-4.6f, -3.0f, -36.58f, 0.0f, 0.0f, // bottom point closest to me
		-4.5f, -2.0f, -36.48f, 0.f, 1.0f, // top point closest to me

		// rail leg bottom left, front side
		-3.65f, -2.0f, -36.45f, 0.0f, 1.0f, // top point closest to me
		-3.7f, -2.0f, -36.45f, 1.0f, 1.0f, // top point furthest from me
		-3.6f, -3.0f, -36.55f, 1.0f, 0.0f, // bottom point furthest from me
		-3.6f, -3.0f, -36.55f, 1.0f, 0.0f, // bottom point furthest from me
		-3.55f, -3.0f, -36.55f, 0.0f, 0.0f, // bottom point closest to me
		-3.65f, -2.0f, -36.45f, 0.f, 1.0f, // top point closest to me

		// rail leg bottom left, back side
		-3.65f, -2.0f, -36.48f, 0.0f, 1.0f, // top point closest to me
		-3.7f, -2.0f, -36.48f, 1.0f, 1.0f, // top point furthest from me
		-3.6f, -3.0f, -36.58f, 1.0f, 0.0f, // bottom point furthest from me
		-3.6f, -3.0f, -36.58f, 1.0f, 0.0f, // bottom point furthest from me
		-3.55f, -3.0f, -36.58f, 0.0f, 0.0f, // bottom point closest to me
		-3.65f, -2.0f, -36.48f, 0.f, 1.0f, // top point closest to me

		// rail leg front right, front
		-4.65f, -2.0f, -35.6f, 0.0f, 1.0f, // top point closest to me
		-4.7f, -2.0f, -35.6f, 1.0f, 1.0f, // top point furthest from me
		-4.8f, -3.0f, -35.55f, 1.0f, 0.0f, // bottom point furthest from me
		-4.8f, -3.0f, -35.55f, 1.0f, 0.0f, // bottom point furthest from me
		-4.75f, -3.0f, -35.55f, 0.0f, 0.0f, // bottom point closest to me
		-4.65f, -2.0f, -35.6f, 0.f, 1.0f, // top point closest to me

		// rail leg front right, back
		-4.65f, -2.0f, -35.63f, 0.0f, 1.0f, // top point closest to me
		-4.7f, -2.0f, -35.63f, 1.0f, 1.0f, // top point furthest from me
		-4.8f, -3.0f, -35.58f, 1.0f, 0.0f, // bottom point furthest from me
		-4.8f, -3.0f, -35.58f, 1.0f, 0.0f, // bottom point furthest from me
		-4.75f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point closest to me
		-4.65f, -2.0f, -35.63f, 0.f, 1.0f, // top point closest to me

		// rail leg front left, front
		-3.5f, -2.0f, -35.6f, 0.0f, 1.0f, // top point closest to me
		-3.55f, -2.0f, -35.6f, 1.0f, 1.0f, // top point furthest from me
		-3.45f, -3.0f, -35.55f, 1.0f, 0.0f, // bottom point furthest from me
		-3.45f, -3.0f, -35.55f, 1.0f, 0.0f, // bottom point furthest from me
		-3.4f, -3.0f, -35.55f, 0.0f, 0.0f, // bottom point closest to me
		-3.5f, -2.0f, -35.6f, 0.f, 1.0f, // top point closest to me

		// rail leg front left, back
		-3.5f, -2.0f, -35.63f, 0.0f, 1.0f, // top point closest to me
		-3.55f, -2.0f, -35.63f, 1.0f, 1.0f, // top point furthest from me
		-3.45f, -3.0f, -35.58f, 1.0f, 0.0f, // bottom point furthest from me
		-3.45f, -3.0f, -35.58f, 1.0f, 0.0f, // bottom point furthest from me
		-3.4f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point closest to me
		-3.5f, -2.0f, -35.63f, 0.f, 1.0f, // top point closest to me

		// rail leg front right, left
		-4.7f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me
		-4.7f, -2.0f, -35.63f, 0.0f, 1.0f, // top point furthest from me
		-4.8f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-4.8f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-4.8, -3.0f, -35.55f, 1.0f, 0.0f, // bottom point closest to me
		-4.7f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me


		// rail leg front right, right
		-4.65f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me
		-4.65f, -2.0f, -35.63f, 0.0f, 1.0f, // top point furthest from me
		-4.75f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-4.75f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-4.75, -3.0f, -35.55f, 1.0f, 0.0f, // bottom point closest to me
		-4.65f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me

		// rail leg back right, right
		-4.55f, -2.0f, -36.48f, 1.0f, 1.0f, // top point closest to me
		-4.55f, -2.0f, -36.45f, 0.0f, 1.0f, // top point furthest from me
		-4.65f, -3.0f, -36.55f, 0.0f, 0.0f, // bottom point furthest from me
		-4.65f, -3.0f, -36.55f, 0.0f, 0.0f, // bottom point furthest from me
		-4.65f, -3.0f, -36.58, 1.0f, 0.0f, // bottom point closest to me
		-4.55f, -2.0f, -36.48f, 1.0f, 1.0f, // top point closest to me

		// rail leg back right, left
		-4.5f, -2.0f, -36.48f, 1.0f, 1.0f, // top point closest to me
		-4.5f, -2.0f, -36.45f, 0.0f, 1.0f, // top point furthest from me
		-4.6f, -3.0f, -36.55f, 0.0f, 0.0f, // bottom point furthest from me
		-4.6f, -3.0f, -36.55f, 0.0f, 0.0f, // bottom point furthest from me
		-4.6f, -3.0f, -36.58, 1.0f, 0.0f, // bottom point closest to me
		-4.5f, -2.0f, -36.48f, 1.0f, 1.0f, // top point closest to me

		// rail leg front left, left
		-3.55f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me
		-3.55f, -2.0f, -35.63f, 0.0f, 1.0f, // top point furthest from me
		-3.45f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.45f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.45f, -3.0f, -35.55, 1.0f, 0.0f, // bottom point closest to me
		-3.55f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me

		// rail leg front left, right
		-3.5f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me
		-3.5f, -2.0f, -35.63f, 0.0f, 1.0f, // top point furthest from me
		-3.4f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.4f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.4f, -3.0f, -35.55, 1.0f, 0.0f, // bottom point closest to me
		-3.5f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me

		// rail leg back left, left
		-3.65f, -2.0f, -36.45f, 1.0f, 1.0f, // top point closest to me
		-3.65f, -2.0f, -36.48f, 0.0f, 1.0f, // top point furthest from me
		-3.55f, -3.0f, -36.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.55f, -3.0f, -36.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.55f, -3.0f, -36.55, 1.0f, 0.0f, // bottom point closest to me
		-3.65f, -2.0f, -36.45f, 1.0f, 1.0f, // top point closest to me

		// rail leg back left, right
		-3.7f, -2.0f, -36.45f, 1.0f, 1.0f, // top point closest to me
		-3.7f, -2.0f, -36.48f, 0.0f, 1.0f, // top point furthest from me
		-3.6f, -3.0f, -36.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.6f, -3.0f, -36.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.6f, -3.0f, -36.55, 1.0f, 0.0f, // bottom point closest to me
		-3.7f, -2.0f, -36.45f, 1.0f, 1.0f, // top point closest to me
	};

	glm::vec3 chairPositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(0.0, 0.0, 3.55),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(0.0, 0.0, 3.55),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-5.75f, 0.0f, 0.2f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(0.0, 0.0, -3.55),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(0.0, 0.0, -3.55),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(0.0, 0.0, -3.7),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		//start second blue chair
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		//up a row
		glm::vec3(0.0f, 0.0f, 3.75f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.99f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.96f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 3.75f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		// up a room
		glm::vec3(0.0f, 0.0f, 11.85f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.99f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.96f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 3.75f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 3.75f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.99f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.96f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 11.75f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 3.75f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.99f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.96f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 3.75f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
			//shift to right side
		glm::vec3(-5.0f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.75f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.99f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.96f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.75f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.75f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.99f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.96f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		//down a room
		glm::vec3(0.0f, 0.0f, -8.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.75f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.99f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.96f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.75f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.75f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.99f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.96f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -8.10f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.75f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.99f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.96f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.75f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.75f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.99f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.96f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		//start 214 2nd chair
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.75f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.99f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.96f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.75f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-5.0f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 3.75f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.99f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(1.96f, 0.0f, 0.0f),
		glm::vec3(1.8f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 3.75f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.96f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.99f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f),
		glm::vec3(-5.5f, 0.0f, 0.0f),
		glm::vec3(-1.8f, 0.0f, 0.0f)
	};


	GLfloat blueChair[] =
	{
		// back support front 1.8 length x 1.6 tall x .1 wide
		-3.5f, -0.95f, -36.5f, 0.0f, 1.0f, // top point closest to me
		-4.7f, -0.95f, -36.5f, 1.0f, 1.0f, // top point furthest from me
		-4.55f, -2.0f, -36.5f, 1.0f, 0.0f, // bottom point furthest from me
		-4.55f, -2.0f, -36.5f, 1.0f, 0.0f, // bottom point furthest from me
		-3.65f, -2.0f, -36.5f, 0.0f, 0.0f, // bottom point closest to me
		-3.5f,  -0.95f, -36.5f, 0.f, 1.0f, // top point closest to me

										   // back support back
		-3.5f, -0.95f, -36.45f, 0.0f, 1.0f, // top point closest to me
		-4.7f, -0.95f, -36.45f, 1.0f, 1.0f, // top point furthest from me
		-4.55f, -2.0f, -36.45f, 1.0f, 0.0f, // bottom point furthest from me
		-4.55f, -2.0f, -36.45f, 1.0f, 0.0f, // bottom point furthest from me
		-3.65f, -2.0f, -36.45f, 0.0f, 0.0f, // bottom point closest to me
		-3.5f,  -0.95f, -36.45f, 0.f, 1.0f, // top point closest to me

											// butt support top
		-4.55f, -1.95f, -36.45f, 1.0f, 0.0f,// furthest left point
		-3.65f, -1.95f, -36.45f, 0.0f, 0.0f,// furthest right point
		-3.5f, -1.95f, -35.6f, 0.0f, 1.0f,// closest right point
		-3.5f, -1.95f, -35.6f, 0.0f, 1.0f,// closest right point
		-4.7f, -1.95f, -35.6f, 1.0f, 1.0f,// closest left point
		-4.55f, -1.95f, -36.45f, 1.0f, 0.0f,// furthest left point

											 // butt support bottom
		-4.55f, -2.0f, -36.45f, 1.0f, 0.0f,// furthest left point
		-3.65f, -2.0f, -36.45f, 0.0f, 0.0f,// furthest right point
		-3.5f,  -2.0f, -35.6f, 0.0f, 1.0f,// closest right point
		-3.5f,  -2.0f, -35.6f, 0.0f, 1.0f,// closest right point
		-4.7f,  -2.0f, -35.6f, 1.0f, 1.0f,// closest left point
		-4.55f, -2.0f, -36.45f, 1.0f, 0.0f,// furthest left point



											// back support top
		-4.7f, -0.95f, -36.5f, 1.0f, 0.0f,// furthest left point
		-3.5f, -0.95f, -36.5f, 0.0f, 0.0f,// furthest right point
		-3.5f, -0.95f, -36.45f, 0.0f, 1.0f,// closest right point
		-3.5f, -0.95f, -36.45f, 0.0f, 1.0f,// closest right point
		-4.7f, -0.95f, -36.45f, 1.0f, 1.0f,// closest left point
		-4.7f, -0.95f, -36.5f, 1.0f, 0.0f,// furthest left point

										   // back support bottom
		-4.55f, -2.0f, -36.5f, 1.0f, 0.0f,// furthest left point
		-3.65f, -2.0f, -36.5f, 0.0f, 0.0f,// furthest right point
		-3.65f, -2.0f, -36.45f, 0.0f, 1.0f,// closest right point
		-3.65f, -2.0f, -36.45f, 0.0f, 1.0f,// closest right point
		-4.55f, -2.0f, -36.45f, 1.0f, 1.0f,// closest left point
		-4.55f, -2.0f, -36.5f, 1.0f, 0.0f,// furthest left point

										   // back support left
		-3.5f, -0.95f, -36.45f, 1.0f, 1.0f, // top point closest to me
		-3.5f, -0.95f, -36.5f, 0.0f, 1.0f, // top point furthest from me
		-3.65f, -2.0f, -36.5f, 0.0f, 0.0f, // bottom point furthest from me
		-3.65f, -2.0f, -36.5f, 0.0f, 0.0f, // bottom point furthest from me
		-3.65, -2.0f, -36.45f, 1.0f, 0.0f, // bottom point closest to me
		-3.5f, -0.95f, -36.45f, 1.0f, 1.0f, // top point closest to me

											// back support right
		-4.7f, -0.95f, -36.45f, 1.0f, 1.0f, // top point closest to me
		-4.7f, -0.95f, -36.5f, 0.0f, 1.0f, // top point furthest from me
		-4.55f, -2.0f, -36.5f, 0.0f, 0.0f, // bottom point furthest from me
		-4.55f, -2.0f, -36.5f, 0.0f, 0.0f, // bottom point furthest from me
		-4.55, -2.0f, -36.45f, 1.0f, 0.0f, // bottom point closest to me
		-4.7f, -0.95f, -36.45f, 1.0f, 1.0f, // top point closest to me

											// butt support front panel
		-3.5f,   -1.95f, -35.6f, 0.0f, 1.0f, // top point closest to me
		-4.7f, -1.95f, -35.6f, 1.0f, 1.0f, // top point furthest from me
		-4.7f, -2.0f, -35.6f, 1.0f, 0.0f, // bottom point furthest from me
		-4.7f, -2.0f, -35.6f, 1.0f, 0.0f, // bottom point furthest from me
		-3.5f,   -2.0f, -35.6f, 0.0f, 0.0f, // bottom point closest to me
		-3.5f,   -1.95f, -35.6f, 0.0f, 1.0f, // top point closest to me

											 // butt support left
		-3.5f, -1.95f, -35.6f, 1.0f, 1.0f, // top point closest to me
		-3.65f, -1.95f, -36.45f, 0.0f, 1.0f, // top point furthest from me
		-3.65f, -2.0f, -36.45f, 0.0f, 0.0f, // bottom point furthest from me
		-3.65f, -2.0f, -36.45f, 0.0f, 0.0f, // bottom point furthest from me
		-3.5f, -2.0f, -35.6f, 1.0f, 0.0f, // bottom point closest to me
		-3.5f, -1.95f, -35.6f, 1.0f, 1.0f, // top point closest to me

										   // butt support right
		-4.7f, -1.95f, -35.6f, 1.0f, 1.0f, // top point closest to me
		-4.55f, -1.95f, -36.45f, 0.0f, 1.0f, // top point furthest from me
		-4.55f, -2.0f, -36.45f, 0.0f, 0.0f, // bottom point furthest from me
		-4.55f, -2.0f, -36.45f, 0.0f, 0.0f, // bottom point furthest from me
		-4.7f, -2.0f, -35.6f, 1.0f, 0.0f, // bottom point closest to me
		-4.7f, -1.95f, -35.6f, 1.0f, 1.0f, // top point closest to me


										   // rail leg bottom right, front side
		-4.5f, -2.0f, -36.45f, 0.0f, 1.0f, // top point closest to me
		-4.55f, -2.0f, -36.45f, 1.0f, 1.0f, // top point furthest from me
		-4.65f, -3.0f, -36.55f, 1.0f, 0.0f, // bottom point furthest from me
		-4.65f, -3.0f, -36.55f, 1.0f, 0.0f, // bottom point furthest from me
		-4.6f, -3.0f, -36.55f, 0.0f, 0.0f, // bottom point closest to me
		-4.5f,  -2.0f, -36.45f, 0.f, 1.0f, // top point closest to me

										   // rail leg bottom right, back side
		-4.5f, -2.0f, -36.48f, 0.0f, 1.0f, // top point closest to me
		-4.55f, -2.0f, -36.48f, 1.0f, 1.0f, // top point furthest from me
		-4.65f, -3.0f, -36.58f, 1.0f, 0.0f, // bottom point furthest from me
		-4.65f, -3.0f, -36.58f, 1.0f, 0.0f, // bottom point furthest from me
		-4.6f, -3.0f, -36.58f, 0.0f, 0.0f, // bottom point closest to me
		-4.5f, -2.0f, -36.48f, 0.f, 1.0f, // top point closest to me

										  // rail leg bottom left, front side
		-3.65f, -2.0f, -36.45f, 0.0f, 1.0f, // top point closest to me
		-3.7f, -2.0f, -36.45f, 1.0f, 1.0f, // top point furthest from me
		-3.6f, -3.0f, -36.55f, 1.0f, 0.0f, // bottom point furthest from me
		-3.6f, -3.0f, -36.55f, 1.0f, 0.0f, // bottom point furthest from me
		-3.55f, -3.0f, -36.55f, 0.0f, 0.0f, // bottom point closest to me
		-3.65f, -2.0f, -36.45f, 0.f, 1.0f, // top point closest to me

										   // rail leg bottom left, back side
		-3.65f, -2.0f, -36.48f, 0.0f, 1.0f, // top point closest to me
		-3.7f, -2.0f, -36.48f, 1.0f, 1.0f, // top point furthest from me
		-3.6f, -3.0f, -36.58f, 1.0f, 0.0f, // bottom point furthest from me
		-3.6f, -3.0f, -36.58f, 1.0f, 0.0f, // bottom point furthest from me
		-3.55f, -3.0f, -36.58f, 0.0f, 0.0f, // bottom point closest to me
		-3.65f, -2.0f, -36.48f, 0.f, 1.0f, // top point closest to me

										   // rail leg front right, front
		-4.65f, -2.0f, -35.6f, 0.0f, 1.0f, // top point closest to me
		-4.7f, -2.0f, -35.6f, 1.0f, 1.0f, // top point furthest from me
		-4.8f, -3.0f, -35.55f, 1.0f, 0.0f, // bottom point furthest from me
		-4.8f, -3.0f, -35.55f, 1.0f, 0.0f, // bottom point furthest from me
		-4.75f, -3.0f, -35.55f, 0.0f, 0.0f, // bottom point closest to me
		-4.65f, -2.0f, -35.6f, 0.f, 1.0f, // top point closest to me

										  // rail leg front right, back
		-4.65f, -2.0f, -35.63f, 0.0f, 1.0f, // top point closest to me
		-4.7f, -2.0f, -35.63f, 1.0f, 1.0f, // top point furthest from me
		-4.8f, -3.0f, -35.58f, 1.0f, 0.0f, // bottom point furthest from me
		-4.8f, -3.0f, -35.58f, 1.0f, 0.0f, // bottom point furthest from me
		-4.75f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point closest to me
		-4.65f, -2.0f, -35.63f, 0.f, 1.0f, // top point closest to me

										   // rail leg front left, front
		-3.5f, -2.0f, -35.6f, 0.0f, 1.0f, // top point closest to me
		-3.55f, -2.0f, -35.6f, 1.0f, 1.0f, // top point furthest from me
		-3.45f, -3.0f, -35.55f, 1.0f, 0.0f, // bottom point furthest from me
		-3.45f, -3.0f, -35.55f, 1.0f, 0.0f, // bottom point furthest from me
		-3.4f, -3.0f, -35.55f, 0.0f, 0.0f, // bottom point closest to me
		-3.5f, -2.0f, -35.6f, 0.f, 1.0f, // top point closest to me

										 // rail leg front left, back
		-3.5f, -2.0f, -35.63f, 0.0f, 1.0f, // top point closest to me
		-3.55f, -2.0f, -35.63f, 1.0f, 1.0f, // top point furthest from me
		-3.45f, -3.0f, -35.58f, 1.0f, 0.0f, // bottom point furthest from me
		-3.45f, -3.0f, -35.58f, 1.0f, 0.0f, // bottom point furthest from me
		-3.4f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point closest to me
		-3.5f, -2.0f, -35.63f, 0.f, 1.0f, // top point closest to me

										  // rail leg front right, left
		-4.7f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me
		-4.7f, -2.0f, -35.63f, 0.0f, 1.0f, // top point furthest from me
		-4.8f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-4.8f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-4.8, -3.0f, -35.55f, 1.0f, 0.0f, // bottom point closest to me
		-4.7f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me


										  // rail leg front right, right
		-4.65f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me
		-4.65f, -2.0f, -35.63f, 0.0f, 1.0f, // top point furthest from me
		-4.75f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-4.75f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-4.75, -3.0f, -35.55f, 1.0f, 0.0f, // bottom point closest to me
		-4.65f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me

										   // rail leg back right, right
		-4.55f, -2.0f, -36.48f, 1.0f, 1.0f, // top point closest to me
		-4.55f, -2.0f, -36.45f, 0.0f, 1.0f, // top point furthest from me
		-4.65f, -3.0f, -36.55f, 0.0f, 0.0f, // bottom point furthest from me
		-4.65f, -3.0f, -36.55f, 0.0f, 0.0f, // bottom point furthest from me
		-4.65f, -3.0f, -36.58, 1.0f, 0.0f, // bottom point closest to me
		-4.55f, -2.0f, -36.48f, 1.0f, 1.0f, // top point closest to me

											// rail leg back right, left
		-4.5f, -2.0f, -36.48f, 1.0f, 1.0f, // top point closest to me
		-4.5f, -2.0f, -36.45f, 0.0f, 1.0f, // top point furthest from me
		-4.6f, -3.0f, -36.55f, 0.0f, 0.0f, // bottom point furthest from me
		-4.6f, -3.0f, -36.55f, 0.0f, 0.0f, // bottom point furthest from me
		-4.6f, -3.0f, -36.58, 1.0f, 0.0f, // bottom point closest to me
		-4.5f, -2.0f, -36.48f, 1.0f, 1.0f, // top point closest to me

										   // rail leg front left, left
		-3.55f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me
		-3.55f, -2.0f, -35.63f, 0.0f, 1.0f, // top point furthest from me
		-3.45f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.45f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.45f, -3.0f, -35.55, 1.0f, 0.0f, // bottom point closest to me
		-3.55f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me

										   // rail leg front left, right
		-3.5f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me
		-3.5f, -2.0f, -35.63f, 0.0f, 1.0f, // top point furthest from me
		-3.4f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.4f, -3.0f, -35.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.4f, -3.0f, -35.55, 1.0f, 0.0f, // bottom point closest to me
		-3.5f, -2.0f, -35.6f, 1.0f, 1.0f, // top point closest to me

										  // rail leg back left, left
		-3.65f, -2.0f, -36.45f, 1.0f, 1.0f, // top point closest to me
		-3.65f, -2.0f, -36.48f, 0.0f, 1.0f, // top point furthest from me
		-3.55f, -3.0f, -36.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.55f, -3.0f, -36.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.55f, -3.0f, -36.55, 1.0f, 0.0f, // bottom point closest to me
		-3.65f, -2.0f, -36.45f, 1.0f, 1.0f, // top point closest to me

											// rail leg back left, right
		-3.7f, -2.0f, -36.45f, 1.0f, 1.0f, // top point closest to me
		-3.7f, -2.0f, -36.48f, 0.0f, 1.0f, // top point furthest from me
		-3.6f, -3.0f, -36.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.6f, -3.0f, -36.58f, 0.0f, 0.0f, // bottom point furthest from me
		-3.6f, -3.0f, -36.55, 1.0f, 0.0f, // bottom point closest to me
		-3.7f, -2.0f, -36.45f, 1.0f, 1.0f, // top point closest to me
	};


	GLfloat table[] =
	{
		//tabletop top
		 3.1f, -1.4f, -30.29f, 1.0f, 0.0f,// furthest left point
		 5.7f, -1.4f, -30.29f, 0.0f, 0.0f,// furthest right point
		 5.7f, -1.4f, -29.09f, 0.0f, 1.0f,// closest right point
		 5.7f, -1.4f, -29.09f, 0.0f, 1.0f,// closest right point
		 3.1f, -1.4f, -29.09f, 1.0f, 1.0f,// closest left point
		 3.1f, -1.4f, -30.29f, 1.0f, 0.0f,// furthest left point

		//tabletop bottom
		3.1f, -1.45f, -30.29f, 1.0f, 0.0f,// furthest left point
		5.7f, -1.45f, -30.29f, 0.0f, 0.0f,// furthest right point
		5.7f, -1.45f, -29.09f, 0.0f, 1.0f,// closest right point
		5.7f, -1.45f, -29.09f, 0.0f, 1.0f,// closest right point
		3.1f, -1.45f, -29.09f, 1.0f, 1.0f,// closest left point
		3.1f, -1.45f, -30.29f, 1.0f, 0.0f,// furthest left point

		// tabletop left
		3.1f, -1.4f, -29.09f, 1.0f, 1.0f, // top point closest to me
		3.1f, -1.4f, -30.29f, 0.0f, 1.0f, // top point furthest from me
		3.1f, -1.45f, -30.29f, 0.0f, 0.0f, // bottom point furthest from me
		3.1f, -1.45f, -30.29f, 0.0f, 0.0f, // bottom point furthest from me
		3.1f, -1.45f, -29.09f, 1.0f, 0.0f, // bottom point closest to me
		3.1f, -1.4f, -29.09f, 1.0f, 1.0f, // top point closest to me

		// tabletop right
		5.7f, -1.4f, -29.09f, 1.0f, 1.0f, // top point closest to me
		5.7f, -1.4f, -30.29f, 0.0f, 1.0f, // top point furthest from me
		5.7f, -1.45f, -30.29f, 0.0f, 0.0f, // bottom point furthest from me
		5.7f, -1.45f, -30.29f, 0.0f, 0.0f, // bottom point furthest from me
		5.7f, -1.45f, -29.09f, 1.0f, 0.0f, // bottom point closest to me
		5.7f, -1.4f, -29.09f, 1.0f, 1.0f, // top point closest to me

		// tabletop front
		5.7f,   -1.4f, -30.29, 0.0f, 1.0f, // top point closest to me
		3.1f, -1.4f, -30.29f, 1.0f, 1.0f, // top point furthest from me
		3.1f, -1.45f, -30.29f, 1.0f, 0.0f, // bottom point furthest from me
		3.1f, -1.45f, -30.29f, 1.0f, 0.0f, // bottom point furthest from me
		5.7f,   -1.45f, -30.29f, 0.0f, 0.0f, // bottom point closest to me
		5.7f,   -1.4f, -30.29f, 0.0f, 1.0f, // top point closest to me

		// tabletop back
		5.7f,   -1.4f, -29.09, 0.0f, 1.0f, // top point closest to me
		3.1f, -1.4f, -29.09f, 1.0f, 1.0f, // top point furthest from me
		3.1f, -1.45f, -29.09f, 1.0f, 0.0f, // bottom point furthest from me
		3.1f, -1.45f, -29.09f, 1.0f, 0.0f, // bottom point furthest from me
		5.7f,   -1.45f, -29.09f, 0.0f, 0.0f, // bottom point closest to me
		5.7f,   -1.4f, -29.09f, 0.0f, 1.0f, // top point closest to me

											// silver bar connector1 back
		5.567f, -1.45f, -29.156, 0.0f, 1.0f, // top point closest to me
		3.234f, -1.45f, -29.156f, 1.0f, 1.0f, // top point furthest from me
		3.234f, -1.516f, -29.156f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f, -1.516f, -29.156f, 1.0f, 0.0f, // bottom point furthest from me
		5.567f, -1.516f, -29.156f, 0.0f, 0.0f, // bottom point closest to me
		5.567f, -1.45f, -29.156f, 0.0f, 1.0f, // top point closest to me

											  // silver bar connector1 forward
		5.567f, -1.45f, -29.22, 0.0f, 1.0f, // top point closest to me
		3.234f, -1.45f, -29.22f, 1.0f, 1.0f, // top point furthest from me
		3.234f, -1.516f, -29.22f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f, -1.516f, -29.22f, 1.0f, 0.0f, // bottom point furthest from me
		5.567f, -1.516f, -29.22f, 0.0f, 0.0f, // bottom point closest to me
		5.567f, -1.45f, -29.22f, 0.0f, 1.0f, // top point closest to me

											 // silver bar connector2 back
		5.567f, -1.45f, -30.15, 0.0f, 1.0f, // top point closest to me
		3.234f, -1.45f, -30.15f, 1.0f, 1.0f, // top point furthest from me
		3.234f, -1.516f, -30.15f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f, -1.516f, -30.15f, 1.0f, 0.0f, // bottom point furthest from me
		5.567f, -1.516f, -30.15f, 0.0f, 0.0f, // bottom point closest to me
		5.567f, -1.45f, -30.15f, 0.0f, 1.0f, // top point closest to me

											 // silver bar connector2 forward
		5.567f, -1.45f, -30.22, 0.0f, 1.0f, // top point closest to me
		3.234f, -1.45f, -30.22f, 1.0f, 1.0f, // top point furthest from me
		3.234f, -1.516f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f, -1.516f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
		5.567f, -1.516f, -30.22f, 0.0f, 0.0f, // bottom point closest to me
		5.567f, -1.45f, -30.22f, 0.0f, 1.0f, // top point closest to me

											 // silver bar connector 1 underside
		3.234f, -1.516f, -29.22f, 1.0f, 0.0f,// furthest left point
		5.567f, -1.516f, -29.22f, 0.0f, 0.0f,// furthest right point
		5.567f, -1.516f, -29.156f, 0.0f, 1.0f,// closest right point
		5.567f, -1.516f, -29.156f, 0.0f, 1.0f,// closest right point
		3.234f, -1.516f, -29.156f, 1.0f, 1.0f,// closest left point
		3.234f, -1.516f, -29.22f, 1.0f, 0.0f,// furthest left point

											 // silver bar connector 2 underside
		3.234f, -1.516f, -30.22f, 1.0f, 0.0f,// furthest left point
		5.567f, -1.516f, -30.22f, 0.0f, 0.0f,// furthest right point
		5.567f, -1.516f, -30.15f, 0.0f, 1.0f,// closest right point
		5.567f, -1.516f, -30.15f, 0.0f, 1.0f,// closest right point
		3.234f, -1.516f, -30.15f, 1.0f, 1.0f,// closest left point
		3.234f, -1.516f, -30.22f, 1.0f, 0.0f,// furthest left point

		// silver bars parallel to floor, top
		3.167f, -2.867f, -30.22f, 1.0f, 0.0f,// furthest left point
		3.234f, -2.867f, -30.22f, 0.0f, 0.0f,// furthest right point
		3.234f, -2.867f, -29.156f, 0.0f, 1.0f,// closest right point
		3.234f, -2.867f, -29.156f, 0.0f, 1.0f,// closest right point
		3.167f, -2.867f, -29.156f, 1.0f, 1.0f,// closest left point
		3.167f, -2.867f, -30.22f, 1.0f, 0.0f,// furthest left point

		// silver bars parallel to floor, bottom
		3.167f, -2.933f, -30.15f, 1.0f, 0.0f,// furthest left point
		3.234f, -2.933f, -30.15f, 0.0f, 0.0f,// furthest right point
		3.234f, -2.933f, -29.22f, 0.0f, 1.0f,// closest right point
		3.234f, -2.933f, -29.22f, 0.0f, 1.0f,// closest right point
		3.167f, -2.933f, -29.22f, 1.0f, 1.0f,// closest left point
		3.167f, -2.933f, -30.15f, 1.0f, 0.0f,// furthest left point

		// silver bar parallel to tabletop forward, bottom
		3.167f, -1.516f, -30.22f, 1.0f, 0.0f,// furthest left point
		3.234f, -1.516f, -30.22f, 0.0f, 0.0f,// furthest right point
		3.234f, -1.516f, -29.156f, 0.0f, 1.0f,// closest right point+
		3.234f, -1.516f, -29.156f, 0.0f, 1.0f,// closest right point
		3.167f, -1.516f, -29.156f, 1.0f, 1.0f,// closest left point
		3.167f, -1.516f, -30.22f, 1.0f, 0.0f,// furthest left point

		// silver bar1 perpendicular to bottom bar, front
		3.234f,   -1.516f, -29.75f, 0.0f, 1.0f, // top point closest to me
		3.167f, -1.516f, -29.75f, 1.0f, 1.0f, // top point furthest from me
		3.167f, -2.867f, -29.75f, 1.0f, 0.0f, // bottom point furthest from me
		3.167f, -2.867f, -29.75f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f,   -2.867f, -29.75f, 0.0f, 0.0f, // bottom point closest to me
		3.234f,   -1.516f, -29.75f, 0.0f, 1.0f, // top point closest to me

		// silver bar1 perpendicular to bottom bar, back
		3.234f,   -1.516f, -29.816f, 0.0f, 1.0f, // top point closest to me
		3.167f, -1.516f, -29.816f, 1.0f, 1.0f, // top point furthest from me
		3.167f, -2.867f, -29.816f, 1.0f, 0.0f, // bottom point furthest from me
		3.167f, -2.867f, -29.816f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f,   -2.867f, -29.816f, 0.0f, 0.0f, // bottom point closest to me
		3.234f,   -1.516f, -29.816f, 0.0f, 1.0f, // top point closest to me

		// silver bar2 perpendicular to bottom bar, front
		3.234f,   -1.516f, -30.15f, 0.0f, 1.0f, // top point closest to me
		3.167f, -1.516f, -30.15f, 1.0f, 1.0f, // top point furthest from me
		3.167f, -2.867f, -30.15f, 1.0f, 0.0f, // bottom point furthest from me
		3.167f, -2.867f, -30.15f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f,   -2.867f, -30.15f, 0.0f, 0.0f, // bottom point closest to me
		3.234f,   -1.516f, -30.15f, 0.0f, 1.0f, // top point closest to me

		// silver bar2 perpendicular to bottom bar, back
		3.234f,   -1.516f, -30.22f, 0.0f, 1.0f, // top point closest to me
		3.167f, -1.516f, -30.22f, 1.0f, 1.0f, // top point furthest from me
		3.167f, -2.867f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
		3.167f, -2.867f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f,   -2.867f, -30.22f, 0.0f, 0.0f, // bottom point closest to me
		3.234f,   -1.516f, -30.22f, 0.0f, 1.0f, // top point closest to me
		
		//silver bar floor bar, front, front
		3.234f, -2.867f, -30.22f, 0.0f, 1.0f, // top point closest to me
		3.167f, -2.867f, -30.22f, 1.0f, 1.0f, // top point furthest from me
		3.167f, -3.0f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
		3.167f, -3.0f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f, -3.0f, -30.22f, 0.0f, 0.0f, // bottom point closest to me
		3.234f, -2.867f, -30.22f, 0.0f, 1.0f, // top point closest to me

		//silver bar floor bar, front, back
		3.234f, -2.93f, -30.15f, 0.0f, 1.0f, // top point closest to me
		3.167f, -2.93f, -30.15f, 1.0f, 1.0f, // top point furthest from me
		3.167f, -3.0f, -30.15f, 1.0f, 0.0f, // bottom point furthest from me
		3.167f, -3.0f, -30.15f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f, -3.0f, -30.15f, 0.0f, 0.0f, // bottom point closest to me
		3.234f, -2.93f, -30.15f, 0.0f, 1.0f, // top point closest to me

		//silver bar floor bar, back, front
		3.234f, -2.93f, -29.22f, 0.0f, 1.0f, // top point closest to me
		3.167f, -2.93f, -29.22f, 1.0f, 1.0f, // top point furthest from me
		3.167f, -3.0f, -29.22f, 1.0f, 0.0f, // bottom point furthest from me
		3.167f, -3.0f, -29.22f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f, -3.0f, -29.22f, 0.0f, 0.0f, // bottom point closest to me
		3.234f, -2.93f, -29.22f, 0.0f, 1.0f, // top point closest to me

		// silver bar floor bar, back, back
		3.234f, -2.867f, -29.156f, 0.0f, 1.0f, // top point closest to me
		3.167f, -2.867f, -29.156f, 1.0f, 1.0f, // top point furthest from me
		3.167f, -3.0f,   -29.156f, 1.0f, 0.0f, // bottom point furthest from me
		3.167f, -3.0f,   -29.156f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f, -3.0f,   -29.156f, 0.0f, 0.0f, // bottom point closest to me
		3.234f, -2.867f, -29.156f, 0.0f, 1.0f, // top point closest to me

		// silver bar tabletop underside, front panel, back
		3.234f, -1.45f, -29.156f, 0.0f, 1.0f, // top point closest to me
		3.167f, -1.45f, -29.156f, 1.0f, 1.0f, // top point furthest from me
		3.167f, -1.516f, -29.156f, 1.0f, 0.0f, // bottom point furthest from me
		3.167f, -1.516f, -29.156f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f, -1.516f, -29.156f, 0.0f, 0.0f, // bottom point closest to me
		3.234f, -1.45f, -29.156f, 0.0f, 1.0f, // top point closest to me

		// silver bar tabletop underside, front panel, front
		3.234f, -1.45f, -30.22f, 0.0f, 1.0f, // top point closest to me
		3.167f, -1.45f, -30.22f, 1.0f, 1.0f, // top point furthest from me
		3.167f, -1.516f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
		3.167f, -1.516f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
		3.234f, -1.516f, -30.22f, 0.0f, 0.0f, // bottom point closest to me
		3.234f, -1.45f, -30.22f, 0.0f, 1.0f, // top point closest to me

		// silver bar tabletop underside, left, right
		3.234f, -1.45f, -29.156f, 1.0f, 1.0f, // top point closest to me
		3.234f, -1.45f, -30.22f, 0.0f, 1.0f, // top point furthest from me
		3.234f, -1.516f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.234f, -1.516f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.234f, -1.516f, -29.156f, 1.0f, 0.0f, // bottom point closest to me
		3.234f, -1.45f, -29.156f, 1.0f, 1.0f, // top point closest to me

		// silver bar tabletop underside, left, left
		3.167f, -1.45f, -29.156f, 1.0f, 1.0f, // top point closest to me
		3.167f, -1.45f, -30.22f, 0.0f, 1.0f, // top point furthest from me
		3.167f, -1.516f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.167f, -1.516f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.167f, -1.516f, -29.156f, 1.0f, 0.0f, // bottom point closest to me
		3.167f, -1.45f, -29.156f, 1.0f, 1.0f, // top point closest to me


		// silver bar floor bar left, left
		3.167f, -2.867f, -29.156f, 1.0f, 1.0f, // top point closest to me
		3.167f, -2.867f, -30.22f, 0.0f, 1.0f, // top point furthest from me
		3.167f, -2.933f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.167f, -2.933f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.167f, -2.933f, -29.156f, 1.0f, 0.0f, // bottom point closest to me
		3.167f, -2.867f, -29.156f, 1.0f, 1.0f, // top point closest to me

		// silver bar floor bar left, right
		3.234f, -2.867f, -29.156f, 1.0f, 1.0f, // top point closest to me
		3.234f, -2.867f, -30.22f, 0.0f, 1.0f, // top point furthest from me
		3.234f, -2.933f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.234f, -2.933f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.234f, -2.933f, -29.156f, 1.0f, 0.0f, // bottom point closest to me
		3.234f, -2.867f, -29.156f, 1.0f, 1.0f, // top point closest to me

		// silver bar floor pegs left, back, right
		3.234f, -2.933f, -29.156f, 1.0f, 1.0f, // top point closest to me
		3.234f, -2.933f, -29.22f, 0.0f, 1.0f, // top point furthest from me
		3.234f, -3.0f, -29.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.234f, -3.0f, -29.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.234f, -3.0f, -29.156f, 1.0f, 0.0f, // bottom point closest to me
		3.234f, -2.933f, -29.156f, 1.0f, 1.0f, // top point closest to me

		// silver bar floor pegs left, back, left
		3.167f, -2.933f, -29.156f, 1.0f, 1.0f, // top point closest to me
		3.167f, -2.933f, -29.22f, 0.0f, 1.0f, // top point furthest from me
		3.167f, -3.0f, -29.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.167f, -3.0f, -29.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.167f, -3.0f, -29.156f, 1.0f, 0.0f, // bottom point closest to me
		3.167f, -2.933f, -29.156f, 1.0f, 1.0f, // top point closest to me

		// silver bar floor pegs left, forward, right
		3.234f, -2.933f, -30.15f, 1.0f, 1.0f, // top point closest to me
		3.234f, -2.933f, -30.22f, 0.0f, 1.0f, // top point furthest from me
		3.234f, -3.0f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.234f, -3.0f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.234f, -3.0f, -30.15f, 1.0f, 0.0f, // bottom point closest to me
		3.234f, -2.933f, -30.15f, 1.0f, 1.0f, // top point closest to me

		// silver bar floor pegs left, forward, left
		3.167f, -2.933f, -30.15f, 1.0f, 1.0f, // top point closest to me
		3.167f, -2.933f, -30.22f, 0.0f, 1.0f, // top point furthest from me
		3.167f, -3.0f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.167f, -3.0f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.167f, -3.0f, -30.15f, 1.0f, 0.0f, // bottom point closest to me
		3.167f, -2.933f, -30.15f, 1.0f, 1.0f, // top point closest to me

		// silver bar1 left, right
		3.234f, -1.516f, -29.75f, 1.0f, 1.0f, // top point closest to me
		3.234f, -1.516f, -29.816f, 0.0f, 1.0f, // top point furthest from me
		3.234f, -2.867f, -29.816f, 0.0f, 0.0f, // bottom point furthest from me
		3.234f, -2.867f, -29.816f, 0.0f, 0.0f, // bottom point furthest from me
		3.234f, -2.867f, -29.75f, 1.0f, 0.0f, // bottom point closest to me
		3.234f, -1.516f, -29.75f, 1.0f, 1.0f, // top point closest to me

		// silver bar1 left, left
		3.167f, -1.516f, -29.75f, 1.0f, 1.0f, // top point closest to me
		3.167f, -1.516f, -29.816f, 0.0f, 1.0f, // top point furthest from me
		3.167f, -2.867f, -29.816f, 0.0f, 0.0f, // bottom point furthest from me
		3.167f, -2.867f, -29.816f, 0.0f, 0.0f, // bottom point furthest from me
		3.167f, -2.867f, -29.75f, 1.0f, 0.0f, // bottom point closest to me
		3.167f, -1.516f, -29.75f, 1.0f, 1.0f, // top point closest to me

		// silver bar2 left, right
		3.234f, -1.516f, -30.15f, 1.0f, 1.0f, // top point closest to me
		3.234f, -1.516f, -30.22f, 0.0f, 1.0f, // top point furthest from me
		3.234f, -2.867f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.234f, -2.867f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.234f, -2.867f, -30.15f, 1.0f, 0.0f, // bottom point closest to me
		3.234f, -1.516f, -30.15f, 1.0f, 1.0f, // top point closest to me

		// silver bar2 left, left
		3.167f, -1.516f, -30.15f, 1.0f, 1.0f, // top point closest to me
		3.167f, -1.516f, -30.22f, 0.0f, 1.0f, // top point furthest from me
		3.167f, -2.867f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.167f, -2.867f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
		3.167f, -2.867f, -30.15f, 1.0f, 0.0f, // bottom point closest to me
		3.167f, -1.516f, -30.15f, 1.0f, 1.0f, // top point closest to me


			/*RIGHT SIDE REFLECTION - ADD 2.4*/



			// silver bars parallel to floor, top
			5.567f, -2.867f, -30.22f, 1.0f, 0.0f,// furthest left point
			5.634f, -2.867f, -30.22f, 0.0f, 0.0f,// furthest right point
			5.634f, -2.867f, -29.156f, 0.0f, 1.0f,// closest right point
			5.634f, -2.867f, -29.156f, 0.0f, 1.0f,// closest right point
			5.567f, -2.867f, -29.156f, 1.0f, 1.0f,// closest left point
			5.567f, -2.867f, -30.22f, 1.0f, 0.0f,// furthest left point

												 // silver bars parallel to floor, bottom
			5.567f, -2.933f, -30.15f, 1.0f, 0.0f,// furthest left point
			5.634f, -2.933f, -30.15f, 0.0f, 0.0f,// furthest right point
			5.634f, -2.933f, -29.22f, 0.0f, 1.0f,// closest right point
			5.634f, -2.933f, -29.22f, 0.0f, 1.0f,// closest right point
			5.567f, -2.933f, -29.22f, 1.0f, 1.0f,// closest left point
			5.567f, -2.933f, -30.15f, 1.0f, 0.0f,// furthest left point

												 // silver bar parallel to tabletop forward, bottom
			5.567f, -1.516f, -30.22f, 1.0f, 0.0f,// furthest left point
			5.634f, -1.516f, -30.22f, 0.0f, 0.0f,// furthest right point
			5.634f, -1.516f, -29.156f, 0.0f, 1.0f,// closest right point+
			5.634f, -1.516f, -29.156f, 0.0f, 1.0f,// closest right point
			5.567f, -1.516f, -29.156f, 1.0f, 1.0f,// closest left point
			5.567f, -1.516f, -30.22f, 1.0f, 0.0f,// furthest left point

												 // silver bar1 perpendicular to bottom bar, front
			5.634f, -1.516f, -29.75f, 0.0f, 1.0f, // top point closest to me
			5.567f, -1.516f, -29.75f, 1.0f, 1.0f, // top point furthest from me
			5.567f, -2.867f, -29.75f, 1.0f, 0.0f, // bottom point furthest from me
			5.567f, -2.867f, -29.75f, 1.0f, 0.0f, // bottom point furthest from me
			5.634f, -2.867f, -29.75f, 0.0f, 0.0f, // bottom point closest to me
			5.634f, -1.516f, -29.75f, 0.0f, 1.0f, // top point closest to me

												  // silver bar1 perpendicular to bottom bar, back
			5.634f, -1.516f, -29.816f, 0.0f, 1.0f, // top point closest to me
			5.567f, -1.516f, -29.816f, 1.0f, 1.0f, // top point furthest from me
			5.567f, -2.867f, -29.816f, 1.0f, 0.0f, // bottom point furthest from me
			5.567f, -2.867f, -29.816f, 1.0f, 0.0f, // bottom point furthest from me
			5.634f, -2.867f, -29.816f, 0.0f, 0.0f, // bottom point closest to me
			5.634f, -1.516f, -29.816f, 0.0f, 1.0f, // top point closest to me

												   // silver bar2 perpendicular to bottom bar, front
			5.634f, -1.516f, -30.15f, 0.0f, 1.0f, // top point closest to me
			5.567f, -1.516f, -30.15f, 1.0f, 1.0f, // top point furthest from me
			5.567f, -2.867f, -30.15f, 1.0f, 0.0f, // bottom point furthest from me
			5.567f, -2.867f, -30.15f, 1.0f, 0.0f, // bottom point furthest from me
			5.634f, -2.867f, -30.15f, 0.0f, 0.0f, // bottom point closest to me
			5.634f, -1.516f, -30.15f, 0.0f, 1.0f, // top point closest to me

												  // silver bar2 perpendicular to bottom bar, back
			5.634f, -1.516f, -30.22f, 0.0f, 1.0f, // top point closest to me
			5.567f, -1.516f, -30.22f, 1.0f, 1.0f, // top point furthest from me
			5.567f, -2.867f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
			5.567f, -2.867f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
			5.634f, -2.867f, -30.22f, 0.0f, 0.0f, // bottom point closest to me
			5.634f, -1.516f, -30.22f, 0.0f, 1.0f, // top point closest to me

												  //silver bar floor bar, front, front
			5.634f, -2.867f, -30.22f, 0.0f, 1.0f, // top point closest to me
			5.567f, -2.867f, -30.22f, 1.0f, 1.0f, // top point furthest from me
			5.567f, -3.0f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
			5.567f, -3.0f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
			5.634f, -3.0f, -30.22f, 0.0f, 0.0f, // bottom point closest to me
			5.634f, -2.867f, -30.22f, 0.0f, 1.0f, // top point closest to me

												  //silver bar floor bar, front, back
			5.634f, -2.93f, -30.15f, 0.0f, 1.0f, // top point closest to me
			5.567f, -2.93f, -30.15f, 1.0f, 1.0f, // top point furthest from me
			5.567f, -3.0f, -30.15f, 1.0f, 0.0f, // bottom point furthest from me
			5.567f, -3.0f, -30.15f, 1.0f, 0.0f, // bottom point furthest from me
			5.634f, -3.0f, -30.15f, 0.0f, 0.0f, // bottom point closest to me
			5.634f, -2.93f, -30.15f, 0.0f, 1.0f, // top point closest to me

												 //silver bar floor bar, back, front
			5.634f, -2.93f, -29.22f, 0.0f, 1.0f, // top point closest to me
			5.567f, -2.93f, -29.22f, 1.0f, 1.0f, // top point furthest from me
			5.567f, -3.0f, -29.22f, 1.0f, 0.0f, // bottom point furthest from me
			5.567f, -3.0f, -29.22f, 1.0f, 0.0f, // bottom point furthest from me
			5.634f, -3.0f, -29.22f, 0.0f, 0.0f, // bottom point closest to me
			5.634f, -2.93f, -29.22f, 0.0f, 1.0f, // top point closest to me

												 // silver bar floor bar, back, back
			5.634f, -2.867f, -29.156f, 0.0f, 1.0f, // top point closest to me
			5.567f, -2.867f, -29.156f, 1.0f, 1.0f, // top point furthest from me
			5.567f, -3.0f, -29.156f, 1.0f, 0.0f, // bottom point furthest from me
			5.567f, -3.0f, -29.156f, 1.0f, 0.0f, // bottom point furthest from me
			5.634f, -3.0f, -29.156f, 0.0f, 0.0f, // bottom point closest to me
			5.634f, -2.867f, -29.156f, 0.0f, 1.0f, // top point closest to me

												   // silver bar tabletop underside, front panel, back
			5.634f, -1.45f, -29.156f, 0.0f, 1.0f, // top point closest to me
			5.567f, -1.45f, -29.156f, 1.0f, 1.0f, // top point furthest from me
			5.567f, -1.516f, -29.156f, 1.0f, 0.0f, // bottom point furthest from me
			5.567f, -1.516f, -29.156f, 1.0f, 0.0f, // bottom point furthest from me
			5.634f, -1.516f, -29.156f, 0.0f, 0.0f, // bottom point closest to me
			5.634f, -1.45f, -29.156f, 0.0f, 1.0f, // top point closest to me

												  // silver bar tabletop underside, front panel, front
			5.634f, -1.45f, -30.22f, 0.0f, 1.0f, // top point closest to me
			5.567f, -1.45f, -30.22f, 1.0f, 1.0f, // top point furthest from me
			5.567f, -1.516f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
			5.567f, -1.516f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
			5.634f, -1.516f, -30.22f, 0.0f, 0.0f, // bottom point closest to me
			5.634f, -1.45f, -30.22f, 0.0f, 1.0f, // top point closest to me

												 // silver bar tabletop underside, left, right
			5.634f, -1.45f, -29.156f, 1.0f, 1.0f, // top point closest to me
			5.634f, -1.45f, -30.22f, 0.0f, 1.0f, // top point furthest from me
			5.634f, -1.516f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.634f, -1.516f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.634f, -1.516f, -29.156f, 1.0f, 0.0f, // bottom point closest to me
			5.634f, -1.45f, -29.156f, 1.0f, 1.0f, // top point closest to me

												  // silver bar tabletop underside, left, left
			5.567f, -1.45f, -29.156f, 1.0f, 1.0f, // top point closest to me
			5.567f, -1.45f, -30.22f, 0.0f, 1.0f, // top point furthest from me
			5.567f, -1.516f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.567f, -1.516f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.567f, -1.516f, -29.156f, 1.0f, 0.0f, // bottom point closest to me
			5.567f, -1.45f, -29.156f, 1.0f, 1.0f, // top point closest to me


												  // silver bar floor bar left, left
			5.567f, -2.867f, -29.156f, 1.0f, 1.0f, // top point closest to me
			5.567f, -2.867f, -30.22f, 0.0f, 1.0f, // top point furthest from me
			5.567f, -2.933f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.567f, -2.933f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.567f, -2.933f, -29.156f, 1.0f, 0.0f, // bottom point closest to me
			5.567f, -2.867f, -29.156f, 1.0f, 1.0f, // top point closest to me

												   // silver bar floor bar left, right
			5.634f, -2.867f, -29.156f, 1.0f, 1.0f, // top point closest to me
			5.634f, -2.867f, -30.22f, 0.0f, 1.0f, // top point furthest from me
			5.634f, -2.933f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.634f, -2.933f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.634f, -2.933f, -29.156f, 1.0f, 0.0f, // bottom point closest to me
			5.634f, -2.867f, -29.156f, 1.0f, 1.0f, // top point closest to me

												   // silver bar floor pegs left, back, right
			5.634f, -2.933f, -29.156f, 1.0f, 1.0f, // top point closest to me
			5.634f, -2.933f, -29.22f, 0.0f, 1.0f, // top point furthest from me
			5.634f, -3.0f, -29.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.634f, -3.0f, -29.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.634f, -3.0f, -29.156f, 1.0f, 0.0f, // bottom point closest to me
			5.634f, -2.933f, -29.156f, 1.0f, 1.0f, // top point closest to me

												   // silver bar floor pegs left, back, left
			5.567f, -2.933f, -29.156f, 1.0f, 1.0f, // top point closest to me
			5.567f, -2.933f, -29.22f, 0.0f, 1.0f, // top point furthest from me
			5.567f, -3.0f, -29.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.567f, -3.0f, -29.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.567f, -3.0f, -29.156f, 1.0f, 0.0f, // bottom point closest to me
			5.567f, -2.933f, -29.156f, 1.0f, 1.0f, // top point closest to me

												   // silver bar floor pegs left, forward, right
			5.634f, -2.933f, -30.15f, 1.0f, 1.0f, // top point closest to me
			5.634f, -2.933f, -30.22f, 0.0f, 1.0f, // top point furthest from me
			5.634f, -3.0f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.634f, -3.0f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.634f, -3.0f, -30.15f, 1.0f, 0.0f, // bottom point closest to me
			5.634f, -2.933f, -30.15f, 1.0f, 1.0f, // top point closest to me

												  // silver bar floor pegs left, forward, left
			5.567f, -2.933f, -30.15f, 1.0f, 1.0f, // top point closest to me
			5.567f, -2.933f, -30.22f, 0.0f, 1.0f, // top point furthest from me
			5.567f, -3.0f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.567f, -3.0f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.567f, -3.0f, -30.15f, 1.0f, 0.0f, // bottom point closest to me
			5.567f, -2.933f, -30.15f, 1.0f, 1.0f, // top point closest to me

												  // silver bar1 left, right
			5.634f, -1.516f, -29.75f, 1.0f, 1.0f, // top point closest to me
			5.634f, -1.516f, -29.816f, 0.0f, 1.0f, // top point furthest from me
			5.634f, -2.867f, -29.816f, 0.0f, 0.0f, // bottom point furthest from me
			5.634f, -2.867f, -29.816f, 0.0f, 0.0f, // bottom point furthest from me
			5.634f, -2.867f, -29.75f, 1.0f, 0.0f, // bottom point closest to me
			5.634f, -1.516f, -29.75f, 1.0f, 1.0f, // top point closest to me

												  // silver bar1 left, left
			5.567f, -1.516f, -29.75f, 1.0f, 1.0f, // top point closest to me
			5.567f, -1.516f, -29.816f, 0.0f, 1.0f, // top point furthest from me
			5.567f, -2.867f, -29.816f, 0.0f, 0.0f, // bottom point furthest from me
			5.567f, -2.867f, -29.816f, 0.0f, 0.0f, // bottom point furthest from me
			5.567f, -2.867f, -29.75f, 1.0f, 0.0f, // bottom point closest to me
			5.567f, -1.516f, -29.75f, 1.0f, 1.0f, // top point closest to me

												  // silver bar2 left, right
			5.634f, -1.516f, -30.15f, 1.0f, 1.0f, // top point closest to me
			5.634f, -1.516f, -30.22f, 0.0f, 1.0f, // top point furthest from me
			5.634f, -2.867f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.634f, -2.867f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.634f, -2.867f, -30.15f, 1.0f, 0.0f, // bottom point closest to me
			5.634f, -1.516f, -30.15f, 1.0f, 1.0f, // top point closest to me

												  // silver bar2 left, left
			5.567f, -1.516f, -30.15f, 1.0f, 1.0f, // top point closest to me
			5.567f, -1.516f, -30.22f, 0.0f, 1.0f, // top point furthest from me
			5.567f, -2.867f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.567f, -2.867f, -30.22f, 0.0f, 0.0f, // bottom point furthest from me
			5.567f, -2.867f, -30.15f, 1.0f, 0.0f, // bottom point closest to me
			5.567f, -1.516f, -30.15f, 1.0f, 1.0f // top point closest to me

	};


	
	glm::vec3 tablePositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, -9.5),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -9.4),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(6.543f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 9.4),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 9.5),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(4.767f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -6.5),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -6.4),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(6.543f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 6.4),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 6.5),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		//start rotated tables
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(6.543f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(4.767f, 0.0f, 0.0),
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(6.543f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, -3.0f),


	};


	GLfloat bigTableTemp[] =
	{
										//tabletop top
		5.71f, -1.4f, -30.29f, 1.0f, 0.0f,// furthest left point
		9.64f, -1.4f, -30.29f, 0.0f, 0.0f,// furthest right point
		9.64f, -1.4f, -29.09f, 0.0f, 1.0f,// closest right point
		9.64f, -1.4f, -29.09f, 0.0f, 1.0f,// closest right point
		5.71f, -1.4f, -29.09f, 1.0f, 1.0f,// closest left point
		5.71f, -1.4f, -30.29f, 1.0f, 0.0f,// furthest left point

										 //tabletop bottom
		5.71f, -1.45f, -30.29f, 1.0f, 0.0f,// furthest left point
		9.64f, -1.45f, -30.29f, 0.0f, 0.0f,// furthest right point
		9.64f, -1.45f, -29.09f, 0.0f, 1.0f,// closest right point
		9.64f, -1.45f, -29.09f, 0.0f, 1.0f,// closest right point
		5.71f, -1.45f, -29.09f, 1.0f, 1.0f,// closest left point
		5.71f, -1.45f, -30.29f, 1.0f, 0.0f,// furthest left point

										  // tabletop left
		5.71f, -1.4f, -29.09f, 1.0f, 1.0f, // top point closest to me
		5.71f, -1.4f, -30.29f, 0.0f, 1.0f, // top point furthest from me
		5.71f, -1.45f, -30.29f, 0.0f, 0.0f, // bottom point furthest from me
		5.71f, -1.45f, -30.29f, 0.0f, 0.0f, // bottom point furthest from me
		5.71f, -1.45f, -29.09f, 1.0f, 0.0f, // bottom point closest to me
		5.71f, -1.4f, -29.09f, 1.0f, 1.0f, // top point closest to me

										  // tabletop right
		9.64f, -1.4f, -29.09f, 1.0f, 1.0f, // top point closest to me
		9.64f, -1.4f, -30.29f, 0.0f, 1.0f, // top point furthest from me
		9.64f, -1.45f, -30.29f, 0.0f, 0.0f, // bottom point furthest from me
		9.64f, -1.45f, -30.29f, 0.0f, 0.0f, // bottom point furthest from me
		9.64f, -1.45f, -29.09f, 1.0f, 0.0f, // bottom point closest to me
		9.64f, -1.4f, -29.09f, 1.0f, 1.0f, // top point closest to me

										  // tabletop front
		9.64f,   -1.4f, -30.29, 0.0f, 1.0f, // top point closest to me
		5.71f, -1.4f, -30.29f, 1.0f, 1.0f, // top point furthest from me
		5.71f, -1.45f, -30.29f, 1.0f, 0.0f, // bottom point furthest from me
		5.71f, -1.45f, -30.29f, 1.0f, 0.0f, // bottom point furthest from me
		9.64f,   -1.45f, -30.29f, 0.0f, 0.0f, // bottom point closest to me
		9.64f,   -1.4f, -30.29f, 0.0f, 1.0f, // top point closest to me

											// tabletop back
		9.64f,   -1.4f, -29.09, 0.0f, 1.0f, // top point closest to me
		5.71f, -1.4f, -29.09f, 1.0f, 1.0f, // top point furthest from me
		5.71f, -1.45f, -29.09f, 1.0f, 0.0f, // bottom point furthest from me
		5.71f, -1.45f, -29.09f, 1.0f, 0.0f, // bottom point furthest from me
		9.64f,   -1.45f, -29.09f, 0.0f, 0.0f, // bottom point closest to me
		9.64f,   -1.4f, -29.09f, 0.0f, 1.0f, // top point closest to me

											 // silver bar connector1 back
		9.509f, -1.45f, -29.156, 0.0f, 1.0f, // top point closest to me
		5.843f, -1.45f, -29.156f, 1.0f, 1.0f, // top point furthest from me
		5.843f, -1.516f, -29.156f, 1.0f, 0.0f, // bottom point furthest from me
		5.843f, -1.516f, -29.156f, 1.0f, 0.0f, // bottom point furthest from me
		9.509f, -1.516f, -29.156f, 0.0f, 0.0f, // bottom point closest to me
		9.509f, -1.45f, -29.156f, 0.0f, 1.0f, // top point closest to me

											  // silver bar connector1 forward
		9.509f, -1.45f, -29.22, 0.0f, 1.0f, // top point closest to me
		5.843f, -1.45f, -29.22f, 1.0f, 1.0f, // top point furthest from me
		5.843f, -1.516f, -29.22f, 1.0f, 0.0f, // bottom point furthest from me
		5.843f, -1.516f, -29.22f, 1.0f, 0.0f, // bottom point furthest from me
		9.509f, -1.516f, -29.22f, 0.0f, 0.0f, // bottom point closest to me
		9.509f, -1.45f, -29.22f, 0.0f, 1.0f, // top point closest to me

											 // silver bar connector2 back
		9.509f, -1.45f, -30.15, 0.0f, 1.0f, // top point closest to me
		5.843f, -1.45f, -30.15f, 1.0f, 1.0f, // top point furthest from me
		5.843f, -1.516f, -30.15f, 1.0f, 0.0f, // bottom point furthest from me
		5.843f, -1.516f, -30.15f, 1.0f, 0.0f, // bottom point furthest from me
		9.509f, -1.516f, -30.15f, 0.0f, 0.0f, // bottom point closest to me
		9.509f, -1.45f, -30.15f, 0.0f, 1.0f, // top point closest to me

											 // silver bar connector2 forward
		9.509f, -1.45f, -30.22, 0.0f, 1.0f, // top point closest to me
		5.843f, -1.45f, -30.22f, 1.0f, 1.0f, // top point furthest from me
		5.843f, -1.516f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
		5.843f, -1.516f, -30.22f, 1.0f, 0.0f, // bottom point furthest from me
		9.509f, -1.516f, -30.22f, 0.0f, 0.0f, // bottom point closest to me
		9.509f, -1.45f, -30.22f, 0.0f, 1.0f, // top point closest to me

											 // silver bar connector 1 underside
		5.843f, -1.516f, -29.22f, 1.0f, 0.0f,// furthest left point
		9.509f, -1.516f, -29.22f, 0.0f, 0.0f,// furthest right point
		9.509f, -1.516f, -29.156f, 0.0f, 1.0f,// closest right point
		9.509f, -1.516f, -29.156f, 0.0f, 1.0f,// closest right point
		5.843f, -1.516f, -29.156f, 1.0f, 1.0f,// closest left point
		5.843f, -1.516f, -29.22f, 1.0f, 0.0f,// furthest left point

											 // silver bar connector 2 underside
		5.843f, -1.516f, -30.22f, 1.0f, 0.0f,// furthest left point
		9.509f, -1.516f, -30.22f, 0.0f, 0.0f,// furthest right point
		9.509f, -1.516f, -30.15f, 0.0f, 1.0f,// closest right point
		9.509f, -1.516f, -30.15f, 0.0f, 1.0f,// closest right point
		5.843f, -1.516f, -30.15f, 1.0f, 1.0f,// closest left point
		5.843f, -1.516f, -30.22f, 1.0f, 0.0f// furthest left point


	};

	GLfloat bigTable[1860];

	for (int i = 0; i < 1860; i++)
	{
		if (i < 360)
		{
			bigTable[i] = bigTableTemp[i];
		}
		
		if (i >= 360 && i < 1110 )
		{
			if (i % 5 == 0)
			{
				bigTable[i] = table[i] + 2.61;
			}
			else
				bigTable[i] = table[i];
		}

		if (i > 1109)
		{

			if (i % 5 == 0)
			{
				bigTable[i] = table[i] + 3.933;
			}
			else
				bigTable[i] = table[i];
		}
	}

	glm::vec3 bigTablePositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, -9.5),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -9.4),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(0.0f, 0.0f, -3.0),
		glm::vec3(11.308f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 6.4),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 6.5),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0),
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(11.308f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	
	};

	GLfloat monitor[] = 
	{
		// base top
		-4.3f, -1.28f, -35.2f, 1.0f, 0.0f,// furthest left point
		-3.9f, -1.28f, -35.2f, 0.0f, 0.0f,// furthest right point
		-3.9f, -1.28f, -34.9f, 0.0f, 1.0f,// closest right point
		-3.9f, -1.28f, -34.9f, 0.0f, 1.0f,// closest right point
		-4.3f, -1.28f, -34.9f, 1.0f, 1.0f,// closest left point
		-4.3f, -1.28f, -35.2f, 1.0f, 0.0f,// furthest left point

		// base back
		-3.9f,   -1.28f, -35.2f, 0.0f, 1.0f, // top point closest to me
		-4.3f, -1.28f, -35.2f, 1.0f, 1.0f, // top point furthest from me
		-4.3f, -1.33f, -35.2f, 1.0f, 0.0f, // bottom point furthest from me
		-4.3f, -1.33f, -35.2f, 1.0f, 0.0f, // bottom point furthest from me
		-3.9f,   -1.33f, -35.2f, 0.0f, 0.0f, // bottom point closest to me
		-3.9f,   -1.28f, -35.2f, 0.0f, 1.0f, // top point closest to me
		
		// base front
		-3.9f,   -1.28f, -34.9f, 0.0f, 1.0f, // top point closest to me
		-4.3f, -1.28f, -34.9f, 1.0f, 1.0f, // top point furthest from me
		-4.3f, -1.33f, -34.9f, 1.0f, 0.0f, // bottom point furthest from me
		-4.3f, -1.33f, -34.9f, 1.0f, 0.0f, // bottom point furthest from me
		-3.9f,   -1.33f, -34.9f, 0.0f, 0.0f, // bottom point closest to me
		-3.9f,   -1.28f, -34.9f, 0.0f, 1.0f, // top point closest to me
		
		// base left
		-3.9f, -1.28f, -34.9f, 1.0f, 1.0f, // top point closest to me
		-3.9f, -1.28f, -35.2f, 0.0f, 1.0f, // top point furthest from me
		-3.9f, -1.33f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-3.9f, -1.33f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-3.9f, -1.33f, -34.9f, 1.0f, 0.0f, // bottom point closest to me
		-3.9f, -1.28f, -34.9f, 1.0f, 1.0f, // top point closest to me
		
		// base right
		-4.3f, -1.28f, -34.9f, 1.0f, 1.0f, // top point closest to me
		-4.3f, -1.28f, -35.2f, 0.0f, 1.0f, // top point furthest from me
		-4.3f, -1.33f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-4.3f, -1.33f, -35.2f, 0.0f, 0.0f, // bottom point furthest from me
		-4.3f, -1.33f, -34.9f, 1.0f, 0.0f, // bottom point closest to me
		-4.3f, -1.28f, -34.9f, 1.0f, 1.0f, // top point closest to me
		
		// neck front
		-4.025f,   -0.78f, -35.1f, 0.0f, 1.0f, // top point closest to me
		-4.175f, -0.78f, -35.1f, 1.0f, 1.0f, // top point furthest from me
		-4.175f, -1.28f, -35.1f, 1.0f, 0.0f, // bottom point furthest from me
		-4.175f, -1.28f, -35.1f, 1.0f, 0.0f, // bottom point furthest from me
		-4.025f,   -1.28f, -35.1f, 0.0f, 0.0f, // bottom point closest to me
		-4.025f,   -0.78f, -35.1f, 0.0f, 1.0f, // top point closest to me
		
		// neck back
		-4.025f,   -0.78f, -35.05f, 0.0f, 1.0f, // top point closest to me
		-4.175f, -0.78f, -35.05f, 1.0f, 1.0f, // top point furthest from me
		-4.175f, -1.28f, -35.05f, 1.0f, 0.0f, // bottom point furthest from me
		-4.175f, -1.28f, -35.05f, 1.0f, 0.0f, // bottom point furthest from me
		-4.025f,   -1.28f, -35.05f, 0.0f, 0.0f, // bottom point closest to me
		-4.025f,   -0.78f, -35.05f, 0.0f, 1.0f, // top point closest to me

		// neck top
		-4.175f, -0.78f, -35.1f, 1.0f, 0.0f,// furthest left point
		-4.025f, -0.78f, -35.1f, 0.0f, 0.0f,// furthest right point
		-4.025f, -0.78f, -35.05f, 0.0f, 1.0f,// closest right point
		-4.025f, -0.78f, -35.05f, 0.0f, 1.0f,// closest right point
		-4.175f, -0.78f, -35.05f, 1.0f, 1.0f,// closest left point
		-4.175f, -0.78f, -35.1f, 1.0f, 0.0f,// furthest left point
		// neck left
		-4.025f, -0.78f, -35.05f, 1.0f, 1.0f, // top point closest to me
		-4.025f, -0.78f, -35.1f, 0.0f, 1.0f, // top point furthest from me
		-4.025f, -1.28f, -35.1f, 0.0f, 0.0f, // bottom point furthest from me
		-4.025f, -1.28f, -35.1f, 0.0f, 0.0f, // bottom point furthest from me
		-4.025f, -1.28f, -35.05f, 1.0f, 0.0f, // bottom point closest to me
		-4.025f, -0.78f, -35.05f, 1.0f, 1.0f, // top point closest to me
		// neck right
		-4.175f, -0.78f, -35.05f, 1.0f, 1.0f, // top point closest to me
		-4.175f, -0.78f, -35.1f, 0.0f, 1.0f, // top point furthest from me
		-4.175f, -1.28f, -35.1f, 0.0f, 0.0f, // bottom point furthest from me
		-4.175f, -1.28f, -35.1f, 0.0f, 0.0f, // bottom point furthest from me
		-4.175f, -1.28f, -35.05f, 1.0f, 0.0f, // bottom point closest to me
		-4.175f, -0.78f, -35.05f, 1.0f, 1.0f, // top point closest to me

		// screen left 
		-3.4f, -0.38f, -35.1f, 1.0f, 1.0f, // top point closest to me
		-3.4f, -0.38f, -35.15f, 0.0f, 1.0f, // top point furthest from me
		-3.4f, -1.08f, -35.15f, 0.0f, 0.0f, // bottom point furthest from me
		-3.4f, -1.08f, -35.15f, 0.0f, 0.0f, // bottom point furthest from me
		-3.4f, -1.08f, -35.1f, 1.0f, 0.0f, // bottom point closest to me
		-3.4f, -0.38f, -35.1f, 1.0f, 1.0f, // top point closest to me
		// screen right
		-4.8f, -0.38f, -35.1f, 1.0f, 1.0f, // top point closest to me
		-4.8f, -0.38f, -35.15f, 0.0f, 1.0f, // top point furthest from me
		-4.8f, -1.08f, -35.15f, 0.0f, 0.0f, // bottom point furthest from me
		-4.8f, -1.08f, -35.15f, 0.0f, 0.0f, // bottom point furthest from me
		-4.8f, -1.08f, -35.1f, 1.0f, 0.0f, // bottom point closest to me
		-4.8f, -0.38f, -35.1f, 1.0f, 1.0f, // top point closest to me
		// screen top
		-4.8f, -0.38f, -35.15f, 1.0f, 0.0f,// furthest left point
		-3.4f, -0.38f, -35.15f, 0.0f, 0.0f,// furthest right point
		-3.4f, -0.38f, -35.1f, 0.0f, 1.0f,// closest right point
		-3.4f, -0.38f, -35.1f, 0.0f, 1.0f,// closest right point
		-4.8f, -0.38f, -35.1f, 1.0f, 1.0f,// closest left point
		-4.8f, -0.38f, -35.15f, 1.0f, 0.0f,// furthest left point
		// screen bottom
		-4.8f, -1.08f, -35.15f, 1.0f, 0.0f,// furthest left point
		-3.4f, -1.08f, -35.15f, 0.0f, 0.0f,// furthest right point
		-3.4f, -1.08f, -35.1f, 0.0f, 1.0f,// closest right point
		-3.4f, -1.08f, -35.1f, 0.0f, 1.0f,// closest right point
		-4.8f, -1.08f, -35.1f, 1.0f, 1.0f,// closest left point
		-4.8f, -1.08f, -35.15f, 1.0f, 0.0f,// furthest left point

		// keyboard back
		-3.7f, -1.30f, -35.8f, 0.0f, 1.0f, // top point closest to me
		-4.5f, -1.30f, -35.8f, 1.0f, 1.0f, // top point furthest from me
		-4.5f, -1.33f, -35.8f, 1.0f, 0.0f, // bottom point furthest from me
		-4.5f, -1.33f, -35.8f, 1.0f, 0.0f, // bottom point furthest from me
		-3.7f, -1.33f, -35.8f, 0.0f, 0.0f, // bottom point closest to me
		-3.7f, -1.30f, -35.8f, 0.0f, 1.0f, // top point closest to me

		// keyboard front
		-3.7f, -1.30f, -35.5f, 0.0f, 1.0f, // top point closest to me
		-4.5f, -1.30f, -35.5f, 1.0f, 1.0f, // top point furthest from me
		-4.5f, -1.33f, -35.5f, 1.0f, 0.0f, // bottom point furthest from me
		-4.5f, -1.33f, -35.5f, 1.0f, 0.0f, // bottom point furthest from me
		-3.7f, -1.33f, -35.5f, 0.0f, 0.0f, // bottom point closest to me
		-3.7f, -1.30f, -35.5f, 0.0f, 1.0f, // top point closest to me

		// keyboard left
		-3.7f, -1.30f, -35.5f, 1.0f, 1.0f, // top point closest to me
		-3.7f, -1.30f, -35.8f, 0.0f, 1.0f, // top point furthest from me
		-3.7f, -1.33f, -35.8f, 0.0f, 0.0f, // bottom point furthest from me
		-3.7f, -1.33f, -35.8f, 0.0f, 0.0f, // bottom point furthest from me
		-3.7f, -1.33f, -35.5f, 1.0f, 0.0f, // bottom point closest to me
		-3.7f, -1.30f, -35.5f, 1.0f, 1.0f, // top point closest to me

		// keyboard right
		-4.5f, -1.30f, -35.5f, 1.0f, 1.0f, // top point closest to me
		-4.5f, -1.30f, -35.8f, 0.0f, 1.0f, // top point furthest from me
		-4.5f, -1.33f, -35.8f, 0.0f, 0.0f, // bottom point furthest from me
		-4.5f, -1.33f, -35.8f, 0.0f, 0.0f, // bottom point furthest from me
		-4.5f, -1.33f, -35.5f, 1.0f, 0.0f, // bottom point closest to me
		-4.5f, -1.30f, -35.5f, 1.0f, 1.0f, // top point closest to me
		// comp left
		-4.9f, -1.33f, -34.4f, 0.0f, 0.0f, // bottom left point
		-4.2f, -1.33f, -34.4f, 1.0f, 0.0f, // bottom right point
		-4.2f, -0.7f, -34.4f, 1.0f, 1.0f, // top right point (shared)
		-4.2f, -0.7f, -34.4f, 1.0f, 1.0f, // top right point (shared)
		-4.9f, -0.7f, -34.4f, 0.0f, 1.0f, // top left point
		-4.9f, -1.33f, -34.4f, 0.0f, 0.0f, // bottom left point
		// comp right
		-4.9f, -1.33f, -34.6f, 0.0f, 0.0f, // bottom left point
		-4.2f, -1.33f, -34.6f, 1.0f, 0.0f, // bottom right point
		-4.2f, -0.7f, -34.6f, 1.0f, 1.0f, // top right point (shared)
		-4.2f, -0.7f, -34.6f, 1.0f, 1.0f, // top right point (shared)
		-4.9f, -0.7f, -34.6f, 0.0f, 1.0f, // top left point
		-4.9f, -1.33f, -34.6f, 0.0f, 0.0f, // bottom left point
		// comp top
		-4.9f, -0.7f, -34.6f, 1.0f, 0.0f,// furthest left point
		-4.2f, -0.7f, -34.6f, 0.0f, 0.0f,// furthest right point
		-4.2f, -0.7f, -34.4f, 0.0f, 1.0f,// closest right point
		-4.2f, -0.7f, -34.4f, 0.0f, 1.0f,// closest right point
		-4.9f, -0.7f, -34.4f, 1.0f, 1.0f,// closest left point
		-4.9f, -0.7f, -34.6f, 1.0f, 0.0f,// furthest left point
		// screen front
		-4.8f, -1.08f, -35.15f,  0.0f, 0.0f, // bottom left point
		-3.4f, -1.08f, -35.15f,  1.0f, 0.0f, // bottom right point
		-3.4f,  -0.38f, -35.15f,  1.0f, 1.0f, // top right point (shared)
		-3.4f,  -0.38f, -35.15f,  1.0f, 1.0f, // top right point (shared)
		-4.8f,  -0.38f, -35.15f,  0.0f, 1.0f, // top left point
		-4.8f, -1.08f, -35.15f,  0.0f, 0.0f, // bottom left point
		// screen back
		-4.8f, -1.08f, -35.1f, 0.0f, 0.0f, // bottom left point
		-3.4f, -1.08f, -35.1f, 1.0f, 0.0f, // bottom right point
		-3.4f, -0.38f, -35.1f, 1.0f, 1.0f, // top right point (shared)
		-3.4f, -0.38f, -35.1f, 1.0f, 1.0f, // top right point (shared)
		-4.8f, -0.38f, -35.1f, 0.0f, 1.0f, // top left point
		-4.8f, -1.08f, -35.1f, 0.0f, 0.0f, // bottom left point
		// keyboard top
		-4.5f, -1.30f, -35.8f, 1.0f, 0.0f,// furthest left point
		-3.7f, -1.30f, -35.8f, 0.0f, 0.0f,// furthest right point
		-3.7f, -1.30f, -35.5f, 0.0f, 1.0f,// closest right point
		-3.7f, -1.30f, -35.5f, 0.0f, 1.0f,// closest right point
		-4.5f, -1.30f, -35.5f, 1.0f, 1.0f,// closest left point
		-4.5f, -1.30f, -35.8f, 1.0f, 0.0f,// furthest left point
		// comp front
		-4.2f, -0.7f, -34.4f, 1.0f, 1.0f, // top point closest to me
		-4.2f, -0.7f, -34.6f, 0.0f, 1.0f, // top point furthest from me
		-4.2f, -1.33f, -34.6f, 0.0f, 0.0f, // bottom point furthest from me
		-4.2f, -1.33f, -34.6f, 0.0f, 0.0f, // bottom point furthest from me
		-4.2f, -1.33f, -34.4f, 1.0f, 0.0f, // bottom point closest to me
		-4.2f, -0.7f, -34.4, 1.0f, 1.0f, // top point closest to me

		// comp back
		-4.9f, -0.7f, -34.4f, 1.0f, 1.0f, // top point closest to me
		-4.9f, -0.7f, -34.6f, 0.0f, 1.0f, // top point furthest from me
		-4.9f, -1.33f, -34.6f, 0.0f, 0.0f, // bottom point furthest from me
		-4.9f, -1.33f, -34.6f, 0.0f, 0.0f, // bottom point furthest from me
		-4.9f, -1.33f, -34.4f, 1.0f, 0.0f, // bottom point closest to me
		-4.9f, -0.7f, -34.4, 1.0f, 1.0f, // top point closest to me
		
	};

	glm::vec3 monitorPositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(-1.9f, 0.0f, 0.0f),
		glm::vec3(-1.9f, 0.0f, 0.0f),
		glm::vec3(-1.9f, 0.0f, 0.0f),
		glm::vec3(-1.9f, 0.0f, 0.0f),
		glm::vec3(0.0, 0.0, 3.55),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(0.0, 0.0, 3.55),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-5.75f, 0.0f, 0.2f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(0.0, 0.0, -3.55),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(0.0, 0.0, -3.55),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(-1.9, 0.0f, 0.0f),
		glm::vec3(0.0, 0.0, -3.84),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f),
		glm::vec3(1.9, 0.0f, 0.0f)
	};
	GLfloat woodDesk[] =
	{
		// top of offices wood desk
		-3.79f,   -2.1f, -83.5, 0.0f, 0.0f,// closest right point
		-6.46f,   -2.1f, -83.5f, 0.0f, 1.0f,// closest left point
		-6.46f,   -2.1f, -84.6f, 1.0f, 1.0f,// furthest left point
		-6.46f,   -2.1f, -84.6f, 1.0f, 1.0f,// furthest left point
		-3.79f,   -2.1f, -84.6f, 1.0f, 0.0f,// furthest right point
		-3.79f,   -2.1f, -83.5f, 0.0f, 0.0f,// closest right point

											// bottom of offices wood desk
		-3.79f, -2.2f, -83.5, 0.0f, 0.0f,// closest right point
		-6.46f, -2.2f, -83.5f, 0.0f, 1.0f,// closest left point
		-6.46f, -2.2f, -84.6f, 1.0f, 1.0f,// furthest left point
		-6.46f, -2.2f, -84.6f, 1.0f, 1.0f,// furthest left point
		-3.79f, -2.2f, -84.6f, 1.0f, 0.0f,// furthest right point
		-3.79f, -2.2f, -83.5f, 0.0f, 0.0f,// closest right point


										  // offices wood desk top right leg, right
		-3.79f, -2.2f, -84.5f, 1.0f, 1.0f, // top point closest to me
		-3.79f, -2.2f, -84.6f, 0.0f, 1.0f, // top point furthest from me
		-3.79f, -3.0f, -84.6f, 0.0f, 0.0f, // bottom point furthest from me
		-3.79f, -3.0f, -84.6f, 0.0f, 0.0f, // bottom point furthest from me
		-3.79f, -3.0f, -84.5f, 1.0f, 0.0f, // bottom point closest to me
		-3.79f, -2.2f, -84.5f, 1.0f, 1.0f, // top point closest to me
										   // offices wood desk top right leg, left
		-3.89f, -2.2f, -84.5f, 1.0f, 1.0f, // top point closest to me
		-3.89f, -2.2f, -84.6f, 0.0f, 1.0f, // top point furthest from me
		-3.89f, -3.0f, -84.6f, 0.0f, 0.0f, // bottom point furthest from me
		-3.89f, -3.0f, -84.6f, 0.0f, 0.0f, // bottom point furthest from me
		-3.89f, -3.0f, -84.5f, 1.0f, 0.0f, // bottom point closest to me
		-3.89f, -2.2f, -84.5f, 1.0f, 1.0f, // top point closest to me
										   // offices wood desk top left leg, right
		-6.36f, -2.2f, -84.5f, 1.0f, 1.0f, // top point closest to me
		-6.36f, -2.2f, -84.6f, 0.0f, 1.0f, // top point furthest from me
		-6.36f, -3.0f, -84.6f, 0.0f, 0.0f, // bottom point furthest from me
		-6.36f, -3.0f, -84.6f, 0.0f, 0.0f, // bottom point furthest from me
		-6.36f, -3.0f, -84.5f, 1.0f, 0.0f, // bottom point closest to me
		-6.36f, -2.2f, -84.5f, 1.0f, 1.0f, // top point closest to me
										   // offices wood desk top left leg, left
		-6.46f, -2.2f, -84.5f, 1.0f, 1.0f, // top point closest to me
		-6.46f, -2.2f, -84.6f, 0.0f, 1.0f, // top point furthest from me
		-6.46f, -3.0f, -84.6f, 0.0f, 0.0f, // bottom point furthest from me
		-6.46f, -3.0f, -84.6f, 0.0f, 0.0f, // bottom point furthest from me
		-6.46f, -3.0f, -84.5f, 1.0f, 0.0f, // bottom point closest to me
		-6.46f, -2.2f, -84.5f, 1.0f, 1.0f, // top point closest to me
										   // offices wood desk back right leg, right
		-3.79f, -2.2f, -83.5f, 1.0f, 1.0f, // top point closest to me
		-3.79f, -2.2f, -83.6f, 0.0f, 1.0f, // top point furthest from me
		-3.79f, -3.0f, -83.6, 0.0f, 0.0f, // bottom point furthest from me
		-3.79f, -3.0f, -83.6f, 0.0f, 0.0f, // bottom point furthest from me
		-3.79f, -3.0f, -83.5f, 1.0f, 0.0f, // bottom point closest to me
		-3.79f, -2.2f, -83.5f, 1.0f, 1.0f, // top point closest to me
										   // offices wood desk back right leg, left
		-3.89f, -2.2f, -83.5f, 1.0f, 1.0f, // top point closest to me
		-3.89f, -2.2f, -83.6f, 0.0f, 1.0f, // top point furthest from me
		-3.89f, -3.0f, -83.6, 0.0f, 0.0f, // bottom point furthest from me
		-3.89f, -3.0f, -83.6f, 0.0f, 0.0f, // bottom point furthest from me
		-3.89f, -3.0f, -83.5f, 1.0f, 0.0f, // bottom point closest to me
		-3.89f, -2.2f, -83.5f, 1.0f, 1.0f, // top point closest to me
										   // offices wood desk back left leg, right
		-6.36f, -2.2f, -83.5f, 1.0f, 1.0f, // top point closest to me
		-6.36f, -2.2f, -83.6f, 0.0f, 1.0f, // top point furthest from me
		-6.36f, -3.0f, -83.6, 0.0f, 0.0f, // bottom point furthest from me
		-6.36f, -3.0f, -83.6f, 0.0f, 0.0f, // bottom point furthest from me
		-6.36f, -3.0f, -83.5f, 1.0f, 0.0f, // bottom point closest to me
		-6.36f, -2.2f, -83.5f, 1.0f, 1.0f, // top point closest to me
										   // offices wood desk back left leg, left
		-6.46f, -2.2f, -83.5f, 1.0f, 1.0f, // top point closest to me
		-6.46f, -2.2f, -83.6f, 0.0f, 1.0f, // top point furthest from me
		-6.46f, -3.0f, -83.6, 0.0f, 0.0f, // bottom point furthest from me
		-6.46f, -3.0f, -83.6f, 0.0f, 0.0f, // bottom point furthest from me
		-6.46f, -3.0f, -83.5f, 1.0f, 0.0f, // bottom point closest to me
		-6.46f, -2.2f, -83.5f, 1.0f, 1.0f, // top point closest to me


										   // offices wood desk top right leg, front
		-3.89f, -3.0f, -84.6f, 0.0f, 0.0f,//bottom left point
		-3.79f, -3.0f, -84.6f, 1.0f, 0.0f,// bottom right point
		-3.79f, -2.2f, -84.6f, 1.0f, 1.0f,// top right point
		-3.79f, -2.2f, -84.6f, 1.0f, 1.0f,// top right point
		-3.89f, -2.2f, -84.6f, 0.0f, 1.0f,// top left point
		-3.89f, -3.0f, -84.6f, 0.0f, 0.0f,//bottom left point
										  // offices wood desk top right leg, back
		-3.89f, -3.0f, -84.5f, 0.0f, 0.0f,//bottom left point
		-3.79f, -3.0f, -84.5f, 1.0f, 0.0f,// bottom right point
		-3.79f, -2.2f, -84.5f, 1.0f, 1.0f,// top right point
		-3.79f, -2.2f, -84.5f, 1.0f, 1.0f,// top right point
		-3.89f, -2.2f, -84.5f, 0.0f, 1.0f,// top left point
		-3.89f, -3.0f, -84.5f, 0.0f, 0.0f,//bottom left point
										  // offices wood desk back right leg, front
		-3.89f, -3.0f, -83.6f, 0.0f, 0.0f,//bottom left point
		-3.79f, -3.0f, -83.6f, 1.0f, 0.0f,// bottom right point
		-3.79f, -2.2f, -83.6f, 1.0f, 1.0f,// top right point
		-3.79f, -2.2f, -83.6f, 1.0f, 1.0f,// top right point
		-3.89f, -2.2f, -83.6f, 0.0f, 1.0f,// top left point
		-3.89f, -3.0f, -83.6f, 0.0f, 0.0f,//bottom left point
										  // offices wood desk back right leg, back
		-3.89f, -3.0f, -83.5f, 0.0f, 0.0f,//bottom left point
		-3.79f, -3.0f, -83.5f, 1.0f, 0.0f,// bottom right point
		-3.79f, -2.2f, -83.5f, 1.0f, 1.0f,// top right point
		-3.79f, -2.2f, -83.5f, 1.0f, 1.0f,// top right point
		-3.89f, -2.2f, -83.5f, 0.0f, 1.0f,// top left point
		-3.89f, -3.0f, -83.5f, 0.0f, 0.0f,//bottom left point
										  // offices wood desk top left leg, front
		-6.46f, -3.0f, -84.6f, 0.0f, 0.0f,//bottom left point
		-6.36f, -3.0f, -84.6f, 1.0f, 0.0f,// bottom right point
		-6.36f, -2.2f, -84.6f, 1.0f, 1.0f,// top right point
		-6.36f, -2.2f, -84.6f, 1.0f, 1.0f,// top right point
		-6.46f, -2.2f, -84.6f, 0.0f, 1.0f,// top left point
		-6.46f, -3.0f, -84.6f, 0.0f, 0.0f,//bottom left point
										  // offices wood desk top left leg, back
		-6.46f, -3.0f, -84.5f, 0.0f, 0.0f,//bottom left point
		-6.36f, -3.0f, -84.5f, 1.0f, 0.0f,// bottom right point
		-6.36f, -2.2f, -84.5f, 1.0f, 1.0f,// top right point
		-6.36f, -2.2f, -84.5f, 1.0f, 1.0f,// top right point
		-6.46f, -2.2f, -84.5f, 0.0f, 1.0f,// top left point
		-6.46f, -3.0f, -84.5f, 0.0f, 0.0f,//bottom left point
										  // offices wood desk back left leg, front
		-6.46f, -3.0f, -83.6f, 0.0f, 0.0f,//bottom left point
		-6.36f, -3.0f, -83.6f, 1.0f, 0.0f,// bottom right point
		-6.36f, -2.2f, -83.6f, 1.0f, 1.0f,// top right point
		-6.36f, -2.2f, -83.6f, 1.0f, 1.0f,// top right point
		-6.46f, -2.2f, -83.6f, 0.0f, 1.0f,// top left point
		-6.46f, -3.0f, -83.6f, 0.0f, 0.0f,//bottom left point
										  // offices wood desk back left leg, back
		-6.46f, -3.0f, -83.5f, 0.0f, 0.0f,//bottom left point
		-6.36f, -3.0f, -83.5f, 1.0f, 0.0f,// bottom right point
		-6.36f, -2.2f, -83.5f, 1.0f, 1.0f,// top right point
		-6.36f, -2.2f, -83.5f, 1.0f, 1.0f,// top right point
		-6.46f, -2.2f, -83.5f, 0.0f, 1.0f,// top left point
		-6.46f, -3.0f, -83.5f, 0.0f, 0.0f,//bottom left point

										  // offices desk wood lining front
		-6.46f, -2.2f, -83.5f, 0.0f, 0.0f,//bottom left point
		-3.79f, -2.2f, -83.5f, 1.0f, 0.0f,// bottom right point
		-3.79f, -2.1f, -83.5f, 1.0f, 1.0f,// top right point
		-3.79f, -2.1f, -83.5f, 1.0f, 1.0f,// top right point
		-6.46f, -2.1f, -83.5f, 0.0f, 1.0f,// top left point
		-6.46f, -2.2f, -83.5f, 0.0f, 0.0f,//bottom left point
										  // offices desk wood lining back
		-6.46f, -2.2f, -84.6f, 0.0f, 0.0f,//bottom left point
		-3.79f, -2.2f, -84.6f, 1.0f, 0.0f,// bottom right point
		-3.79f, -2.1f, -84.6f, 1.0f, 1.0f,// top right point
		-3.79f, -2.1f, -84.6f, 1.0f, 1.0f,// top right point
		-6.46f, -2.1f, -84.6f, 0.0f, 1.0f,// top left point
		-6.46f, -2.2f, -84.6f, 0.0f, 0.0f,//bottom left point
										  // offices desk wood lining right
		-3.79f, -2.1f, -83.5f, 1.0f, 1.0f, // top point closest to me
		-3.79f, -2.1f, -84.6f, 0.0f, 1.0f, // top point furthest from me
		-3.79f, -2.2f, -84.6f, 0.0f, 0.0f, // bottom point furthest from me
		-3.79f, -2.2f, -84.6f, 0.0f, 0.0f, // bottom point furthest from me
		-3.79f, -2.2f, -83.5f, 1.0f, 0.0f, // bottom point closest to me
		-3.79f, -2.1f, -83.5f, 1.0f, 1.0f, // top point closest to me
										   // offices desk wood lining left
		-6.46f, -2.1f, -83.5f, 1.0f, 1.0f, // top point closest to me
		-6.46f, -2.1f, -84.6f, 0.0f, 1.0f, // top point furthest from me
		-6.46f, -2.2f, -84.6f, 0.0f, 0.0f, // bottom point furthest from me
		-6.46f, -2.2f, -84.6f, 0.0f, 0.0f, // bottom point furthest from me
		-6.46f, -2.2f, -83.5f, 1.0f, 0.0f, // bottom point closest to me
		-6.46f, -2.1f, -83.5f, 1.0f, 1.0f, // top point closest to me
	};

	glm::vec3 benchPositions[] =
	{
		glm::vec3(-3.4f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(3.4f, 0.0f, 0.0f)
	};

	GLfloat desk[] =
	{
		// top of offices desk north 8.5length
		-2.63f,   -1.1f, -74.36, 0.0f, 0.0f,// closest right point
		-3.96f,   -1.1f, -74.36f, 0.0f, 1.0f,// closest left point
		-3.96f,   -1.1f, -80.02f, 1.0f, 1.0f,// furthest left point
		-3.96f,   -1.1f, -80.02f, 1.0f, 1.0f,// furthest left point
		-2.63f,   -1.1f, -80.02f, 1.0f, 0.0f,// furthest right point
		-2.63f,   -1.1f, -74.36f, 0.0f, 0.0f,// closest right point
		// bottom of offices desk north
		-2.63f,   -1.2f, -74.36, 0.0f, 0.0f,// closest right point
		-3.96f,   -1.2f, -74.36f, 0.0f, 1.0f,// closest left point
		-3.96f,   -1.2f, -80.02f, 1.0f, 1.0f,// furthest left point
		-3.96f,   -1.2f, -80.02f, 1.0f, 1.0f,// furthest left point
		-2.63f,   -1.2f, -80.02f, 1.0f, 0.0f,// furthest right point
		-2.63f,   -1.2f, -74.36f, 0.0f, 0.0f,// closest right point
	
		// offices desk west top 9length 
		-3.96f,   -1.1f, -78.69, 0.0f, 0.0f,// closest right point
		-8.63f,   -1.1f, -78.69f, 0.0f, 1.0f,// closest left point
		-8.63f,   -1.1f, -80.02f, 1.0f, 1.0f,// furthest left point
		-8.63f,   -1.1f, -80.02f, 1.0f, 1.0f,// furthest left point
		-3.96f,   -1.1f, -80.02f, 1.0f, 0.0f,// furthest right point
		-3.96f,   -1.1f, -78.69f, 0.0f, 0.0f,// closest right point
		// offices desk west bottom
		-3.96f,   -1.2f, -78.69, 0.0f, 0.0f,// closest right point
		-8.63f,   -1.2f, -78.69f, 0.0f, 1.0f,// closest left point
		-8.63f,   -1.2f, -80.02f, 1.0f, 1.0f,// furthest left point
		-8.63f,   -1.2f, -80.02f, 1.0f, 1.0f,// furthest left point
		-3.96f,   -1.2f, -80.02f, 1.0f, 0.0f,// furthest right point
		-3.96f,   -1.2f, -78.69f, 0.0f, 0.0f,// closest right point
		

		// offices desk north bottom right 8length
		-3.03f,  -1.2f,  -74.36f,  1.0f, 1.0f, // top point closest to me
		-3.03f,  -1.2f, -79.62f,  0.0f, 1.0f, // top point furthest from me
		-3.03f, -3.0f, -79.62f,  0.0f, 0.0f, // bottom point furthest from me
		-3.03f, -3.0f, -79.62f,  0.0f, 0.0f, // bottom point furthest from me
		-3.03f, -3.0f,  -74.36f,  1.0f, 0.0f, // bottom point closest to me
		-3.03f,  -1.2f,  -74.36f,  1.0f, 1.0f, // top point closest to me
		// offices desk north bottom left
		-3.33f,  -1.2f,  -74.36f,  1.0f, 1.0f, // top point closest to me
		-3.33f,  -1.2f, -79.32f,  0.0f, 1.0f, // top point furthest from me
		-3.33f, -3.0f, -79.32f,  0.0f, 0.0f, // bottom point furthest from me
		-3.33f, -3.0f, -79.32f,  0.0f, 0.0f, // bottom point furthest from me
		-3.33f, -3.0f,  -74.36f,  1.0f, 0.0f, // bottom point closest to me
		-3.33f,  -1.2f,  -74.36f,  1.0f, 1.0f, // top point closest to me

		// offices desk west bottom left 8.5length
		-8.33f, -3.0f, -79.32f, 0.0f, 0.0f,//bottom left point
		-3.33f, -3.0f, -79.32f, 1.0f, 0.0f,// bottom right point
		-3.33f,  -1.2f, -79.32f, 1.0f, 1.0f,// top right point
		-3.33f,  -1.2f, -79.32f, 1.0f, 1.0f,// top right point
		-8.33f,  -1.2f, -79.32f, 0.0f, 1.0f,// top left point
		-8.33f, -3.0f, -79.32f, 0.0f, 0.0f,//bottom left point
		// offices desk west bottom right
		-8.33f, -3.0f, -79.62f, 0.0f, 0.0f,//bottom left point
		-3.03f, -3.0f, -79.62f, 1.0f, 0.0f,// bottom right point
		-3.03f,  -1.2f, -79.62f, 1.0f, 1.0f,// top right point
		-3.03f,  -1.2f, -79.62f, 1.0f, 1.0f,// top right point
		-8.33f,  -1.2f, -79.62f, 0.0f, 1.0f,// top left point
		-8.33f, -3.0f, -79.62f, 0.0f, 0.0f,//bottom left point

		// offices desk north top left
		-3.96f,  -1.1f,  -74.36f,  1.0f, 1.0f, // top point closest to me
		-3.96f,  -1.1f, -78.69f,  0.0f, 1.0f, // top point furthest from me
		-3.96f, -1.2f, -78.69f,  0.0f, 0.0f, // bottom point furthest from me
		-3.96f, -1.2f, -78.69f,  0.0f, 0.0f, // bottom point furthest from me
		-3.96f, -1.2f,  -74.36f,  1.0f, 0.0f, // bottom point closest to me
		-3.96f,  -1.1f,  -74.36f,  1.0f, 1.0f, // top point closest to me
		// offices desk north top right
		-2.63f,  -1.1f,  -74.36f,  1.0f, 1.0f, // top point closest to me
		-2.63f,  -1.1f, -80.02f,  0.0f, 1.0f, // top point furthest from me
		-2.63f, -1.2f, -80.02f,  0.0f, 0.0f, // bottom point furthest from me
		-2.63f, -1.2f, -80.02f,  0.0f, 0.0f, // bottom point furthest from me
		-2.63f, -1.2f,  -74.36f,  1.0f, 0.0f, // bottom point closest to me
		-2.63f,  -1.1f,  -74.36f,  1.0f, 1.0f, // top point closest to me
		// offices desk west top left
		-8.63f, -1.2f, -78.69f, 0.0f, 0.0f,//bottom left point
		-3.96f, -1.2f, -78.69f, 1.0f, 0.0f,// bottom right point
		-3.96f,  -1.1f, -78.69f, 1.0f, 1.0f,// top right point
		-3.96f,  -1.1f, -78.69f, 1.0f, 1.0f,// top right point
		-8.63f,  -1.1f, -78.69f, 0.0f, 1.0f,// top left point
		-8.63f, -1.2f, -78.69f, 0.0f, 0.0f,//bottom left point
		// offices desk west top right
		-8.63f, -1.2f, -80.02f, 0.0f, 0.0f,//bottom left point
		-2.63f, -1.2f, -80.02f, 1.0f, 0.0f,// bottom right point
		-2.63f,  -1.1f, -80.02f, 1.0f, 1.0f,// top right point
		-2.63f,  -1.1f, -80.02f, 1.0f, 1.0f,// top right point
		-8.63f,  -1.1f, -80.02f, 0.0f, 1.0f,// top left point
		-8.63f, -1.2f, -80.02f, 0.0f, 0.0f,//bottom left point
		// offices desk west burton side lining
		-8.63f,  -1.1f,  -78.69f,  1.0f, 1.0f, // top point closest to me
		-8.63f,  -1.1f, -80.02f,  0.0f, 1.0f, // top point furthest from me
		-8.63f, -1.2f, -80.02f,  0.0f, 0.0f, // bottom point furthest from me
		-8.63f, -1.2f, -80.02f,  0.0f, 0.0f, // bottom point furthest from me
		-8.63f, -1.2f,  -78.69f,  1.0f, 0.0f, // bottom point closest to me
		-8.63f,  -1.1f,  -78.69f,  1.0f, 1.0f, // top point closest to me

	};

	GLuint VBO, VBO2, VBO3, VBO4, VBO5,VBO6, VBO7, VBO8, VBO9, VBO10,
		   VAO, VAO2, VAO3, VAO4, VAO5, VAO6,VAO7, VAO8, VAO9, VAO10;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO

	//cabinets
    glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cabinets), cabinets, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO

	//ll tables
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(llTable), llTable, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO


	//ll chairs
	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(chair), chair, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO


	//classroom small tables
	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(table), table, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO


	//classroom big tables
	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO6);

	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bigTable), bigTable, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO

	
	//blue chair
	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO7);

	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blueChair), blueChair, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO

	//monitor
	glGenVertexArrays(1, &VAO8);
	glGenBuffers(1, &VBO8);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO8);

	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(monitor), monitor, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO

	//office desk
	glGenVertexArrays(1, &VAO9);
	glGenBuffers(1, &VBO9);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO9);

	glBindBuffer(GL_ARRAY_BUFFER, VBO9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(desk), desk, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO

	//wood desk
	glGenVertexArrays(1, &VAO10);
	glGenBuffers(1, &VBO10);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO10);

	glBindBuffer(GL_ARRAY_BUFFER, VBO10);
	glBufferData(GL_ARRAY_BUFFER, sizeof(woodDesk), woodDesk, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO


	char* imageNames[] =
	{
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/cabinetsWall.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/floor_texture.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/ceiling_texture.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/double_doors.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/singleDoorLeft.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/carpet.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/clockWall.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/whiteboards.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/cabinetsWall.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/whiteboards2.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/concrete.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/windows.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/classCeiling.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/coloredWall.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/codeDoors.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/singleDoorRight.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/windows.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/linuxLabWall.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/cabinetsBottom.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/wood.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/woodTableTop2.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/cabinetsTop.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/llTableTop.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/woodTableTop.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/blackChair.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/blackChair2.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/classroomTable.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/silver.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/blueChair.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/blackMonitor.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/blackMonitor2.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/monitorFront.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/monitorBack.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/keyboard.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/compFront.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/compBack.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/officeDeskTop.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/officeDeskBottom.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/officeDeskLining.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/benchTop.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/wall.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/elevator.bmp",
		"C:/Users/ric11060/Documents/Visual Studio 2015/Projects/test Senior project/images/men.bmp"
		

	};

	// Load and create a texture 

	GLuint * textures = new GLuint[43];
	glGenTextures(43, textures);

	for (int i = 0; i < 43; i++)
	{
		Image* image = Image::loadBMP(imageNames[i]);
		textures[i] = Image::loadTextureFromImage(image);
		glBindTexture(GL_TEXTURE_2D, textures[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
		// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glBindTexture(GL_TEXTURE_2D, texture);
	}
	
	

	

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();

		//check collisions
		if (!Collision(camera))
			Do_Movement();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw our first triangle
		ourShader.Use();

		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture"), 0);

		// Create camera transformation
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);

		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, textures[3]);
		glDrawArrays(GL_TRIANGLES, 0, 6);// doubledoors textures
		glBindTexture(GL_TEXTURE_2D, textures[40]);
		glDrawArrays(GL_TRIANGLES, 6, 30);// top of doors
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glDrawArrays(GL_TRIANGLES, 36, 222); // left wall + right wall+ hallways
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glDrawArrays(GL_TRIANGLES, 258, 36); //floor
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glDrawArrays(GL_TRIANGLES, 294, 36); //ceiling
		glBindTexture(GL_TEXTURE_2D, textures[4]);
		glDrawArrays(GL_TRIANGLES, 330, 18); //single door
		glBindTexture(GL_TEXTURE_2D, textures[5]);
		glDrawArrays(GL_TRIANGLES, 348, 36); //carpets
		glBindTexture(GL_TEXTURE_2D, textures[6]);
		glDrawArrays(GL_TRIANGLES, 384, 18); //clockwall
		glBindTexture(GL_TEXTURE_2D, textures[7]);
		glDrawArrays(GL_TRIANGLES, 402, 66); //backwall/frontwall whiteboards
		glBindTexture(GL_TEXTURE_2D, textures[8]);
		glDrawArrays(GL_TRIANGLES, 468, 30); //backwall cabinets
		glBindTexture(GL_TEXTURE_2D, textures[9]);
		glDrawArrays(GL_TRIANGLES, 498, 24); //frontwall whiteboard mirrored
		glBindTexture(GL_TEXTURE_2D, textures[10]);
		glDrawArrays(GL_TRIANGLES, 522, 48); //concrete walls
		glBindTexture(GL_TEXTURE_2D, textures[11]);
		glDrawArrays(GL_TRIANGLES, 570, 18); //classroom windows
		glBindTexture(GL_TEXTURE_2D, textures[12]);
		glDrawArrays(GL_TRIANGLES, 588, 36); //classroom ceilings
		glBindTexture(GL_TEXTURE_2D, textures[13]);
		glDrawArrays(GL_TRIANGLES, 624, 6); //colored wall
		glBindTexture(GL_TEXTURE_2D, textures[14]);
		glDrawArrays(GL_TRIANGLES, 630, 18); //code doors
		glBindTexture(GL_TEXTURE_2D, textures[15]);
		glDrawArrays(GL_TRIANGLES, 648, 78); //singleDoor inverted
		glBindTexture(GL_TEXTURE_2D, textures[16]);
		glDrawArrays(GL_TRIANGLES, 726, 6); //double window
		glBindTexture(GL_TEXTURE_2D, textures[17]);
		glDrawArrays(GL_TRIANGLES, 732, 12); //linux lab walls
		glBindTexture(GL_TEXTURE_2D, textures[41]);
		glDrawArrays(GL_TRIANGLES, 744, 6); //elevator
		glBindTexture(GL_TEXTURE_2D, textures[42]);
		glDrawArrays(GL_TRIANGLES, 750, 12); //men
		
		glBindVertexArray(0);
		glBindVertexArray(VAO2);
		glm::mat4 cabinetModel;
		for (GLuint i = 0; i < 3; i++)
		{
			cabinetModel = glm::translate(cabinetModel, cabinetPositions[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cabinetModel));
			glBindTexture(GL_TEXTURE_2D, textures[18]);
			glDrawArrays(GL_TRIANGLES, 0, 6); //bottom cabinets front
			glBindTexture(GL_TEXTURE_2D, textures[19]);
			glDrawArrays(GL_TRIANGLES, 6, 36); //cabinets sides
			glBindTexture(GL_TEXTURE_2D, textures[20]);
			glDrawArrays(GL_TRIANGLES, 42, 6); //cabinets top
			glBindTexture(GL_TEXTURE_2D, textures[21]);
			glDrawArrays(GL_TRIANGLES, 48, 6); //top cabinets front

		}

		glm::mat4 rotatedCabinet = glm::mat4(1.0f);
		glm::vec3 negCenterCab(-9.365f, 0.54f, 25.225f);
		glm::vec3 centerCab(9.365f, -0.54f, -25.225f);

		rotatedCabinet = glm::translate(rotatedCabinet, centerCab);
		rotatedCabinet = glm::rotate(rotatedCabinet, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rotatedCabinet = glm::translate(rotatedCabinet, negCenterCab);
		rotatedCabinet = glm::translate(rotatedCabinet, glm::vec3(18.5f, 0.0f, 15.85f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotatedCabinet));
		glBindTexture(GL_TEXTURE_2D, textures[18]);
		glDrawArrays(GL_TRIANGLES, 0, 6); //bottom cabinets front
		glBindTexture(GL_TEXTURE_2D, textures[19]);
		glDrawArrays(GL_TRIANGLES, 6, 36); //cabinets sides
		glBindTexture(GL_TEXTURE_2D, textures[20]);
		glDrawArrays(GL_TRIANGLES, 42, 6); //cabinets top
		glBindTexture(GL_TEXTURE_2D, textures[21]);
		glDrawArrays(GL_TRIANGLES, 48, 6); //top cabinets front

		glm::mat4 rotatedCabinet2 = glm::mat4(1.0f);
		glm::vec3 negCenterCab2(-9.365f, 0.54f, 25.225f);
		glm::vec3 centerCab2(9.365f, -0.54f, -25.225f);

		rotatedCabinet2 = glm::translate(rotatedCabinet2, centerCab2);
		rotatedCabinet2 = glm::rotate(rotatedCabinet2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rotatedCabinet2 = glm::translate(rotatedCabinet2, negCenterCab2);
		rotatedCabinet2 = glm::translate(rotatedCabinet2, glm::vec3(64.0f, 0.0f, 13.63f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotatedCabinet2));
		glBindTexture(GL_TEXTURE_2D, textures[18]);
		glDrawArrays(GL_TRIANGLES, 0, 6); //bottom cabinets front
		glBindTexture(GL_TEXTURE_2D, textures[19]);
		glDrawArrays(GL_TRIANGLES, 6, 36); //cabinets sides
		glBindTexture(GL_TEXTURE_2D, textures[20]);
		glDrawArrays(GL_TRIANGLES, 42, 6); //cabinets top
		glBindTexture(GL_TEXTURE_2D, textures[21]);
		glDrawArrays(GL_TRIANGLES, 48, 6); //top cabinets front
		glBindVertexArray(0);
		
		
		glBindVertexArray(VAO3);
		glm::mat4 llTables;
		for (GLuint i = 0; i < 7; i++)
		{
			llTables = glm::translate(llTables, llTablePositions[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(llTables));
			glBindTexture(GL_TEXTURE_2D, textures[22]);
			glDrawArrays(GL_TRIANGLES, 0, 12); //ll tabletop top
			glBindTexture(GL_TEXTURE_2D, textures[23]);
			glDrawArrays(GL_TRIANGLES, 12, 138); //ll table no wood lining
			glBindTexture(GL_TEXTURE_2D, textures[19]);
			glDrawArrays(GL_TRIANGLES, 150, 18); //ll table wood lining
		}
		glBindVertexArray(0);

		
		glBindVertexArray(VAO4);
		glm::mat4 chairs;
		for (GLuint i = 0; i < 35; i++)
		{
			chairs = glm::translate(chairs, chairPositions[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(chairs));
			glBindTexture(GL_TEXTURE_2D, textures[24]);
			glDrawArrays(GL_TRIANGLES, 0, 24); //black chair

			glBindTexture(GL_TEXTURE_2D, textures[25]);
			glDrawArrays(GL_TRIANGLES, 24, 138); //black chair2
			
		}

		glBindVertexArray(VAO7);
		glm::mat4 rotatedChair = glm::mat4(1.0f);
		glm::vec3 negCenterChair(-4.4f, 2.2f, 29.69);
		glm::vec3 centerChair(4.4f, -2.2f, -29.69f);

		rotatedChair = glm::translate(rotatedChair, centerChair);
		rotatedChair = glm::rotate(rotatedChair, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rotatedChair = glm::translate(rotatedChair, negCenterChair);
		rotatedChair = glm::scale(rotatedChair, glm::vec3(0.7f, 0.85f, 0.8f));
		rotatedChair = glm::translate(rotatedChair, glm::vec3(11.3f, -0.525f, -1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotatedChair));
		glBindTexture(GL_TEXTURE_2D, textures[28]);
		glDrawArrays(GL_TRIANGLES, 0, 24); //blue chair
		glBindTexture(GL_TEXTURE_2D, textures[25]);
		glDrawArrays(GL_TRIANGLES, 24, 138); //black rods

		for (GLuint i = 35; i < 181; i++)
		{
			rotatedChair = glm::translate(rotatedChair, chairPositions[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotatedChair));
			glBindTexture(GL_TEXTURE_2D, textures[28]);
			glDrawArrays(GL_TRIANGLES, 0, 24); //blue chair
			glBindTexture(GL_TEXTURE_2D, textures[25]);
			glDrawArrays(GL_TRIANGLES, 24, 138); //black rods
		}

		glm::mat4 rotatedChair2 = glm::mat4(1.0f);
		glm::vec3 negCenterChair2(-4.4f, 2.2f, 29.69);
		glm::vec3 centerChair2(4.4f, -2.2f, -29.69f);

		rotatedChair2 = glm::translate(rotatedChair2, centerChair2);
		rotatedChair2 = glm::rotate(rotatedChair2, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rotatedChair2 = glm::translate(rotatedChair2, negCenterChair2);
		rotatedChair2 = glm::scale(rotatedChair2, glm::vec3(0.7f, 0.85f, 0.8f));
		rotatedChair2 = glm::translate(rotatedChair2, glm::vec3(-53.55f, -0.525f, -13.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotatedChair2));
		glBindTexture(GL_TEXTURE_2D, textures[28]);
		glDrawArrays(GL_TRIANGLES, 0, 24); //blue chair
		glBindTexture(GL_TEXTURE_2D, textures[25]);
		glDrawArrays(GL_TRIANGLES, 24, 138); //black rods

		for (GLuint i = 181; i < 222; i++)
		{
			rotatedChair2 = glm::translate(rotatedChair2, chairPositions[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotatedChair2));
			glBindTexture(GL_TEXTURE_2D, textures[28]);
			glDrawArrays(GL_TRIANGLES, 0, 24); //blue chair
			glBindTexture(GL_TEXTURE_2D, textures[25]);
			glDrawArrays(GL_TRIANGLES, 24, 138); //black rods
		}

		glm::mat4 officeChairs;
		officeChairs = glm::scale(officeChairs, glm::vec3(0.7f, 0.85f, 0.8f));
		officeChairs = glm::translate(officeChairs, glm::vec3(1.4f, -0.525f, -69.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(officeChairs));
		glBindTexture(GL_TEXTURE_2D, textures[28]);
		glDrawArrays(GL_TRIANGLES, 0, 24); //blue chair
		glBindTexture(GL_TEXTURE_2D, textures[25]);
		glDrawArrays(GL_TRIANGLES, 24, 138); //black rods

		for (GLuint i = 222; i < 225; i++)
		{
			officeChairs = glm::translate(officeChairs, chairPositions[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(officeChairs));
			glBindTexture(GL_TEXTURE_2D, textures[28]);
			glDrawArrays(GL_TRIANGLES, 0, 24); //blue chair
			glBindTexture(GL_TEXTURE_2D, textures[25]);
			glDrawArrays(GL_TRIANGLES, 24, 138); //black rods
		}
		glBindVertexArray(0);
		
		
		
		glBindVertexArray(VAO5);
		glm::mat4 tables;
		for (GLuint i = 0; i < 42; i++)
		{
			
			tables = glm::translate(tables, tablePositions[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(tables));
			glBindTexture(GL_TEXTURE_2D, textures[26]);
			glDrawArrays(GL_TRIANGLES, 0, 36); //classroom Tables
			glBindTexture(GL_TEXTURE_2D, textures[27]);
			glDrawArrays(GL_TRIANGLES, 36, 336); //silver
		}

		glm::mat4 rotatedTable = glm::mat4(1.0f);
		glm::vec3 negCenter(-4.4f, 2.2f, 29.69);
		glm::vec3 center(4.4f, -2.2f, -29.69f);
		
		rotatedTable = glm::translate(rotatedTable, center);
		rotatedTable = glm::rotate(rotatedTable, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rotatedTable = glm::translate(rotatedTable, negCenter);

		rotatedTable = glm::translate(rotatedTable, glm::vec3(45.4f, 0.0f, 9.2f));// z is x
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotatedTable));
		glBindTexture(GL_TEXTURE_2D, textures[26]);
		glDrawArrays(GL_TRIANGLES, 0, 36); //classroom Tables
		glBindTexture(GL_TEXTURE_2D, textures[27]);
		glDrawArrays(GL_TRIANGLES, 36, 336); //silver

		for (GLuint i = 42; i < 53; i++)
		{

			rotatedTable = glm::translate(rotatedTable, tablePositions[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotatedTable));
			glBindTexture(GL_TEXTURE_2D, textures[26]);
			glDrawArrays(GL_TRIANGLES, 0, 36); //classroom Tables
			glBindTexture(GL_TEXTURE_2D, textures[27]);
			glDrawArrays(GL_TRIANGLES, 36, 336); //silver
		}
		glBindVertexArray(0);


		glBindVertexArray(VAO6);
		glm::mat4 bigTables;
		for (GLuint i = 0; i < 21; i++)
		{
			bigTables = glm::translate(bigTables, bigTablePositions[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(bigTables));
			glBindTexture(GL_TEXTURE_2D, textures[26]);
			glDrawArrays(GL_TRIANGLES, 0, 36); //classroom Tables
			glBindTexture(GL_TEXTURE_2D, textures[27]);
			glDrawArrays(GL_TRIANGLES, 36, 336); //silver
		}

		glm::mat4 rotatedTable2 = glm::mat4(1.0f);
		glm::vec3 negCenter2(-7.675f, 2.2f, 29.69);
		glm::vec3 center2(7.675f, -2.2f, -29.69f);

		rotatedTable2 = glm::translate(rotatedTable2, center2);
		rotatedTable2 = glm::rotate(rotatedTable2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rotatedTable2 = glm::translate(rotatedTable2, negCenter2);

		rotatedTable2 = glm::translate(rotatedTable2, glm::vec3(48.6712f, 0.0f, 5.925f));// z is x
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotatedTable2));
		glBindTexture(GL_TEXTURE_2D, textures[26]);
		glDrawArrays(GL_TRIANGLES, 0, 36); //classroom Tables
		glBindTexture(GL_TEXTURE_2D, textures[27]);
		glDrawArrays(GL_TRIANGLES, 36, 336); //silver

		for (GLuint i = 21; i < 26; i++)
		{

			rotatedTable2 = glm::translate(rotatedTable2, bigTablePositions[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotatedTable2));
			glBindTexture(GL_TEXTURE_2D, textures[26]);
			glDrawArrays(GL_TRIANGLES, 0, 36); //classroom Tables
			glBindTexture(GL_TEXTURE_2D, textures[27]);
			glDrawArrays(GL_TRIANGLES, 36, 336); //silver
		}
		
		glBindVertexArray(VAO8);
		glm::mat4 monitors;
		for (GLuint i = 0; i < 35; i++)
		{
			monitors = glm::translate(monitors, monitorPositions[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(monitors));
			glBindTexture(GL_TEXTURE_2D, textures[30]);
			glDrawArrays(GL_TRIANGLES, 0, 6); //monitor base
			glBindTexture(GL_TEXTURE_2D, textures[29]);
			glDrawArrays(GL_TRIANGLES, 6, 120);// sides,front, top
			glBindTexture(GL_TEXTURE_2D, textures[31]);
			glDrawArrays(GL_TRIANGLES, 126, 6); //front monitor
			glBindTexture(GL_TEXTURE_2D, textures[32]);
			glDrawArrays(GL_TRIANGLES, 132, 6);// back monitor
			glBindTexture(GL_TEXTURE_2D, textures[33]);
			glDrawArrays(GL_TRIANGLES, 138, 6);// keyboard
			glBindTexture(GL_TEXTURE_2D, textures[34]);
			glDrawArrays(GL_TRIANGLES, 144, 6);// comp front
			glBindTexture(GL_TEXTURE_2D, textures[35]);
			glDrawArrays(GL_TRIANGLES, 150, 6);// comp back

		}

		glBindVertexArray(VAO9);
		glm::mat4 desks;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(desks));
		glBindTexture(GL_TEXTURE_2D, textures[36]);
		glDrawArrays(GL_TRIANGLES, 0, 24); // top of offices desk
		glBindTexture(GL_TEXTURE_2D, textures[37]);
		glDrawArrays(GL_TRIANGLES, 24, 24); // bottom of offices 
		glBindTexture(GL_TEXTURE_2D, textures[38]);
		glDrawArrays(GL_TRIANGLES, 48, 30); // bottom of offices desk
		

		glBindVertexArray(VAO10);
		glm::mat4 woodDesks;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(woodDesks));
		glBindTexture(GL_TEXTURE_2D, textures[19]);
		glDrawArrays(GL_TRIANGLES, 0, 140); // top of offices desk

		glm::mat4 woodDesk2;
		woodDesk2 = glm::scale(woodDesk2, glm::vec3(1.6f, 2.0f, 1.0f));
		woodDesk2 = glm::translate(woodDesk2, glm::vec3(2.5f, 1.5f, 12.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(woodDesk2));
		glBindTexture(GL_TEXTURE_2D, textures[19]);
		glDrawArrays(GL_TRIANGLES, 0, 140); // top of offices desk
		
		glm::mat4 benches;
		benches = glm::scale(benches, glm::vec3(0.7f, 1.0f, 1.0f));
		benches = glm::translate(benches, glm::vec3(1.0f, 0.0f, 10.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(benches));
		glBindTexture(GL_TEXTURE_2D, textures[39]);
		glDrawArrays(GL_TRIANGLES, 0, 6); // top of offices desk
		glBindTexture(GL_TEXTURE_2D, textures[19]);
		glDrawArrays(GL_TRIANGLES, 6, 134); // top of offices desk

		for (GLuint i = 0; i < 3; i++)
		{
			benches = glm::translate(benches, benchPositions[i]);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(benches));
			glBindTexture(GL_TEXTURE_2D, textures[39]);
			glDrawArrays(GL_TRIANGLES, 0, 6); // top of bench
			glBindTexture(GL_TEXTURE_2D, textures[19]);
			glDrawArrays(GL_TRIANGLES, 6, 134); // wood
		}
		glBindVertexArray(0);
		// Swap the buffers
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteVertexArrays(1, &VAO3);
	glDeleteVertexArrays(1, &VAO4);
	glDeleteVertexArrays(1, &VAO5);
	glDeleteVertexArrays(1, &VAO6);
	glDeleteVertexArrays(1, &VAO7);
	glDeleteVertexArrays(1, &VAO8);
	glDeleteVertexArrays(1, &VAO9);
	glDeleteVertexArrays(1, &VAO10);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &VBO5);
	glDeleteBuffers(1, &VBO6);
	glDeleteBuffers(1, &VBO7);
	glDeleteBuffers(1, &VBO8);
	glDeleteBuffers(1, &VBO9);
	glDeleteBuffers(1, &VBO10);
	glfwTerminate();
	glDeleteTextures(43, textures);
	delete[] textures;
	return 0;
}


bool Collision(Camera &camera)
{
	bool collide = false;



	double x = floor((camera.Position.x * 10) + 0.5) / 10;
	double y = floor((camera.Position.y * 10) + 0.5) / 10;
	double z = floor((camera.Position.z * 10) + 0.5) / 10;

	double revertPlus = 0.0005;
	double revertNeg = -0.0005;

	//ceiling/floor collisions
	if (y == 2.8)
	{
		collide = true;
		camera.Position.y += revertNeg;
		return collide;
	}
	if (y == -2.8)
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}

	//furthest back wall collision
	if (z == -0.1)
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 214 window-side wall
	if (x == 23.2 && z < -71.4)
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	// all window side walls
	if (x == 23.9)
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	// All left extreme boundary walls
	if (x == -26.2)
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}

	//stairs wall
	if (x < -3.0 && z == -7.1)
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}

	// bathroom wall
	if (x < -3.0 && z == -11.6)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}

	//linux lab front wall
	if (x < -3.0 && z == -23.2)
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// linux lab tables by door
	if (z == -34.2 && (x > -12.9 && x < -3.1) && (y > -2.9 && y < 0.1) )
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// linux lab table row 2
	if (z == -30.5 && (x > -12.9 && x < -3.1) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// linux lab table row 3 
	if (z == -26.8 && (x > -12.9 && x < -3.1) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// linux lab table row 4 col 2
	if (z == -26.7 && (x > -26.2 && x < -16.3) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}

	// linux lab table row 3 col 2
	if (z == -30.2 && (x > -26.2 && x < -16.3) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// linux lab table row 2 col 2
	if (z == -33.9 && (x > -26.2 && x < -16.3) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// linux lab table row 1 col 2
	if (z == -37.7&& (x > -26.2 && x < -16.3) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// linux lab cabinets bot
	if (z == -40.2 && (x > -12.6 && x < -6.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// linux lab cabinets top
	if (z == -40.8 && (x > -12.6 && x < -6.3) && (y > -0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// linux lab table original chair side
	if (z == -36.6 && (x > -12.9 && x < -3.1) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// linux lab table chair side row2 col1
	if (z == -33.0 && (x > -12.9 && x < -3.1) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// linux lab table chair side row3 col1
	if (z == -29.5 && (x > -12.9 && x < -3.1) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// linux lab chair side row 4 col 2
	if (z == -29.4 && (x > -26.2 && x < -16.3) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// linux lab chair side row 4 col 2
	if (z == -32.9 && (x > -26.2 && x < -16.3) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// linux lab chair side row 4 col 2
	if (z == -36.4 && (x > -26.2 && x < -16.3) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// linux lab chair side row 4 col 2
	if (z == -40.1 && (x > -26.2 && x < -16.3) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}

	
	// 206 backwall
	if (x > 3.0 && z == -24.7)
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}

	// 206 desk chair side col 1 row 1
	if (z == -25.8 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}

	// 206 desk chair side col 1 row 2
	if (z == -28.8 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 206 desk chair side col 2 row 2
	if (z == -28.8 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 206 desk chair side col 1 row 3
	if (z == -31.8 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 206 desk chair side col 2 row 3
	if (z == -31.8 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 206 desk chair side col 1 row 4
	if (z == -34.8 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 206 desk chair side col 2 row 4
	if (z == -34.8 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 206 desk side col 2 row 4
	if (z == -36.4 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 206 desk side col 2 row 3
	if (z == -33.4 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 206 desk side col 2 row 2
	if (z == -30.4 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 206 desk side col 1 row 4
	if (z == -36.4 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 206 desk side col 1 row 3
	if (z == -33.4 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 206 desk side col 1 row 2
	if (z == -30.4 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 206 desk side col 1 row 1
	if (z == -27.4 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 206 cabinets bot
	if (z == -26.1 && (x > 6.3 && x < 12.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 206 cabinets top
	if (z == -25.6 && (x > 6.3 && x < 12.6) && (y > -0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 206 front wall
	if (x > 3.0 && z == -39.1)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 208 back wall
	if (x > 3.0 && z == -40.2)
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 208 desk chair side col 1 row 1
	if (z == -41.3 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}

	// 208 desk chair side col 1 row 2
	if (z == -44.3 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 208 desk chair side col 2 row 2
	if (z == -44.3 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 208 desk chair side col 1 row 3
	if (z == -47.3 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 208 desk chair side col 2 row 3
	if (z == -47.3 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 208 desk chair side col 1 row 4
	if (z == -50.3 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 208 desk chair side col 2 row 4
	if (z == -50.3 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 208 desk side col 2 row 4
	if (z == -51.9 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 208 desk side col 2 row 3
	if (z == -48.9 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 208 desk side col 2 row 2
	if (z == -45.9 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 208 desk side col 1 row 4
	if (z == -51.9 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 208 desk side col 1 row 3
	if (z == -48.9 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 208 desk side col 1 row 2
	if (z == -45.9 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 208 desk side col 1 row 1
	if (z == -42.9 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 208 cabinets bot
	if (z == -41.6 && (x > 6.3 && x < 12.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 208 cabinets top
	if (z == -41.1 && (x > 6.3 && x < 12.6) && (y > -0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}

	// linux lab back wall
	if (x < -3.0 && z == -41.6)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 208 front wall
	if (x > 3.0 && z == -54.6)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 210 back wall
	if (x > 3.0 && z == -55.6)
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}

	// 210 desk chair side col 1 row 1
	if (z == -56.7 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}

	// 210 desk chair side col 1 row 2
	if (z == -59.7 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 210 desk chair side col 2 row 2
	if (z == -59.7 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 210 desk chair side col 1 row 3
	if (z == -62.7 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 210 desk chair side col 2 row 3
	if (z == -62.7 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 210 desk chair side col 1 row 4
	if (z == -65.7 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 210 desk chair side col 2 row 4
	if (z == -65.7 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 210 desk side col 2 row 4
	if (z == -67.3 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 210 desk side col 2 row 3
	if (z == -64.3 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 210 desk side col 2 row 2
	if (z == -61.3 && (x > 3.1 && x < 12.3) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 210 desk side col 1 row 4
	if (z == -67.3 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 210 desk side col 1 row 3
	if (z == -64.3 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 210 desk side col 1 row 2
	if (z == -61.3 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 210 desk side col 1 row 1
	if (z == -58.3 && (x > 14.33 && x < 23.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 210 cabinets bot
	if (z == -57.0 && (x > 6.3 && x < 12.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 210 cabinets top
	if (z == -56.5 && (x > 6.3 && x < 12.6) && (y > -0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// windows lab back wall
	if (x < -3.0 && z == -64.8)
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// windows lab parallel wall
	if ((x < -9.0 && x > -20.7) && z == -69.5)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 210 front wall
	if (x > 3.0 && z == -70.0)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 214 left wall
	if (x > 3.0 && z == -71.4)
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 214 hallway right wall
	if ((x > 3.0 && x < 9.0)  && z == -71.8)
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// 214 hallway left wall
	if ((x < 9.0 && x > 1.3) && z == -74.2)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}

	//214 desks z sides
	if (z == -85.4 && (x > 12.9 && x < 14.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	if (z == -85.4 && (x > 15.9 && x < 17.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	if (z == -85.4 && (x > 18.9 && x < 20.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	if (z == -83.0 && (x > 12.9 && x < 14.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	if (z == -83.0 && (x > 15.9 && x < 17.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	if (z == -83.0 && (x > 18.9 && x < 20.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	if (z == -73.8 && (x > 12.9 && x < 14.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	if (z == -73.8 && (x > 15.9 && x < 17.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	if (z == -73.8 && (x > 18.9 && x < 20.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// offices/microwave shared wall
	if ((x < -0.6 && x > -9.0) && z == -74.2)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// offices microwave wall inside office
	if ((x > -9.0 && x < -0.6) && z == -74.4)
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// offices neff door
	if ((x > -11.7 && x < -8.9) && z == -75.1)
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// elevator exit wall
	if ((x < -20.5 && x > -26.8) && z == -77.2)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// offices desk sister price side
	if (z == -78.6 && (x > -8.7 && x < -3.9) && (y > -2.9 && y < -0.8))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// offices desk further side
	if (z == -80.1 && (x > -8.7 && x < -2.6) && (y > -2.9 && y < -0.8))
	{
		collide = true;
		camera.Position.z += revertNeg;
		return collide;
	}
	// offices chair and desk 
	if (z == -83.5 && (x < -1.2 && x > -9.0) && (y > -2.9 && y < -1.2))
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// offices barrier wall
	if ((x < -1.2 && x > -9.0) && z == -84.5)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// offices right wing
	if ((x < 1.4 && x > -1.2) && z == -88.2)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// offices left wing
	if ((x < -8.9 && x > -11.7) && z == -88.2)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}
	// 214 right wall
	if (x > 9.0 && z == -94.4)
	{
		collide = true;
		camera.Position.z += revertPlus;
		return collide;
	}



		//START X AXIS CHECKS



	// 202 wall door
	if ((z < -0.1 && z > -7.1) && x == -2.9)
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// hallway right wall up to 206
	if ((z < -0.1 && z > -24.9) && x == 2.9)
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	// hallway left wall up to linux lab
	if ((z < -11.6 && z > -39.4) && x == -2.9)
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// 206 clock wall
	if ((z < -26.9 && z > -39.4) && x == 3.1)
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// hallway right wall between 206 and 208
	if ((z < -26.9 && z > -40.5) && x == 2.9)
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	// linux lab left wall
	if ((z < -23.0 && z > -39.4) && x == -3.1)
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	// hallway left wall linux lab to end
	if ((z < -41.4 && z > -64.7) && x == -2.9)
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// hallway right wall between 208 and 210
	if ((z < -42.5 && z > -55.9) && x == 2.9)
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	// 208 clock wall
	if ((z < -42.5 && z > -54.6) && x == 3.1)
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	//210 clock wall
	if ((z < -58.0 && z > -70.2) && x == 3.1)
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// hallway right wall 210 to end
	if ((z < -57.9 && z > -71.8) && x == 2.9)
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	
	// microwave wall 
	if ((z < -69.5 && z > -74.2) && x == -8.9)
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}

	// parallel microwave wall 
	if ((z < -69.5 && z > -77.2) && x == -20.8)
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	// offices desk sister price side north
	if (x == -4.0 && (z < -74.4 && z > -78.6) && (y > -2.9 && y < -0.8))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	// offices desk jones side 
	if (x == -2.6 && (z < -74.4 && z > -80.1) && (y > -2.9 && y < -0.8))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// offices right wing wall
	if (x == 1.3 && (z < -74.4 && z > -88.3))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	// offices left wing wall
	if (x == -11.5 && (z < -74.4 && z > -88.3))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// offices barrier right
	if (x == -1.2 && (z < -83.7 && z > -88.3))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// offices barrier left
	if (x == -9.0 && (z < -83.7 && z > -88.3))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	// 214 desk side start
	if (x == 12.9 && (z < -85.4 && z > -94.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	
	if (x == 15.9 && (z < -85.4 && z > -94.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 18.9 && (z < -85.4 && z > -94.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 21.9 && (z < -86.0 && z > -92.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 21.9 && (z < -86.0 && z > -92.6) && (y > -0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	// start next col
	if (x == 12.9 && (z < -73.8 && z > -83.0) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 15.9 && (z < -73.8 && z > -83.0) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 18.9 && (z < -73.8 && z > -83.0) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	//214 chair side 
	if (x == 14.5 && (z < -85.4 && z > -94.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}

	if (x == 17.5 && (z < -85.4 && z > -94.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 20.5 && (z < -85.4 && z > -94.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 21.9 && (z < -86.0 && z > -92.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 21.9 && (z < -86.0 && z > -92.6) && (y > -0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// start next col
	if (x == 14.5 && (z < -73.8 && z > -83.0) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 17.5 && (z < -73.8 && z > -83.0) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 20.5 && (z < -73.8 && z > -83.0) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}


	// 214 front wall
	if ((z < -74.3 && z > -94.6) && x == 9.2)
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// 206 cabinets sides
	if (x == 6.1 && (z < -24.7 && z > -26.0) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 12.7 && (z < -24.7 && z > -26.0) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 6.1 && (z < -24.7 && z > -26.0) && (y > 0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 12.7 && (z < -24.7 && z > -26.0) && (y > 0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// 206 sides of desks
	if (x == 14.3 && (z < -25.8 && z > -27.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 14.3 && (z < -28.8 && z > -30.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 14.3 && (z < -31.8 & z > -33.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 14.3 && (z < -34.8 && z > -36.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	

	if (x == 12.3 && (z < -28.8 && z > -30.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 12.3 && (z < -31.8 && z > -33.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 12.3 && (z < -34.8 && z > -36.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}

	// 208 x axis desks
	// 208 cabinets sides
	if (x == 6.1 && (z < -40.2 && z > -41.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 12.7 && (z < -40.2 && z > -41.5) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 6.1 && (z < -40.2 && z > -41.5) && (y > 0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 12.7 && (z < -40.2 && z > -41.5) && (y > 0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// 208 sides of desks
	if (x == 14.3 && (z < -41.3 && z > -42.9) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 14.3 && (z < -44.3 && z > -45.9) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 14.3 && (z < -47.3 & z > -48.9) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 14.3 && (z < -50.3 && z > -51.9) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}


	if (x == 12.3 && (z < -44.3 && z > -45.9) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 12.3 && (z < -47.3 && z > -48.9) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 12.3 && (z < -50.3 && z > -51.9) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// 210 x axis desks
	// 210 cabinets sides
	if (x == 6.1 && (z < -55.7 && z > -57.0) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 12.7 && (z < -55.7 && z > -57.0) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 6.1 && (z < -55.7 && z > -57.0) && (y > 0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 12.7 && (z < -55.7 && z > -57.0) && (y > 0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// 210 sides of desks
	if (x == 14.3 && (z < -56.8 && z > -58.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 14.3 && (z < -59.8 && z > -61.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 14.3 && (z < -62.8 & z > -64.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == 14.3 && (z < -65.8 && z > -67.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}


	if (x == 12.3 && (z < -59.8 && z > -61.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 12.3 && (z < -62.8 && z > -64.4) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == 12.3 && (z < -65.8 && z > -67.7) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}

	// linux lab sides
	if (x == -12.9 && (z < -34.2 & z > -36.6) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == -12.9 && (z < -30.5 & z > -33.0) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == -12.9 && (z < -26.8 & z > -29.5) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == -16.3 && (z < -37.7 & z > -40.1) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == -16.3 && (z < -33.9 & z > -36.4) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == -16.3 && (z < -30.2 & z > -32.9) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == -16.3 && (z < -26.7 & z > -29.4) && (y > -2.9 && y < 0.1))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	// linux lab cabinets sides
	if (x == -5.8 && (z < -40.27 & z > -41.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == -5.8 && (z < -40.8 & z > -41.6) && (y > 0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.x += revertPlus;
		return collide;
	}
	if (x == -12.7 && (z < -40.27 & z > -41.6) && (y > -2.9 && y < -1.0))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}
	if (x == -12.7 && (z < -40.8 & z > -41.6) && (y > 0.2 && y < 1.9))
	{
		collide = true;
		camera.Position.x += revertNeg;
		return collide;
	}



	// Y axis collisions


	// 206 y's
	if (y == -1.0 && (z < -25.8 & z > -27.4) && (x > 14.3 && x < 23.5))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -28.8 & z > -30.4) && (x > 14.3 && x < 23.5))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -31.8 & z > -33.4) && (x > 14.3 && x < 23.5))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -34.8 & z > -36.4) && (x > 14.3 && x < 23.5))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	// new col
	if (y == -1.0 && (z < -28.8 & z > -30.4) && (x > 3.1 && x < 12.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -31.8 & z > -33.4) && (x > 3.1 && x < 12.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -34.8 & z > -36.4) && (x > 3.1 && x < 12.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}

	//208 y's
	if (y == -1.0 && (z < -41.3 && z > -42.9) && (x > 14.3 && x < 23.5))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -44.3 & z > -45.9) && (x > 14.3 && x < 23.5))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -47.3 & z > -48.9) && (x > 14.3 && x < 23.5))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -50.3 & z > -51.9) && (x > 14.3 && x < 23.5))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	// new col
	if (y == -1.0 && (z < -44.3 & z > -45.9) && (x > 3.1 && x < 12.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -47.3 & z > -48.9) && (x > 3.1 && x < 12.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -50.3 & z > -51.9) && (x > 3.1 && x < 12.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	//210 y's
	if (y == -1.0 && (z < -56.8 && z > -58.4) && (x > 14.3 && x < 23.5))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -59.8 & z > -61.4) && (x > 14.3 && x < 23.5))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -62.8 & z > -64.4) && (x > 14.3 && x < 23.5))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -65.8 & z > -67.4) && (x > 14.3 && x < 23.5))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	// new col
	if (y == -1.0 && (z < -59.8 & z > -61.4) && (x > 3.1 && x < 12.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -62.8 & z > -64.4) && (x > 3.1 && x < 12.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -1.0 && (z < -64.8 & z > -67.4) && (x > 3.1 && x < 12.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}


	//linux lab y's
	if (y == 0.1 && (z < -34.2 && z > -36.6) && (x > -12.9 && x < -3.1))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 0.1 && (z < -30.5 && z > -33.0) && (x > -12.9 && x < -3.1))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 0.1 && (z < -26.8 && z > -29.5) && (x > -12.9 && x < -3.1))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	//next col
	if (y == 0.1 && (z < -37.7 & z > -40.1) && (x > -26.2 && x < -16.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 0.1 && (z < -34.2 && z > -36.6) && (x > -26.2 && x < -16.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 0.1 && (z < -30.5 && z > -33.0) && (x > -26.2 && x < -16.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 0.1 && (z < -26.8 && z > -29.5) && (x > -26.2 && x < -16.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}

	// 206 cabinet y's
	if (y == -0.9 && (z < -24.7 && z > -26.0) && (x > 6.1 && x < 12.7))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 2.0 && (z < -24.7 && z > -26.0) && (x > 6.1 && x < 12.7))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 0.1 && (z < -24.7 && z > -26.0) && (x > 6.1 && x < 12.7))
	{
		collide = true;
		camera.Position.y += revertNeg;
		return collide;
	}

	// 208 cabinets y
	if (y == -0.9 && (z < -40.2 && z > -41.5) && (x > 6.1 && x < 12.7))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 2.0 && (z < -40.2 && z > -41.5) && (x > 6.1 && x < 12.7))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 0.1 && (z < -40.2 && z > -41.5) && (x > 6.1 && x < 12.7))
	{
		collide = true;
		camera.Position.y += revertNeg;
		return collide;
	}

	// 210 cabinets y
	if (y == -0.9 && (z < -55.7 && z > -57.0) && (x > 6.1 && x < 12.7))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 2.0 && (z < -55.7 && z > -57.0) && (x > 6.1 && x < 12.7))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 0.1 && (z < -55.7 && z > -57.0) && (x > 6.1 && x < 12.7))
	{
		collide = true;
		camera.Position.y += revertNeg;
		return collide;
	}

	// linux lab cabinets
	if (y == -0.9 && (z < -40.27 && z > -41.6) && (x > -12.6 && x < -6.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 2.0 && (z < -40.27 && z > -41.6) && (x > -12.6 && x < -6.3))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == 0.1 && (z < -40.27 && z > -41.6) && (x > -12.6 && x < -6.3))
	{
		collide = true;
		camera.Position.y += revertNeg;
		return collide;
	}

	// offices desk y
	if (y == -0.7 && (z < -74.3 && z > -80.1) && (x > -4.0 && x < -2.6))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	if (y == -0.7 && (z < -78.6 && z > -80.1) && (x > -8.7 && x < -3.9))
	{
		collide = true;
		camera.Position.y += revertPlus;
		return collide;
	}
	return collide;
}


// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset, true);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
