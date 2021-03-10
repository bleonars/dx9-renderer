#pragma once

namespace Colors {

    extern Color red;
    extern Color green;
    extern Color blue;
    extern Color black;
    extern Color white;
    extern Color yellow;
    extern Color orange;
    extern Color purple;
    extern Color light_red;
    extern Color light_green;
    extern Color light_blue;
    extern Color light_yellow;
    extern Color light_orange;
    extern Color light_purple;  

}

namespace D3D9 {

    extern IDirect3D9       *d3d;
    extern IDirect3DDevice9 *d3ddev;

    extern size_t arial_font_id;

    // initialize d3d9 frame rendering
    NOINLINE void init_render( HWND wnd );

    // render current frame
    NOINLINE void render_frame();

    // clean up
    NOINLINE void cleanup();

}