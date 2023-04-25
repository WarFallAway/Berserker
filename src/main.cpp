#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
extern "C"
_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;

GLfloat point[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
}; //координаты точек

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
}; //массив для цветов точек

//далее нужно написать vertex шейдеры позиций и и цветов на GLSL

const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec color"
"void main() {"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0f);"
"}";

//фрагментный шейдер

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0f);"
"}";

/*теперь эти шейдеры нужно скомпилировать и передать видеокарте,
так как мы задали глобальные переменные, мы их передадим в наш мейн
где они будут уже обрабатываться функциями встроенными */


int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwSetWindowSizeCallback(GLFWwindow* pWindow, int Width, int Height) {
    g_windowSizeX = Width;
    g_windowSizeY = Height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY); //отрисовка от начала координат (левый нижний угол)
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(void)
{
    

    /* Инициализация джеэльфэвэ */
    if (!glfwInit()) {
        std::cout << "glfw can't initialize\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); //указание версии опенжеэль
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //подмножество функций опенжеэля
    
    GLFWwindow* pwindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Berserker", nullptr, nullptr); //создание окон96
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
	
	
	glClearColor(0, 1, 0, 1);
	
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr); //показали где брать код шейдеров для предыдущей переменной
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    GLuint shader_program = glCreateProgram(); //возвращает идентификатор программы
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLineWidth(shader_program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    /* цикл отрисовки (пока окно не должно быть закрыто, оно и не будет!) */
    while (!glfwWindowShouldClose(pwindow))
    {
        /* Рендеринг */
        glClear(GL_COLOR_BUFFER_BIT);

        /* буфферная отрисовка (задний буффер и передний буффер) а эта команда меняет буффера местами*/
        glfwSwapBuffers(pwindow);

        /* Мы позволяем обработать все ивенты следующей командой */
        glfwPollEvents();
    }

    glfwTerminate(); //ну это деструктор (освобождение памяти)
    return 0;
}