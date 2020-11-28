MAYANK WADHWANI 
170101038

COMMAND TO RUN:
	g++ draw_mesh_170101038.cpp -lGL -lGLU -lglut


INNOVATION TRIED BY ME:

1) The object will get zoomed in if left mouse click is pressed
2) The object will be zoomed out if right mouse click is pressed
3) If letter 'l' is pressed on keyboard, the object will be filled with white color
4) If letter 'c' is pressed on keyboard, the skeleton of the object will be displayed
5) If letter 'z' is pressed on keyboard, the x axis orientation will increase
6) If letter 'x' is pressed on keyboard, the x axis orientation will decrease
7) If letter 'n' is pressed on keyboard, the y axis orientation will decrease
8) If letter 'm' is pressed on keyboard, the y axis orientation will decrease
9) Window is closed if escape key is pressed
We are assuming that each stl file follows the following format:

Header: First 80 bytes
Number of triangles: next 4 bytes

For all triangles, we follow this break-up
	normal: 12 bytes 
	vertex 1: 12 bytes
	vertex 2: 12 bytes
	vertex 3: 12 bytes
	(Here, normal and vertices take 12 bytes because they comprise of 3 floating points each)
	Byte count: 2bytes
end

