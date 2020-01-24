#include "gl_prog.h"
#include "duffing.h"
#include "pngscene.h"

#define ww 800
#define wh 600

const float pi = acos(-1);
const float tau = 2*pi;
const float fps = 50;
const float ifps = 1/fps;
const int framep = ceil(1000000/fps);

int main(){

    GLFWwindow* window = glfw_init_window(ww, wh);

    PngScene pngscene(ww, wh, "shaders/test.vs", "shaders/test.fs", "resources");
    Duffing duffing(ww, wh, "shaders/duffpoints.vs", "shaders/duffpoints.fs", 200);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 

    while(!glfwWindowShouldClose(window)){

        time_point t1 = sysclock::now();

        glfwPollEvents();  
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        
        duffing.render();
        pngscene.render();

        time_point t2 = sysclock::now();
        long t = duration_ms(t2 - t1).count();

        long diff = framep - t;
        if(diff > 0){
            std::this_thread::sleep_for(microseconds(diff));
        }
        
        glfwSwapBuffers(window);

    }

    glfwTerminate();
    
    return 0;
    
}