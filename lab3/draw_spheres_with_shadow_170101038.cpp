// MAYANK WADHWANI 
// 170101038

/*

  NOTE TO READER:
  
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
*/

// As mentioned above, we check if the operating system we are working is mac,
// if it is mac we include the GLUT/GLUT.h library
#include <iostream>
#ifdef __APPLE__
  #include<GLUT/GLUT.h>
//else we include the library required to run it on a Linux machine
#else
  #include<GL/glut.h>
#endif 

// We define macros for different types of sources, like 3 represents glossy surface
#define LIGHT_SOURCE 0
#define PLASTIC 1
#define TEXTURED 2
#define GLOSSY 3

// Since we will be using the plane of interest, we define an array of 4 floats which
// corresponds to different values in the plane's normal equation
// ie. ax+by+cz+d = 0 
float parameters_equation_plane[4];

// We define variables for all the components that will be used in our program
GLUquadricObj *light_source_sphere , *plastic_sphere, *textured_sphere, *glossy_sphere;

// The float pointer that points to an array of values
// representing the texture that will be drawn on the sphere 
float *sphere_texture;

// We set some initial values for the light source
float coordinates_light_source[] = {20, 30, -320, 1};

// Function declaration that makes the texture which will be later drawn on ball 2
float *construct_texture_for_sphere(int width_limit, int height_limit);

// The Shadow matrices for each wall, will be computed by the plane and light source vector later
float shadow_matrix_left_wall[4][4];
float shadow_matrix_right_wall[4][4];
float shadow_matrix_bottom_wall[4][4];
// float leftwallshadow[4][4];
// float rightwallshadow[4][4];
// float floorshadow[4][4];

// Function that given the vectors for normal of a plane and for the light source, 
// generates a corresponding shadow matrix that is used to project shadows of spheres on the plane
void generate_shadow_matrix(float final_shadow_matrix[4][4], float normal_vector_ground[4], float vector_light[4])
{
  float dot_product=0;
  
  // We first take dot product of the 2 vectors
  for( int i = 0; i <= 3 ; i ++){
    dot_product += normal_vector_ground[i] * vector_light[i];
  }

  // We can find the shadow matrix by the following expression:
  // Shadow Matrix = dot_product * I (identity matrix of n order) - Light Vector * Transpose(Ground Normal)
  // We use 2 for loops to evaluate the above expression
  for( int i = 0; i <= 3 ; i ++ ){
    for (int j = 0 ; j <= 3 ;j ++){
      if (i == j){
        final_shadow_matrix[i][j] = dot_product;
      }
      else{
        final_shadow_matrix[i][j] = 0;
      }
      final_shadow_matrix[i][j] -= vector_light[j] * normal_vector_ground[i];
    }
  }
}

// This method will be called to draw the sphere which puts the sphere at a designated place based on the input material type
void draw_sphere(int material_type)
{
    glPushMatrix();
    // If it is a light source, we place it at the top
    if(material_type == LIGHT_SOURCE){
      glTranslatef(coordinates_light_source[0] , coordinates_light_source[1], coordinates_light_source[2]);
      gluSphere(plastic_sphere, 12, 20, 20);
    }
    
    // If it is a plastic ball, we place it at the left end
    if(material_type == PLASTIC){
      glTranslatef(-50, -50, -360);
      gluSphere(plastic_sphere, 20, 20, 20);
    }
    
    // If it is a textured ball, we place it at middle
    if(material_type == TEXTURED){
      glTranslatef(0, -50, -360);
      gluSphere(textured_sphere, 20, 20, 20);
    }

    // If it is a glossy ball, we place it at the right end
    if(material_type == GLOSSY){
      glTranslatef(60, -50, -360);
      gluSphere(textured_sphere, 20, 20, 20);
    }
    glPopMatrix();
}

// The display function which is called to render all the components of the system
void render(void)
{
    // We define the arrays for different materials
    // These arrays provide the visible colours along with 
    // other material properties
    static float material_array_bottom_wall[] = {0, 1, 0, 1};
    static float material_array_other_wall[] = {1, 0, 10, 1};
    static float material_array_textures_sphere[] = {3, 2, 0, 1};

    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    
    // We first draw the bottom wall in our system, we have kept the material to be
    // ambient and diffusive both
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_array_bottom_wall);

    // We define the normal of the botttom wall, 
    // and also assign 4 vertices representing the four corners 
    // of the wall
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-100, -100, -320);
    glVertex3f( 100, -100, -320);
    glVertex3f( 100, -100, -520);
    glVertex3f(-100, -100, -520);
    glEnd();
      
    // We next attempt to draw the shadows of the different components 
    // using the shadow matrix as computed earlier
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    // The colour of the shadow is black
    glColor3f(0, 0, 0); 
    glPushMatrix();
    // We multiply the shadow matrix with the first sphere
    glMultMatrixf((float *)shadow_matrix_bottom_wall);
    draw_sphere(1);
    
    glPopMatrix();
    glPushMatrix();
    // We multiply the shadow matrix with the second sphere
    glMultMatrixf((float *)shadow_matrix_bottom_wall);
    draw_sphere(2);
    glPopMatrix();
    
    glPushMatrix();
    // We multiply the shadow matrix with the third sphere
    glMultMatrixf((float *)shadow_matrix_bottom_wall);
    draw_sphere(3);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // We then draw the other walls in our system, we have kept the material to be
    // ambient and diffusive both
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_array_other_wall);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // We define the values for the right wall
    // the normal and the 4 vertices that represents the different corners of the system
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glVertex3f( 100, -100, -320);
    glVertex3f( 100,  100, -320);
    glVertex3f( 100,  100, -520);
    glVertex3f( 100, -100, -520);
    glEnd();
    
    // We then draw the shadows on the right wall
    // Even though only one objects shadow will be present, we
    // still assign shadows for other objects since it can be present for different 
    // locations of light source
    glStencilFunc(GL_EQUAL, 1, 1);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    // We set the colour of the shadow to be black
    glColor3f(0, 0, 0); 
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    // We multiply the shadow matrix with the first sphere
    glMultMatrixf((float *)shadow_matrix_right_wall);
    draw_sphere(1);
    glPopMatrix();

    glPushMatrix();
    // We multiply the shadow matrix with the second sphere
    glMultMatrixf((float *)shadow_matrix_right_wall);
    draw_sphere(2);
    glPopMatrix();

    glPushMatrix();
    // We multiply the shadow matrix with the third sphere
    glMultMatrixf((float *)shadow_matrix_right_wall);
    draw_sphere(3);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // In a similar way, we draw the shadows on the left wall also
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_array_other_wall);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glBegin(GL_QUADS);

    // We define the values for the left wall
    // the normal and the 4 vertices that represents the different corners of the system
    glNormal3f(1, 0, 0);
    glVertex3f(-100, -100, -320);
    glVertex3f(-100, -100, -520);
    glVertex3f(-100,  100, -520);
    glVertex3f(-100,  100, -320);
    glEnd();

    glStencilFunc(GL_EQUAL, 1, 1);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    // We set the colour of the shadow to be black
    glColor3f(0, 0, 0); 
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    // We multiply the shadow matrix with the first sphere
    glMultMatrixf((float *)shadow_matrix_left_wall);
    draw_sphere(1);
    glPopMatrix();

    glPushMatrix();
    // We multiply the shadow matrix with the second sphere
    glMultMatrixf((float *)shadow_matrix_left_wall);
    draw_sphere(2);
    glPopMatrix();
    glPushMatrix();
    // We multiply the shadow matrix with the third sphere
    glMultMatrixf((float *)shadow_matrix_left_wall);
    draw_sphere(3);
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // We finally draw the remaining walls ie. the ceiling and the back wall,
    glBegin(GL_QUADS);

    // We define the values for the ceiling
    // the normal and the 4 vertices that represents the different corners of the system
    glNormal3f(0, -1, 0);
    glVertex3f(-100,  100, -320);
    glVertex3f(-100,  100, -520);
    glVertex3f( 100,  100, -520);
    glVertex3f( 100,  100, -320);

    // We define the values for the back wall
    // the normal and the 4 vertices that represents the different corners of the system
    glNormal3f(0, 0, 1);
    glVertex3f(-100, -100, -520);
    glVertex3f( 100, -100, -520);
    glVertex3f( 100,  100, -520);
    glVertex3f(-100,  100, -520);
    glEnd();

    // After drawing all the walls and shadows, we finally draw the 
    // actual components of the system, the spheres

    // We have three different matrices for the three different types of 
    // parameters in the system ie ambient, diffuse and specular
    // The different parameters represents the values for the RG and B parts
    float ambient_matrix_values[4];
    float diffuse_matrix_values[3];
    float specular_matrix_values[3];

    // These values represent the plastic surface and values have been taken 
    // from the above mentioned link
    // So for example, since only the G value for the ambient is 1, 
    // we expect the sphere 1 to be of green colour
    // yellow plastic  0.0 0.0 0.0 0.5 0.5 0.0 0.60  0.60  0.50  .25

    ambient_matrix_values[0] = 5.0;
    ambient_matrix_values[1] = 1.0;
    ambient_matrix_values[2] = 0.0;
    ambient_matrix_values[3] = 1.0;
    diffuse_matrix_values[0] = 0.5;
    diffuse_matrix_values[1] = 0.5;
    diffuse_matrix_values[2] = 0.0;
    specular_matrix_values[0] = 0.6;
    specular_matrix_values[1] = 0.6;
    specular_matrix_values[2] = 0.5;

    // We finally set these values as an environemnt and then draw the first sphere
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_matrix_values);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_matrix_values);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_matrix_values);
    glMaterialf(GL_FRONT, GL_SHININESS, 1 * 128.0);
    
    // We draw the first sphere
    draw_sphere(1);

    // Since the second sphere involved a texture, we enable the GL_TEXTURE_2D
    glEnable(GL_TEXTURE_2D);
    
    // We set the material properties for the textured sphere 
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_array_textures_sphere);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // To enable textures to be applied, we have to set the gluQuadraticTexture property to be GL_TRUE
    gluQuadricTexture(textured_sphere, GL_TRUE);
    gluQuadricNormals(textured_sphere, GLU_SMOOTH);
    
    // We finally draw the textured sphere and then disable the GL_TEXTURE_2D since it is not required for the later components
    draw_sphere(2);
    glDisable(GL_TEXTURE_2D);

    // We set the material properties for the third sphere
    ambient_matrix_values[0] = 6.25;
    ambient_matrix_values[1] = 0.20725;
    ambient_matrix_values[2] = 0.20725;
    ambient_matrix_values[3] = 1.0;
    diffuse_matrix_values[0] = 1;
    diffuse_matrix_values[1] = 0.829;
    diffuse_matrix_values[2] = 0.829;
    specular_matrix_values[0] = 0.296648;
    specular_matrix_values[1] = 0.296648;
    specular_matrix_values[2] = 0.296648;
    
    // After initliazation, we set these properties to our sphere
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_matrix_values);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_matrix_values);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_matrix_values);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.088 * 128.0);
    
    // We finally draw the third sphere
    draw_sphere(3);
    
    // We set the colour for enviornment as yellow
    glColor3f(2, 2, 0.5);
    // We disable the lighting since the source is the producer of light, so reflections on it is not required.
    glDisable(GL_LIGHTING);
    // We draw the source of light
    draw_sphere(0);
    glEnable(GL_LIGHTING);
    
    glutSwapBuffers(); 
}

// We exit the program if the user presses escape
void keyFunc(unsigned char key, int x, int y)
{
    // If the user presses escape
    if(key == 27)
      exit(0);
}

// Setting up opengl interface and calling the glut main loop
int main(int argc, char *argv[])
{
    //initialize the window by providing required fields like window position, size,
    // name of window, etc
    glutInit(&argc, argv);
    glutInitWindowSize(700, 700);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL|GLUT_DOUBLE);
    glutCreateWindow("Spheres With Shadows");
    glutDisplayFunc(render);
    glutKeyboardFunc(keyFunc);

    // We construct our perspective view for the program,
    // the different values passed in glFrustum represents the clipping planes
    // and near and far planes, so like -100 is the coordinates for the left 
    // clipping plane
    glMatrixMode(GL_PROJECTION);
    glFrustum(-100, 100, -100, 100, 330, 10000); 
    glMatrixMode(GL_MODELVIEW);

    // We initialze the required features
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // We then generate the different shadow matrices 
    // for different planes

    // For the bottom plane, 
    // the equation is y+100 = 0
    // so a=0, b=1, c=0, d=100
    parameters_equation_plane[0]=0;
    parameters_equation_plane[1]=1;
    parameters_equation_plane[2]=0;
    parameters_equation_plane[3]=100;
    generate_shadow_matrix(shadow_matrix_bottom_wall, parameters_equation_plane, coordinates_light_source);

    // For the left plane, 
    // the equation is x+100 = 0
    // so a=1, b=0, c=0, d=100
    parameters_equation_plane[0]=1;
    parameters_equation_plane[1]=0;
    parameters_equation_plane[2]=0;
    parameters_equation_plane[3]=100;
    generate_shadow_matrix(shadow_matrix_left_wall, parameters_equation_plane, coordinates_light_source);

    // For the right plane, 
    // the equation is -x+100 = 0
    // so a=-1, b=0, c=0, d=100
    parameters_equation_plane[0]=-1;
    parameters_equation_plane[1]=0;
    parameters_equation_plane[2]=0;
    parameters_equation_plane[3]=100;
    generate_shadow_matrix(shadow_matrix_right_wall, parameters_equation_plane, coordinates_light_source);
    
    // We place the light in the defined location
    glLightfv(GL_LIGHT0, GL_POSITION, coordinates_light_source);
    glCullFace(GL_BACK);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // We finally initialize all the different components present in the system,
    // the different types of spheres
    plastic_sphere = gluNewQuadric();
    textured_sphere = gluNewQuadric();
    glossy_sphere = gluNewQuadric();
    light_source_sphere = gluNewQuadric();
    // We also declare the array representing the texture and set sphere_texture pointer
    // point to it    
    sphere_texture = construct_texture_for_sphere(1024, 512);

    // We map the texture in the 2D form so that it can be referenced later
    glTexImage2D(GL_TEXTURE_2D, 0, 1, 512, 512, 0, GL_RED, GL_FLOAT, sphere_texture);
    
    // We start the main loop
    glutMainLoop();
    return 0;
}

// Function definition that makes the texture which will be later drawn on ball 2
float *construct_texture_for_sphere(int width_limit, int height_limit)
{
    int width, height;
    
    // We declare a static function for texture and allocate new memory for it
    // There will be width_limit * height_limit different values, 
    static float *texture;
    texture = (float *)malloc(width_limit * height_limit * sizeof(float));
    
    // We define our pattern in such a way:
    // for the width, we assign 0 for 3 consecutive 1's and similarly 1 for next 3
    // a similar thing is applied to height also, just that we assign 0 for 2 consecutive 1's and respectively for 0's also
    for(height = 0; height < height_limit; height++) {
      for(width = 0; width < width_limit; width++) {
        texture[width + width_limit * height] = ((width >> 8) & 0x1) ^ ((height >> 4) & 0x1);
      }
    }
    return texture;
}
