#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Header_Files/shaderClass.h"
#include "Header_Files/VAO.h"
#include "Header_Files/VBO.h"
#include "Header_Files/EBO.h"

using namespace std;



int main()
{
    try {
    // Initalize GLFW
    glfwInit();

    // Tell GLFW what version we are using (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW  we are using the CORE profile which only includes modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object that is 800 by 800 named Plot-a-Lot that is not fullscreen
    GLFWwindow *window = glfwCreateWindow(800, 800, "Plot-a-Lot", NULL, NULL);

    // If the window did not create output an error message
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    // Introduce the window in the the current context
    glfwMakeContextCurrent(window);

    // Load GLAD so it configures OpenGL
    if (!gladLoadGL())
    {
        cout << "Failed to initialize GLAD" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    // Specify the viewport of OpenGL in the window
    glViewport(0, 0, 800, 800);

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");

    // Vertices coordinates
    GLfloat vertices[] =
        {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
        };
    // Indices for lines: each pair of consecutive indices draws a line
    GLuint indices[] =
        {
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Upper triangle
		5, 4, 1 // Lower right triangle
        };
   	
    
    // Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
    

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		// GL_LINES mode: each pair of consecutive indices draws one line
		// 16 indices = 8 lines (8 pairs)
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
    }

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

    // Terminate the window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
    }
    catch (int err) {
        cout << "Error loading shader files. Error code: " << err << endl;
        cout << "Make sure the shader files exist in Resource_Files/Shaders/" << endl;
        glfwTerminate();
        return -1;
    }
    catch (...) {
        cout << "An unexpected error occurred!" << endl;
        glfwTerminate();
        return -1;
    }
}