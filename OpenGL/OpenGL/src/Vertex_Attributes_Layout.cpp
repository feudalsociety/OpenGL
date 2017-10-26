#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <iostream>

int main(void)
{
	GLFWwindow * window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

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

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[6] = {
		0.5f, -0.5f,
		0.0f,  0.5f,
		0.5f, -0.5f
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer); // with the buffer bound
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
	// copying position into buffer (by specifying pointer to them and how big they are)

	// tell the layout of the buffer

	/* enable vertex attribute
	OpenGL is state machine, can call any where you like as long as your buffer is bound
	*/
	glEnableVertexAttribArray(0);

	/* we just have position, specifying single attribute, only need to call the function once
	as long as buffer is bound can do whenever, before or after fill in with the data
	*/
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	/* stride : next vertex
	pointer : next attribute
	if there is another attribute like texture coordinate
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)8);
	- pointer needs to be a const void pointer
	if you do this proper - you probably have a struct to define your vertex and use offsetof macro
	*/

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
