#include <iostream>
#include "sprites.hpp"

int main() try {
	gfx::context ctx{"Test Program", 720, 720};
	ctx.clear_to(.4f, .6f, 1, .05f);
	gfx::sprite wall{gfx::image{"textures/wall.jpg"}};
	gfx::sprite smiley{gfx::image{"textures/awesomeface.png"}};
	gfx::sprite_renderer sprites;
	sprites.use();
	unsigned long count{};
	float transform[4][4];
	auto begin_time = ctx.time();
	while(ctx.update([&](auto width, auto height) {
		auto time = ctx.time();
		auto sin = static_cast<float>(std::sin(time));
		float s = static_cast<float>(height) / static_cast<float>(width);
		wall.use();
		sprites.set_transformation(transform |
			gfx::identity |
			gfx::translate(sin / 2, 0) |
			gfx::scale(s, 1));
		gfx::draw_sprite();
		smiley.use();
		// draw in other order for optimal results without using alpha testing at all
		sprites.set_alpha_treshold(.4f);
		sprites.set_transformation(transform |
			gfx::identity |
			gfx::scale(.5f) |
			gfx::rotate(sin * 3.1415926535f) |
			gfx::translate(0, 0, sin) |
			gfx::scale(s, 1));
		gfx::draw_sprite();
		sprites.set_alpha_treshold(0);
		sprites.set_transformation(transform |
			gfx::identity |
			gfx::translate(0, 0, -0.001f) |
			gfx::scale(s, 1));
		gfx::draw_sprite();
	})) ++count;
	std::cout << static_cast<double>(count) / (ctx.time() - begin_time) << '\n';
} catch(std::exception const & e) {
	std::cerr << e.what() << '\n';
	return -1;
}

