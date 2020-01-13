/*
 * Soul: a small object-oriented OpenGL Rendering Kernel.
 */

#include "Soul/ui/Window.h"

using namespace std;

namespace Soul
{

Window::Parameters::Parameters() :
    _name("IceCryStal_Earth"), _width(640), _height(480), _major(3), _minor(3),
    _debug(false), _alpha(false), _depth(false), _stencil(false), _multiSample(false)
{
}

string Window::Parameters::name() const
{
    return _name;
}

int Window::Parameters::width() const
{
    return _width;
}

int Window::Parameters::height() const
{
    return _height;
}

vec2<int> Window::Parameters::version() const
{
    return vec2<int>(_major, _minor);
}

bool Window::Parameters::debug() const
{
    return _debug;
}

bool Window::Parameters::alpha() const
{
    return _alpha;
}

bool Window::Parameters::depth() const
{
    return _depth;
}

bool Window::Parameters::stencil() const
{
    return _stencil;
}

bool Window::Parameters::multiSample() const
{
    return _multiSample;
}

Window::Parameters &Window::Parameters::name(const string name)
{
    _name = name;
    return *this;
}

Window::Parameters &Window::Parameters::size(int width, int height)
{
    _width = width;
    _height = height;
    return *this;
}

Window::Parameters &Window::Parameters::version(int major, int minor, bool debug)
{
    _major = major;
    _minor = minor;
    _debug = debug;
    return *this;
}

Window::Parameters &Window::Parameters::alpha(bool alpha)
{
    _alpha = alpha;
    return *this;
}

Window::Parameters &Window::Parameters::depth(bool depth)
{
    _depth = depth;
    return *this;
}

Window::Parameters &Window::Parameters::stencil(bool stencil)
{
    _stencil = stencil;
    return *this;
}

Window::Parameters &Window::Parameters::multiSample(bool multiSample)
{
    _multiSample = multiSample;
    return *this;
}

Window::Window(const Parameters &params) : EventHandler("Window")
{
}

Window::~Window()
{
}

}
