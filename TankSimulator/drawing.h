#pragma once
#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include "starting_vertices.h"

void drawPanelSurfaceBackground(unsigned int* VAO, unsigned int basicTextureShader, unsigned int u_texLoc, unsigned texture, unsigned int u_texMoveLoc, float x, float y) {
    glBindVertexArray(VAO[6]);
    glUseProgram(basicTextureShader);
    glUniform1i(u_texLoc, 0);
    glUniform2f(u_texMoveLoc, x, y);
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawAmmunitionStatus(unsigned int* VAO, unsigned int ammunitionShader, unsigned int basicShader, unsigned int alphaUniform, unsigned int u_Loc, unsigned int u_Col, int bulletsCount) {
    glUseProgram(ammunitionShader);
    glBindVertexArray(VAO[0]);
    for (int i = 0; i < AMMUNITION_SIZE; i++) {
        if (i <= AMMUNITION_SIZE - bulletsCount - 1) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
;           glUniform4f(alphaUniform, 0.0, 0.0, 0.0, 0.4);
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

void drawFireLED(unsigned int* VAO, unsigned int shader, unsigned int u_Loc, unsigned int colorUniform, bool fire_led_on) {
    glUseProgram(shader);
    glBindVertexArray(VAO[2]);
    if (fire_led_on) {
        glUniform4f(colorUniform, 0.9, 0.1, 0.0, 1.0);
    }
    else {
        glUniform4f(colorUniform, 0.25, 0.03, 0.0, 1.0);
    }
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

void drawVoltmeter(unsigned int* VAO, unsigned int shader, unsigned int lineShader, int arraySize, unsigned int u_Loc, unsigned int u_colorLoc, unsigned int u_voltmeterLineLoc, bool hydraulicsOn, float voltage) {
    glUseProgram(shader);
    glUniform4f(u_colorLoc, 0.0, 0.0, 0.0, 1.0);
    glUniform2f(u_Loc, 0.0, -0.2);
    glBindVertexArray(VAO[4]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, arraySize / (2 * sizeof(float)));

    //Calculate line position by voltage (0 volts -> arrow points left)
    float noise = 0.0;
    float r = 0.5;
    if (voltage > 0) {
        noise = sin(100 * glfwGetTime())/5;
    }

    float line_x= r * cos((3.141592 / 180) * ((MAX_VOLTAGE - voltage + noise) * 180 / CRES));
    float line_y= r * sin((3.141592 / 180) * ((MAX_VOLTAGE - voltage + noise) * 180 / CRES));
    
    glUseProgram(lineShader);
    glUniform2f(u_voltmeterLineLoc, line_x, line_y);
    glBindVertexArray(VAO[5]);
    glLineWidth(5);
    glDrawArrays(GL_LINES, 0, 2);
    glLineWidth(1);
}

void drawTargets(unsigned int* VAO, unsigned int shader, int arraySize, unsigned int u_Loc, unsigned int u_colorLoc, float x, float y) {
    glUseProgram(shader);
    glUniform4f(u_colorLoc, 1.0, 0.0, 0.0, 1.0);
    glUniform2f(u_Loc, x, y);
    glBindVertexArray(VAO[7]);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(10);
    for (int i = 3; i < TARGETS_NUM; i++) {
        glDrawArrays(GL_POINTS, i * 2, 1);
    }
    glPointSize(1);
}