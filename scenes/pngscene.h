#pragma once
#include "../scene.h"
#include "../texloader.h"
#include <cmath>


struct Sig{
    float freq = 0;
    float phase = 0;
    float out = 0;
};

void inc_sin(Sig* osc, float dt){
    osc->phase += dt*tau*ifps;

    if(osc->phase >= tau)
        osc->phase -= tau;

    osc->out = (sinf(osc->phase)+1)*0.5;
}

void inc_tri(Sig* osc, float dt){
    osc->phase += dt*ifps;
    if(osc->phase >= 1){ osc->phase -= 1;} 
    osc->out = fabs(osc->phase - 0.5)*2;
}

void inc_tri_int(Sig* osc, float dt, int* _int, int max){
	osc->phase += dt*ifps;
    if(osc->phase >= 1){ 
    	osc->phase -= 1; 
    	*_int = (*_int+1)%max;
    }
    if(osc->phase <= 0.5){ 
    	osc->out = osc->phase*2;
    }else{
    	osc->out = (1.0 - osc->phase)*2;
    }  
}

void inc_ramp(Sig* osc, float dt){
	osc->phase += dt*ifps;
	if(osc->phase >= 1){ osc->phase -= 1;}
	osc->out = osc->phase;
}

void inc_ramp_int(Sig* osc, float dt, int* _int, int max){
	osc->phase += dt*ifps;
	if(osc->phase >= 1){
		osc->phase -= 1;
		*_int = (*_int+1)%max;
	}
	osc->out = osc->phase;	
}


class PngScene : public Scene{
public:

	Shader* texShader;
	TextureLoader* texLoader;
	const char* pngdir;
	int usig;
	int uidx, idx = 0;
	Sig sig;

   // rectangle strip rect
    float vertices[16] = {
        //vertex     texture
        -0.8f, -0.8f,  0.0f, 1.0f,
        -0.8f,  0.8f,  0.0f, 0.0f,
         0.8f, -0.8f,  1.0f, 1.0f,
         0.8f,  0.8f,  1.0f, 0.0f
    };

	PngScene(int w, int h, const char* v, const char* f, const char* dir): Scene(w, h, v, f), pngdir(dir){
		setup();
	} 



	~PngScene(){
		delete texShader, texLoader;
	}

	void setup(const char* arg = NULL){

	    glGenVertexArrays(1, &VAO);
	    glGenBuffers(1, &VBO);

	    glBindVertexArray(VAO);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	    texShader = new Shader(vsrc, fsrc);

	    // position attribute
	    int posAttrib = glGetAttribLocation(texShader->program, "aPos");
	    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,  4* sizeof(float), (void*)0);
	    glEnableVertexAttribArray(posAttrib);

	    // texcoord attribute
	    int texAttrib = glGetAttribLocation(texShader->program, "aTexCoord");
	    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,  4* sizeof(float), (void*)(2* sizeof(float)));
	    glEnableVertexAttribArray(texAttrib);

	    glBindBuffer(GL_ARRAY_BUFFER, 0); 
	    // glEnable(GL_DEPTH_TEST);
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if(texShader->valid()){
       		texShader->use();
    	}else{ std::cerr << "shader not valid: " << vsrc << " " << fsrc << "\n"; }

    	texLoader = new TextureLoader(W, H, texShader->program);
    	texLoader->loadPngDirArray(pngdir, "mysampler");
    	
    	usig = texShader->makeUniform("mixsig");
    	uidx = texShader->makeUniform("idx");
	}


	void render(){

		glBindVertexArray(VAO);
		texShader->use();

        // inc_sin(&sig, 0.2);
		inc_ramp_int(&sig , 0.5, &idx, 5);
        texShader->setUniform(uidx, &idx, Shader::UNIFORM_INT);
   		texShader->setUniform(usig, &sig.out, 0);  

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	}	

};