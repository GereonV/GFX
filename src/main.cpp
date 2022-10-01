#include <cmath>
#include <iostream>
#include "gfx.hpp"

// auto [w, h] = window.size();
// gfx::gl::set_viewport(0, 0, w, h);
// gfx::gl::set_polygon_mode(gfx::gl::polygon_mode::line);

int main() try {
	gfx::gl::window window{std::make_shared<gfx::gl::creator>(4, 2),
		"Test Program", 720, 720};
	window.make_current();
	gfx::gl::load();
	gfx::gl::depth_testing(true);
	gfx::gl::blending(true);
	gfx::gl::set_blending(gfx::gl::blending_factor::src_alpha, // use alpha
		gfx::gl::blending_factor::inv_src_alpha); // fade other fragments
	gfx::gl::set_background_color(.4f, .6f, 1, 1);
	gfx::gl::clearer clear{true, true};
	gfx::sprite wall{gfx::image{"textures/wall.jpg"}};
	gfx::sprite smiley{gfx::image{"textures/awesomeface.png"}};
	while(!window.should_close()) {
		clear();
		auto x = static_cast<float>(std::sin(window.owner().time()) / 2 - .5f);
		wall.draw_at(x, -.5f, 1, 1);
		smiley.draw_at(-.25f, -.25f, .5f, .5f, x + .5f);
		window.swap_buffers();
		window.owner().poll();
	}
} catch(std::exception const & e) {
	std::cerr << e.what() << '\n';
	return -1;
}

