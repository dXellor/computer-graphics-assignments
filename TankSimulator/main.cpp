#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>

//Custom
#include "starting_vertices.h"
#include "drawing.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
void control(GLFWwindow* window, int* bulletCount, bool* fireLedOn);

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

    GLFWwindow* window;
    unsigned int wWidth = 1280;
    unsigned int wHeight = 720;
    const char wTitle[] = "Tank simulator - RA32-2020";
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

    /*
        VAO list
        [0] - ammunition status
        [1] - ammunition border
        [2] - fire LED
        [3] - fire LED cage
        [4] - voltmeter
        [5] - voltmeter line
    */

    unsigned int VAO[10];
    glGenVertexArrays(10, VAO);
    unsigned int VBO[10];
    glGenBuffers(10, VBO);

    //Ammunition
    int stride = 2 * sizeof(float);
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ammunition_status_vert), ammunition_status_vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    
    //Ammunition border
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ammunition_status_border_vert), ammunition_status_border_vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);

    //Fire LED
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fire_led_vert), fire_led_vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);

    //Fire LED Cage
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fire_led_cage), fire_led_cage, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);

    //Voltmeter
    float voltmeter_vert[CRES * 2 + 4];
    float r = 0.7;

    voltmeter_vert[0] = 0;
    voltmeter_vert[1] = 0;
    int i;
    for (i = 0; i <= CRES; i++)
    {

        voltmeter_vert[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 180 / CRES));
        voltmeter_vert[2 + 2 * i + 1] = r * sin((3.141592 / 180) * (i * 180 / CRES));
    }
    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(voltmeter_vert), voltmeter_vert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Voltmeter line
    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(voltmeter_line_vert), voltmeter_line_vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);

    //Detach
    glBindVertexArray(0);

    //Shaders
    unsigned int ammunitionShader = createShader("basic.vert", "ammunition.frag");
    unsigned int basicShader = createShader("basic.vert", "basic.frag");

    //Uniforms
    unsigned int u_colorLocAmm = glGetUniformLocation(ammunitionShader, "u_col");
    unsigned int u_colorLoc = glGetUniformLocation(basicShader, "u_col");
    unsigned int u_basicMoveLoc = glGetUniformLocation(basicShader, "u_move");

    //Variables
    int bulletCount = 7;
    bool fireLedOn = true;

    //Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        control(window, &bulletCount, &fireLedOn);

        glClearColor(0.1, 0.3, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    
        glViewport(0, 0, wWidth / 2, wHeight);
        drawAmmunitionStatus(VAO, ammunitionShader, basicShader, u_colorLocAmm, u_basicMoveLoc, u_colorLoc, bulletCount);
        drawFireLED(VAO, basicShader, u_basicMoveLoc, u_colorLoc, fireLedOn);

        glViewport(wWidth / 2 + 1, 0, wWidth / 2, wHeight);
        drawVoltmeter(VAO, basicShader, sizeof(voltmeter_vert), u_basicMoveLoc, u_colorLoc);

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

void control(GLFWwindow* window, int* bulletCount, bool* fireLedOn) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        (*(bulletCount))--;
        *(fireLedOn) = false;
    }
}
