#include <GL/glew.h> // place this before any other OpenGL thing 
#include <GLFW/glfw3.h>
#include <iostream>

int main(void)
{
	GLFWwindow * window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// glewInit(); // need to read documentation or reverse engineer
	// we need to define GLEW_STATIC; at C++ properties - preprocessor 

	// usage

	/*
	if (glewInit() != GLEW_OK)
	std::cout << "Error!" << std::endl;
	1. can't use OpenGL function from GLEW until you call glewInit()
	2. need to create valid OpenGL rendering context before call glewInit()
	*/

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) // after the valid context
		std::cout << "Error!" << std::endl;

	// print out OpenGL version, nothing to do with GLEW
	std::cout << glGetString(GL_VERSION) << std::endl;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}