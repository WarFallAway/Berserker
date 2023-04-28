#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Sprite.h"


extern "C"
_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;

GLfloat point[] = {
    0.0f, 50.f, 0.0f,
    50.f, -50.f, 0.0f,
    -50.f, -50.f, 0.0f
}; //координаты точек

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
}; //массив для РГБ координат цветов

GLfloat texCoord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
}; //массив для координат текстуры (левый нижний угол модели (0, 0) и по аналогии в нормированной системе координит


glm::ivec2 g_windowSize(640, 400);


void glfwSetWindowSizeCallback(GLFWwindow* pWindow, int Width, int Height) {
    g_windowSize.x = Width;
    g_windowSize.y = Height;
    glViewport(0, 0, Width, Height); //отрисовка от начала координат (левый нижний угол)
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char* argv[])
{


    /* Инициализация джеэльфэвэ */
    if (!glfwInit()) {
        std::cout << "glfw can't initialize\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //указание версии опенжеэль
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //подмножество функций опенжеэля
    
    GLFWwindow* pwindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Berserker", nullptr, nullptr); //создание окон96
    if (!pwindow)
    {
        std::cout << "OKNA96 can't to create((\n";
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pwindow, glfwSetWindowSizeCallback);
    glfwSetKeyCallback(pwindow, glfwKeyCallback);

    
    glfwMakeContextCurrent(pwindow); //контекс опенжеэля для текущего окна
	if(!gladLoadGL()){
		std::cout << "Can't load GLAD!\n";
		return -1;
	}
	
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl; //информация о версии опенгл
	
	
	glClearColor(1, 1, 0, 1);
	
    
    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram)
        {
            std::cerr << "Can't create shader program: " << "SpriteShader" << std::endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/wall.jpg");
        auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTexture", "SpriteShader", 50, 100);
        pSprite->setPosition(glm::vec2(300, 100));

        /*std::string vertexShader; // (vertex_shader);
        std::string fragmentShader; // (fragment_shader);
        Renderer::ShaderProgram shaderProgram(vertexShader, fragmentShader);
        if (!shaderProgram.is_Compiled())
        {
            std::cerr << "Can't create shader program!" << std::endl;
        }*/

        //нужно передать теперь наши шейдеры в видеокарту
        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo); //гененерирует 1 виртекс буффер объект и записывает её по указателю
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo); //подключили буффер и сделали текущим
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW); //заполяем текущий буффер информацией (а текущим стал тот, что забиндили только что
        //далее все тоже самое но для цветов
        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        //теперь у нас есть два виртексных буффера в памяти видеокарты
        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

        //далее укажем видеокарте че делать с этими шейдерами и буфферами
        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        //включаем слои для позиций и цветов, ну то есть оперируем с буфферами
        glEnableVertexAttribArray(0); //включили нулевую позицию
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1); //включили первую позицию
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2); //включили нулевую позицию
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);


        glm::mat4 modelMatrix_1 = glm::mat4(1.f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));
        glm::mat4 modelMatrix_2 = glm::mat4(1.f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));
        
        
        glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(g_windowSize.x), 0.0f, static_cast<float>(g_windowSize.y), -100.f, 100.f);

        pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        //с шейдерами закончили далее необходима отрисовка, она будет производиться в цикле

        /* цикл отрисовки (пока окно не должно быть закрыто, оно и не будет!) */
        while (!glfwWindowShouldClose(pwindow))
        {
            /* Рендеринг */
            glClear(GL_COLOR_BUFFER_BIT);
            //подключим шейдер, который хотим использовать для рисования
            pDefaultShaderProgram->use();
            //подключаем то, что хотим отрисовать
            glBindVertexArray(vao);

            tex->bind();

            //сама команда отрисовки
            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pSprite->render();

            /* буфферная отрисовка (задний буффер и передний буффер) а эта команда меняет буффера местами*/
            glfwSwapBuffers(pwindow);

            /* Мы позволяем обработать все ивенты следующей командой */
            glfwPollEvents();
        }
    }
    glfwTerminate(); //ну это деструктор (освобождение памяти)
    return 0;
}