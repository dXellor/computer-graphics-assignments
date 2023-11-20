#pragma once
#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include "starting_vertices.h"
#define AMMUNITION_SIZE 7
#define CAGE_SIZE 5

void drawAmmunitionStatus(unsigned int* VAO, unsigned int fillShader, unsigned int borderShader, unsigned int alphaUniform) {
    int bulletsCount = 2;
    glUseProgram(fillShader);
    glBindVertexArray(VAO[0]);
    for (int i = 0; i < AMMUNITION_SIZE; i++) {
        if (i <= AMMUNITION_SIZE - bulletsCount - 1) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
;           glUniform2f(alphaUniform, 0.0, 0.0);
        }
        else {
            glUniform2f(alphaUniform, 1.0, 0.0);
            glDisable(GL_BLEND);
        }
        glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4); 
    }
    
    glUseProgram(borderShader);
    glBindVertexArray(VAO[1]);
    glLineWidth(5);
    for (int i = 0; i < AMMUNITION_SIZE; i++) {
        glDrawArrays(GL_LINE_LOOP, i * 4, 4);
    }
    glLineWidth(1);
}

void drawFireLED(unsigned int* VAO, unsigned int shader, unsigned int borderShader, unsigned int colorUniform) {
    glUseProgram(shader);
    glBindVertexArray(VAO[2]);
    glUniform3f(colorUniform, 0.25, 0.03, 0.0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUseProgram(borderShader);
    glBindVertexArray(VAO[3]);
    glLineWidth(5);
    for (int i = 0; i < CAGE_SIZE; i++) {
        glDrawArrays(GL_LINE_LOOP, i * 4, 4);
    }
    glLineWidth(1);
}