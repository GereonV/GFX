#ifndef _GFX_GFX_HPP_
#define _GFX_GFX_HPP_

#include <stb_image.h>
#include "opengl/loader.hpp"
#include "opengl/buffers.hpp"
#include "opengl/shaders.hpp"
#include "opengl/textures.hpp"
#include "opengl/drawing.hpp"
#include "shadersrc.hpp"

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
		sprite(image const & img) noexcept : texture_{gl::texture_target::_2d} {
			unsigned char indices[] {
				0, 1, 2,
				0, 3, 2
			};
			vao_.bind();
			bo_.bind();
			bo_.vbo().buffer_data({nullptr, sizeof(vertices_)}, gl::data_store_usage::dynamic_draw);
			bo_.ebo().buffer_data(			   indices, gl::data_store_usage::static_draw);
			gl::vertex_attrib_pointer pos_ptr{0}, texcoord_ptr{1};
			vao_.enable_attrib_pointers(pos_ptr, texcoord_ptr);
			pos_ptr     .set(2, gl::data_type::_float, false, 4 * sizeof(float), 0);
			texcoord_ptr.set(2, gl::data_type::_float, false, 4 * sizeof(float), 2 * sizeof(float));
			// wrapping & filtering missing
			texture_.bind();
			gl::texture2d(img.width, img.height, img.format(), gl::image_type::unsigned_byte, img.data, gl::image_format::rgba);
		}

		void draw_at(float x, float y, float width, float height, float z = 0) const noexcept {
			vertices_[0] = vertices_[12] = x;
			vertices_[1] = vertices_[ 5] = y;
			vertices_[4] = vertices_[ 8] = x + width;
			vertices_[9] = vertices_[13] = y + height;
			bo_.vbo().sub_data(vertices_, 0);
			vao_.bind();
			gl::bind_texture_to(texture_, 0);
			use_program(z);
			gl::draw(gl::primitive::triangles, 6, gl::index_type::unsigned_byte, 0);
		}

	private:
		void use_program(float z) const {
			auto sprite_program = []() {
				gl::shader_program p;
				gl::shader vert{gl::shader_type::vertex  , SPRITE_VERT},
					   frag{gl::shader_type::fragment, SPRITE_FRAG};
				vert.compile();
				frag.compile();
				p.attach(vert, frag);
				p.link();
				return p;
			};
			static auto program = sprite_program();
			static auto uZ = program.uniform("uZ");
			program.use();
			gl::set_uniform_float(uZ, z);
		}

	private:
		static inline float vertices_[16] {
		// 	pos   texcoords
			0, 0, 0, 0, // lower left
			0, 0, 1, 0, // lower right
			0, 0, 1, 1, // top right
			0, 0, 0, 1  // top left
		};

		gl::vertex_array_object vao_;
		gl::buffer_object bo_;
		gl::texture texture_;
	};

}

#endif // _GFX_GFX_HPP_

