#pragma once
#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include "starting_vertices.h"
#define AMMUNITION_SIZE 7
#define CAGE_SIZE 5

void drawAmmunitionStatus(unsigned int* VAO, unsigned int ammunitionShader, unsigned int basicShader, unsigned int alphaUniform, unsigned int u_Loc, unsigned int u_Col) {
    int bulletsCount = 2;
    glUseProgram(ammunitionShader);
    glBindVertexArray(VAO[0]);
    for (int i = 0; i < AMMUNITION_SIZE; i++) {
        if (i <= AMMUNITION_SIZE - bulletsCount - 1) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
;           glUniform4f(alphaUniform, 0.0, 0.0, 0.0, 0.0);
        }
        else {
            glUniform4f(alphaUniform, 1.0, 0.0, 0.0, 0.0);
            glDisable(GL_BLEND);
        }
        glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4); 
    }
    
    glUseProgram(basicShader);
    glUniform2f(u_Loc, 0.0, 0.0);
    glUniform4f(u_Col, 0.0, 0.0, 0.0, 1.0);
    glBindVertexArray(VAO[1]);
    glLineWidth(5);
    for (int i = 0; i < AMMUNITION_SIZE; i++) {
        glDrawArrays(GL_LINE_LOOP, i * 4, 4);
    }
    glLineWidth(1);
}

void drawFireLED(unsigned int* VAO, unsigned int shader, unsigned int u_Loc, unsigned int colorUniform) {
    glUseProgram(shader);
    glBindVertexArray(VAO[2]);
    glUniform4f(colorUniform, 0.25, 0.03, 0.0, 1.0);
    glUniform2f(u_Loc, 0.0, 0.0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUniform4f(colorUniform, 0.0, 0.0, 0.0, 1.0);
    glBindVertexArray(VAO[3]);
    glLineWidth(5);
    for (int i = 0; i < CAGE_SIZE; i++) {
        glDrawArrays(GL_LINE_LOOP, i * 4, 4);
    }
    glLineWidth(1);
}

void drawVoltmeter(unsigned int* VAO, unsigned int shader, int arraySize, unsigned int u_Loc, unsigned int u_colorLoc) {
    glUseProgram(shader);
    glUniform4f(u_colorLoc, 0.0, 0.0, 0.0, 1.0);
    glUniform2f(u_Loc, 0.0, -0.2);
    glBindVertexArray(VAO[4]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, arraySize / (2 * sizeof(float)));

    glUniform2f(u_Loc, 0.0, -0.15);
    glUniform4f(u_colorLoc, 1.0, 0.0, 0.0, 1.0);
    glBindVertexArray(VAO[5]);
    glLineWidth(5);
    glDrawArrays(GL_LINES, 0, 2);
    glLineWidth(1);
}