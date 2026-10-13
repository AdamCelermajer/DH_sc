// gameswf_shape.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Quadratic bezier outline shapes, the basis for most SWF rendering.


#ifndef GAMESWF_SHAPE_H
#define GAMESWF_SHAPE_H


#include "gameswf/gameswf_styles.h"


namespace gameswf
{
	struct character;
	struct stream;
	struct shape_character_def;
	namespace tesselate_new {
		struct mesh_accepter;
	}
	namespace tesselate {
		struct trapezoid_accepter;
		struct tesselating_shape {
			virtual ~tesselating_shape() {}
			virtual void tesselate(float error_tolerance, 
					       trapezoid_accepter *accepter) const = 0;
			virtual void tesselate_new(float error_tolerance, 
					           gameswf::tesselate_new::mesh_accepter *accepter) const = 0;
		};
	}


	struct edge
	// Together with the previous anchor, defines a quadratic
	// curve segment.
	{
		edge();
		edge(float cx, float cy, float ax, float ay);
		void	tesselate_curve() const;
		void	tesselate_curve_new() const;
		bool	is_straight() const;
		
	//private:
		// *quadratic* bezier: point = p0 * t^2 + p1 * 2t(1-t) + p2 * (1-t)^2
		float	m_cx, m_cy;		// "control" point
		float	m_ax, m_ay;		// "anchor" point
	};


	struct path
	// A subset of a shape -- a series of edges sharing a single set
	// of styles.
	{
		path();
		path(float ax, float ay, int fill0, int fill1, int line);

		void	reset(float ax, float ay, int fill0, int fill1, int line);
		bool	is_empty() const;

		bool	point_test(float x, float y);

		// Push the path into the tesselator.
		void	tesselate() const;
		void	tesselate_new() const;

	//private:
		int	m_fill0, m_fill1, m_line;
		float	m_ax, m_ay;	// starting point
		array<edge>	m_edges;
		bool	m_new_shape;
	};

	struct mesh
	// For holding a pre-tesselated shape.
	{
		mesh();
		~mesh();  // DH2: 0x77bf20, user destructor (resizes the three arrays to 0)

		void	set_tri_strip(const point pts[], int count); // TODO remove
		void reserve_triangles(int expected_triangle_count);
		void add_triangle(const float pts[6]);  // DH2: binary add_triangle(float const*); the triangle list holds floats
		void set_triangles(const float* verts, int float_count, const unsigned short* indices, int index_count);  // DH2: mesh::set_triangles, called by tesselator_accepter::end_shape

		void	display(const base_fill_style& style, float ratio, render_handler::bitmap_blend_mode bm) const;

		void	output_cached_data(tu_file* out);
		void	input_cached_data(tu_file* in);
	private:
		array<coord_component>	m_triangle_strip;// TODO remove
		array<float> m_triangle_list;  // DH2: float coordinates (binary add_triangle and set_triangles take float const*)
		array<unsigned short> m_triangle_indices;  // DH2: +0x20, index list filled by set_triangles
	};


	struct line_strip
	// For holding a line-strip (i.e. polyline).
	{
		line_strip();
		line_strip(int style, const point coords[], int coord_count);

		void	display(const base_line_style& style, float ratio) const;

		int	get_style() const { return m_style; }
		void	output_cached_data(tu_file* out);
		void	input_cached_data(tu_file* in);
	private:
		int	m_style;
		array<coord_component>	m_coords;
	};


	struct mesh_set
	// A whole shape, divided into layers and tesselated to a
	// certain error tolerance.
	{
		mesh_set();
		mesh_set(const tesselate::tesselating_shape* sh,
			 float error_tolerance);
		~mesh_set();

		float	get_error_tolerance() const { return m_error_tolerance; }

		void display( const matrix& m, const cxform& cx, const array<fill_style>& fills,
			const array<line_style>& line_styles, render_handler::bitmap_blend_mode bm) const;
			// DH2: four-argument overload (0x7795ac). The binary mesh_set::display takes no blend mode.
			void display( const matrix& m, const cxform& cx, const array<fill_style>& fills,	// DH2
				const array<line_style>& line_styles) const;	// DH2

		void new_layer();
		void	set_tri_strip(int style, const point pts[], int count);
		void	add_line_strip(int style, const point coords[], int coord_count);

		mesh* get_mutable_mesh(int style);
		
		void	output_cached_data(tu_file* out);
		void	input_cached_data(tu_file* in);

	private:
		void expand_styles_to_include(int style);
		
		float	m_error_tolerance;
		struct layer {
			array<mesh*> m_meshes;  // one mesh per style.
			array<line_strip*> m_line_strips;

			~layer();  // delete m_meshes & m_line_strips
		};
		array<layer> m_layers;
	};

	// DH2: 0x789d98 (queued in vendor-gameswf, own batch). Tesselates sh into ms; mesh_set(sh, err) calls it.
	// sh is the shape_character_def that the tesselating_shape subobject belongs to.
	void execute_tesselate(mesh_set* ms, const shape_character_def* sh, float error_tolerance);  // DH2


	struct shape_character_def : public character_def, public tesselate::tesselating_shape
	// Represents the outline of one or more shapes, along with
	// information on fill and line styles.
	{
		shape_character_def(player* player);
		virtual ~shape_character_def();

		const 	shape_character_def & operator =( const shape_character_def & def );

		virtual void	display(character* inst);
		bool	point_test_local(float x, float y);

		void get_bound(rect* bound);
		const rect&	get_bound_local() const { return m_bound; }

		void	read(stream* in, int tag_type, bool with_style, movie_definition_sub* m);
		void	display( const matrix& mat, const cxform& cx, float pixel_scale, const array<fill_style>& fill_styles,
				const array<line_style>& line_styles, render_handler::bitmap_blend_mode bm) const;
			// DH2: five-argument overload (0x77a990). The binary display takes no blend mode.
		void	display( const matrix& mat, const cxform& cx, float pixel_scale, const array<fill_style>& fill_styles,	// DH2
				const array<line_style>& line_styles) const;	// DH2
			// DH2: 0x77be00, overrides character_def::create_character_instance (player pool, not new).
		virtual character*	create_character_instance(character* parent, int id);	// DH2

		virtual void	tesselate(float error_tolerance, tesselate::trapezoid_accepter* accepter) const;
		virtual void	tesselate_new(float error_tolerance, tesselate_new::mesh_accepter* accepter) const;

		void	compute_bound(rect* r) const;	// @@ what's the difference between this and get_bound?

		void	output_cached_data(tu_file* out, const cache_options& options);
		void	input_cached_data(tu_file* in);

		const array<fill_style>&	get_fill_styles() const { return m_fill_styles; }
		const array<line_style>&	get_line_styles() const { return m_line_styles; }
		const array<path>&	get_paths() const { return m_paths; }

		// morph uses this
		void	set_bound(const rect& r) { m_bound = r; /* should do some verifying */ }
		
		void	flush_cache();

	protected:
		friend struct morph2_character_def;
		friend struct canvas;

		// derived morph classes changes these
		array<fill_style>	m_fill_styles;
		array<line_style>	m_line_styles;
		array<path>	m_paths;

	private:
		void	sort_and_clean_meshes() const;
		
		rect	m_bound;

		// for DefineShape4
		rect	m_edge_bounds;
		bool	m_uses_nonscaling_strokes;
		bool	m_uses_scaling_strokes;

		// Cached pre-tesselated meshes.
		mutable array<mesh_set*>	m_cached_meshes;
	};

}	// end namespace gameswf


#endif // GAMESWF_SHAPE_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
