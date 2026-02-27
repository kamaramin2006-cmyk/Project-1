#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
// Window dimensions
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

float angleDeg = 0.0f;//ÒÇæíÉ
bool rotateFan = false;//åÇÏ íÔÛá æíØİí ÇáãÑæÍÉ

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)// íÔÛá ÇáãÑæÍÉ
        rotateFan = true;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)//íØİí ÇáãÑæÍÉ
        rotateFan = false;
}

int main()
{
    // ÊåíÆÉ GLFW
    glfwInit();
    //ÊÚííä ÌãíÚ ÇáÎíÇÑÇÊ ÇáãØáæÈÉ á GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Create GLFWwindow object that we can use for GLFW's functions 
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fan Project", NULL, NULL);
    glfwMakeContextCurrent(window);
    // Úáì ÅÓÊÎÏÇã ÃÓáæÈ ÍÏíË áÇÓÊÑÏÇÏ ãÄÔÑÇÊ ÇáÏæÇá æÇáÇãÊÏÇÏÇÊ GLEW Şã ÈÊÚííä åĞÇ ÇáÎíÇÑ Åáì ÕÍ áßí íÊÚÑİ 
    glewExperimental = GL_TRUE;
    glewInit();
    //ÊÍÏíÏ ÇÈÚÇÏ ãäİĞ ÇáÚÑÖ
    glViewport(0, 0, WIDTH, HEIGHT);

    float vertices[] = { //ãÓÊØíá åæ ÔİÑÇÊ ÇáãÑæÍÉ
        -0.05f, 0.0f, 0.0f,
         0.05f, 0.0f, 0.0f,
         0.05f, 0.5f, 0.0f,
        -0.05f, 0.5f, 0.0f
    };

    unsigned int indices[] = { 0,1,2, 2,3,0 };//ÊÎÈÑ OpenGL ßíİ íÔßá ãËáËíä áÕäÚ ãÓÊØíá

	unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);//íÍİÙ ØÑíŞÉ ŞÑÇÁÉ ÇáÈíÇäÇÊ
    glGenBuffers(1, &VBO);//íÎÒä äŞÇØ ÇáÔßá İí GPU
    glGenBuffers(1, &EBO);//íÎÒä ÊÑÊíÈ ÇáÑÓã (ÇáãËáËÇÊ)

    glBindVertexArray(VAO);//íÍÏÏ ÅÚÏÇÏÇÊ ŞÑÇÁÉ ÈíÇäÇÊ ÇáÑÄæÓ ÇáÊí ÓíÓÊÎÏãåÇ OpenGL ÃËäÇÁ ÇáÑÓã

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//åäÇ äÑÓá äŞÇØ ÇáÔßá ãä CPU Åáì GPU.

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//åäÇ äÑÓá ÊÑÊíÈ ÇáÑÓã (ÇáãËáËÇÊ) ãä CPU Åáì GPU.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//ÊÎÈÑ OpenGL ßíİ íİÓÑ ÇáÈíÇäÇÊ İí VBO. İí åĞÇ ÇáãËÇá¡ ßá ÑÃÓ íÊßæä ãä 3 ÃÚÏÇÏ ÕÍíÍÉ ÊãËá ÅÍÏÇËíÇÊ x æ y æ z.
	glEnableVertexAttribArray(0);//ÊİÚíá ÎÇÕíÉ ŞÑÇÁÉ ÈíÇäÇÊ ÇáÑÄæÓ İí OpenGL

    const char* vs =
		"#version 330 core\n"//íÍÏÏ ÅÕÏÇÑ GLSL ÇáĞí äÓÊÎÏãå İí åĞÇ ÇáÔÇÏÑ
        "layout (location=0) in vec3 aPos;\n"//ÊÍÏÏ äÓÎÉ GLSL
        "uniform float angle;\n"
        "void main(){\n"
		"float r=radians(angle);\n"//ÊÍæíá ÇáÒÇæíÉ ãä ÏÑÌÇÊ Åáì ÑÇÏíÇä
        "mat2 rot=mat2(cos(r),-sin(r),sin(r),cos(r));\n"//äÈäí ãÕİæİÉ ÏæÑÇä
        "vec2 p=rot*aPos.xy;\n"//äÈäí ãÕİæİÉ ÏæÑÇä
        "gl_Position=vec4(p,0,1);\n"//ÇáäÊíÌÉ = äŞØÉ ÈÚÏ ÇáÏæÑÇä
        "}";

    const char* fs =
		"#version 330 core\n"//íÍÏÏ ÅÕÏÇÑ GLSL ÇáĞí äÓÊÎÏãå İí åĞÇ ÇáÔÇÏÑ
        "out vec4 FragColor;\n"//ŞíãÉ íÎÑÌåÇ ÇáÔíÏÑ¡ãÊÌå ãä 4 Şíã¡ÇÓã ãÊÛíÑ Çááæä ÇáäåÇÆí
        "void main(){FragColor=vec4(0.2,0.7,0.9,1);}";//íÚØí Çááæä ÇáäåÇÆí ááÔßá.

    unsigned int v = glCreateShader(GL_VERTEX_SHADER);//ÅäÔÇÁ ßÇÆä ÔíÏÑ ÎÇÕ ÈÍÓÇÈ ãæÇŞÚ ÇáäŞÇØ.
    glShaderSource(v, 1, &vs, NULL);//ÇáÔíÏÑ ÇáĞí ÃäÔÃäÇå¡ÚÏÏ ÇáäÕæÕ ÇáãÑÓáÉ¡äÕ ÇáÔíÏÑ¡áÇ íæÌÏ Øæá ãÍÏÏ
	glCompileShader(v);//ÊÑÌãÉ ÇáÔíÏÑ Åáì áÛÉ ÇáÂáÉ ÇáÊí íİåãåÇ GPU.

	unsigned int f = glCreateShader(GL_FRAGMENT_SHADER);//ÅäÔÇÁ ßÇÆä ÔíÏÑ ÎÇÕ ÈÍÓÇÈ áæä ßá äŞØÉ.
	glShaderSource(f, 1, &fs, NULL);//ÇáÔíÏÑ ÇáĞí ÃäÔÃäÇå¡ÚÏÏ ÇáäÕæÕ ÇáãÑÓáÉ¡äÕ ÇáÔíÏÑ¡áÇ íæÌÏ Øæá ãÍÏÏ
	glCompileShader(f);//ÊÑÌãÉ ÇáÔíÏÑ Åáì áÛÉ ÇáÂáÉ ÇáÊí íİåãåÇ GPU.

	unsigned int program = glCreateProgram();//ÅäÔÇÁ ÈÑäÇãÌ ÔíÏÑ áÑÈØ ÇáÔíÏÑÇÊ ãÚğÇ.
	glAttachShader(program, v);//ÅÑİÇŞ ÔíÏÑ ÇáãæÇŞÚ Åáì ÇáÈÑäÇãÌ
	glAttachShader(program, f);//ÅÑİÇŞ ÔíÏÑ ÇáÃáæÇä Åáì ÇáÈÑäÇãÌ
	glLinkProgram(program);//ÑÈØ ÇáÔíÏÑÇÊ ãÚğÇ İí ÈÑäÇãÌ æÇÍÏ íãßä ÇÓÊÎÏÇãå ááÑÓã.
    // ÇáÍáŞÉ ÇáÃÓÇÓíÉ
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        if (rotateFan) angleDeg += 1.0f;//ÅĞÇ ÇáãÑæÍÉ ÊÚãá  äÒíÏ ÇáÒÇæíÉ
        //ãÓÍ ãÎÒä ÇáÃáæÇä
        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);//ÊİÚíá ÈÑäÇãÌ ÇáÔíÏÑ ÇáĞí ÃäÔÃäÇå
		glBindVertexArray(VAO);//ÊİÚíá ÅÚÏÇÏÇÊ ŞÑÇÁÉ ÈíÇäÇÊ ÇáÑÄæÓ ÇáÊí ŞãäÇ ÈÊÍÏíÏåÇ İí VAO

		int angleLoc = glGetUniformLocation(program, "angle");//ÇáÍÕæá Úáì ãæŞÚ ÇáãÊÛíÑ ÇáãæÍÏ "angle" İí ÈÑäÇãÌ ÇáÔíÏÑ

        glUniform1f(angleLoc, angleDeg);//ÃÑÓá ŞíãÉ ÇáÒÇæíÉ ãä ÇáÈÑäÇãÌ Åáì ÇáÔíÏÑ.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//ÑÓã ÔİÑÉ ÇáãÑæÍÉ ÇáÃæáì

        glUniform1f(angleLoc, angleDeg + 120);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//ÑÓã ÇáÔİÑÉ ÇáËÇäíÉ ááãÑæÍÉ ãÚ ÇÎÊáÇİ ÇáÒÇæíÉ 360 ÷ 3 = 120 ÏÑÌÉ

        glUniform1f(angleLoc, angleDeg + 240);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//ÑÓã ÇáÔİÑÉ ÇáËÇáËÉ ááãÑæÍÉ ãÚ ÇÎÊáÇİ ÇáÒÇæíÉ
        //ÑÓã OpenGL
        glfwSwapBuffers(window);
        //ÇáÊÍŞŞ ããÇ ÅĞÇ Êã ÊäÔíØ Ãí ÃÍÏÇË ÇáÖİØ Úáì ãİÊÇÍ
        glfwPollEvents();
    }
}