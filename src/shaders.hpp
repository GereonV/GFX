#ifndef _GFX_SHADERS_HPP_
#define _GFX_SHADERS_HPP_

#include <exception>

namespace gfx {

	struct shader_error : std::exception {
		char const * what() const noexcept override { return msg; }
		static constexpr std::size_t buf_size{512};
		char msg[buf_size];
	};

	enum class shader_type : unsigned int {
		vertex = 0x8B31,
		fragment = 0x8B30,
		geometry = 0x8DD9,
		compute = 0x91B9,
		tess_control = 0x8E88,
		tess_evaluation = 0x8e87
	};

	class shader {
	friend class shader_program;
	public:
		shader(shader_type type, char const * source) noexcept;
		shader(shader const &) = delete;
		~shader();
		void compile();
	private:
		unsigned int shader_;
	};

	class shader_program {
	public:
		shader_program(shader const & vertex, shader const & fragment) noexcept;
		shader_program(shader_program const &) = delete;
		~shader_program();
		void link();
		void use() const noexcept;
	private:
		unsigned int program_;
	};

}

#endif // _GFX_SHADERS_HPP_

