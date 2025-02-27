#version 410 core


layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;

out vec3 vertexclor;

void main(){
   vertexclor = vertexColors;
   gl_Position = vec4(position.x, position.y, position.z, 1.0f);

}

