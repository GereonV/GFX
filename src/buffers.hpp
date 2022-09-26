#ifndef _GFX_BUFFERS_HPP_
#define _GFX_BUFFERS_HPP_

namespace gfx {

	enum class data_store_usage : unsigned int {
		stream_draw  = 0x88E0,
		stream_read  = 0x88E1,
		stream_copy  = 0x88E2,
		static_draw  = 0x88E4,
		static_read  = 0x88E5,
		static_copy  = 0x88E6,
		dynamic_draw = 0x88E8,
		dynamic_read = 0x88E9,
		dynamic_copy = 0x88EA
	};

	enum class data_type : unsigned int {
		byte 	       = 0x1400,
		unsigned_byte  = 0x1401,
		short_ 	       = 0x1402,
		unsigned_short = 0x1403,
		int_ 	       = 0x1404,
		unsigned_int   = 0x1405,
		half_float     = 0x140B,
		float_ 	       = 0x1406,
		double_        = 0x140A,
		fixed 	       = 0x140C
	};

	struct buffer {
		void const * data;
		long size;
	};

	class buffer_object {
	public:
		buffer_object() noexcept;
		buffer_object(buffer_object const &) = delete;
		~buffer_object();
		void bind() const noexcept;
		void buffer_vertices(buffer vertices, data_store_usage usage) noexcept;
		void  buffer_indices(buffer  indices, data_store_usage usage) noexcept;
		void sub_vertices(buffer vertices, long from = 0) noexcept;
		void  sub_indices(buffer  indices, long from = 0) noexcept;
	private:
		unsigned int vbo_, ebo_;
	};

	class vertex_attrib_pointer {
	public:
		constexpr vertex_attrib_pointer(unsigned int index) noexcept : index_{index} {}
		void set(int size, data_type type, bool normalized, unsigned int stride, unsigned int offset) noexcept;
		void enable() const noexcept;
		void disable() const noexcept;
	private:
		unsigned int index_;
	};

}

#endif // _GFX_BUFFERS_HPP_

