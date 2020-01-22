#pragma once
#include "shader.h"
#include <iostream>

class Scene{
public:
	unsigned int VAO, VBO, W, H;
	const char* vsrc;
	const char* fsrc;
	Scene(int w, int h, const char* v, const char* f):W(w), H(h), vsrc(v), fsrc(f){}
	virtual void setup(const char* arg = NULL){}
	virtual void render(){}
};
