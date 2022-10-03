#ifndef _GFX_GFX_HPP_
#define _GFX_GFX_HPP_

#include <cmath>
#include <stb_image.h>
#include "opengl/loader.hpp"
#include "opengl/buffers.hpp"
#include "opengl/shaders.hpp"
#include "opengl/textures.hpp"
#include "opengl/drawing.hpp"
#include "shadersrc.hpp"

namespace gfx {

	using matrix = float [4][4];

	constexpr matrix & operator|(matrix & mat, auto f) noexcept {
		f(mat);
		return mat;
	}

	inline constexpr auto identity = [](matrix & mat) noexcept {
		mat[0][0] = mat[1][1] = mat[2][2] = mat[3][3] = 1;
			    mat[1][0] = mat[2][0] = mat[3][0] =
		mat[0][1] 	      = mat[2][1] = mat[3][1] =
		mat[0][2] = mat[1][2] 		  = mat[3][2] =
		mat[0][3] = mat[1][3] = mat[2][3]	      = 0;
	};

	constexpr auto scale(float x, float y) noexcept {
		return [=](matrix & mat) noexcept {
			auto column = [&](auto i) {
				mat[i][0] *= x;
				mat[i][1] *= y;
			};
			column(0);
			column(1);
			column(2);
			column(3);
		};
	}

	constexpr auto scale(float fac) noexcept { return scale(fac, fac); }

	constexpr auto rotate(float alpha) noexcept {
		return [=](matrix & mat) noexcept {
			auto column = [&mat, s = std::sin(alpha), c = std::cos(alpha)](auto i) {
				auto a = mat[i][0], b = mat[i][1];
				mat[i][0] = b * s + a * c;
				mat[i][1] = b * c - a * s;
			};
			column(0);
			column(1);
			column(2);
			column(3);
		};
	}

	constexpr auto translate(float x, float y, float z = 0) noexcept {
		return [=](matrix & mat) noexcept {
			auto column = [&](auto i) {
				mat[i][0] += mat[i][3] * x;
				mat[i][1] += mat[i][3] * y;
				mat[i][2] += mat[i][3] * z;
			};
			column(0);
			column(1);
			column(2);
			column(3);
		};
	}

	class context {
	public:
		context(char const * name)
		: window_{creator(), name} { init(); }

		context(char const * name, int width, int height)
		: window_{creator(), name, width, height} { init(); }

		void clear_to(float r, float g, float b, float a = 1) noexcept {
			gl::set_background_color(r, g, b, a);
		}

		void focus() { window_.make_current(); }
		auto time() const noexcept { return window_.owner().time(); }
		[[nodiscard]] bool update(auto f) {
			static gl::clearer clear{true, true};
			auto [w, h] = window_.size();
			gl::set_viewport(0, 0, w, h);
			clear();
			f(w, h);
			window_.swap_buffers();
			window_.owner().poll();
			return !window_.should_close();
		}

#ifdef _DEBUG
		void wireframe() const noexcept { gl::set_polygon_mode(gl::polygon_mode::line); }
		void normal() const noexcept { gl::set_polygon_mode(gl::polygon_mode::fill); }
#endif

	private:
		static std::shared_ptr<gl::creator const> creator() noexcept {
			if(!creator_.expired())
				return creator_.lock();
			auto ptr = std::make_shared<gl::creator>(4, 2);
			ptr->set_hint(gl::hint::translucent); // HUGE performance loss
			creator_ = ptr;
			return ptr;
		}

		void init() {
			window_.make_current();
			gl::disable_vsync();
			gl::load();
			gl::depth_testing(true);
			gl::blending(true);
			gl::set_blending(gl::blending_factor::src_alpha, // use alpha
				gl::blending_factor::inv_src_alpha); // fade other fragments
		}

	private:
		static inline constinit std::weak_ptr<gl::creator const> creator_{};
		gl::window window_;
	};

	class image {
	public:
		explicit image(char const * file) {
			stbi_set_flip_vertically_on_load(true);
			data = stbi_load(file, &width, &height, &channel_count, 0);
			if(!data)
				throw gl::error{"Error loading image"};
		}

		~image() { stbi_image_free(data); }
		constexpr gl::image_format format() const {
			switch(channel_count) {
			case 3:	 return gl::image_format::rgb;
			case 4:	 return gl::image_format::rgba;
			}
			throw gl::error{"Unsupported bytes per pixel"};
		}

	public:
		int width, height, channel_count;
		unsigned char * data;
	};

	class sprite {
	public:
		sprite(image const & img) noexcept 
		: texture_{gl::texture_target::_2d} {
			// no wrapping possible
			texture_.mag_filter(gl::texture_filtering::linear);
			texture_.min_filter(gl::texture_mipmap_filtering::linear_on_linear);
			texture_.bind();
			gl::texture2d(img.width, img.height, img.format(), gl::image_type::unsigned_byte, img.data, gl::image_format::rgba);
		}

		void prepare() const noexcept { gl::bind_texture_to(texture_, 0); }
	private:
		gl::texture texture_;
	};

	class sprite_renderer {
	public:
		sprite_renderer() noexcept {
			static constexpr float vertices[16] {
			// 	pos   texcoords
				-.5f, -.5f, 0, 0, // lower left
				 .5f, -.5f, 1, 0, // lower right
				 .5f,  .5f, 1, 1, // top right
				-.5f,  .5f, 0, 1  // top left
			};
			static constexpr unsigned char indices[] {
				0, 1, 2,
				0, 3, 2
			};
			vao_.bind();
			bo_.bind();
			bo_.vbo().buffer_data(vertices, gl::data_store_usage::static_draw);
			bo_.ebo().buffer_data( indices, gl::data_store_usage::static_draw);
			gl::vertex_attrib_pointer pos_ptr{0}, texcoord_ptr{1};
			vao_.enable_attrib_pointers(pos_ptr, texcoord_ptr);
			pos_ptr     .set(2, gl::data_type::_float, false, 4 * sizeof(float), 0);
			texcoord_ptr.set(2, gl::data_type::_float, false, 4 * sizeof(float), 2 * sizeof(float));
			gl::shader vert{gl::shader_type::vertex  , SPRITE_VERT},
				   frag{gl::shader_type::fragment, SPRITE_FRAG};
			vert.compile();
			frag.compile();
			program_.attach(vert, frag);
			program_.link();
			location_ = program_.uniform("uTransformation");
		}

		void prepare() const noexcept {
			vao_.bind();
			program_.use();
		}

		void use(matrix const & mat) const noexcept {
			gl::set_uniform_4_mats(location_, 1, false, mat[0]);
		}

	private:
		gl::vertex_array_object vao_;
		gl::buffer_object bo_;
		gl::shader_program program_;
		GLint location_;
	};

	void draw_sprite() noexcept {
		gl::draw(gl::primitive::triangles, 6, gl::index_type::unsigned_byte, 0);
	}

}

#endif // _GFX_GFX_HPP_

