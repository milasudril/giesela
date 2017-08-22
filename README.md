GieShadErLA -- Gl ShadEr LAboratory
===================================

This is a test application for OpenGL fragment shaders. You can manipulate the fragment shader in the left panel, and click on compile to see the result. You can load models from Wavefront files. Currently, the model must have an extent close to unity in all directions. Also Z is up.

Dependencies
------------
Theese are the runtime dependencies:

 * GLEW
 * gtk+-3.0
 * gtksourceview-3.0
 * An up-to-date graphics driver and a decent video card. I targeted OpenGL 4.5+ because I like "Direct State Access".

Compilation
-----------
Run `maike(1)` from the project root, and you should find giesela in `__targets`. Make sure to have `g++`, `incscape`, `python3`, and `pkg-config` installed.
