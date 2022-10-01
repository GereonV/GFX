#include <cmath>
#include <iostream>
#include "gfx.hpp"

// auto [w, h] = window.size();
// gfx::gl::set_viewport(0, 0, w, h);
// gfx::gl::set_polygon_mode(gfx::gl::polygon_mode::line);

inline constexpr auto out_string_length = 50;

int main() try {
	gfx::context ctx{"Test Program", 720, 720};
	ctx.clear_to(.4f, .6f, 1, 1);
	gfx::sprite wall{gfx::image{"textures/wall.jpg"}};
	gfx::sprite smiley{gfx::image{"textures/awesomeface.png"}};
	std::string str;
	str.reserve(out_string_length);
	auto last_time = static_cast<float>(ctx.time());
	while(ctx.update([&]() {
		auto time = static_cast<float>(ctx.time());
		auto x = std::sin(time) / 2 - .5f;
		wall.draw_at(x, -.5f, 1, 1);
		smiley.draw_at(-.25f, -.25f, .5f, .5f, x + .5f);
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

