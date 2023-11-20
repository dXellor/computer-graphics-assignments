#version 330 core

layout(location = 0) in vec2 inPos;
out vec4 chCol;
uniform vec2 u_move;
uniform vec4 u_col;

void main(){
	chCol = u_col;
	gl_Position = vec4(inPos.xy + u_move.xy, 0.0, 1.0);
}