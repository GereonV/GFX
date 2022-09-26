#ifndef _GFX_GL_BUFFERS_HPP_
#define _GFX_GL_BUFFERS_HPP_

#include "loader.hpp"

/*
 * GL_ARRAY_BUFFER 		Vertex attributes
 * GL_ATOMIC_COUNTER_BUFFER 	Atomic counter storage
 * GL_COPY_READ_BUFFER 		Buffer copy source
 * GL_COPY_WRITE_BUFFER 	Buffer copy destination
 * GL_DISPATCH_INDIRECT_BUFFER 	Indirect compute dispatch commands
 * GL_DRAW_INDIRECT_BUFFER 	Indirect command arguments
 * GL_ELEMENT_ARRAY_BUFFER 	Vertex array indices
 * GL_PIXEL_PACK_BUFFER 	Pixel read target
 * GL_PIXEL_UNPACK_BUFFER 	Texture data source
 * GL_QUERY_BUFFER 		Query result buffer
 * GL_SHADER_STORAGE_BUFFER 	Read-write storage for shaders
 * GL_TEXTURE_BUFFER 		Texture data buffer
 * GL_TRANSFORM_FEEDBACK_BUFFER	Transform feedback buffer
 * GL_UNIFORM_BUFFER 		Uniform block storage
 */

namespace gfx::gl {

	enum class data_store_usage : GLenum {
		stream_draw  = GL_STREAM_DRAW,
		stream_read  = GL_STREAM_READ,
		stream_copy  = GL_STREAM_COPY,
		static_draw  = GL_STATIC_DRAW,
		static_read  = GL_STATIC_READ,
		static_copy  = GL_STATIC_COPY,
		dynamic_draw = GL_DYNAMIC_DRAW,
		dynamic_read = GL_DYNAMIC_READ,
		dynamic_copy = GL_DYNAMIC_COPY
	};

	enum class data_type : GLenum {
		byte 	       = GL_BYTE,
		unsigned_byte  = GL_UNSIGNED_BYTE,
		short_ 	       = GL_SHORT,
		unsigned_short = GL_UNSIGNED_SHORT,
		int_ 	       = GL_INT,
		unsigned_int   = GL_UNSIGNED_INT,
		half_float     = GL_HALF_FLOAT,
		float_ 	       = GL_FLOAT,
		double_        = GL_DOUBLE,
		fixed 	       = GL_FIXED
	};

	struct buffer {
		void const * data;
		long size;
	};

	class buffer_name {
	public:
		constexpr buffer_name(GLuint name) noexcept : bo_{name} {}
		void buffer_data(buffer data, data_store_usage usage) const noexcept {
			glNamedBufferData(bo_, data.size, data.data, static_cast<GLenum>(usage));
		}

		void sub_data(buffer data, GLintptr offset = 0) const noexcept {
			glNamedBufferSubData(bo_, offset, data.size, data.data);
		}

	private:
		GLuint bo_;
	};

	class buffer_object {
	public:
		buffer_object() noexcept { glGenBuffers(2, &vbo_); }
		buffer_object(buffer_object const &) = delete;
		~buffer_object() noexcept { glDeleteBuffers(2, &vbo_); }
		buffer_name vbo() noexcept { return vbo_; }
		buffer_name ebo() noexcept { return ebo_; }
		void bind() const noexcept {
			glBindBuffer(GL_ARRAY_BUFFER, vbo_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		}

	private:
		GLuint vbo_, ebo_;
	};

	class vertex_attrib_pointer {
	public:
		constexpr vertex_attrib_pointer(GLuint index) noexcept : index_{index} {}
		void enable() const noexcept { glEnableVertexAttribArray(index_); }
		void disable() const noexcept { glDisableVertexAttribArray(index_); }
		void set(GLint size, data_type type, GLboolean normalized, GLsizei stride, auto offset) noexcept {
			glVertexAttribPointer(index_, size, static_cast<GLenum>(type), normalized, stride, reinterpret_cast<void *>(offset));
		}

	private:
		GLuint index_;
	};

}

#endif // _GFX_GL_BUFFERS_HPP_