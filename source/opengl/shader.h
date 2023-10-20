#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <string>
#ifndef CLIENT

#endif

#ifndef CLIENT
class shader
#else
class shader
#endif
{
public:
	unsigned int id;
    std::string vert,frag,error;
    bool success;
	shader(std::string vertPath, std::string fragpath);
	void activate();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVector(const std::string& name, float* value) const;
	void setVec2(const std::string& name, float* value) const;
    void setVec3(const std::string& name, float* value) const;
	void setVec4(const std::string& name, float* value) const;
    void setMat3(const std::string& name, float* value) const;

};

#endif
