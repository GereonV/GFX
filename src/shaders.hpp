#ifndef _GFX_SHADERS_HPP_
#define _GFX_SHADERS_HPP_

#include <exception>

namespace gfx {

	struct shader_error : std::exception {
		char const * what() const noexcept override { return msg; }
		static constexpr std::size_t buf_size{512};
		char msg[buf_size];
	};

	class shader_program {
	public:
		shader_program(char const * vertex, char const * fragment, char const * geometry = nullptr);
		shader_program(shader_program const &) = delete;
		~shader_program();
		void use() const noexcept;
	private:
		unsigned int prog_;
	};

}

#endif // _GFX_SHADERS_HPP_

