# dx9-renderer
DirectX 9 2D renderer using FreeType bitmap font rendering

# Usage

Initialization of the renderer, somehwere before the rendering loop and after call to D3D CreateDevice. 

```cpp
g_d3d9_renderer = std::make_shared< Renderer >();
g_d3d9_renderer->init( d3ddev, 1536 );

arial_font_id = g_d3d9_renderer->create_font( g_d3d9_renderer->get_font_path( "Arial (TrueType)" ), 30, true );
```

Rendering itself, inside the rendering loop before the call to D3D Present.

```cpp
d3ddev->BeginScene();

g_d3d9_renderer->render();

d3ddev->EndScene();

g_d3d9_renderer->draw_filled_rect( 50.f, 50.f, 100.f, 100.f, Colors::red );
g_d3d9_renderer->draw_text( arial_font_id, "testing d3d9 rendering", { 50.f, 50.f }, Font::NONE, Colors::light_orange );

// present the scene
d3ddev->Present( NULL, NULL, NULL, NULL );
```
