#version 410 core


layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;

uniform mat4 ModelMatrix;
uniform mat4 Perspective;

out vec3 vertexclor;

void main(){
   vertexclor = vertexColors;
   vec4 newPosition = Perspective * ModelMatrix * vec4 (position,1.0f);
   gl_Position = vec4(newPosition.x, newPosition.y , newPosition.z , newPosition.z);

}

