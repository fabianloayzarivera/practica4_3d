
#include "shader.h"
#include <iostream>
#include "vertex.h"

ShaderPtr Shader::create(const std::string&vertex, const std::string& fragment) 
{
	std::shared_ptr<Shader> p(new Shader(vertex, fragment), [](Shader* p) {
		delete p;
	});
	if (p->program == 0) p = nullptr;
	return p;
}

Shader::Shader(std::string vertexCode, std::string fragmentCode) 
{
	int retCode;
	error = "";
	vertexShaderSource = vertexCode;
	fragmentShaderSource = fragmentCode;
	
	const char* cVertexShaderSource = vertexShaderSource.c_str();
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &cVertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		char errorLog[1024];
		glGetShaderInfoLog(vertexShader, sizeof(errorLog), nullptr, errorLog);
		glDeleteShader(vertexShader);
		//set error
		error = error + "vertex shader could not be compiled:" + errorLog;
	}

	// create fragment shader
	const char* cFragmentShaderSource = fragmentShaderSource.c_str();
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &cFragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		char errorLog[1024];
		glGetShaderInfoLog(fragmentShader, sizeof(errorLog), nullptr, errorLog);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		error = error + "fragment shader could not be compiled:" +  errorLog;
	}

	// create and link program
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glGetProgramiv(program, GL_LINK_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		char errorLog[1024];
		glGetProgramInfoLog(program, sizeof(errorLog), nullptr, errorLog);
		glDeleteProgram(program);
		error = error + "program could not be linked:" + errorLog;
	}

	// use program and get locations
	vPosLoc = glGetAttribLocation(program, "vpos");
	vColorLoc = glGetAttribLocation(program, "vcolor");
	vTexLoc = glGetAttribLocation(program, "vtex");
}

void Shader::setupAttribs() const 
{
	if (vPosLoc != -1) {
		glEnableVertexAttribArray(vPosLoc);
		glVertexAttribPointer(vPosLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, pos)));

	}
	if (vColorLoc != -1) {
		glEnableVertexAttribArray(vColorLoc);
		glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, color)));

	}

	if (vTexLoc != -1) {
		glEnableVertexAttribArray(vTexLoc);
		glVertexAttribPointer(vTexLoc, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, uv)));

	}
		
}

Shader::~Shader() 
{
	if(program) glDeleteProgram(program);
}