#ifndef _GFX_SPRITES_HPP_
#define _GFX_SPRITES_HPP_

#include <stb_image.h>
#include "gfx.hpp"
#include "opengl/buffers.hpp"
#include "opengl/shaders.hpp"
#include "opengl/textures.hpp"

namespace gfx {

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
		sprite(auto const & img, bool pixelated = false, bool mipmap = true) noexcept
		: texture_{gl::texture_target::_2d} {
			texture_.bind();
			gl::texture2d(img.width, img.height, img.format(), gl::image_type::unsigned_byte, img.data, gl::image_format::rgba);
			// no wrapping possible
			// defaults: mag = linear & min = nearest on linear
			if(pixelated)
				texture_.mag_filter(gl::texture_filtering::nearest);
			if(mipmap) {
				texture_.mipmap();
				texture_.min_filter(gl::texture_mipmap_filtering::linear_on_linear);
				return;
			}
			texture_.min_filter(gl::texture_filtering::nearest);
		}

		void use() const noexcept { texture_.bind(); }
	private:
		gl::texture texture_;
	};

	class sprite_renderer {
	public:
		sprite_renderer() noexcept {
			static constexpr float vertices[] {
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
		}

		void use() const noexcept {
			vao_.bind();
			program_.use();
			gl::set_active_texture_unit(0);
		}

		void set_transformation(matrix const & mat) const noexcept {
			gl::set_uniform_4_mats(0, 1, false, mat[0]);
		}

		void set_alpha_treshold(float thresh) const noexcept {
			gl::set_uniform_float(1, thresh);
		}

	private:
		gl::vertex_array_object vao_;
		gl::buffer_object bo_;
		gl::shader_program program_;
	};

	inline void draw_sprite() noexcept {
		gl::draw(gl::primitive::triangles, 6, gl::index_type::unsigned_byte, 0);
	}

}

#endif // _GFX_SPRITES_HPP_

