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

	/* we don't want to do this every frame, just define buffer to begin with and then fill with data
	and then inside the game's render loop we can draw the data
	if it some reason require change that data frame to frame we can do that by updating the buffer
	*/

	unsigned int buffer;
	glGenBuffers(1, &buffer); // return void, instead of returning ID of what we generate, instead provided integer

							  /* everything you generate get assigned integer - ID for object (vertex buffer, vertex array, texture, shader...)
							  If I want to bind or select buffer, passing the integer
							  */

	glBindBuffer(GL_ARRAY_BUFFER, buffer); // selecting(bound), just about to work on it
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);// specify the data, important to know size is in bytes (~documentation)
																				// need to tell how to interpret data, how the data will layed out - shader (later on)

	while (!glfwWindowShouldClose(window)) // game loop
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//issue a draw poll for buffer, 2 ways (documentation)
		glDrawArrays(GL_TRIANGLES, 0, 3); // one that is currently bound is going to be drawn
										  //glDrawElements(GL_TRIANGLES, 3, ...) - index buffer 

										  /* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}