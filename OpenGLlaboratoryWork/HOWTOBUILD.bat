:: From the solution directory:

:: 1. Build GL3f
cd ./OpenGLlaboratoryWork/modules/gl3w
py gl3w_gen.py --ext

:: 2. Build GLFW
cd ./OpenGLlaboratoryWork/modules/glfw
mkdir build
cd ./build
cmake .. -G"Visual Studio 15"
:: !!! Then run GLFW.sln in your Viaual Studio and build both Debug and Release configuration
cd ./src/Debug
ren glfw3.lib glfw3_d.lib