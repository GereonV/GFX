#include <iostream>
#include "gfx.hpp"
#include "shadersrc.hpp"

static gfx::gl::window init() {
	gfx::gl::window window{std::make_shared<gfx::gl::creator>(3, 3), "My Window", 1080, 720};
	window.make_current();
	gfx::gl::load();
	// auto [w, h] = window.size();
	// glViewport(0, 0, w, h);
	return window;
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
			  0,  .5, 0
		};

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// vertex attribute 0 are 3 not normalized floats with nothing in-between starting at the very beginning
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		gfx::shader_program shaders{SHADER_VERT, SHADER_FRAG};

		//glClearColor(.4f, .6f, 1, 1);
		loop(window, [&]() {
			glClear(GL_COLOR_BUFFER_BIT);
			glBindVertexArray(vao);
			shaders.use();
			glDrawArrays(GL_TRIANGLES, 0, 3); // start at the beginning of buffer and draw 3 vertices
		});
	} catch(std::exception const & e) {
		std::cerr << e.what() << '\n';
		return -1;
	}

}

