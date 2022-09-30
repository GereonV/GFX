#include <cmath>
#include <iostream>
#include <stb_image.h>
#include "opengl/opengl.hpp"
#include "shadersrc.hpp"

static gfx::gl::window init() {
	gfx::gl::window window{std::make_shared<gfx::gl::creator>(3, 3), "My Window", 1080, 720};
	window.make_current();
	gfx::gl::load();
	// auto [w, h] = window.size();
	// gfx::gl::set_viewport(0, 0, w, h);
	return window;
}

static void setup_program(gfx::gl::shader_program & program) {
	gfx::gl::shader vert{gfx::gl::shader_type::vertex  , SHADER_VERT};
	vert.compile();
	gfx::gl::shader frag{gfx::gl::shader_type::fragment, SHADER_FRAG};
	frag.compile();
	program.attach(vert);
	program.attach(frag);
	program.link();
}

static void image_to_texture(char const * file) {
	int width, height, channel_count;
	stbi_set_flip_vertically_on_load(true);
	auto data = stbi_load(file, &width, &height, &channel_count, 0);
	if(!data)
		throw std::runtime_error{"Error loading image"};
	gfx::gl::image_format format;
	switch(channel_count) {
	case 3:
		format = gfx::gl::image_format::rgb;
		break;
	case 4:
		format = gfx::gl::image_format::rgba;
		break;
	default:
		throw std::runtime_error{"File contains invalid amount of channels"};
	}
	gfx::gl::texture2d(width, height, format, gfx::gl::image_type::unsigned_byte, data, gfx::gl::image_format::rgb);
	stbi_image_free(data);
}

static void loop(gfx::gl::window & window, auto f) {
	while(!window.should_close()) {
		f();
		window.swap_buffers();
		window.owner().poll();
	}
}

int main(int, char **) {
	try {
		auto window = init();

		float vertices[]{
		//	positions    tex-coords
			-.5, -.5, 0, 0, 0,
			 .5, -.5, 0, 1, 0,
			 .5,  .5, 0, 1, 1,
			-.5,  .5, 0, 0, 1
		};

		float verts2[]{
			-.25, -.25, -1, 0, 0,
			 .25, -.25, -1, 1, 0,
			 .25,  .25, -1, 1, 1,
			-.25,  .25, -1, 0, 1
		};

		unsigned char indices[]{
			0, 1, 2,
			0, 3, 2
		};


		gfx::gl::vertex_array_object vao, vao2;
		vao.bind(), vao2.bind(); // no idea why, but needed for attrib_pointers
		gfx::gl::buffer_object bo, bo2;

		gfx::gl::vertex_attrib_pointer ptr{0}, ptr2{1};
		vao.enable_attrib_pointers(ptr, ptr2);
		vao2.enable_attrib_pointers(ptr, ptr2);

		vao.bind();
		bo.bind();
		bo.vbo().buffer_data(vertices, gfx::gl::data_store_usage::static_draw);
		bo.ebo().buffer_data(indices, gfx::gl::data_store_usage::static_draw);

		ptr.set(3, gfx::gl::data_type::_float, false, 5 * sizeof(float), 0);
		ptr2.set(2, gfx::gl::data_type::_float, false, 5 * sizeof(float), 3 * sizeof(float));

		vao2.bind();
		bo2.bind();
		bo2.vbo().buffer_data(verts2, gfx::gl::data_store_usage::static_draw);
		bo2.ebo().buffer_data(indices, gfx::gl::data_store_usage::static_draw);

		ptr.set(3, gfx::gl::data_type::_float, false, 5 * sizeof(float), 0);
		ptr2.set(2, gfx::gl::data_type::_float, false, 5 * sizeof(float), 3 * sizeof(float));

		gfx::gl::shader_program shaders;
		setup_program(shaders);
		// auto uColor = shaders.uniform("uColor");

		// texture.wrap_horizontal(gfx::gl::texture_wrapping::repeat);
		// texture.wrap_vertical(gfx::gl::texture_wrapping::repeat);
		gfx::gl::texture texture1{gfx::gl::texture_target::_2d};
		texture1.mag_filter(gfx::gl::texture_filtering::linear);
		texture1.min_filter(gfx::gl::texture_mipmap_filtering::linear_on_linear);
		gfx::gl::bind_texture_to(texture1, 0);
		image_to_texture("textures/container.jpg");
		gfx::gl::texture texture2{gfx::gl::texture_target::_2d};
		texture2.mag_filter(gfx::gl::texture_filtering::linear);
		texture2.min_filter(gfx::gl::texture_mipmap_filtering::linear_on_linear);
		gfx::gl::bind_texture_to(texture2, 1);
		image_to_texture("textures/awesomeface.png");

		// gfx::gl::set_background_color(.4f, .6f, 1, 1);
		// gfx::gl::set_polygon_mode(gfx::gl::polygon_mode::line);
		gfx::gl::depth_testing(true);
		gfx::gl::clearer clear{true, true};
		loop(window, [&]() {
			clear();
			shaders.use();
			gfx::gl::bind_texture_to(texture1, 0);
			gfx::gl::bind_texture_to(texture2, 1);
			// gfx::gl::set_uniform_4_floats(uColor, 0, static_cast<float>(std::sin(window.owner().time())), 0, 1);
			vao.bind();
			// draw 6 vertices with indicies specified by EBO's first unsigned chars
			gfx::gl::draw(gfx::gl::primitive::triangles, 6, gfx::gl::index_type::unsigned_byte, 0);
			vao2.bind();
			gfx::gl::draw(gfx::gl::primitive::triangles, 6, gfx::gl::index_type::unsigned_byte, 0);
		});
	} catch(std::exception const & e) {
		std::cerr << e.what() << '\n';
		return -1;
	}

}

