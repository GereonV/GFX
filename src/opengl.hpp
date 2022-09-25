#ifndef _GFX_CONTEXT_HPP_
#define _GFX_CONTEXT_HPP_

#include <exception>
#include <memory>
#include <string>
#include <utility>

namespace gfx::gl {

	class error : public std::exception {
	public:
		error(std::string && msg) noexcept : msg_{std::move(msg)} {}
		char const * what() const noexcept override { return msg_.data(); }
	private:
		std::string msg_;
	};

	inline void load();

}

#ifdef GLEW

#include <GL/glew.h>

inline void gfx::gl::load() {
	using namespace std::literals;
	if(auto err = glewInit(); err)
		throw error{"glewInit() failed ("s + reinterpret_cast<char const *>(glewGetErrorString(err)) + ')'};
}

#endif // GLEW

#ifdef GLFW

#include <GLFW/glfw3.h>

namespace gfx::gl {

	class creator {
	public:
		creator(int major, int minor) {
			if(!glfwInit())
				throw error{"glfwInit() failed"};
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true); // for Mac OS X
			glfwSetErrorCallback([](int code, char const * description) {
				throw error{"GLFW-Error (" + std::to_string(code) + "): " + description};
			});
		}

		creator(creator const &) = delete;
		~creator() { glfwTerminate(); }
		void poll() const noexcept { glfwPollEvents(); }
		void wait() const noexcept { glfwWaitEvents(); }
		void wait(double timeout) const noexcept { glfwWaitEventsTimeout(timeout); }
	};

	class window {
	public:
		window(std::shared_ptr<creator const> creator, char const * title, int width, int height)
		: ctx_{std::move(creator)}, win_{glfwCreateWindow(width, height, title, nullptr, nullptr)} {}

		window(window && other) noexcept
		: ctx_{std::move(other.ctx_)} { win_ = std::exchange(other.win_, nullptr); }

		window(creator const &) = delete;
		~window() { glfwDestroyWindow(win_); }
		void destroy() { glfwDestroyWindow(win_); win_ = nullptr; }
		auto & owner() const noexcept { return *ctx_; }
		void close() noexcept { glfwSetWindowShouldClose(win_, true); }
		bool should_close() const noexcept { return glfwWindowShouldClose(win_); }
		void make_current() { glfwMakeContextCurrent(win_); }
		bool current() const noexcept { return glfwGetCurrentContext() == win_; }
		void swap_buffers() { glfwSwapBuffers(win_); }
		std::pair<int, int> size() const {
			std::pair<int, int> dimensions;
			glfwGetFramebufferSize(win_, &dimensions.first, &dimensions.second);
			return dimensions;
		}

	private:
		std::shared_ptr<creator const> ctx_;
		GLFWwindow * win_;
	};

}

#endif // GLFW

#endif // _GFX_CONTEXT_HPP_

