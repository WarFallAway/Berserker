#include "Texture2D.h"


namespace Renderer {

	Texture2D::Texture2D(const GLuint width, const GLuint height,
		const unsigned char* data,
		const unsigned int channels = 4,
		const GLenum filter = GL_LINEAR,
		const GLenum wrapMode = GL_CLAMP_TO_EDGE)
		: m_width(width), m_height(height)
	{
		switch (channels)
		{
		case 4:
			m_mode = GL_RGBA;
		case 3:
			m_mode = GL_RGB;
		default:
			m_mode = GL_RGBA;
			break;
		}

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	}


}