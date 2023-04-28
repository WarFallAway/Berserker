#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_ONLY_PNG
#include "stb_image.h"

ResourceManager::ResourceManager(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\"); //����� ���������� � ������ exe
	m_path = executablePath.substr(0, found); //�������� �������������� ����, ����� ����� ������ ������ ���� �� exe, ��� �������� ������

}
std::string ResourceManager::getFileString(const std::string& relativeFilePath) const
{
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary); //��������� ���� �� ������ �������������� ����
	if (!f.is_open()) {
		std::cerr << "Failed to open file: " << relativeFilePath << '\n';
		return std::string{};
	}
	std::stringstream buffer;
	buffer << f.rdbuf(); //��������� �������� �� ����� � ������
	return buffer.str(); //����������� ������ � string � ���������� �������� �������
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName,
	const std::string& vertexPath, const std::string& fragmentPath) //������� �������� ��������
{
	std::string vertexString = getFileString(vertexPath); //����� ������ �������������� ���� �� ����������� �������
	if (vertexString.empty())
	{
		std::cerr << "No vertex shader!" << '\n';
		return nullptr;
	}
	std::string fragmentString = getFileString(fragmentPath); //���������� ��� ������������ �������
	if (fragmentString.empty())
	{
		std::cerr << "No fragment shader!" << '\n';
		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->is_Compiled())
	{
		return newShader;
	}
	std::cerr << "Can't load shader program:\n"
		<< "Vertex: " << vertexPath << '\n'
		<< "Fragment: " << fragmentPath << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end()) {
		return it->second;
	}
	std::cerr << "Can't find shader program: " << shaderName << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channels = 0; //���������� �������
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true); //��� �����, ����� ������� ����������� � ��� �� �������, ��� � � openGL
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0); //���������� �������� ����������� � ���������
	if (!pixels)
	{
		std::cerr << "Can't load image: " << texturePath << std::endl;
		return nullptr;
	}
	std::shared_ptr<Renderer::Texture2D>  newTexture = m_textures.emplace(textureName,
																			std::make_shared<Renderer::Texture2D>(width, height, pixels,
																											channels,
																											GL_NEAREST,
																										GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels); //������������ ������
	return newTexture;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end()) {
		return it->second;
	}
	std::cerr << "Can't find texture: " << textureName << std::endl;
	return nullptr;
}