#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <cstdio>

class Shader{

public:

	unsigned int program = 0;

	enum UType {
        UNIFORM_FLOAT = 0,
        UNIFORM_VEC2,
        UNIFORM_VEC3,
        UNIFORM_VEC4,
        UNIFORM_INT,
        UNIFORM_IVEC2,
        UNIFORM_IVEC3,
        UNIFORM_IVEC4,
        UNIFORM_SAMPLER2D
	};

	Shader(const char* vPath, const char* fPath){
		int rtn = init(vPath, fPath);
		if(rtn) printf("shader compiled\n");
		else printf("shader not compiled\n");
	}

	~Shader(){
		glDeleteProgram(program);
	}

	void use(){ glUseProgram(program); }
	
	unsigned int valid(){
		return program;
	}

	unsigned int makeUniform(const char* name){
		return glGetUniformLocation(program, name);
	}

	// from raylib:
	void setUniform(int uniformLoc, void *value, int uniformType){
	    SetShaderValueV(uniformLoc, value, uniformType, 1);
	}
	
	void setUniformV(int uniformLoc, void *value, int uniformType, int count){
		SetShaderValueV(uniformLoc, value, uniformType, count);
	}

	void setTexUniform(const char* name, int texUnit){
		glUniform1i(glGetUniformLocation(program, name), texUnit);
	}

private:

	int init(const char* vPath, const char* fPath){

		if(!glfwGetCurrentContext()){
			printf("no glfw context\n");
			return 0;
		}
		if(!glad_glDrawArrays){
			printf("glad extensions not loaded\n");
			return 0;
		}

	    FILE* f;
	    f = fopen(vPath, "rb" );  
	    if(f == NULL){
	    	printf("error opening %s\n", vPath);
	    	return 0;
	    } 
	       
		fseek(f , 0 , SEEK_END);
		unsigned long len = ftell(f);
		rewind(f);

		char* vShaderSrc = (char*)calloc(len + 1, 1);

	    fread(vShaderSrc, 1, len, f);	

	    fclose(f);
	    freopen(fPath, "rb", f);
	    if(f == NULL){
	    	printf("error opening %s\n", fPath);
	    	return 0;
	    }

		fseek(f , 0 , SEEK_END);
		len = ftell(f);
		rewind(f);

		char* fShaderSrc = (char*)calloc(len + 1, 1);

		fread(fShaderSrc, 1, len, f);
		fclose(f);

		// printf("%s\n%s\n", vShaderSrc, fShaderSrc);

		int rtn = compile(vShaderSrc, fShaderSrc);

		free(vShaderSrc);
		free(fShaderSrc);

		return rtn; 
	}

	// from raylib:
	void SetShaderValueV(int uniformLoc, void *value, int uniformType, int count){

		switch (uniformType){
	        case UNIFORM_FLOAT: glUniform1fv(uniformLoc, count, (float *)value); break;
	        case UNIFORM_VEC2: glUniform2fv(uniformLoc, count, (float *)value); break;
	        case UNIFORM_VEC3: glUniform3fv(uniformLoc, count, (float *)value); break;
	        case UNIFORM_VEC4: glUniform4fv(uniformLoc, count, (float *)value); break;
	        case UNIFORM_INT: glUniform1iv(uniformLoc, count, (int *)value); break;
	        case UNIFORM_IVEC2: glUniform2iv(uniformLoc, count, (int *)value); break;
	        case UNIFORM_IVEC3: glUniform3iv(uniformLoc, count, (int *)value); break;
	        case UNIFORM_IVEC4: glUniform4iv(uniformLoc, count, (int *)value); break;
	        case UNIFORM_SAMPLER2D: glUniform1iv(uniformLoc, count, (int *)value); break;
	        default: printf("uniform enum type not recognized: %u\n", uniformType);
		}

	}

	int compile(const char* vSrc, const char* fSrc){

		int vertexShader, fragmentShader, success = 0;
		char infoLog[512];
		// compile vetex shader
	    vertexShader = glCreateShader(GL_VERTEX_SHADER);   
	    glShaderSource(vertexShader, 1, &vSrc, NULL);
	    glCompileShader(vertexShader);
	    
	    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	    if(!success)
	    {
	        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	        printf("vertex shader compilation failed:\n%s\n", infoLog);
	        return 0;
	    }
	    // compile fragment shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	    glShaderSource(fragmentShader, 1, &fSrc, NULL);
	    glCompileShader(fragmentShader);

	    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	    if(!success)
	    {
	        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			printf("fragment shader compilation failed:\n%s\n", infoLog);
			return 0;
	    }

	    // link program
        program = glCreateProgram();
	    glAttachShader(program, vertexShader);
	    glAttachShader(program, fragmentShader);
	    glLinkProgram(program);

	    // check for linking errors
	    glGetProgramiv(program, GL_LINK_STATUS, &success);
	    if(!success)
	    {
	        glGetProgramInfoLog(program, 512, NULL, infoLog);
			printf("shader program linking failed:\n%s\n", infoLog);
			return 0;
	    }

	    // delete shaders
	    glDeleteShader(vertexShader);
	    glDeleteShader(fragmentShader);

	    return 1;
	}

};

#endif