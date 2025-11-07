#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// Vertex Shader source code
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
// Fragment Shader source code
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
                                   "}\n\0";

int main()
{
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
    gladLoadGL();
    // Specify the viewport of OpenGL in the window
    glViewport(0, 0, 800, 800);

    // Reference value for vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader source to the object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Compile it right away for the GPU
    glCompileShader(vertexShader);

    // Reference value for the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attache Fragment Shader source to the object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Compile it right away fro the GPU
    glCompileShader(fragmentShader);

    // Reference value for shader program
    GLuint shaderProgram = glCreateProgram();
    // Attach each shader to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Link all the shaders together into the shader program
    glLinkProgram(shaderProgram);

    // Delete the shaders after we are done using them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertices coordinates
    GLfloat vertices[] =
        {
            0.5f, 0.5f, 0.0f,    // Right Top 0
            0.5f, -0.5f, 0.0f,   // Right Bottom 1
            -0.5f, -0.5f, 0.0f,  // Left Bottom 2
            -0.5f, 0.5f, 0.0f,   // Left Top 3
            0.45f, 0.45f, 0.0f,  // Right Top1 4
            -0.45, 0.45f, 0.0f,  // Left Top1 5
            0.45, -0.5f, 0.0f,   // Right Bottom1 6
            -0.45, -0.5f, 0.0f}; // Left Bottom1 7
    GLuint indices[] =
        {
            0, 1, 6, 4, 5, 7, 2, 3, 0};
    // Create reference containers for the vertex array and buffer object
    GLuint VAOs[1], VBOs[1], EBOs[1];

    // Generate the VAO and VBO with only 1 object each
    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, VBOs);
    glGenBuffers(1, EBOs);

    // Make the VAO the current vertex arrary object by binding it
    glBindVertexArray(VAOs[0]);

    // Bind the VBO spcifying it is a GL ARRay Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    // Introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(0);

    // Bind both the VBO and VAO to 0 so we do not accidentally modify it
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_LINE_LOOP, 9, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Delete all the objects we created
    glDeleteVertexArrays(1, VAOs);
    glDeleteBuffers(1, VBOs);
    glDeleteBuffers(1, EBOs);
    glDeleteProgram(shaderProgram);

    // Terminate the window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}