#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); // return pointer to null terminated immutable array

	/* const char* src = &source[0]; - could also write code like this
	because string API does provide us c string prefer write upper code
	*/
	glShaderSource(id, 1, &src, nullptr); // specifies the source of our shader 
	// id, count : how many source codes are we specifying, double pointer, length 
	// length : If you don't want to use the whole string or anything - pass nullptr
	glCompileShader(id); // specify which shader want to compile 

	// TODO : Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) // GL_FALSE == 0, if(!result) (alt + f12)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		/* meesage[length]; - error
		most people probably solve this by making this a heap allocation and delete it later
		or maybe assigning to a unqiue pointer
		I still want this to just be a normal char allocated on the stack
		*/
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message); // documentation
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id); // handle this result, because our shader compilation didn't work out
		return 0; // return unsigned int
	}
	return id;
}
// static - don't want to link it to other translation unit
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// code necessary to compile these two shader
	// all we doing is taking in actual source code of the shaders as these actual string
	// strings are meant to be the actual source code

	unsigned int program = glCreateProgram();
	// GLuint program = glCreateProgram(); prefer to use raw c++ types
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); // type of shader
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	/* we are not checking here if it works, we probably assert or something like that if this doesn't work out
	because it is kind of vital to our program running succesfully
	but anyway at least print out error message to the console should be enough to debugg in our situation
	*/

	/*need to attach both of these shader to our program
	think of this almost like compiling c++ code we've got two different files,
	and now we want to link them into kind of one program so we can use both of them
	*/
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	// one for each shader

	glLinkProgram(program);
	glValidateProgram(program);
	// not going to explain these too much right now, because it's kind of code you write once and forget about

	// fianlly can delete our shader because they've linked into a program
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

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

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	// write our source code
	std::string vertexshader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n" // specify actual attribute
		"\n" // don't really need to write newline, this is more for me than actual c++, but do it anyway
		"void main()\n" // this is main function for our shader, work the same way as our actual main function does in c++
		"{\n"           // this main function will get called when this vertex shader gets called
		"	gl_Position = position;\n" // gl_Postion = vec4(position.xy, 0.0, 1.0);
		"}\n";
	// another reason why defining these kind of shaders in files is a lot better (next episode)

	std::string fragmentshader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		// don't need layout stuff, do need to do remember is fragment shader should be outputting a color
		// can keep layout(location = 0), not necessary it will be 0 by default anyway
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n" // (r,g,b,a)
		"}\n";

	unsigned int shader = CreateShader(vertexshader, fragmentshader);
	glUseProgram(shader); // bound our shader

	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		// draw current bound buffer
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// to clean up the shader, should also doing this for kind of vertex buffer and all that
	// we are going to abstract a lot of stuff into classes 
	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}