#version 410 core


in vec3 vertexclor;
out vec4 color;



void main(){

    color = vec4(vertexclor.r, vertexclor.g ,vertexclor.b , 1.0f);

}
