#include "texture.h"
#ifdef CLIENT
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_mixer_ext.h>
#include <SDL2/SDL_opengl.h> // otherwise we want to use OpenGL
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif
#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>
#include "stb_image.h"

texture::texture(std::string path) {
    	glGenTextures(1, &id);
		glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, id);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// load and generate the texture
		int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);  
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
            std::cout << "Failed to load texture filename: " <<path << std::endl;
		}
		stbi_image_free(data);
		w = width;
		h = height;
        this->fliph=false;
        this->flipv=true;
        this->type="";
        this->path=path;
        std::cout << "sprite width: " << w << " sprite height: " << h << "\n";
        glBindTexture(GL_TEXTURE_2D,0);
}
texture::texture(unsigned int glID,int w, int h) {
    id = glID;
    this->w = w;
    this->h = h;
}
texture::~texture() {
    //glDeleteTextures(1, &this->id);
}
texture::texture(std::string path,bool type, bool fliph, bool flipv) {
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, id);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flipv);
    int pixtype = GL_RGBA;
    if(type) {
        pixtype = GL_RGB;
    }
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, pixtype, width, height, 0, pixtype, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    w = width;
    h = height;
    this->fliph=false;
    this->flipv=flipv;
    this->type=type;
    this->path=path;
    this->name = path.substr(path.find_last_of("/\\") + 1);
}
void texture::activate(int slot) {
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(GL_TEXTURE_2D, id);
}
