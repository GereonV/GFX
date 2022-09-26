#include "shaders.hpp"
#include "opengl.hpp"

gfx::shader::shader(GLenum type, char const * source) : shader_{glCreateShader(type)} {
	glShaderSource(shader_, 1, &source, 0); // 1 null-terminated string (source)
}

gfx::shader::~shader() { glDeleteShader(shader_); }

void gfx::shader::compile() {
	glCompileShader(shader_);
	GLint success;
	glGetShaderiv(shader_, GL_COMPILE_STATUS, &success);
	if(success)
		return;
	gfx::shader_error err;
	glGetShaderInfoLog(shader_, err.buf_size, nullptr, err.msg);
	throw err;
}

gfx::shader_program::shader_program(gfx::shader const & vertex, gfx::shader const & fragment) noexcept : program_{glCreateProgram()} {
	glAttachShader(prog_, vertex.shader_);
	glAttachShader(prog_, fragment.shader_);
}

gfx::shader_program::~shader_program() { glDeleteProgram(prog_); }

void gfx::shader_program::link() {
	glLinkProgram(program_);
	GLint success;
	glGetProgramiv(prog_, GL_LINK_STATUS, &success);
	if(success)
		return;
	gfx::shader_error err;
	glGetProgramInfoLog(prog_, err.buf_size, nullptr, err.msg);
	throw err;
}

void gfx::shader_program::use() const noexcept { glUseProgram(prog_); }

