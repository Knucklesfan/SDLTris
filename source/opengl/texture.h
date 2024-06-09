#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class texture
{
public:
	unsigned int id;
	int w;
	int h;
    bool fliph,flipv,color;
    std::string path;
    std::string name;
    std::string type;
    texture(unsigned int,int w, int h);
	texture() {}
	texture(std::string path);
    texture(std::string path,bool type, bool fliph, bool flipv);
    ~texture();
	void activate(int);
};

#endif
