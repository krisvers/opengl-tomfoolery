#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600
#define TITLE "app"

#define ERR_GLFW(v, err, str) { if (v) { fprintf(stderr, "Error: %s\n", str); glfwTerminate(); return err; } };
#define ERR_GLFW_WIN(v, err, str) { if (v) { fprintf(stderr, "Error: %s\n", str); glfwDestroyWindow(win); glfwTerminate(); return err; } };

static void error(int err, const char * desc) {
	fprintf(stderr, "%i: %s\n", err, desc);
}

static void * file_load(char * filename) {
	FILE * fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "file_load(): no such file %s\n", filename);
		abort();
	}

	fseek(fp, 0L, SEEK_END);
	long long unsigned int size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	char * file = malloc(size + 1);
	if (file == NULL) {
		fprintf(stderr, "file_load(): failed to allocate file %s buffer\n", filename);
		abort();
	}

	if (fread(file, size, 1, fp) != 1) {
		fprintf(stderr, "file_load(): error while reading file %s\n", filename);
		abort();
	}

	file[size] = '\0';

	return (void *) file;
}

float vertices[] = {
	0.0f, 0.5f,
	0.5f, -0.5f,
	-0.5f, -0.5f,
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

// shaders

	char * shader_vert_src = file_load("./vert.glsl");
	char * shader_frag_src = file_load("./frag.glsl");
	printf("%s\n", shader_vert_src);
	printf("%s\n", shader_frag_src);

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

	GLint position_attrib = glGetAttribLocation(shader_program, "position");
	glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position_attrib);

	while (!glfwWindowShouldClose(win)) {
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
