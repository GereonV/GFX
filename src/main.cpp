#include <iostream>
#include "gfx.hpp"

int main(int, char **) {
	try {
		gfx::gl::window window{std::make_shared<gfx::gl::creator>(3, 3), "My Window", 1080, 720};
		window.make_current();
		gfx::gl::load();
		// auto [w, h] = window.size();
		// glViewport(0, 0, w, h);
		glClearColor(.4, .6, 1, 1);
		while(!window.should_close()) {
			glClear(GL_COLOR_BUFFER_BIT);
			window.swap_buffers();
			window.owner().poll();
		}
	} catch(std::exception const & e) {
		std::cerr << e.what() << '\n';
		return -1;
	}

}

