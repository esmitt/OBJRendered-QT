#version 330

// vertex attributes 
in vec3 position;
in vec3 normal;

uniform mat4 MVP; // model-view-projection matrix

uniform float offset; // offset along normal

void main(void){
   vec4 newPos   = vec4(position + normal * offset, 1.0);
   gl_Position	= MVP * newPos;
}