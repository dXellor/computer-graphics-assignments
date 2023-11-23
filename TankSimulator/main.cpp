#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "starting_vertices.h"
#include "drawing.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);
void control(GLFWwindow* window, int* bulletCount, bool* fireLedOn, float* texX, float* texY, bool* isInside, double* fireTimeStamp,
    bool* hydraulicsOn, double* hydraulicsTimeStamp, float* voltage, float* targets_vert, bool* targets_hit);

int main(void)
{

    if (!glfwInit())
    {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 1920;
    unsigned int wHeight = 1080;
    const char wTitle[] = "Tank simulator - RA32-2020";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, glfwGetPrimaryMonitor(), NULL);
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
        [6] - background
        [7] - targets
        [8] - signature
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
    generateVoltmeterVert(voltmeter_vert);
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

    //Background
    stride = 4 * sizeof(float);
    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(panel_surface_background_vert), panel_surface_background_vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));

    //Targets
    float target_positions[TARGETS_NUM * 2];
    float target_vert[TARGETS_NUM * 16];
    generateRandomTargetPositions(target_positions);
    generateTargetVert(target_positions, target_vert);
    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(target_vert), target_vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));

    //Signature
    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(target_vert), target_vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));

    //Detach
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Textures
    unsigned metalSurfaceTexture = loadImageToTexture("textures/internal-board-surface.png");
    glBindTexture(GL_TEXTURE_2D, metalSurfaceTexture); 
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned shootingRangeTexture = loadImageToTexture("textures/panorama.jpg");
    glBindTexture(GL_TEXTURE_2D, shootingRangeTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned outsideVisionTexture = loadImageToTexture("textures/outside-vision.png");
    glBindTexture(GL_TEXTURE_2D, outsideVisionTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned targetTexture = loadImageToTexture("textures/target.png");
    glBindTexture(GL_TEXTURE_2D, targetTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned signatureTexture = loadImageToTexture("textures/signature.png");
    glBindTexture(GL_TEXTURE_2D, signatureTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Shaders
    unsigned int ammunitionShader = createShader("basic.vert", "ammunition.frag");
    unsigned int basicShader = createShader("basic.vert", "basic.frag");
    unsigned int basicTextureShader = createShader("basic_texture.vert", "basic_texture.frag");
    unsigned int voltmeterLineShader = createShader("voltmeter_line.vert", "voltmeter_line.frag");
    unsigned int transparentTextureShader = createShader("basic_texture.vert", "transparent_texture.frag");
    unsigned int targetShader = createShader("target.vert", "target.frag");

    //Uniforms
    unsigned int u_colorLocAmm = glGetUniformLocation(ammunitionShader, "u_col");
    unsigned int u_colorLoc = glGetUniformLocation(basicShader, "u_col");
    unsigned int u_basicMoveLoc = glGetUniformLocation(basicShader, "u_move");
    unsigned int u_basicTexMoveLoc = glGetUniformLocation(basicTextureShader, "u_moveTex");
    unsigned int u_transparentTexMoveLoc = glGetUniformLocation(transparentTextureShader, "u_moveTex");
    unsigned int u_voltmeterLineMoveLoc = glGetUniformLocation(voltmeterLineShader, "u_moveVoltmeterLine");
    unsigned u_TexLoc = glGetUniformLocation(basicTextureShader, "uTex");
    unsigned u_transparentTexLoc = glGetUniformLocation(transparentTextureShader, "uTex");
    glUniform1i(u_TexLoc, 0);
    glUniform1i(u_transparentTexLoc, 0);

    unsigned int u_moveTarget = glGetUniformLocation(targetShader, "u_moveTarget");

    //Variables
    int bulletCount = AMMUNITION_SIZE;
    bool fireLedOn = true;
    bool isInside = true;
    float texX = 0.0;
    float texY = 0.0;
    double fireTimeStamp = 0.0;
    double hydraulicsSwitchTimestamp = 0.0;
    bool hydraulicsOn = false;
    float voltage = 0.0;

    //Rendering loop
    glfwSetTime(0);
    while (!glfwWindowShouldClose(window))
    {
        control(window, &bulletCount, &fireLedOn, &texX, &texY, &isInside, &fireTimeStamp, &hydraulicsOn, &hydraulicsSwitchTimestamp, &voltage, target_positions, target_hit);

        glClearColor(0.1, 0.3, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, wWidth, wHeight);
        if (isInside) {
            drawPanelSurfaceBackground(VAO, basicTextureShader, u_TexLoc, metalSurfaceTexture, u_basicTexMoveLoc, 0.0, 0.0);
            glViewport(0, 0, wWidth / 2, wHeight);
            drawAmmunitionStatus(VAO, ammunitionShader, basicShader, u_colorLocAmm, u_basicMoveLoc, u_colorLoc, bulletCount);
            drawFireLED(VAO, basicShader, u_basicMoveLoc, u_colorLoc, fireLedOn);

            glViewport(wWidth / 2, 0, wWidth / 2, wHeight);
            drawVoltmeter(VAO, basicShader, voltmeterLineShader, sizeof(voltmeter_vert), u_basicMoveLoc, u_colorLoc, u_voltmeterLineMoveLoc, hydraulicsOn, voltage);
        }
        else {
            drawPanelSurfaceBackground(VAO, basicTextureShader, u_TexLoc, shootingRangeTexture, u_basicTexMoveLoc, texX, texY);
            glViewport(0, 0, wWidth, wHeight);
            drawTargets(VAO, targetShader, targetTexture, u_TexLoc, u_moveTarget, texX, texY, target_positions, target_hit);
            drawPanelSurfaceBackground(VAO, transparentTextureShader, u_transparentTexLoc, outsideVisionTexture, u_transparentTexMoveLoc, 0.0, 0.0);
        }
        glViewport(wWidth-700, wHeight-100, 700, 100);
        drawPanelSurfaceBackground(VAO, transparentTextureShader, u_transparentTexLoc, signatureTexture, u_transparentTexMoveLoc, 0.0, 0.0);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &metalSurfaceTexture);
    glDeleteTextures(1, &shootingRangeTexture);
    glDeleteBuffers(10, VBO);
    glDeleteVertexArrays(10, VAO);
    glDeleteProgram(basicShader);
    glDeleteProgram(basicTextureShader);
    glDeleteProgram(ammunitionShader);
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

static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}

void control(GLFWwindow* window, int* bulletCount, bool* fireLedOn, float* texX, float* texY, bool* isInside, double* fireTimeStamp, 
    bool* hydraulicsOn, double* hydraulicsTimeStamp, float* voltage, float* targets_vert, bool* targets_hit) {
    
    //FireLED check
    if (!(*fireLedOn) && (glfwGetTime() - (*fireTimeStamp)) > 7.5) {
        *fireLedOn = true;
    }

    //Lower voltage if hydraulics is off
    if (!(*hydraulicsOn) && (*voltage) > 0) {
        (*voltage) -= 0.02;
    }

    //Actions START
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (*fireTimeStamp == 0 || ((glfwGetTime() - (*fireTimeStamp)) > 7.5 && (*bulletCount) > 0)) {
            (*bulletCount)--;
            *(fireLedOn) = false;
            *fireTimeStamp = glfwGetTime();
            scanForHit(targets_hit, targets_vert, *texX, *texY);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (*bulletCount == 0) {
            *bulletCount = 7;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        if (*hydraulicsTimeStamp == 0 || ((glfwGetTime() - (*hydraulicsTimeStamp)) > 3)) {
            *hydraulicsOn = !(*hydraulicsOn);
            *hydraulicsTimeStamp = glfwGetTime();
        } 
    }

    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
        if ((*hydraulicsOn) && (*voltage) < MAX_VOLTAGE) {
            (*voltage) += 0.05;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
        if ((*hydraulicsOn) && (*voltage) > 0) {
            (*voltage) -= 0.05;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        *isInside = false;
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        *isInside = true;
    }
    //Actions END

    //Movement START
    if (!(*isInside)) {
        float speed;
        if (!(*hydraulicsOn)) {
            speed = BASE_TANK_SPEED / 10;
        }
        else {
            speed = BASE_TANK_SPEED + ((*voltage) / MAX_VOLTAGE) * TANK_SPEED_C;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            if ((*texY) < 0.27)
                (*texY) += speed;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            if ((*texY) > -0.27)
                (*texY) -= speed;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            (*texX) += speed;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            (*texX) -= speed;
        }
    }
    //Movement END
}
