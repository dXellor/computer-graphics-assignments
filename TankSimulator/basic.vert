#version 330 core

layout(location = 0) in vec2 inPos;
out vec4 chCol;
uniform vec2 u_alpha;
uniform vec2 u_move;

void main(){
	chCol = vec4(0.0, 0.0, 0.0, u_alpha.x);
	gl_Position = vec4(inPos.xy + u_move.xy, 0.0, 1.0);
}