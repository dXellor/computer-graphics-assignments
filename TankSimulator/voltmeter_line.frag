#version 330 core

layout(location = 0) in vec2 inPos;
out vec4 chCol;
uniform epic
uniform vec3 u_col;

void main(){
	chCol = vec4(u_col.rgb, 1.0);
	gl_Position = vec4(inPos.xy, 0.0, 1.0);
}