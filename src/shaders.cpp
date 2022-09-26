#include "shaders.hpp"
#include "opengl.hpp"

gfx::shader::shader(gfx::shader_type type, char const * source) noexcept
: shader_{glCreateShader(static_cast<GLenum>(type))} {
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

gfx::shader_program::shader_program() noexcept : program_{glCreateProgram()} {}
gfx::shader_program::~shader_program() { glDeleteProgram(program_); }
void gfx::shader_program::attach(shader const & shader) noexcept { glAttachShader(program_, shader.shader_); }

void gfx::shader_program::link() {
	glLinkProgram(program_);
	GLint success;
	glGetProgramiv(program_, GL_LINK_STATUS, &success);
	if(success)
		return;
	gfx::shader_error err;
	glGetProgramInfoLog(program_, err.buf_size, nullptr, err.msg);
	throw err;
}

void gfx::shader_program::use() const noexcept { glUseProgram(program_); }

