#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fileio.h>
#include <utils.h>

#define WIDTH 800
#define HEIGHT 600
#define TITLE "app"

#define ERR_GLFW(v, err, str) { if (v) { fprintf(stderr, "Error: %s\n", str); glfwTerminate(); return err; } };
#define ERR_GLFW_WIN(v, err, str) { if (v) { fprintf(stderr, "Error: %s\n", str); glfwDestroyWindow(win); glfwTerminate(); return err; } };

static void error(int err, const char * desc) {
	fprintf(stderr, "%i: %s\n", err, desc);
}

GLfloat vertices[] = {
	1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top right (yellow)
	1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom right (magenta)
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top left (cyan)
	-1.0f, -1.0f, 0.75f, 0.25f, 1.0f, 1.0f, 1.0f, // bottom left (white)
};

GLuint elements[] = {
	0, 1, 2,
	2, 3, 1
};

GLFWwindow * win = NULL;
int main(void) {
	printf("Hello, World!\n");

	if (glfwInit() == 0) {
		fprintf(stderr, "Error: GLFW initialization failure\n");
		abort();
	}

	glfwSetErrorCallback(error);

// window creation

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	win = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
	ERR_GLFW(win == NULL, -1, "failed to create window");

	glfwMakeContextCurrent(win);
	glfwSwapInterval(1);

// glew initialization

	glewExperimental = GL_TRUE;
	GLenum glew_err = glewInit();
	ERR_GLFW_WIN(glew_err != GLEW_OK, -2, glewGetErrorString(glew_err));

// vertex buffer and vertices

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

// elements buffer and elements

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

// textures

	GLuint tex;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	float border[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

	u8 image[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);

// shaders

	char * shader_vert_src = file_read_str("./vert.glsl");
	char * shader_frag_src = file_read_str("./frag.glsl");
	printf("vertex shader source code:\n%s\n[end of file]\n", shader_vert_src);
	printf("fragment shader source code:\n%s\n[end of file]\n", shader_frag_src);

	GLuint shader_vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader_vert, 1, (const GLchar * const *) &shader_vert_src, NULL);
	glCompileShader(shader_vert);

	GLuint shader_frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader_frag, 1, (const GLchar * const *) &shader_frag_src, NULL);
	glCompileShader(shader_frag);

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, shader_vert);
	glAttachShader(shader_program, shader_frag);

	glBindFragDataLocation(shader_program, 0, "out_color");
	glLinkProgram(shader_program);
	glUseProgram(shader_program);

	GLint pos_attr = glGetAttribLocation(shader_program, "position");
	glVertexAttribPointer(pos_attr, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(pos_attr);

	GLint col_attr = glGetAttribLocation(shader_program, "color");
	glVertexAttribPointer(col_attr, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *) (2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(col_attr);

	GLint uv_attr = glGetAttribLocation(shader_program, "uv");
	glVertexAttribPointer(uv_attr, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (5 * sizeof(float)));
	glEnableVertexAttribArray(uv_attr);

	while (!glfwWindowShouldClose(win)) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glDrawElements(GL_TRIANGLES, sizeof(elements) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(win);
		glfwPollEvents();

		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(win, GL_TRUE);
		}
	}

	glfwDestroyWindow(win);
	glfwTerminate();

	printf("Goodbye, World!\n");
	return 0;
}
