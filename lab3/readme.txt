
This program generates three spheres kept in a room using OpenGL
The first sphere is a simple plastic one
The second one is textured, it contains a pretty pattern drawn on it
and the last one is a glossy one, one can appreciate the glossiness 
by keeping a light source at the top and seeing that the shine on the sphere

We have referred to the following source:
  https://cs.wellesley.edu/~cs307/readings/OpenGL-VRML-Materials.html
to find the necessary values for different types of surfaces like plastic, etc
in the system

Also,
I have compiled the following program on macOS 
However, if you are using a different os, the required header file 
for OpenGL may be different
for ex. Linux users may include GL/glut.h instead of GLUT/GLUT.h(used by macos)

Command used by me:
g++ draw_spheres_with_shadow_170101038.cpp -framework OpenGL -framework GLUT -w

Command to be used by linux users:
g++ draw_spheres_with_shadow_170101038.cpp -lGL -lGLU -lglut

Press escape to exit the program
