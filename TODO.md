```
Aliases
{
    [ ] - Build without PhysicsEngine
    [ ] - Build without SoundEngine
    [X] - Build Server
    [X] - Build With Tracy
}

Graphics API
{
    [X] - OpenGL
    [ ] - Vulkan
    [ ] - DX11
    [ ] - CPU
}

UI
{
    Support
    {
       [X] - Input Management
       [X] - HitTest
       [ ] - FreeType Full Support
       [ ] - Clipping

       [ ] - WorldSpace Rendering
       [X] - ScreenSpace Rendering
       
       [X] - Layout System
       [X] - UI Material System
    }

    Elements
    {
        [X] - UIControl
        [X] - Decorator
        [X] - ProgressBar
        [X] - Panel
        [X] - StackPanel
        [X] - Grid
        [ ] - ScrollViewer
        [ ] - List
        [X] - Button
        [ ] - TextBox
        [ ] - Image
        [ ] - Rectangle
        [ ] - Circle
        [ ] - ContextMenu
        [ ] - ViewPort3D
    }
}

3D
{
    Rendering
    {
        [X] - BaseRender
        [ ] - PBR
    }
}
```

Убрать Ref из кода так как он является std::shared_ptr заменить на сырые указатели

