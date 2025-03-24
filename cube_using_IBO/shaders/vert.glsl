#version 410 core


layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;

//Uniform Variable
uniform mat4 ModelMatrix;
uniform mat4 Perspective;
uniform mat4 ViewMatrix;

//pass vertex colors into fragment shader
out vec3 vertexclor;

void main(){
   vertexclor = vertexColors;
   vec4 newPosition = Perspective *ViewMatrix * ModelMatrix * vec4 (position,1.0f);
   gl_Position = vec4(newPosition.x, newPosition.y , newPosition.z , newPosition.z);

}

