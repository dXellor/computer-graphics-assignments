#version 330 core

in vec4 chCol;
out vec4 outCol;

void main(){
    outCol = vec4(0.49, 0.5, 0.0, chCol.a);
}
