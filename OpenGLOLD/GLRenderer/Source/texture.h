#pragma once

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

// Library Includes

// Local Includes

// Types

// Constants

// Prototypes
class OpenGLRenderer;

class Texture
{
//Member functions
public:
	Texture();
	~Texture();

	bool Initialise(OpenGLRenderer* _pRenderer, char* _pcFilename, unsigned int _uiTextureUnit);
	bool LoadFromPNG(OpenGLRenderer* _pRenderer, char* _pcFilename, unsigned int _uiTextureUnit);
	bool LoadFromTarga(OpenGLRenderer* _pRenderer, char* _pcFilename, unsigned int _uiTextureUnit);
	
	unsigned int GetTextureID() const;

private:
	Texture( const Texture& _krInstanceToCopy );
	const Texture& operator =( const Texture& _krInstanceToCopy );

//Member variables
private:
	OpenGLRenderer* m_pRenderer;
	bool m_bIsLoaded;
	unsigned int m_uiTextureID;
	int m_iTextureDimensions;

};

#endif //__TEXTURE_H__
