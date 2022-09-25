#include "shaders.hpp"
#include "opengl.hpp"

class shader {
public:
	shader(GLenum type, GLchar const * source) : id_{glCreateShader(type)} {
		glShaderSource(id_, 1, &source, 0); // 1 null-terminated string (source)
		glCompileShader(id_);
		GLint success;
		glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
		if(success)
			return;
		glDeleteShader(id_);
		gfx::shader_error err;
		glGetShaderInfoLog(id_, err.buf_size, nullptr, err.msg);
		throw err;
	}

	~shader() { glDeleteShader(id_); }
	operator GLuint() const noexcept { return id_; }
private:
	GLuint id_;
};

gfx::shader_program::shader_program(char const * vertex, char const * fragment, char const * geometry) : prog_{glCreateProgram()} {
	shader vert{GL_VERTEX_SHADER, vertex}, frag{GL_FRAGMENT_SHADER, fragment};
	glAttachShader(prog_, vert);
	glAttachShader(prog_, frag);
	if(geometry) {
		shader geom{GL_GEOMETRY_SHADER, geometry};
		glAttachShader(prog_, geom);
		glLinkProgram(prog_); // not outside of if due to geom's lifetime
	} else {
		glLinkProgram(prog_);
	}
	GLint success;
	glGetProgramiv(prog_, GL_LINK_STATUS, &success);
	if(success)
		return;
	glDeleteProgram(prog_);
	gfx::shader_error err;
	glGetProgramInfoLog(prog_, err.buf_size, nullptr, err.msg);
	throw err;
}

gfx::shader_program::~shader_program() { glDeleteProgram(prog_); }

void gfx::shader_program::use() const noexcept { glUseProgram(prog_); }

