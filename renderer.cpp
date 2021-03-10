#include "includes.h"

NOINLINE bool Renderer::init( IDirect3DDevice9 *device, size_t max_vertices ) {
    D3DVIEWPORT9 viewport;

    if( !device || !max_vertices )
        return false;

    // get viewport parameters
    if( device->GetViewport( &viewport ) < 0 )
        return false;

    // set local data
    m_device       = device;
    m_max_vertices = max_vertices;
    m_width        = viewport.Width;
    m_height       = viewport.Height;

    // create vertex buffer / etc.
    if( !reacquire() )
        return false;

    return true;
}

NOINLINE bool Renderer::reacquire() {
    release();

    // create vertex buffer
    if( m_device->CreateVertexBuffer( m_max_vertices * sizeof( Vertex_t ), ( D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY ), CUSTOM_VERTEX_TYPE, D3DPOOL_DEFAULT, &m_vertex_buffer, nullptr ) < 0 )
        return false;

    // retrive current state block
    if( m_device->CreateStateBlock( D3DSBT_ALL, &m_render_state_block ) < 0 )
        return false;

    return true;
}

NOINLINE void Renderer::begin() {
    D3DVIEWPORT9 viewport; 

    // capture old render state
    m_render_state_block->Capture();

    // select flexible vertex format and vertex buffer to display
    m_device->SetFVF( CUSTOM_VERTEX_TYPE );
    m_device->SetStreamSource( 0, m_vertex_buffer, 0, sizeof( Vertex_t ) );

    // setup viewport
    viewport = { 0, 0, m_width, m_height, 0.f, 1.f };

    // set viewport
    m_device->SetViewport( &viewport );

    // release shaders, reset texture stage
    m_device->SetVertexShader( nullptr );
    m_device->SetPixelShader( nullptr );
    m_device->SetTexture( 0, nullptr );

    // set desired render states
    m_device->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
    m_device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
    m_device->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
    m_device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    m_device->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
    m_device->SetRenderState( D3DRS_LASTPIXEL, TRUE );
    m_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    m_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    m_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    m_device->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS );
    m_device->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_ALWAYS );
    m_device->SetRenderState( D3DRS_DITHERENABLE, FALSE );
    m_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_device->SetRenderState( D3DRS_FOGENABLE, FALSE );
    m_device->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    m_device->SetRenderState( D3DRS_STENCILENABLE, FALSE );
    m_device->SetRenderState( D3DRS_CLIPPING, TRUE );
    m_device->SetRenderState( D3DRS_LIGHTING, FALSE );
    m_device->SetRenderState( D3DRS_AMBIENT, FALSE );
    m_device->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE );
    m_device->SetRenderState( D3DRS_CLIPPLANEENABLE, FALSE );
    m_device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );
    m_device->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
    m_device->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
    m_device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
    m_device->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
    m_device->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, FALSE );
    m_device->SetRenderState( D3DRS_SRGBWRITEENABLE, FALSE );
    m_device->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
    m_device->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
    m_device->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );
    m_device->SetRenderState( D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD );
    
    m_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
    m_device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_CURRENT );
    m_device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE );
    m_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
    m_device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    m_device->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
    m_device->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
    m_device->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE );
    m_device->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
    m_device->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

    m_device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
    m_device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
    m_device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    m_device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
}

NOINLINE void Renderer::flush() {
    void   *data;
    size_t order, primitive_count, batch_pos;

    // lock vertex buffer and copy our vertices over.
    if( m_vertex_buffer->Lock( 0, 0, &data, D3DLOCK_DISCARD ) < 0 )
        return;

    std::memcpy( data, m_render_list.m_vertices.data(), m_render_list.m_vertices.size() * sizeof( Vertex_t ) );

    m_vertex_buffer->Unlock();

    batch_pos = 0;

    // render batch
    for( const auto &b : m_render_list.m_batches ) {
        order = get_topology_order( b.m_topology );
        if( !b.m_count || !order )
            continue;

        primitive_count = b.m_count;

        // calculate new total prim count
        if( is_toplogy_list( b.m_topology ) )
            primitive_count /= order;
        else
            primitive_count -= ( order - 1 );

        // render
        m_device->SetTexture( 0, b.m_texture );
        m_device->DrawPrimitive( b.m_topology, batch_pos, primitive_count );

        batch_pos += b.m_count;
    }

    m_render_list.clear();
}

NOINLINE void Renderer::render() {
    size_t num_vertices;

    // dont render if list entry
    num_vertices = m_render_list.m_vertices.size();
    if( !num_vertices )
        return;

    // increase vertex buffer
    if( num_vertices > m_max_vertices ) {
        m_max_vertices = num_vertices;

        if( !reacquire() )
            return;
    }

    // render
    begin();
    flush();
    end();
}

NOINLINE void Renderer::end() {
    // apply old render state
    m_render_state_block->Apply();
}

NOINLINE void Renderer::release() {
    Utils::safe_release( &m_vertex_buffer );
    Utils::safe_release( &m_render_state_block );
}

NOINLINE void Renderer::add_vertices( Vertex_t *vertex_array, size_t vertex_count, D3DPRIMITIVETYPE topology, IDirect3DTexture9 *texture ) {
    auto vertices = &m_render_list.m_vertices;
    auto batches  = &m_render_list.m_batches;

    // add verticies to list
    for( size_t i = 0; i < vertex_count; ++i )
        vertices->push_back( vertex_array[ i ] );

    //  create new batch if needed
    if( batches->empty() 
      || batches->back().m_topology != topology 
      || batches->back().m_texture != texture )
        batches->push_back( { topology, texture } );

    batches->back().m_count += vertex_count;
}

NOINLINE font_id_t Renderer::create_font( const std::string &ttf_font, size_t size, bool anti_alias ) {
    font_ptr_t font = std::make_unique< Font >();

    if( !font->init( m_device, ttf_font, size, anti_alias ) )
        return 0;

    m_fonts.push_back( std::move( font ) );

    return m_fonts.size() - 1;
}

NOINLINE std::string Renderer::get_font_path( const std::string &font_name ) {
    HKEY        reg_key;
    char        name_buf[ MAX_PATH ];
    size_t      reg_index;
    ulong_t     ret_buf_size;
    std::string ttf_font_name;

    // open registry key
    RegOpenKeyEx( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &reg_key );

    // start itteration at first registry entry
    reg_index = 0;

    // iterate over registry key values
    while( true ) {
        // clear name buffer
        memset( name_buf, 0, MAX_PATH );
        
        ret_buf_size = MAX_PATH;

        // get name of regsitry value
        // layout:
        // Name | Type | Data
        // Arial (True Type) | REG_SZ | arial.ttf
        if( RegEnumValueA( reg_key, reg_index, name_buf, &ret_buf_size, 0, 0, 0, 0 ) != ERROR_SUCCESS )
            return {};

        // have we found the font we are looking for?
        // the name of the value will contain the font name 
        // ex. Arial (TrueType)
        if( std::string( name_buf ).find( font_name ) != std::string::npos ) {
            ret_buf_size = MAX_PATH;

            // get data stored at registry key entry
            // layout:
            // Name | Type | Data
            // Arial (True Type) | REG_SZ | arial.ttf
            RegQueryValueExA( reg_key, name_buf, 0, 0, ( LPBYTE ) name_buf, &ret_buf_size );

            break;
        }

        reg_index++;
    }

    // save font name
    ttf_font_name = name_buf;

    // clear name buffer
    memset( name_buf, 0, MAX_PATH );

    // get default folder name
    SHGetFolderPathA( 0, CSIDL_FONTS, 0, 0, name_buf );

    return std::string( name_buf ) + '\\' + ttf_font_name;
}

NOINLINE void Renderer::draw_line( const Vec2_t &start, const Vec2_t &end, const Color color, float thickness ) {
    Vec2_t   diff, norm, a, b, c, d;
    Vertex_t vertices[ 6 ];

    if( start == end || !color.a )
        return;

    // draw 1 pixel line
    if( thickness <= 1.f ) {
        vertices[ 0 ] = { start, color };
        vertices[ 1 ] = { end, color };

        add_vertices( vertices, 2, D3DPT_LINELIST );
    }

    // draw a line with some thickness
    // https://forum.libcinder.org/topic/smooth-thick-lines-using-geometry-shader
    else {
        // calculate the normal vector of this line.
        diff = end - start; 
        norm = Vec2_t( -diff.y, diff.x ).normalized();

        // calculate corners for quad vertices.
        a = start - norm * thickness;
        b = start + norm * thickness;
        c = end - norm * thickness;
        d = end + norm * thickness;

        vertices[ 0 ] = { a, color };
        vertices[ 1 ] = { b, color };
        vertices[ 2 ] = { d, color };

        vertices[ 3 ] = { c, color };
        vertices[ 4 ] = { d, color };
        vertices[ 5 ] = { a, color };

        add_vertices( vertices, 6, D3DPT_TRIANGLELIST );
    }
}

NOINLINE void Renderer::draw_line( float start_x, float start_y, float end_x, float end_y, const Color color, float thickness ) {
    draw_line( { start_x, start_y }, { end_x, end_y }, color, thickness );
}

NOINLINE void Renderer::draw_filled_rect( const Vec2_t &pos, const Vec2_t &size, const Color color ) { 
    Vertex_t vertices[ 6 ];

    if( !color.a )
        return;

    vertices[ 0 ] = { { pos.x,          pos.y }, color };
    vertices[ 1 ] = { { pos.x + size.x, pos.y }, color };
    vertices[ 2 ] = { { pos.x,          pos.y + size.y }, color };

    vertices[ 3 ] = { { pos.x + size.x, pos.y }, color };
    vertices[ 4 ] = { { pos.x + size.x, pos.y + size.y }, color };
    vertices[ 5 ] = { { pos.x,          pos.y + size.y }, color };

    add_vertices( vertices, 6, D3DPT_TRIANGLELIST );
}

NOINLINE void Renderer::draw_filled_rect( float x, float y, float w, float h, const Color color ) {
    draw_filled_rect( { x, y }, { w, h }, color );
}

NOINLINE void Renderer::draw_filled_gradient_rect( const Vec2_t &pos, const Vec2_t &size, const ColorRect &color_rect ) { //
    Vertex_t vertices[ 6 ];

    if( !color_rect.valid() )
        return;

    vertices[ 0 ] = { { pos.x,          pos.y }, color_rect.m_top_left };
    vertices[ 1 ] = { { pos.x + size.x, pos.y }, color_rect.m_top_right };
    vertices[ 2 ] = { { pos.x,          pos.y + size.y }, color_rect.m_bottom_left };

    vertices[ 3 ] = { { pos.x + size.x, pos.y }, color_rect.m_top_right };
    vertices[ 4 ] = { { pos.x + size.x, pos.y + size.y }, color_rect.m_bottom_right };
    vertices[ 5 ] = { { pos.x,          pos.y + size.y }, color_rect.m_bottom_left };

    add_vertices( vertices, 6, D3DPT_TRIANGLELIST );
}

NOINLINE void Renderer::draw_filled_gradient_rect( float x, float y, float w, float h, const ColorRect &color_rect ) {
    draw_filled_gradient_rect( { x, y }, { w, h }, color_rect );
}

NOINLINE void Renderer::draw_rect( const Vec2_t &pos, const Vec2_t &size, const Color color, float thickness ) { 
    // bottom line.
    draw_filled_rect( pos.x, pos.y + size.y - thickness, size.x, thickness, color );
      
    // top line.
    draw_filled_rect( pos.x, pos.y, size.x, thickness, color );
    
    // left line.
    draw_filled_rect( pos.x, pos.y, thickness, size.y, color );

    // right line.
    draw_filled_rect( pos.x + size.x - thickness, pos.y, thickness, size.y, color );
}

NOINLINE void Renderer::draw_rect( float x, float y, float w, float h, const Color color, float thickness ) {
    draw_rect( { x, y }, { w, h }, color, thickness );
}

NOINLINE void Renderer::draw_outlined_filled_rect( const Vec2_t &pos, const Vec2_t &size, const Color fill_color, const Color outline_color ) {
    draw_filled_rect( pos.x, pos.y, size.x, size.y, fill_color );
    draw_rect( pos.x - 1.f, pos.y - 1.f, size.x + 2.f, size.y + 2.f, outline_color );
}

NOINLINE void Renderer::draw_outlined_filled_rect( float x, float y, float w, float h, const Color fill_color, const Color outline_color ) {
    draw_outlined_filled_rect( { x, y }, { w, h }, fill_color, outline_color );
}

NOINLINE void Renderer::draw_outlined_rect( const Vec2_t &pos, const Vec2_t &size, const Color inner_color, const Color outline_color ) {
    // outline
    draw_rect( pos.x - 1.f, pos.y - 1.f, size.x + 2.f, size.y + 2.f, outline_color );
    draw_rect( pos.x + 1.f, pos.y + 1.f, size.x - 2.f, size.y - 2.f, outline_color );

    // inner line
    draw_rect( pos.x, pos.y, size.x, size.y, inner_color );
}

NOINLINE void Renderer::draw_outlined_rect( float x, float y, float w, float h, const Color inner_color, const Color outline_color ) {
    draw_outlined_rect( { x, y }, { w, h }, inner_color, outline_color );
}

NOINLINE void Renderer::draw_circle( const Vec2_t &pos, float radius, const Color color ) {
    static constexpr auto segment_count = 32;
    std::array< Vertex_t, segment_count + 1 > vertices;

    if( !color.a )
        return;

    for( auto i = 0; i <= segment_count; ++i ) {
        const auto angle = 2.f * Math::pi * ( float ) i / ( float ) segment_count;

        const auto x_pos = pos.x + ( radius * std::cos( angle ) );
        const auto y_pos = pos.y + ( radius * std::sin( angle ) );

        vertices[ i ] = {
            { x_pos, y_pos },
            color
        };
    }

    add_vertices( vertices.data(), vertices.size(), D3DPT_LINESTRIP );
}

NOINLINE void Renderer::draw_circle( float x, float y, float radius, const Color color ) {
    draw_circle( { x, y }, radius, color );
}

NOINLINE void Renderer::draw_filled_circle( const Vec2_t &pos, float radius, const Color color ) {
    static constexpr auto segment_count = 32;
    std::array< Vertex_t, segment_count + 1 > vertices;

    if( !color.a )
        return;

    for( auto i = 0; i <= segment_count; ++i) {
        const auto angle = 2.f * Math::pi * ( float ) i / ( float ) segment_count;

        const auto x_pos = pos.x + ( radius * std::cos( angle ) );
        const auto y_pos = pos.y + ( radius * std::sin( angle ) );

        vertices[ i ] = { { x_pos, y_pos }, color };
    }

    add_vertices( vertices.data(), vertices.size(), D3DPT_TRIANGLEFAN );
}

NOINLINE void Renderer::draw_filled_circle( float x, float y, float radius, const Color color ) {
    draw_filled_circle( { x, y }, radius, color );
}

NOINLINE void Renderer::draw_texture_quad( const Vec2_t &pos, const Vec2_t &size, const Color color, IDirect3DTexture9 *texture, const std::array<Vector2, 6> &uv_coords ) {
    std::array< Vertex_t, 6 > vertices;

    if( !color.a )
        return;

    vertices[ 0 ] = { { pos.x,          pos.y + size.y }, color, uv_coords[ 0 ] };
    vertices[ 1 ] = { { pos.x + size.x, pos.y + size.y }, color, uv_coords[ 1 ] };
    vertices[ 2 ] = { { pos.x,          pos.y          }, color, uv_coords[ 2 ] };

    vertices[ 3 ] = { { pos.x + size.x, pos.y + size.y }, color, uv_coords[ 3 ] };
    vertices[ 4 ] = { { pos.x + size.x, pos.y          }, color, uv_coords[ 4 ] };
    vertices[ 5 ] = { { pos.x,          pos.y          }, color, uv_coords[ 5 ] };

    add_vertices( vertices.data(), 6, D3DPT_TRIANGLELIST, texture );
}

NOINLINE void Renderer::draw_texture_quad( float x, float y, float w, float h, const Color color, IDirect3DTexture9 *texture, const std::array<Vector2, 6> &uv_coords ) {
    draw_texture_quad( { x, y }, { w, h }, color, texture, uv_coords );
}

NOINLINE void Renderer::draw_text( font_id_t font_id, const std::string &str, const Vec2_t &pos, uint32_t flags, const Color color, float scale ) {
    std::array< Vertex_t, 6 > vertices;
    Vec2_t offset;

    const auto &font = get_fonts().at( font_id );

    // get size of text string
    const auto text_size = font->get_text_size( str );

    // texture UV coords
    const std::array< Vec2_t, 6 > &uv_coords = {
        {
            { 0.f, 1.f },
            { 1.f, 1.f },
            { 0.f, 0.f },
            { 1.f, 1.f },
            { 1.f, 0.f },
            { 0.f, 0.f }
        }
    };

    // does font have align flags?
    const auto has_align_flag = ( flags & (
        Font::ALIGN_LEFT | Font::ALIGN_RIGHT | Font::ALIGN_CENTER_X | Font::ALIGN_CENTER_Y | Font::ALIGN_CENTER
        ) );

    // align font position
    if( has_align_flag ) {
        // left align
        if( ( flags & Font::ALIGN_LEFT ) )
            offset.x += text_size.x;

        // right align
        else if( ( flags & Font::ALIGN_RIGHT ) )
            offset.x -= text_size.x;

        // center only x
        else if( ( flags & Font::ALIGN_CENTER_X ) )
            offset.x -= ( text_size.x * 0.5f );

        // center only y
        else if( ( flags & Font::ALIGN_CENTER_Y ) )
            offset.y -= ( text_size.y * 0.5f );

        // center on x and y
        else if( ( flags & Font::ALIGN_CENTER ) ) {
            offset.x -= ( text_size.x * 0.5f );
            offset.y -= ( text_size.y * 0.5f );
        }
    }

    // current drawing position
    auto pen_pos = pos;

    // parse through the text string
    for( const auto &ch : str ) {
        // find corresponding glyph
        const auto &glyph = font->get_glyphs().at( ch );
        if( !glyph.valid() )
            continue;

        // don't run rendering code on spaces
        if( ch != ' ' ) {
            // calculate render pos
            auto x = pen_pos.x + ( glyph.m_bearing.x * scale );
            auto y = pen_pos.y + ( text_size.y * scale ) - ( glyph.m_bearing.y * scale );

            // apply alignment 
            x += offset.x * scale;
            y += offset.y * scale;

            // calculate width, height of texture
            const auto w = glyph.m_size.x * scale;
            const auto h = glyph.m_size.y * scale;

            m_device->SetTextureStageState( 0, D3DTSS_COLOROP, glyph.m_colored ? D3DTOP_SELECTARG2 : D3DTOP_SELECTARG1 );

            // draw glyph texture quad
            draw_texture_quad( x, y, w, h, color, glyph.m_texture, uv_coords );
        }

        // move pen position 
        pen_pos.x += ( glyph.m_advance >> 6 ) * scale;
    }
}

NOINLINE void Renderer::draw_text( font_id_t font_id, const std::string & str, float x, float y, uint32_t flags, const Color color, float scale ) {
    draw_text( font_id, str, { x, y }, flags, color, scale );
}

namespace {

    NOINLINE bool is_toplogy_list( D3DPRIMITIVETYPE topology ) {
        return topology == D3DPT_POINTLIST || topology == D3DPT_LINELIST || topology == D3DPT_TRIANGLELIST;
    }

    NOINLINE int get_topology_order( D3DPRIMITIVETYPE topology ) {
        switch( topology ) {
        case D3DPT_POINTLIST:
            return 1;
        case D3DPT_LINELIST:
        case D3DPT_LINESTRIP:
            return 2;
        case D3DPT_TRIANGLELIST:
        case D3DPT_TRIANGLESTRIP:
        case D3DPT_TRIANGLEFAN:
            return 3;
        default:
            return 0;
        }
    }

}

NOINLINE bool Font::init( IDirect3DDevice9 *device, const std::string &ttf_font, size_t size, bool anti_alias ) {
    FT_Error   ft_error;
    FT_ULong   ft_charcode;
    FT_UInt    ft_index; 
    FT_Bitmap  ft_bitmap;

    auto glyphs = &m_glyphs;

    // store font data
    store( device, ttf_font, size, anti_alias );

    // initialize freetype
    ft_error = FT_Init_FreeType( &m_ft_library );
    if( ft_error )
        return false;

    // notes;
    // a face describes a given typeface and style 
    // FT_Err_Unknown_File_Format - the font file could be opened and read, but it appears that the format isn't supported

    // initialize the new font face
    ft_error = FT_New_Face( m_ft_library, ttf_font.c_str(), 0, &m_ft_face );
    if( ft_error == FT_Err_Unknown_File_Format || ft_error )
        return false;

    // notes;
    // The horizontal and vertical device resolutions are expressed in dots-per-inch, or dpi. 
    // Standard values are 72 or 96 dpi for display devices like the screen. 
    // The resolution is used to compute the character pixel size from the character point size.

    // set char size 
    ft_error = FT_Set_Char_Size( m_ft_face, 0, size * 64, 96, 0 );
    if( ft_error )
        return false;

    m_ft_flags = FT_LOAD_RENDER;
    m_ft_flags |= anti_alias ? FT_LOAD_TARGET_NORMAL : FT_LOAD_TARGET_MONO;
    m_ft_flags |= FT_HAS_COLOR( m_ft_face ) ? FT_LOAD_COLOR : 0;

    // parse all character codes available in a given charmap, starting from the first charcode
    // generate textures for them
    ft_charcode = FT_Get_First_Char( m_ft_face, &ft_index );
    while( ft_index != 0 ) {
        // load character glyph
        ft_error = FT_Load_Glyph( m_ft_face, ft_index, m_ft_flags );
        if( ft_error )
            return false;
        
        // create bitmap for current glpyh
        FT_Bitmap_New( &ft_bitmap );
   
        // http://paulbourke.net/dataformats/bitmaps/
        //  32 bit RGB - This is normally the same as 24 bit colour but with an extra 8 bit bitmap known as an alpha channel.
        // This channel can be used to create masked areas or represent transparency.
        // http://prntscr.com/ns5r4i
        ft_error = FT_Bitmap_Convert( m_ft_library, &m_ft_face->glyph->bitmap, &ft_bitmap, 4 );
        if( ft_error )
            return false;

        GlyphData_t glyph_data;
        glyph_data.m_charcode    = ft_charcode;
        glyph_data.m_glyph_index = ft_index;
        glyph_data.m_size        = { ( float ) ft_bitmap.width, ( float ) ft_bitmap.rows };
        glyph_data.m_bearing     = { ( float ) m_ft_face->glyph->bitmap_left, ( float ) m_ft_face->glyph->bitmap_top };
        glyph_data.m_advance     = m_ft_face->glyph->advance.x;

        // is rendering in monochrome mode ( anti-aliasing off )
        if( !anti_alias ) {
            // convert to 0-255 alpha for A8 format
            for( auto it = ft_bitmap.buffer; it != &ft_bitmap.buffer[ ft_bitmap.rows * ft_bitmap.pitch ]; it++ ) 
                *it *= 255;
        }

        glyph_data.m_colored = ( m_ft_face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_BGRA );

        // create glyph texture
        if( D3DXCreateTexture( m_device, ft_bitmap.width, ft_bitmap.rows, 1, 0, glyph_data.m_colored ? D3DFMT_A8R8G8B8 : D3DFMT_A8, D3DPOOL_MANAGED, &glyph_data.m_texture ) != D3D_OK )
            return false;

        D3DLOCKED_RECT locked_rect;
        glyph_data.m_texture->LockRect( 0, &locked_rect, nullptr, D3DLOCK_DISCARD );

        // if glyph is colored use original non-converted bitmap using ARGB/BRGA format
        if( glyph_data.m_colored )
            std::memcpy( locked_rect.pBits, m_ft_face->glyph->bitmap.buffer, m_ft_face->glyph->bitmap.rows * m_ft_face->glyph->bitmap.pitch );
        else
            std::memcpy( locked_rect.pBits, ft_bitmap.buffer, ( size_t ) glyph_data.m_size.y * ft_bitmap.pitch );

        glyph_data.m_texture->UnlockRect( 0 );
        
        // save glyph data
        glyphs->emplace( ft_charcode, std::move( glyph_data ) );

        // go to the next character in charmap
        ft_charcode = FT_Get_Next_Char( m_ft_face, ft_charcode, &ft_index );
    }

    FT_Bitmap_Done( m_ft_library, &ft_bitmap );

    return true;
}

NOINLINE Vec2_t Font::get_text_size( const std::string &str ) const {
    Vec2_t size;
    
    // parse through the text string
    for( const auto &ch : str ) {
        // find corresponding glyph
        const auto &glyph = get_glyphs().at( ch );
        if( !glyph.valid() )
            continue;

        // length of text is the glyph width
        size.x += ( glyph.m_advance >> 6 );

        // height of text is the tallest letter
        if( size.y < glyph.m_size.y )
            size.y = glyph.m_size.y;
    }

    return size;
}
