#pragma once
// By Oleksiy Grechnyev

namespace GtkGL {
/// The Vao interface. Anything that can be drawn with OpenGL.
class IVao{
public: //==== Abstract methods
    /// Draw this object
    virtual void draw() = 0;

    /// Optional Destructor
    virtual ~IVao() {}
};
}
