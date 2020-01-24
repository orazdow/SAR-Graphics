#pragma once
#include "scene.h"
#include <cmath>


class Duffing : public Scene{
public:

	Duffing(int w, int h, const char* v, const char* f, int n): Scene(w, h, v, f), num(n){
		setup();
	} 

	double dx, dy, dz;
	double dt = 0.006;
	double x = 0.1, y = 0.1, z = 0.1, t = 0;
	double a = 0.1, b = 0.35, c = 1.4;
	double px, py;
	double section = 0.1;

	const double ctau = tau/c;
	static const int POINTS_MAX = 2048;
	const int maxsteps = 500000;
	int num = 256;

	float points[POINTS_MAX*2];
	Shader* pointShader;
	unsigned int pos = 0;
	float pointSize = 18;

	~Duffing(){
		delete pointShader;
	}

	void setup(const char* arg = NULL){

	    glGenVertexArrays(1, &VAO);
	    glGenBuffers(1, &VBO);

	    glBindVertexArray(VAO);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	    // fill VBO
	    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);
	    // position attribute
	    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	    glEnableVertexAttribArray(0);

	    // glEnable(GL_DEPTH_TEST);
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	    glPointSize(18.0);

	    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 

	    pointShader = new Shader(vsrc, fsrc);
        if(pointShader->valid()){
       		pointShader->use();
    	}else{ std::cerr << "shader not valid: " << vsrc << " " << fsrc << "\n"; }
	}


	void render(){

		glBindVertexArray(VAO);
		pointShader->use();

        int n = duffing(points, 0, POINTS_MAX*2, num, maxsteps);
        // printf("%u\n", n);

        glBufferSubData(GL_ARRAY_BUFFER, pos*2*sizeof(float), n*2*sizeof(float), points);

        glDrawArrays(GL_POINTS, pos, n);

        // pos = (pos+n)%num;
        rotate(section, 0.33);

	}	

	int duffing(float* arr, int pos, int arrlen, int drawlen, int max){
	    unsigned int n = 0;
	    // pos = 0;

	    for(int i = 0; i < max; i++){

	        dx = y;
	        dy = x - x*x*x - a*y + b*cos(c*t);

	        x += dx*dt;
	        y += dy*dt;
	        z += dz*dt;
	        t += dt;

	        px = x*0.5;
	        py = y*0.5;

	        if(fabs((c*t)-section) < 0.004){
	            if(pos >= drawlen-1 || pos*2 >= arrlen-2) break;
	            arr[pos*2] = px;
	            arr[pos*2+1] = py;
	            pos++;
	            n++;
	        }

	        if(c*t >= tau) 
	            t -= ctau;

	        if(n >= drawlen) break;
	    }

	    return n;

	}

	void rotate(double& n, double rate){
	    n += rate*tau*ifps;
	    if(n > tau) n -= tau;
	}

};