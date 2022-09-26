#include "buffers.hpp"
#include "opengl.hpp"

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

gfx::buffer_object::buffer_object() noexcept { glGenBuffers(2, &vbo_); }
gfx::buffer_object::~buffer_object() { glDeleteBuffers(2, &vbo_); }
void gfx::buffer_object::bind() const noexcept {
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
}

void gfx::buffer_object::buffer_vertices(gfx::buffer vertices, gfx::data_store_usage usage) noexcept {
	glNamedBufferData(vbo_, vertices.size, vertices.data, static_cast<GLenum>(usage));
}

void gfx::buffer_object::buffer_indices(gfx::buffer indices, gfx::data_store_usage usage) noexcept {
	glNamedBufferData(ebo_, indices.size, indices.data, static_cast<GLenum>(usage));
}

void gfx::buffer_object::sub_vertices(gfx::buffer vertices, long from) noexcept {
	glNamedBufferSubData(vbo_, from, vertices.size, vertices.data);
}

void gfx::buffer_object::sub_indices(gfx::buffer indices, long from) noexcept {
	glNamedBufferSubData(ebo_, from, indices.size, indices.data);
}

void gfx::vertex_attrib_pointer::set(int size, gfx::data_type type, bool normalized, unsigned int stride, unsigned int offset) noexcept {
	glVertexAttribPointer(index_, size, static_cast<GLenum>(type), normalized, stride, reinterpret_cast<void *>(offset));
}

void gfx::vertex_attrib_pointer::enable() const noexcept {
	glEnableVertexAttribArray(index_);
}

void gfx::vertex_attrib_pointer::disable() const noexcept {
	glDisableVertexAttribArray(index_);
}

