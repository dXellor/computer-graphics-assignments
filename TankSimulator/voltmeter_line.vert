#version 330 core

layout(location = 0) in vec2 inPos;
out vec4 chCol;
uniform vec2 u_moveVoltmeterLine;

void main(){
	chCol = vec4(1.0, 0.0, 0.0, 1.0);
	if(inPos.x != 0.0 && inPos.y != 0.2){
		gl_Position = vec4(u_moveVoltmeterLine.xy, 0.0, 1.0);
	}else{
		gl_Position = vec4(inPos.xy, 0.0, 1.0);
	}
}