#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "soil.h"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <cstdio>
#include <stdint.h>

#ifndef _max
#define _max(a, b) (a > b ? a : b)
#endif

typedef uint32_t uint;

uint32_t swap4(uint32_t num){
    return ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000);
}

#ifdef OS_W32
	#include "windows.h"

	static void read_directory(const char* name, std::vector<std::string>& v)
	{
	    WIN32_FIND_DATA data;
	    std::string str(name);
	    str+="\\*";
	    HANDLE hFind = FindFirstFile(str.c_str(), &data); 

	    if ( hFind != INVALID_HANDLE_VALUE ) {
	        do {
	            v.push_back(data.cFileName);
	        } while (FindNextFile(hFind, &data));
	        FindClose(hFind);
	    }
	}
#else
	#include <sys/types.h>
	#include <dirent.h>

	static void read_directory(const char* name, std::vector<std::string>& v)
	{
	    DIR* dirp = opendir(name);
	    struct dirent * dp;
	    while ((dp = readdir(dirp)) != NULL) {
	        v.push_back(dp->d_name);
	    }
	    closedir(dirp);
	}
#endif

static void scan_png(const char* path, uint* size, uint* w, uint* h){
	FILE* f = fopen(path, "rb");
	if(f == NULL){ std::cout << "error opening: " << path << "\n"; return;}
    uint32_t width, height;

    fseek(f , 16 , SEEK_SET);
    fread((unsigned char *)&width, 1, 4, f);
    fread((unsigned char *)&height, 1, 4, f);
    width = swap4(width);
    height = swap4(height);
    *w = width; *h = height;
    // fseek(f, 0, SEEK_END); 
    // uint32_t s = ftell(f);
    // *size = s;
    fclose(f);
}

class TextureLoader{
public:

	uint tex[256];
	uint texunit = 0;
	uint W, H, maxW, maxH, program = 0;

	TextureLoader(uint targetW, uint tragetH, uint shaderProgram): W(targetW), H(tragetH), program(shaderProgram){
		maxW = W; maxH = H;
	}

	void loadPngDirArray(const char* dir, const char* uname){
		std::string resourcepath = dir;
		std::vector<std::string> pngs;
		read_directory(dir, pngs);

		pngs.erase(std::remove_if(
			pngs.begin(), 
			pngs.end(), 
			[](std::string s){return s.substr(_max((int)(s.size())-4, 0)) != ".png";}), 
			pngs.end());

		int n = pngs.size();

		std::string path;
		uint size, width, height;
		for(int i = 0; i < n; i++){
			path = (resourcepath + "/")+pngs[i];
			scan_png(path.c_str(), &size, &width, &height);
			maxW = _max(maxW, width);
			maxH = _max(maxH, height);
			std::cout << path << " w: " << width << " h: " << height << "\n";
		}

		glGenTextures(1, &tex[texunit]);
		unsigned char** imgdata = new unsigned char*[n];

		int r = loadAndBindArray(imgdata, pngs, resourcepath, n);
		if(r){
			glUniform1i(glGetUniformLocation(program, uname), texunit);
			texunit++;
		}

		delete[] imgdata;

		std::cout << "loaded " << r << " pngs\n";

	}

	int loadAndBindArray(unsigned char** imgdata, std::vector<std::string> pngs, std::string resoucePath, uint n){
		glActiveTexture(GL_TEXTURE0+texunit);
		glBindTexture(GL_TEXTURE_2D_ARRAY, tex[texunit]);
		std::string path;
    	int imgchannels = 0, imgw = maxW, imgh = maxH;
    	int index = 0;
    	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, imgw, imgh, n, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		for(int i = 0; i < n; i++){
			path = (resoucePath + "/")+pngs[i];
			imgchannels = 0, imgw = W, imgh = H;
			imgdata[index] = SOIL_load_image(path.c_str(), &imgw, &imgh, &imgchannels, SOIL_LOAD_RGBA);
			if(imgdata[index]){
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, imgw, imgh, 1, GL_RGBA, GL_UNSIGNED_BYTE, imgdata[index]);
		        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        		SOIL_free_image_data(imgdata[index]);
        		index++;
        		//std::cout << "loaded " << path << "\n";
			}else{
				std::cout << path << " not loaded\n";
			}
		}	
		std::cout << maxW <<", " << maxH << std::endl;	
		return index;
	}

};