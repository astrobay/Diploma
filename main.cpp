
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
const GLuint WIDTH = 800, HEIGHT = 600;


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

		Shader firstShader("vertex.glsl","fragment1.glsl");
		Shader secondShader("vertex.glsl","fragment2.glsl");


    // Set up vertex data (and buffer(s)) and attribute pointers
  /*	GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // Left
         0.5f, -0.5f, 0.0f, // Right
         0.0f,  0.5f, 0.0f  // Top
    }; */
		GLfloat vertices[2][9] = {
			{-1.0f,0.0f,0.0f,
			-0.5f,1.0f,0.0f,
			0.0f,0.0f,0.0f},

			{0.0f,0.0f,0.0f,
			1.0f,0.0f,0.0f,
			0.5f,1.0f,0.0f}
		};


    GLuint VBO[2],VAO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
  	//VAO[1]
    glBindVertexArray(VAO[0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]),vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

		//VAO[2]
		glBindVertexArray(VAO[1]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[1]), vertices[1], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0);



    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw our first triangle
				firstShader.Use();
        glBindVertexArray(VAO[0]);
				glDrawArrays(GL_TRIANGLES, 0, 3);
				glBindVertexArray(0);
				secondShader.Use();
				glBindVertexArray(VAO[1]);
				glDrawArrays(GL_TRIANGLES, 0, 3);
				glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
