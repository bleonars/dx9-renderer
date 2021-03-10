#include "includes.h"

namespace Colors {

    Color red          = { 255, 255, 0, 0 };
    Color green        = { 255, 0, 255, 0 };
    Color blue         = { 255, 0, 0, 255 };
    Color black        = { 255, 0, 0, 0 };
    Color white        = { 255, 255, 255, 255 };
    Color yellow       = { 255, 255, 255, 0 };
    Color orange       = { 255, 255, 128, 0 };
    Color purple       = { 255, 255, 0, 255 };
    Color light_red    = { 255, 255, 51, 51 };
    Color light_green  = { 255, 51, 255, 51 };
    Color light_blue   = { 255, 102, 255, 255 };
    Color light_yellow = { 255, 255, 255, 51 };
    Color light_orange = { 255, 255, 153, 51 };
    Color light_purple = { 255, 255, 51, 255 };

}

namespace D3D9 {

    IDirect3D9       *d3d;
    IDirect3DDevice9 *d3ddev;

    size_t arial_font_id;

    NOINLINE void init_render( HWND wnd ) {
        D3DPRESENT_PARAMETERS d3dpp;

        d3d = Direct3DCreate9( D3D_SDK_VERSION );

        // set device data
        ZeroMemory( &d3dpp, sizeof( D3DPRESENT_PARAMETERS ) );
        d3dpp.SwapEffect    = D3DSWAPEFFECT_DISCARD;
        d3dpp.hDeviceWindow = wnd;
        d3dpp.Windowed      = TRUE;
     
        // create d3d9 device
        d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev );

        g_d3d9_renderer = std::make_shared< Renderer >();
        g_d3d9_renderer->init( d3ddev, 1536 );

        arial_font_id = g_d3d9_renderer->create_font( g_d3d9_renderer->get_font_path( "Arial (TrueType)" ), 30, true );
    }

    NOINLINE void render_frame() {
        // clear the window to a deep blue
        d3ddev->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 255, 255, 255 ), 1.f, 0 );

        d3ddev->BeginScene();

        g_d3d9_renderer->render();

        d3ddev->EndScene();

        g_d3d9_renderer->draw_text( arial_font_id, "testing d3d9 rendering", { 50.f, 50.f }, Font::NONE, Colors::light_orange );

        // present the scene
        d3ddev->Present( NULL, NULL, NULL, NULL );
    }

    NOINLINE void cleanup() {
        d3ddev->Release(); 
        d3d->Release();    
    }

}