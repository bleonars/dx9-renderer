#pragma once

#define CUSTOM_VERTEX_TYPE ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

using Vec4_t = Vector4;
using Vec3_t = Vector3;
using Vec2_t = Vector2;

using font_id_t = size_t;

namespace {

    // is primitve type a list?
    NOINLINE bool is_toplogy_list( D3DPRIMITIVETYPE topology );

    // primitive type grouping order
    NOINLINE int get_topology_order( D3DPRIMITIVETYPE topology );

}

class Color {
public:
    uint8_t a, r, g, b;

    // ctor(s)
    FORCEINLINE Color() : a{}, r{}, g{}, b{}{

    }
    
    FORCEINLINE Color( uint8_t a, uint8_t r, uint8_t g, uint8_t b ) : a{ a }, r{ r }, g{ g }, b{ b } {

    }

    FORCEINLINE uint32_t get() {
        return ( uint32_t ) ( ( ( a & 0xff ) << 24 ) | ( ( r & 0xff ) << 16 ) | ( ( g & 0xff ) << 8 ) | ( b & 0xff ) );
    }
};

class ColorRect {
public:
    Color m_top_left, m_top_right, m_bottom_left, m_bottom_right;

    // ctor(s)
    FORCEINLINE ColorRect() : m_top_left{}, m_top_right{}, m_bottom_left{}, m_bottom_right{} {

    }

    FORCEINLINE ColorRect( Color top_left, Color top_right, Color bottom_left, Color bottom_right ) : m_top_left{ top_left }, m_top_right{ top_right }, 
        m_bottom_left{ bottom_left }, m_bottom_right{ bottom_right } {

    }

    // returns true if all colors aren't invisible.
    FORCEINLINE bool valid() const {
        return m_top_left.a && m_top_right.a && m_bottom_left.a && m_bottom_right.a;
    }
};

struct Vertex_t {
    Vec4_t   m_pos;           // x, y, z, rhw
    uint32_t m_color;         // diffuse
    Vec2_t   m_texture_coord; // texture position

    // ctor(s)
    FORCEINLINE Vertex_t() : m_pos{}, m_color{}, m_texture_coord{} {

    }

    FORCEINLINE Vertex_t( Vec4_t pos, Color color, Vec2_t tex ) : m_pos{ pos }, m_color{ color.get() }, m_texture_coord{ tex } {

    }

    FORCEINLINE Vertex_t( Vec2_t pos, Color color, Vec2_t tex ) : m_pos{ pos.x, pos.y, 1.f, 1.f }, m_color{ color.get() }, m_texture_coord{ tex } {

    }

    FORCEINLINE Vertex_t( Vec4_t pos, Color color ) : m_pos{ pos }, m_color{ color.get() }, m_texture_coord{} {

    }

    FORCEINLINE Vertex_t( Vec3_t pos, Color color ) : m_pos{ pos.x, pos.y, pos.z, 1.f }, m_color{ color.get() } {

    }

    FORCEINLINE Vertex_t( Vec2_t pos, Color color ) : m_pos{ pos.x, pos.y, 1.f, 1.f }, m_color{ color.get() } {

    }
};

struct Batch_t {
    size_t            m_count;
    D3DPRIMITIVETYPE  m_topology;
    IDirect3DTexture9 *m_texture;

    // ctor(s)
    FORCEINLINE Batch_t( D3DPRIMITIVETYPE topology, IDirect3DTexture9 *texture, size_t count = 0 ) : m_count{ count }, m_topology{ topology }, m_texture{ texture } {

    }
};

class RenderList {
public:
    std::vector< Vertex_t > m_vertices;
    std::vector< Batch_t >  m_batches;

    // ctor(s)
    FORCEINLINE RenderList() : m_vertices{}, m_batches{} {
        
    }

    // clear draw lists
    FORCEINLINE void clear() {
        m_vertices.clear();
        m_batches.clear();
    }
};

//
// Stores information about glyph in a bitmap font
//
struct GlyphData_t {
    FT_ULong m_charcode;          // glyph character code
    FT_UInt  m_glyph_index;       // glyph index in face

    // https://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html
    Vec2_t m_size;
    Vec2_t m_bearing;      
    size_t m_advance; 

    bool m_colored;               // glyph colored?

    IDirect3DTexture9 *m_texture; // d3d9 glyph texture

    // ctor(s))
    FORCEINLINE GlyphData_t() : m_charcode{}, m_glyph_index{}, m_size{}, m_bearing{}, m_advance{}, m_colored{}, m_texture{} {

    }

    FORCEINLINE bool valid() const {
        return ( m_texture != nullptr );
    }
};

//
// Font renderer implementation
//
class Font {
public:
    IDirect3DDevice9 *m_device;

    FT_Library  m_ft_library; // freetype library
    FT_Face     m_ft_face;    // freetype font face

    std::string m_name;       // ttf font name
    size_t      m_size;       // font render size
    bool        m_anti_alias; // font render anti-aliasing
    uint32_t    m_ft_flags;   // font load flags

    using glyphmap_t = std::unordered_map< FT_ULong, GlyphData_t >;

    glyphmap_t m_glyphs; // glyph info

    enum FontRenderFlags : uint32_t {
        NONE = 0,
        ALIGN_CENTER_X = ( 1 << 0 ),
        ALIGN_CENTER_Y = ( 1 << 1 ),
        ALIGN_CENTER = ( 1 << 2 ),
        ALIGN_LEFT = ( 1 << 3 ),
        ALIGN_RIGHT = ( 1 << 4 )
    };

    // ctor(s)
    FORCEINLINE Font() : m_device{}, m_ft_library {}, m_ft_face{}, m_size{}, m_anti_alias{}, m_ft_flags{}, m_glyphs{} {

    }

    // dtor
    FORCEINLINE ~Font() {
        m_device = nullptr;

        FT_Done_FreeType( m_ft_library );
        FT_Done_Face( m_ft_face );      
    }

    FORCEINLINE void store( IDirect3DDevice9 *device, const std::string &name, size_t size, bool anti_alias ) {
        m_device     = device;
        m_name       = name.substr( name.rfind( "\\" ) + 1, name.rfind( ".ttf" ) );
        m_size       = size;
        m_anti_alias = anti_alias;
    }

    // initialize font
    NOINLINE bool init( IDirect3DDevice9 *device, const std::string &ttf_font, size_t size, bool anti_alias );

    // get size of glyphs for given text string
    NOINLINE Vec2_t get_text_size( const std::string &str ) const;

    // get const reference to glyph map
    FORCEINLINE const glyphmap_t &get_glyphs() const {
        return m_glyphs;
    }
};

using font_ptr_t = std::unique_ptr< Font >;

//
// Direct3D 9 renderer implementation
//
class Renderer {
private:
    IDirect3DDevice9          *m_device;             // current d3d device
    IDirect3DVertexBuffer9    *m_vertex_buffer;      // buffer for storing verticies
    IDirect3DStateBlock9      *m_render_state_block; // current render state
    RenderList                m_render_list;         // render list
    size_t                    m_max_vertices;        // max amount of verticies we can draw
    size_t                    m_width, m_height;     // width and height of viewport

    // reacquire vertex buffer
    NOINLINE bool reacquire();

    // beging rendering 
    NOINLINE void begin();

    // flush the buffer
    NOINLINE void flush();

    // end rendering
    NOINLINE void end();

    // release buffer and render state
    NOINLINE void release();

public:
    using fonts_t = std::vector< font_ptr_t >;

    fonts_t m_fonts;

    // ctor(s)
    FORCEINLINE Renderer() : m_device{ nullptr }, m_vertex_buffer{ nullptr }, m_render_state_block{ nullptr }, m_render_list{}, m_max_vertices{}, m_width{}, m_height{}, m_fonts{} {
   
    }

    // dtor
    FORCEINLINE ~Renderer() {
        release();

        m_device    = nullptr;
        m_max_vertices = 0;
        m_width     = 0;
        m_height    = 0;
        
        for( auto &font : m_fonts ) {
            // release all textures
            for( auto &glyph: font->m_glyphs ) 
                Utils::safe_release( &glyph.second.m_texture );

            font.reset();
        }

        m_fonts.clear();
    }

    // initialize renderer
    NOINLINE bool init( IDirect3DDevice9 *device, size_t max_vertices );

    // render the buffer
    NOINLINE void render();

    // add verticies to draw
    NOINLINE void add_vertices( Vertex_t *vertex_array, size_t vertex_count, D3DPRIMITIVETYPE topology, IDirect3DTexture9 *texture = nullptr );

    // create ttf font 
    NOINLINE font_id_t create_font( const std::string &ttf_font, size_t size, bool anti_alias );

    // windows font path
    NOINLINE std::string get_font_path( const std::string &font_name );

    //
    // utility
    //
    FORCEINLINE const fonts_t &get_fonts() {
        return m_fonts;
    }

    //
    // drawing utility functions
    //
    // draw line between 2d vectors
    NOINLINE void draw_line( const Vec2_t &start, const Vec2_t &end, const Color color, float thickness = 1.f );

    // draw line between points
    NOINLINE void draw_line( float start_x, float start_y, float end_x, float end_y, const Color color, float thickness = 1.f );

    // draw filled rectangle from vector dimensions
    NOINLINE void draw_filled_rect( const Vec2_t &pos, const Vec2_t &size, const Color color );

    // draw filled rectangle from dimensions
    NOINLINE void draw_filled_rect( float x, float y, float w, float h, const Color color );

    // draw filled rectangle from vector dimensions
    NOINLINE void draw_filled_gradient_rect( const Vec2_t &pos, const Vec2_t &size, const ColorRect &color_rect );

    // draw filled gradient rectangle from dimensions
    NOINLINE void draw_filled_gradient_rect( float x, float y, float w, float h, const ColorRect &color_rect );

    // draw rectangle from vector dimensions
    NOINLINE void draw_rect( const Vec2_t &pos, const Vec2_t &size, const Color color, float thickness = 1.f );

    // draw rectnagle from dimensions
    NOINLINE void draw_rect( float x, float y, float w, float h, const Color color, float thickness = 1.f );

    // draw outlined filled rectangle from vector dimensions
    NOINLINE void draw_outlined_filled_rect( const Vec2_t &pos, const Vec2_t &size, const Color fill_color, const Color outline_color );

    // draw outlined filled rectangle from dimensions
    NOINLINE void draw_outlined_filled_rect( float x, float y, float w, float h, const Color fill_color, const Color outline_color );

    // draw outlined rectangle from vector dimensions
    NOINLINE void draw_outlined_rect( const Vec2_t &pos, const Vec2_t &size, const Color inner_color, const Color outline_color );

    // draw outlined rectangle from dimensions
    NOINLINE void draw_outlined_rect( float x, float y, float w, float h, const Color inner_color, const Color outline_color );

    // draw cicrle from vector dimensions
    NOINLINE void draw_circle( const Vec2_t &pos, float radius, const Color color );

    // draw circle from dimensions
    NOINLINE void draw_circle( float x, float y, float radius, const Color color );

    // draw filled circle from vector dimensions
    NOINLINE void draw_filled_circle( const Vec2_t &pos, float radius, const Color color );

    // draw filled circle from dimensions
    NOINLINE void draw_filled_circle( float x, float y, float radius, const Color color );

    // draw texture quad from vector dimensions
    NOINLINE void draw_texture_quad( const Vec2_t &pos, const Vec2_t &size, const Color color, IDirect3DTexture9 *texture, const std::array< Vector2, 6 > & uv_coords );

    // draw texture quad from dimensions
    NOINLINE void draw_texture_quad( float x, float y, float w, float h, const Color color, IDirect3DTexture9 *texture, const std::array< Vector2, 6 > & uv_coords );

    //
    // text drawing functions
    //
    // draww text from vector dimensions
    NOINLINE void draw_text( font_id_t font_id, const std::string &str, const Vec2_t &pos, uint32_t flags, const Color color, float scale = 1.f );

    // draw text from dimensions
    NOINLINE void draw_text( font_id_t font_id, const std::string &str, float x, float y, uint32_t flags, const Color color, float scale = 1.f );
};

extern std::shared_ptr< Renderer > g_d3d9_renderer;