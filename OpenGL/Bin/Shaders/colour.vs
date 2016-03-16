//===========================================
//
//	SHADER FILE
//	Vertex shader
//
//===========================================

#version 400

//INPUT
in vec3 inputPosition;
in vec3 inputColour;

//OUTPUT
out vec3 colour;

//UNIFORM Variables
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//================== VERTEX SHADER ==============
void main(void)
{
	gl_Position = worldMatrix * vec4(inputPosition, 1.0f);
	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;

	colour = inputColour;
}