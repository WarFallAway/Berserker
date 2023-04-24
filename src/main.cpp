#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
extern "C"
_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;

int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwSetWindowSizeCallback(GLFWwindow* pWindow, int Width, int Height) {
    g_windowSizeX = Width;
    g_windowSizeY = Height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY); //��������� �� ������ ��������� (����� ������ ����)
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(void)
{
    

    /* ������������� ���������� */
    if (!glfwInit()) {
        std::cout << "glfw can't initialize\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //�������� ������ ���������
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //������������ ������� ���������
    
    GLFWwindow* pwindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Berserker", nullptr, nullptr); //�������� ����96
    if (!pwindow)
    {
        std::cout << "OKNA96 can't to create((\n";
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pwindow, glfwSetWindowSizeCallback);
    glfwSetKeyCallback(pwindow, glfwKeyCallback);

    
    glfwMakeContextCurrent(pwindow); //������� ��������� ��� �������� ����
	if(!gladLoadGL()){
		std::cout << "Can't load GLAD!\n";
		return -1;
	}
	
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl; //���������� � ������ ������
	
	
	glClearColor(0, 1, 0, 1);
	
    /* ���� ��������� (���� ���� �� ������ ���� �������, ��� � �� �����!) */
    while (!glfwWindowShouldClose(pwindow))
    {
        /* ��������� */
        glClear(GL_COLOR_BUFFER_BIT);

        /* ��������� ��������� (������ ������ � �������� ������) � ��� ������� ������ ������� �������*/
        glfwSwapBuffers(pwindow);

        /* �� ��������� ���������� ��� ������ ��������� �������� */
        glfwPollEvents();
    }

    glfwTerminate(); //�� ��� ���������� (������������ ������)
    return 0;
}