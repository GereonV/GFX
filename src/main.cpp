#include <iostream>
#include "gfx.hpp"

// auto [w, h] = window.size();
// gfx::gl::set_viewport(0, 0, w, h);
// gfx::gl::set_polygon_mode(gfx::gl::polygon_mode::line);

int main() try {
	gfx::gl::window window{std::make_shared<gfx::gl::creator>(4, 2),
		"Test Program", 1080, 720};
	window.make_current();
	gfx::gl::load();
	gfx::gl::depth_testing(true);
	gfx::gl::set_background_color(.4f, .6f, 1, 1);
	gfx::gl::clearer{true, true}();
	gfx::sprite{gfx::image{"textures/wall.jpg"}}.draw_at(-.5f, -.5f, 1, 1);
	window.swap_buffers();
	while(!window.should_close())
		window.owner().poll();
} catch(std::exception const & e) {
	std::cerr << e.what() << '\n';
	return -1;
}

