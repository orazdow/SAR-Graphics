#pragma once
#include "../scene.h"
#include <cmath>


class Duffing_iso : public Scene{
public:

	Duffing_iso(int w, int h, const char* v, const char* f, int n, int s = 0): Scene(w, h, v, f), num(n), stride(s){
		setup();
		if(num > POINTS_MAX) num = POINTS_MAX;
		if(!stride || stride > num) stride = num;
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
	int num;
	int stride;

	float points[POINTS_MAX*2];
	Shader* pointShader;
	unsigned int pos = 0;
	float pointSize = 18;

   // rectangle strip rect
    float vertices[16] = {
        //vertex     texture
        -0.8f, -0.8f,  0.0f, 1.0f,
        -0.8f,  0.8f,  0.0f, 0.0f,
         0.8f, -0.8f,  1.0f, 1.0f,
         0.8f,  0.8f,  1.0f, 0.0f
    };

    unsigned int upoints, npoints, ures, n = 0;

	~Duffing_iso(){
		delete pointShader;
	}

	void setup(const char* arg = NULL){

	    glGenVertexArrays(1, &VAO);
	    glGenBuffers(1, &VBO);

	    glBindVertexArray(VAO);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	    // fill VBO
	    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	    // position attribute
	    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	    glEnableVertexAttribArray(0);

	    // glEnable(GL_DEPTH_TEST);
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	    // glPointSize(18.0);

	    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 

	    pointShader = new Shader(vsrc, fsrc);

        if(pointShader->valid()){
       		pointShader->use();
    	}else{ std::cerr << "shader not valid: " << vsrc << " " << fsrc << "\n"; }

    	upoints = pointShader->makeUniform("points");
    	npoints = pointShader->makeUniform("num");
    	ures = pointShader->makeUniform("res");
    	float res[2] = {(float)W, (float)H};
    	pointShader->setUniform(ures, res, Shader::UNIFORM_VEC2);
	}


	void render(){

		glBindVertexArray(VAO);
		pointShader->use();


		n = poincare(points, pos, POINTS_MAX*2, stride, num, maxsteps);
        // printf("%u\n", n);

		// n = 1;

		// duffing(points, 0.04, 4);

        pointShader->setUniform(npoints, &num, Shader::UNIFORM_INT);
        pointShader->setUniformV(upoints, points, Shader::UNIFORM_VEC2, num);

        // glBufferSubData(GL_ARRAY_BUFFER, pos*2*sizeof(float), n*2*sizeof(float), points);

        // glDrawArrays(GL_POINTS, pos, n);

        pos = (pos+n)%(num);
        rotate(section, 0.22);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	}	

	int poincare(float* arr, int pos, int arrlen, int drawlen, int limit, int max){
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
	            if(pos >= limit || pos*2 >= arrlen-2) break;
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

	void duffing(float* arr, float dt, int n, int idx = 0){

		for(int i = 0; i < n; i++){

	        dx = y;
	        dy = x - x*x*x - a*y + b*cos(c*t);

	        x += dx*dt;
	        y += dy*dt;
	        z += dz*dt;
	        t += dt;

	        px = (0.5*x+1.1);
	        py = (0.5*y+1.0);

	        arr[idx] = px;
	        arr[idx+1] = py;

	        if(c*t >= tau) 
	            t -= ctau;
		}
		// printf("%f, %f\n", px, py);

	}

	void rotate(double& n, double rate){
	    n += rate*tau*ifps;
	    if(n > tau) n -= tau;
	}

};