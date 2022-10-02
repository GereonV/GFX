#include <iostream>
#include "gfx.hpp"

int main() try {
	gfx::context ctx{"Test Program", 720, 720};
	ctx.clear_to(.4f, .6f, 1, .05f);
	gfx::sprite wall{gfx::image{"textures/wall.jpg"}};
	gfx::sprite smiley{gfx::image{"textures/awesomeface.png"}};
	unsigned long count{};
	float transform[4][4];
	auto begin_time = ctx.time();
	while(ctx.update([&](auto width, auto height) {
		auto time = ctx.time();
		auto sin = static_cast<float>(std::sin(time));
		auto s = gfx::screen_scaling(width, height);
		wall.prepare();
		wall.draw(transform | gfx::identity | gfx::translate(sin / 2, 0) | gfx::scale(s));
		smiley.prepare();
		smiley.draw(transform | gfx::identity | gfx::scale(.5f) | gfx::rotate(sin * 3.1415926535f) | gfx::translate(0, 0, sin) | gfx::scale(s));
	})) ++count;
	std::cout << count / (ctx.time() - begin_time) << '\n';
} catch(std::exception const & e) {
	std::cerr << e.what() << '\n';
	return -1;
}

