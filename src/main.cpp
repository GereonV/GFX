#include <cmath>
#include <iostream>
#include "gfx.hpp"

// auto [w, h] = window.size();
// gfx::gl::set_viewport(0, 0, w, h);
// gfx::gl::set_polygon_mode(gfx::gl::polygon_mode::line);

int main() try {
	gfx::context ctx{"Test Program", 720, 720};
	ctx.clear_to(.4f, .6f, 1, 1);
	gfx::sprite wall{gfx::image{"textures/wall.jpg"}};
	gfx::sprite smiley{gfx::image{"textures/awesomeface.png"}};
	while(ctx.update([&]() {
		auto x = static_cast<float>(std::sin(ctx.time()) / 2 - .5f);
		wall.draw_at(x, -.5f, 1, 1);
		smiley.draw_at(-.25f, -.25f, .5f, .5f, x + .5f);
	}));
} catch(std::exception const & e) {
	std::cerr << e.what() << '\n';
	return -1;
}

