#ifndef _GFX_GL_SHADERS_HPP_
#define _GFX_GL_SHADERS_HPP_

namespace gfx::gl {

	struct shader_error : std::exception {
		char const * what() const noexcept override { return msg; }
		static constexpr std::size_t buf_size{512};
		char msg[buf_size];
	};

	enum class shader_type : unsigned int {
		vertex 	        = GL_VERTEX_SHADER,
		fragment        = GL_FRAGMENT_SHADER,
		geometry        = GL_GEOMETRY_SHADER,
		compute         = GL_COMPUTE_SHADER,
		tess_control 	= GL_TESS_CONTROL_SHADER,
		tess_evaluation = GL_TESS_EVALUATION_SHADER
	};

	class shader {
	friend class shader_program;
	public:
		shader(shader_type type, char const * source) noexcept
		: shader_{glCreateShader(static_cast<GLenum>(type))} {
			glShaderSource(shader_, 1, &source, 0);
		}

		shader(shader const &) = delete;
		~shader() { glDeleteShader(shader_); }
		void compile() {
			glCompileShader(shader_);
			GLint success;
			glGetShaderiv(shader_, GL_COMPILE_STATUS, &success);
			if(success)
				return;
			shader_error err;
			glGetShaderInfoLog(shader_, err.buf_size, nullptr, err.msg);
			throw err;
		}

	private:
		GLuint shader_;
	};

	class shader_program {
	public:
		shader_program() noexcept : program_{glCreateProgram()} {}
		shader_program(shader_program const &) = delete;
		~shader_program() { glDeleteProgram(program_); }
		void use() const noexcept { glUseProgram(program_); }
		void attach(shader const & shader) noexcept { glAttachShader(program_, shader.shader_); }
		void link() {
			glLinkProgram(program_);
			GLint success;
			glGetProgramiv(program_, GL_LINK_STATUS, &success);
			if(success)
				return;
			shader_error err;
			glGetProgramInfoLog(program_, err.buf_size, nullptr, err.msg);
			throw err;
		}

		auto uniform(char const * name) const {
			if(auto location = glGetUniformLocation(program_, name); location != -1)
				return location;
			throw error{"Invalid uniform name"};
		}

	private:
		GLuint program_;
	};

}

#endif // _GFX_GL_SHADERS_HPP_
