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
		200.0f, 200.0f, 0.0f, 0.0f, 0.0f, // Lower left corner
		400.0f, 200.0f, 0.0f, 1.0f, 0.0f, // Lower right corner
		200.0f, 400.0f, 0.0f, 0.0f, 1.0f, // Upper left corner
		400.0f, 400.0f, 0.0f, 1.0f, 1.0f, // Upper right corner
        };
    // Indices for lines: each pair of consecutive indices draws a line
    GLuint indices[] =
    {
		0, 3, 2, 
        1, 0, 3 // Lower right triangle
    };  
    // Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	// Position attribute (layout 0: x, y, z) - stride is 5 floats, offset is 0
	VAO1.Bind();
	VBO1.Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Texture coordinate attribute (layout 1: u, v) - stride is 5 floats, offset is 3 floats
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Textures
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
    
    unsigned char *bytes = stbi_load("C:/Users/handr/Documents/GitHub/plot-a-lot/lib/deadpool.png", &widthImg, &heightImg, &numColCh, 0);


	if (bytes == NULL)
	{
		cout << "Failed to load texture: deadpool.png" << endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Determine the format based on number of color channels
	GLenum format = GL_RGB;
	if (numColCh == 4)
		format = GL_RGBA;
	else if (numColCh == 1)
		format = GL_RED;

	glTexImage2D(GL_TEXTURE_2D, 0, format, widthImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

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

		// Bind the texture and set the texture uniform
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex0"), 0);
        
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
	glDeleteTextures(1, &texture);
	shaderProgram.Delete();

    // Terminate the window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}