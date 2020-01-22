#include "gl_prog.h"
#include "duffing.h"
#include "pngscene.h"

#define ww 800
#define wh 600


int main(){

    GLFWwindow* window = glfw_init_window(ww, wh);

    PngScene pngscene(ww, wh, "shaders/test.vs", "shaders/test.fs", "resources");
    Duffing duffing(ww, wh, "shaders/duffpoints.vs", "shaders/duffpoints.fs", 180);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 

    while(!glfwWindowShouldClose(window)){

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        
        duffing.render();
        pngscene.render();

        glfwSwapBuffers(window);
        glfwPollEvents();  
    }

    glfwTerminate();
    
    return 0;
    
}