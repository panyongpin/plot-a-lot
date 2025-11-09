#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Header_Files/shaderClass.h"
#include "Header_Files/VAO.h"
#include "Header_Files/VBO.h"
#include "Header_Files/EBO.h"

using namespace std;



int main()
{
    float width = 800;
    float height = 800;
    
    cout << "input the width:"; 
    cin >> height;

    // Initalize GLFW
    glfwInit();

    // Tell GLFW what version we are using (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW  we are using the CORE profile which only includes modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object that is 800 by 800 named Plot-a-Lot that is not fullscreen
    GLFWwindow *window = glfwCreateWindow(width, height, "Plot-a-Lot", NULL, NULL);

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
    glViewport(0, 0, width, height);

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");
    
    // Create orthographic projection matrix (2D view: 0,0 at bottom-left, 800x800)
    glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
    
    // Get the projection matrix uniform location (only need to do this once)
    GLuint projLocation = glGetUniformLocation(shaderProgram.ID, "projection");
    
    // Vertices coordinates
    GLfloat vertices[] =
        {
		200.0f, 200.0f, 0.0f, // Lower left corner
		400.0f, 400.0f, 0.0f, // Lower right corner
		200.0f, 400.0f, 0.0f, // Upper corner
		400.0f, 200.0f, 0.0f, // Inner left
        };
    // Indices for lines: each pair of consecutive indices draws a line
    GLuint indices[] =
    {
		0, 3, 2, 
        2, 1, 3 // Lower right triangle
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
		
		// Set the projection matrix uniform
		GLuint projLocation = glGetUniformLocation(shaderProgram.ID, "projection");
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
        
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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