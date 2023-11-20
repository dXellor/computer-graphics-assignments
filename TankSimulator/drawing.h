#pragma once
#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#define AMMUNITION_SIZE 7

void drawAmmunitionStatus(unsigned int* amunitionVAO, unsigned int fillShader, unsigned int borderShader, unsigned int alphaUniform) {
    int bulletsCount = 2;
    glUseProgram(fillShader);
    glBindVertexArray(amunitionVAO[0]);
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
    glBindVertexArray(amunitionVAO[1]);
    glLineWidth(5);
    for (int i = 0; i < AMMUNITION_SIZE; i++) {
        glDrawArrays(GL_LINE_LOOP, i * 4, 4);
    }
    glLineWidth(1);
}