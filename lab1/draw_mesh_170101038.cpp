// MAYANK WADHWANI 
// 170101038

/*

	NOTE TO READER:
	I have compiled the following program on macOS 
	However, if you are using a different os, the required header file 
	for OpenGL may be different
	for ex. Linux users may include GL/glut.h instead of GLUT/GLUT.h(used by macos)

	Command used by me:
	g++ draw_mesh_170101038.cpp -framework OpenGL -framework GLUT -w

	Command to be used by linux users:
	g++ draw_mesh_170101038.cpp -lGL -lGLU -lglut

*/

/*
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
*/

/*
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

*/
#include<vector>
#include<fstream>
#include<iostream>

// for mac
#include<GLUT/GLUT.h>

// for Linux
// #include<GL/glut.h>

using namespace std;


#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP    3
#  define GLUT_WHEEL_DOWN  4
#  define GLUT_WHEEL_LEFT  5
#  define GLUT_WHEEL_RIGHT 6
#endif



// structure for one triangle that will be used
struct Triangle
{
	float vertex1[3];
	float vertex2[3];
	float vertex3[3];
};

vector<Triangle> triangles;

// this method will be used to put values of coordinates in object to get one full
// vertex
void getVertex(char *ptr , float *obj){
	float vertex[3];
	char x_coordinate[4] = {ptr[0],ptr[1],ptr[2],ptr[3]};
	char y_coordinate[4] = {ptr[4],ptr[5],ptr[6],ptr[7]};
	char z_coordinate[4] = {ptr[8],ptr[9],ptr[10],ptr[11]};
	obj[0] = *((float *)x_coordinate);
	obj[1] = *((float *)y_coordinate);
	obj[2] = *((float *)z_coordinate);
}

// these quantifiers will be changed by user interaction
float zoom = 4.1;
// mode represents the type of bunny we want to see, either filled with white color
// or in skeleton mode
int mode = 0;
float rotate_x = 0;
float rotate_y = 0;

// funtion called to output the triangles
void render(void){
	
	// clear all initial values if any
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// shift the origin
	glTranslatef(0.1f,-0.9f,0.0f); 
	// rotate the axis depending on values of rotate_x and rotate_y which can be 
	// changed by pressing the keys
	// z,x,m,n
	glRotatef( rotate_x, 1.0, 0.0, 0.0 );
  	glRotatef( rotate_y, 0.0, 1.0, 0.0 );
  	// zoom can be changed by mouse clicks (left or right)
	glScalef(zoom, zoom, 1.0f);

	if ( mode == 0 )
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// traverse through all triangles and print each of them
	for(int i = 0 ; i < triangles.size() ; i++){
		glBegin(GL_TRIANGLES);
			glVertex3f( triangles[i].vertex1[0] , triangles[i].vertex1[1] , triangles[i].vertex1[2] );
			glVertex3f( triangles[i].vertex2[0] , triangles[i].vertex2[1] , triangles[i].vertex2[2] );
			glVertex3f( triangles[i].vertex3[0] , triangles[i].vertex3[1] , triangles[i].vertex3[2] );
		glEnd();
	}
	glutSwapBuffers();
}

// function to allow keyboard interaction
void keyFunc(unsigned char c , int x, int y){
	
	// if escape is pressed, close the window
	if( c==27 )
		exit(0);
	else if( c == 'c'){
		// user switched to color mode
		mode = 1;
		glutPostRedisplay();
	}
	else if( c == 'l'){
		// user switched to line mode (skeleton view of object)
		mode = 0;
		glutPostRedisplay();
	}
	else if( c == 'z'){
		// user wants to rotate in positive x axis
		rotate_x+=3;
		glutPostRedisplay();
	}
	else if( c == 'x'){
		// user wants to rotate in negative x axis
		rotate_x-=3;
		glutPostRedisplay();
	}
	else if( c == 'n'){
		// user wants to rotate in positive y axis
		rotate_y+=3;
		glutPostRedisplay();
	}
	else if( c == 'm'){
		// user wants to rotate in negative y axis
		rotate_y-=3;
		glutPostRedisplay();
	}
}

// function to allow mouse interaction
void mouseFunc(int button , int state , int x, int y){
	if( state == GLUT_DOWN && button == GLUT_LEFT_BUTTON){
		// user wants to see zoomed in image
		zoom+=0.1;
		glutPostRedisplay();
	}
	else if( state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON){
		// user wants to see zoomed out image
		zoom-=0.1;
		glutPostRedisplay();
	}
}

int main(int argc , char **argv){
	
	// open the stl file
	ifstream file("lowpolybunny.stl" ,  ios::in | ios::binary);
	
	// if file not opened 
	if(!file)
	{
		cout<<"Error..!!\n";
		return 0;
	}
	// first 80 bytes are reserved for header
	char header[80]="";
	file.read(header , 80 );

	//next 4 bytes reserved for no of triangles
	int no_of_triangles = 0;
	char no_of_triangles_string[4];
	file.read(no_of_triangles_string , 4 );
	no_of_triangles = *((int *)(no_of_triangles_string));

	// now for each triangle, we will get the corresponding three vertices by calling
	// getVertex function sending float[3] as arguments
	for( int i = 0 ; i < no_of_triangles; i ++){
		char one_triangle[50] = "";
		file.read(one_triangle , 50);

		Triangle obj;
		getVertex(one_triangle + 12 , obj.vertex1);
		getVertex(one_triangle + 24 , obj.vertex2);
		getVertex(one_triangle + 36 , obj.vertex3);

		// once we create the object, we push the object in the vector of objects
		triangles.push_back(obj);
	}
	//initialize the window by providing required fields like window position, size,
	// name of window, mouseFunctions, etc
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("INTERACTIVE BUNNY");
	glutDisplayFunc(render);
	glutKeyboardFunc(keyFunc);
	glutMouseFunc(mouseFunc);
	glutMainLoop();

	return 0;
}