#include <cmath>
#include <iostream>
#include "opengl/opengl.hpp"
#include "shadersrc.hpp"

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
			-.5, -.5, 0,
			 .5, -.5, 0,
			 .5,  .5, 0,
			-.5,  .5, 0,
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
		ptr.set(3, gfx::gl::data_type::float_, false, 3 * sizeof(float), 0);
		ptr.enable();

		gfx::gl::shader_program shaders;
		setup_program(shaders);
		auto uColor = shaders.uniform("uColor");

		// glClearColor(.4f, .6f, 1, 1);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		loop(window, [&]() {
			glClear(GL_COLOR_BUFFER_BIT);
			glBindVertexArray(vao);
			shaders.use();
			// glUniform4f(uColor, .4f, .6f, 1, 1);
			glUniform4f(uColor, 0, static_cast<float>(std::sin(window.owner().time())), 0, 1);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0); // draw 6 vertices with indicies specified by EBO's first unsigned chars
		});
	} catch(std::exception const & e) {
		std::cerr << e.what() << '\n';
		return -1;
	}

}

