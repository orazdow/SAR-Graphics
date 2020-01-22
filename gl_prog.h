#ifndef GL_PROG
#define GL_PROG

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

#define gl_err(str) glfw_err(0, (str));

void glfw_err(int error, const char* description);
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCB(GLFWwindow* window, int key, int scancode, int action, int mods);

GLFWwindow* glfw_init_window(int w, int h, const char* str);

class GlProg{
public:
	GLFWwindow* window;
	int w, h;

	GlProg(int _w, int _h, const char* str = NULL){
		w = _w; h = _h;
		this->window = glfw_init_window(w, h, str);
	}
	~GlProg(){
		glfwTerminate();
	}
};

// glfw init
GLFWwindow* glfw_init_window(int w, int h, const char* str = NULL){

	glfwSetErrorCallback(glfw_err);

	if(!glfwInit()){
    	glfw_err(GLFW_NOT_INITIALIZED, "no init");
    	return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

    // glfwWindowHint(GLFW_RESIZABLE, true);
    // glfwWindowHint(GLFW_SAMPLES, 4);  // MSAA

	GLFWwindow* window = glfwCreateWindow(w, h, str ? str : "", NULL, NULL);

	if(!window){
		gl_err("failed to create window");
	    glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
	    gl_err("failed initialize glad");
	    return NULL;
	} 

	return window;
}


void keyCB(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, 1);
	}
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void glfw_err(int error, const char* description){
    fprintf(stderr, "GFLW error: %s\n", description);
}

#endif