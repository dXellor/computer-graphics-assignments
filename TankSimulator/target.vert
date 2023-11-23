#version 330 core 

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex;
out vec2 chTex;
uniform vec2 u_moveTarget;

void main()
{
	gl_Position = vec4(inPos.xy + u_moveTarget.xy, 0.0, 1.0);
	chTex = inTex;
}