#version 410 core

in vec3 vertexclor;
out vec4 color;

uniform float u_Offset;  //uniform variable


void main(){

    color = vec4(vertexclor.r-u_Offset, vertexclor.g ,vertexclor.b , 1.0f);

}
