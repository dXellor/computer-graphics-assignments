#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inCol;
out vec3 chCol;
uniform vec2 u_movementPos;

void main(){
	chCol = inCol;
	gl_Position = vec4(inPos.xy + u_movementPos.xy, 0.0, 1.0);
}