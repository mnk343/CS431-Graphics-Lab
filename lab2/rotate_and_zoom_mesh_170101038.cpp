// MAYANK WADHWANI 
// 170101038

// GENERAL INSTRUCTIONS:
// To rotate: Left click and drag your mouse in the desired direction
// To zoom: Scroll using your mouse upwards or downwards depending on whether you want to zoom in or zoom out
// Use arrow keys to translate the object in the desired coordinates
// Use escape key to exit

/*

	NOTE TO READER:
	I have compiled the following program on macOS 
	However, if you are using a different os, the required header file 
	for OpenGL may be different
	for ex. Linux users may include GL/glut.h instead of GLUT/GLUT.h(used by macos)

	Command used by me:
	g++ rotate_and_zoom_mesh_170101038.cpp -framework OpenGL -framework GLUT -w

	Command to be used by linux users:
	g++ rotate_and_zoom_mesh_170101038.cpp -lGL -lGLU -lglut

*/

// As mentioned above, we check if the operating system we are working is mac,
// if it is mac we include the GLUT/GLUT.h library
#ifdef __APPLE__
	#include<GLUT/GLUT.h>
//else we include the library required to run it on a Linux machine
#else
	#include<GL/glut.h>

#endif 

#include<vector>
#include<fstream>
#include<iostream>

using namespace std;

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
float rotate_x = 0;
float rotate_y = 0;

// Translate the object in x and y axis
float x_translate = 0;
float y_translate = 0;

// these represents the difference in the x and y coordinates when the user clicks
// on the mouse and drag it
float diff_y_coordinate = 0;
float diff_x_coordinate = 0;
// this represents if the mouse is still clicked, once the user releases it 
// the value is set to false
bool mouse_still_clicked = false;

// funtion called to output the triangles
void render(void){
	// clear all initial values if any
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// shift the origin
	glTranslatef(0.1f+x_translate,-0.9f+y_translate,0.0f); 
	// rotate the axis depending on values of rotate_x and rotate_y which can be 
	// changed by pressing the keys
	// z,x,m,n
	glRotatef( rotate_x, 1.0, 0.0, 0.0 );
  	glRotatef( rotate_y, 0.0, 1.0, 0.0 );
  	// zoom can be changed by mouse clicks (left or right)
	glScalef(zoom, zoom, zoom);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

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
void keyFunc(unsigned char key , int x, int y){
	
	// if escape is pressed, close the window
	if( key==27 )
		exit(0);
}
// Spcial keyboard cases are added to translate the object when user user arrow keys
void keyboardSpecialFunc( int c, int x, int y){
	// Translate the object upwards (increase y coordinates)
	if( c == GLUT_KEY_UP){
		y_translate += 0.1;
		glutPostRedisplay();
	}

	// Translate the object downwards (decrease y coordinates)
	else if( c == GLUT_KEY_DOWN){
		y_translate -= 0.1;
		glutPostRedisplay();
	}
	
	// Translate the object towards the left (decrease x coordinates)
	else if( c == GLUT_KEY_LEFT){
		x_translate -= 0.1;
		glutPostRedisplay();
	}
	
	// Translate the object towards the right (increase x coordinates)
	else if( c == GLUT_KEY_RIGHT){
		x_translate += 0.1;
		glutPostRedisplay();
	}
}

// function to allow mouse interaction
void mouseFunc(int button , int state , int x, int y){
	
	// if user has pressed the left button
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//set mouse clicked to true
		mouse_still_clicked = true;
		// and update the difference in x and y coordinates
		diff_x_coordinate = x - rotate_y;
		diff_y_coordinate = -y + rotate_x;
	}
	// if user is scrolling up then the value of button is set to 3 
	// and if he/she is scrolling down then button is set to 4
	else if (button == 3 || button == 4){
		// GLUT_UP state is ignored
		if(state == GLUT_UP)
			return;
		// if scrolling up, then increase the zoom and redraw
		else if( button == 3){
			zoom += 0.1;
			glutPostRedisplay();
		}
		// if scrolling up, then decrease the zoom and redraw
		// if zoom is zero, no need to further decrease it
		else{
			if(zoom - 0.1 >= 0)
				zoom -= 0.1 ;
			glutPostRedisplay();
		}
	}
	else
		mouse_still_clicked = false;
}

// when the mouse if moved, this function is called
void mouseMotionFunc(int x, int y)
{
	// if mouse is still clicked, then change the rotate_x and rotate_y 
	// variables and redraw
	if (mouse_still_clicked)
	{
		// make the appropriate changes
		rotate_y = x - diff_x_coordinate;
		rotate_x = y + diff_y_coordinate;
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
	//this function is used when the mouse is moved
	glutMotionFunc(mouseMotionFunc);
	glutSpecialFunc(keyboardSpecialFunc);
	glutMainLoop();

	return 0;
}
