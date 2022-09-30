#ifndef _GFX_GL_TEXTURES_HPP_
#define _GFX_GL_TEXTURES_HPP_

#include "loader.hpp"

namespace gfx::gl {

	inline constexpr auto max_texture_units = GL_MAX_TEXTURE_UNITS;

	enum class texture_target : GLenum {
		_1d 		     = GL_TEXTURE_1D,
		_2d 		     = GL_TEXTURE_2D,
		_3d 	    	     = GL_TEXTURE_3D,
		array_1d 	     = GL_TEXTURE_1D_ARRAY,
		array_2d 	     = GL_TEXTURE_2D_ARRAY,
		rectangle 	     = GL_TEXTURE_RECTANGLE,
		cube_map 	     = GL_TEXTURE_CUBE_MAP,
		array_cube_map 	     = GL_TEXTURE_CUBE_MAP_ARRAY,
		buffer 		     = GL_TEXTURE_BUFFER,
		multisample_2d 	     = GL_TEXTURE_2D_MULTISAMPLE,
		multisample_array_2d = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
	};

	enum class texture_wrapping : GLint {
		repeat 		= GL_REPEAT,
		mirrored_repeat = GL_MIRRORED_REPEAT,
		clamp_to_edge   = GL_CLAMP_TO_EDGE,
		clamp_to_border = GL_CLAMP_TO_BORDER
	};

	enum class texture_filtering : GLint {
		linear  = GL_LINEAR,
		nearest = GL_NEAREST
	};

	enum class texture_mipmap_filtering : GLint {
		nearest_on_nearest = GL_NEAREST_MIPMAP_NEAREST,
		linear_on_nearest  = GL_LINEAR_MIPMAP_NEAREST,
		nearest_on_linear  = GL_NEAREST_MIPMAP_LINEAR,
		linear_on_linear   = GL_LINEAR_MIPMAP_LINEAR,
	};

	enum class image_format : GLenum {
		red 		= GL_RED,
		red_int 	= GL_RED_INTEGER,
		rg 		= GL_RG,
		rg_int 		= GL_RG_INTEGER,
		rgb 		= GL_RGB,
		rgb_int 	= GL_RGB_INTEGER,
		rgba 		= GL_RGBA,
		rgba_int 	= GL_RGBA_INTEGER,
		depth_component = GL_DEPTH_COMPONENT,
		depth_stencil 	= GL_DEPTH_STENCIL,
		alpha 		= GL_ALPHA,
		luminance 	= GL_LUMINANCE,
		luminance_alpha = GL_LUMINANCE_ALPHA
	};

	enum class image_type : GLenum {
		byte = GL_BYTE,
		unsigned_byte = GL_UNSIGNED_BYTE,
		_short = GL_SHORT,
		unsigned_short = GL_UNSIGNED_SHORT,
		_int = GL_INT,
		unsigned_int = GL_UNSIGNED_INT,
		half_float = GL_HALF_FLOAT,
		_float = GL_FLOAT
	};

	inline void texture2d(GLsizei width, GLsizei height, image_format format, image_type type, GLvoid const * data, image_format internal_format) noexcept {
		glTexImage2D(GL_TEXTURE_2D, 0, // base-level (mipmaps are generated below)
			static_cast<GLint>(internal_format), width, height, 0, // border (legacy, always 0)
			static_cast<GLenum>(format), static_cast<GLenum>(type), data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	class texture {
	public:
		explicit texture(texture_target target) noexcept
		: target_{static_cast<GLenum>(target)} { glCreateTextures(target_, 1, &texture_); }

		texture(texture const &) = delete;
		~texture() { glDeleteTextures(1, &texture_); }
		void bind() const noexcept { glBindTexture(target_, texture_); }
		void wrap_horizontal(texture_wrapping wrapping) noexcept {
			glTextureParameteri(texture_, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapping));
		}

		void wrap_vertical(texture_wrapping wrapping) noexcept {
			glTextureParameteri(texture_, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapping));
		}

		void border_color(float r, float g, float b, float a = 1) noexcept {
			GLfloat color[]{r, g, b, a};
			glTextureParameterfv(texture_, GL_TEXTURE_BORDER_COLOR, color);
		}

		void mag_filter(texture_filtering filter) noexcept {
			glTextureParameteri(texture_, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
		}

		void min_filter(texture_filtering filter) noexcept {
			glTextureParameteri(texture_, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
		}

		void min_filter(texture_mipmap_filtering filter) noexcept {
			glTextureParameteri(texture_, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
		}

	private:
		GLenum target_;
		GLuint texture_;
	};

	inline void bind_texture_to(texture const & texture, unsigned char texture_unit) noexcept {
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		texture.bind();
	}

}

#endif // _GFX_GL_TEXTURES_HPP_

