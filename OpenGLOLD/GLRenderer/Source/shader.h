//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   shader.h
//  Description :   Class of Shader
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __SHADER_H__
#define __SHADER_H__

// Library Includes
#include <glm\glm.hpp>

// Local Includes

// Types

// Constants

// Prototypes
class OpenGLRenderer;

class Shader
{
public:
	Shader();
	~Shader();

	bool InitialiseShader(OpenGLRenderer* _pRenderer, char* _pcVertexShader, char* _pcFragmentShader);
	char* LoadShader(char* _pcFilename);
	void SetShader(OpenGLRenderer* _pRenderer);
	void SetShaderMatrix(OpenGLRenderer* _pRenderer, char* _pcVariableName, float* _pMatrix);
	void SetShaderInteger(OpenGLRenderer* _pRenderer, char* _pcVariableName, int _iInteger);
	void SetShaderFloat(OpenGLRenderer* _pRenderer, char* _pcVariableName, float _fFloat);
	void SetShaderVector3(OpenGLRenderer* _pRenderer, char* _pcVariableName, glm::vec3& _rVector);
	void SetShaderVector4(OpenGLRenderer* _pRenderer, char* _pcVariableName, glm::vec4& _rVector);
	void OutputShaderErrorMessage(OpenGLRenderer* _pRenderer, unsigned int _uiShaderId);

	int GetShaderProgram() const;

private:
	Shader(const Shader& _krInstanceToCopy);
	const Shader& operator =(const Shader& _krInstanceToCopy);


private:
	OpenGLRenderer* m_pRenderer;
	unsigned int m_uiVertexShader;
	unsigned int m_uiFragmentShader;
	unsigned int m_uiShaderProgram;

};

#endif //__SHADER_H__