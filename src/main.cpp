#include <iostream>
#include "gfx.hpp"

inline constexpr auto out_string_length = 50;

int main() try {
	gfx::context ctx{"Test Program", 720, 720};
	ctx.clear_to(.4f, .6f, 1, .05f);
	gfx::sprite wall{gfx::image{"textures/wall.jpg"}};
	gfx::sprite smiley{gfx::image{"textures/awesomeface.png"}};
	std::string str;
	str.reserve(out_string_length);
	auto last_time = ctx.time();
	float transform[4][4];
	while(ctx.update([&](int width, int height) {
		auto time = ctx.time();
		auto sin = static_cast<float>(std::sin(time));
		auto s = gfx::screen_scaling(width, height);
		wall.draw(transform | gfx::identity | gfx::translate(sin / 2, 0) | gfx::scale(s));
		smiley.draw(transform | gfx::identity | gfx::scale(.5f) | gfx::rotate(sin * 3.1415926535f) | gfx::translate(0, 0, sin) | gfx::scale(s));
		auto delta = time - last_time;
		last_time = time;
		str = "Frame Time: " + std::to_string(delta) + "\t(" + std::to_string(1 / delta) + "fps)";
		str.resize(out_string_length, ' ');
		str[out_string_length - 1] = '\r';
		std::cout << str;
	}));
	std::cout << '\n';
} catch(std::exception const & e) {
	std::cerr << e.what() << '\n';
	return -1;
}

