#include "includes.h"

std::shared_ptr< Renderer > g_d3d9_renderer;

LRESULT __stdcall WindowProc( HWND wnd, UINT message, WPARAM wparam, LPARAM lparam ) {
    switch( message ) {
    // is the window closing?
    case WM_DESTROY:
        // close the application
        PostQuitMessage( 0 );
        return 0;

    default:
        break;
    }

    // handle other messages
    return DefWindowProc( wnd, message, wparam, lparam );
}

int __stdcall WinMain( HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show ) {
    HWND       wnd;
    WNDCLASSEX window_class_ex;
    MSG        current_msg;

    // set window data
    ZeroMemory( &window_class_ex, sizeof( WNDCLASSEX ) );
    window_class_ex.cbSize        = sizeof( WNDCLASSEX );
    window_class_ex.style         = CS_HREDRAW | CS_VREDRAW;
    window_class_ex.lpfnWndProc   = WindowProc;
    window_class_ex.hInstance     = instance;
    window_class_ex.hCursor       = LoadCursor( NULL, IDC_ARROW );
    window_class_ex.hbrBackground = ( HBRUSH ) COLOR_WINDOW;
    window_class_ex.lpszClassName = "D3D";

    RegisterClassEx( &window_class_ex );

    // create window
    wnd = CreateWindowEx( NULL, "D3D", "D3D", WS_OVERLAPPEDWINDOW, 300, 300, 800, 600, NULL, NULL, instance, NULL );
    ShowWindow( wnd, cmd_show );

    // init d3d9 for rendering
    D3D9::init_render( wnd );

    while( true ) {
        // check if messages are waiting in queue
        while( PeekMessage( &current_msg, NULL, 0, 0, PM_REMOVE ) ) {
            // did we get the quit message?
            if( current_msg.message == WM_QUIT )
                break;

            // translate message and forward to winproc
            TranslateMessage( &current_msg );
            DispatchMessage( &current_msg );
        }

        // render d3d9 frame
        D3D9::render_frame();
    }

    // cleanup d3d9 after rendering
    D3D9::cleanup();
    
    return current_msg.wParam;
}