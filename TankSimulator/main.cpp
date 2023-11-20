#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include "main.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void)
{

    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Uniforms 
    PositionVector2D movementVec;
    movementVec.x = 0.0;
    movementVec.y = 0.0;

    GLFWwindow* window;
    unsigned int wWidth = 1280;
    unsigned int wHeight = 720;
    const char wTitle[] = "Wow, this is epic";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 1;
    }

    glfwSetWindowUserPointer(window, &movementVec);
    glfwSetKeyCallback(window, keyCallback);

    float vertex_positions[] = {
        -0.25, -0.25,
         0.25, -0.25,
         0.0, 0.0,
         0.25, 0.25,
         -0.25, 0.25
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 4
    };

    unsigned int VAO;
    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    int stride = 2 * sizeof(float);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);*/

    unsigned int index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    unsigned int basicShader = createShader("basic.vs", "basic.fs");

    //kretanje...
    unsigned int u_movementPosLoc = glGetUniformLocation(basicShader, "u_movementPos");

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.7, 0.6, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    
        glBindVertexArray(VAO);
        glUseProgram(basicShader);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glUniform2f(u_movementPosLoc, movementVec.x, movementVec.y);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    //Oslobadjanje buffera

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);
    
    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    PositionVector2D* movementVec = (PositionVector2D *) glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_UP && action != GLFW_RELEASE) {
        movementVec->y += 0.01;
    }

    if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE) {
        movementVec->y -= 0.01;
    }

    if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE) {
        movementVec->x -= 0.01;
    }

    if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE) {
        movementVec->x += 0.01;
    }
}
