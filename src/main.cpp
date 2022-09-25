#include <iostream>
#include "gfx.hpp"

int main(int, char **) {
	try {
		gfx::gl::window window{std::make_shared<gfx::gl::creator>(), "My Window", 1080, 720};
		window.make_current();
		gfx::gl::load();
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

