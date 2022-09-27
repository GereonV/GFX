#include <cmath>
#include <iostream>
#include "opengl/opengl.hpp"
#include "shadersrc.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static gfx::gl::window init() {
	gfx::gl::window window{std::make_shared<gfx::gl::creator>(3, 3), "My Window", 1080, 720};
	window.make_current();
	gfx::gl::load();
	// auto [w, h] = window.size();
	// glViewport(0, 0, w, h);
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

class stb_image : public gfx::gl::image2d {
// using gfx::gl::image::width, gfx::gl::image::height, gfx::gl::image::format, gfx::gl::image::type, gfx::gl::image::data;
public:
	explicit stb_image(char const * file) {
		if(!(data = stbi_load(file, &width, &height, &channel_count, 0)))
			throw std::runtime_error{"Error loading image"};
		format = gfx::gl::image_format::rgb;
		type = gfx::gl::image_type::unsigned_byte;
	}

	stb_image(stb_image const &) = delete;
	~stb_image() { stbi_image_free(data); }
public:
	int channel_count;
};

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

		unsigned char indices[]{
			0, 1, 2,
			0, 3, 2
		};

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		gfx::gl::buffer_object bo;
		bo.bind();
		bo.vbo().buffer_data(vertices, gfx::gl::data_store_usage::static_draw);
		bo.ebo().buffer_data(indices, gfx::gl::data_store_usage::static_draw);
		
		gfx::gl::vertex_attrib_pointer ptr{0};
		ptr.set(3, gfx::gl::data_type::_float, false, 5 * sizeof(float), 0);
		ptr.enable();

		gfx::gl::vertex_attrib_pointer ptr2{1};
		ptr2.set(2, gfx::gl::data_type::_float, false, 5 * sizeof(float), 3);
		ptr2.enable();

		gfx::gl::shader_program shaders;
		setup_program(shaders);
		// auto uColor = shaders.uniform("uColor");

		gfx::gl::texture texture{gfx::gl::texture_target::_2d};
		texture.bind();
		texture.mag_filter(gfx::gl::texture_filtering::linear);
		texture.min_filter(gfx::gl::texture_mipmap_filtering::linear_on_nearest);

		stb_image img{"textures/container.jpg"};
		img.texture(gfx::gl::image_format::rgb);

		// glClearColor(.4f, .6f, 1, 1);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		loop(window, [&]() {
			glClear(GL_COLOR_BUFFER_BIT);
			glBindVertexArray(vao);
			shaders.use();
			texture.bind();
			// glUniform4f(uColor, 0, static_cast<float>(std::sin(window.owner().time())), 0, 1);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0); // draw 6 vertices with indicies specified by EBO's first unsigned chars
		});
	} catch(std::exception const & e) {
		std::cerr << e.what() << '\n';
		return -1;
	}

}

