#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int NumVertices;
    float x;
    float y;
    float z;
};
typedef struct VAO VAO;

struct COLOR {
    float r;
    float g;
    float b;
};
typedef struct COLOR COLOR;

struct laser {
  VAO *ray;
  float x;
  float y;
  float angle;
  float flag;
  float c;
  float c1;
  float speed;
};
typedef struct laser laser;

struct Point {
  float x;
  float y;
};
typedef struct Point Point;

struct GLMatrices {
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 view;
	GLuint MatrixID;
} Matrices;

GLuint programID;
    COLOR grey = {168.0/255.0,168.0/255.0,168.0/255.0};
    COLOR gold = {218.0/255.0,165.0/255.0,32.0/255.0};
    COLOR coingold = {255.0/255.0,223.0/255.0,0.0/255.0};
    COLOR red = {255.0/255.0,51.0/255.0,51.0/255.0};
    COLOR lightgreen = {57/255.0,230/255.0,0/255.0};
    COLOR darkgreen = {51/255.0,102/255.0,0/255.0};
    COLOR black = {30/255.0,30/255.0,21/255.0};
    COLOR blue = {0,0,1};
    COLOR darkbrown = {46/255.0,46/255.0,31/255.0};
    COLOR lightbrown = {95/255.0,63/255.0,32/255.0};
    COLOR brown1 = {117/255.0,78/255.0,40/255.0};
    COLOR brown2 = {134/255.0,89/255.0,40/255.0};
    COLOR brown3 = {46/255.0,46/255.0,31/255.0};
    COLOR cratebrown = {153/255.0,102/255.0,0/255.0};
    COLOR cratebrown1 = {121/255.0,85/255.0,0/255.0};
    COLOR cratebrown2 = {102/255.0,68/255.0,0/255.0};
    COLOR skyblue2 = {113/255.0,185/255.0,209/255.0};
    COLOR skyblue1 = {123/255.0,201/255.0,227/255.0};
    COLOR skyblue = {132/255.0,217/255.0,245/255.0};
    COLOR cloudwhite = {229/255.0,255/255.0,255/255.0};
    COLOR cloudwhite1 = {204/255.0,255/255.0,255/255.0};
    COLOR lightpink = {255/255.0,122/255.0,173/255.0};
    COLOR darkpink = {255/255.0,51/255.0,119/255.0};
    COLOR white = {255/255.0,255/255.0,255/255.0};
    COLOR score = {117/255.0,78/255.0,40/255.0};

/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
//    exit(EXIT_SUCCESS);
}


/* Generate VAO, VBOs and return VAO handle */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
    struct VAO* vao = new struct VAO;
    vao->PrimitiveMode = primitive_mode;
    vao->NumVertices = numVertices;
    vao->FillMode = fill_mode;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
    glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

    glBindVertexArray (vao->VertexArrayID); // Bind the VAO
    glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
                          0,                  // attribute 0. Vertices
                          3,                  // size (x,y,z)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
    glVertexAttribPointer(
                          1,                  // attribute 1. Color
                          3,                  // size (r,g,b)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
{
    GLfloat* color_buffer_data = new GLfloat [3*numVertices];
    for (int i=0; i<numVertices; i++) {
        color_buffer_data [3*i] = red;
        color_buffer_data [3*i + 1] = green;
        color_buffer_data [3*i + 2] = blue;
    }

    return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
}

/* Render the VBOs handled by VAO */
void draw3DObject (struct VAO* vao)
{
    // Change the Fill Mode for this object
    glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

    // Bind the VAO to use
    glBindVertexArray (vao->VertexArrayID);

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray(0);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    // Enable Vertex Attribute 1 - Color
    glEnableVertexAttribArray(1);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    // Draw the geometry !
    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}

/**************************
 * Customizable functions *
 **************************/
float camera_rotation_angle = 90;
float rectangle_rotation = 0;
float triangle_rotation = 0;
float circle_rotation = 0;
int laser_count=0;
float box1=1,box2=-2,guny=0;
int k=0,collide=0;
float gun_rotate=0,bricks=4.0,laser_angle[10000]={0}, mirror_angle[4];
float mirror_cor[4][4]={0};
int total_score=0,game=0;
int brick_color[10000]={-1};
float intersection_x, intersection_y;
laser lasers[1000];
float triangle_rot_dir = 1;
float rectangle_rot_dir = 1;
float box1_translate=0;
float box2_translate=0;
float gun=0,flag=0;
bool triangle_rot_status = false;
bool rectangle_rot_status =false;
bool box1_status=false;
bool box2_status=false;
int present=0;
float current_time1,last_update_time1= glfwGetTime();
VAO *triangle, *rectangle[10009], *circle[10009], *mirror[5];
int rect_count=0,circle_count=0, mirror_count=0;
int disappear[100000]={0};
int zoom=0,flag_z=0;
float pan=0;
int misfire=0;
float speed=0.03;
int flag_box1=0,flag_box2=0;

VAO *createCircle (float radius,COLOR color)
{
  GLfloat vertex_buffer_data [360*9]={0};
  for(int i=0; i<3241; i++)
    vertex_buffer_data[i]=0;
  for(int i=0; i<360; i++)
  {
    vertex_buffer_data[9*i+3]=radius*(cos(i*M_PI/180));
    vertex_buffer_data[9*i+4]=radius*(sin(i*M_PI/180));
    vertex_buffer_data[9*i+6]=radius*(cos((i+1)*M_PI/180));
    vertex_buffer_data[9*i+7]=radius*(sin((i+1)*M_PI/180));
  }

  GLfloat color_buffer_data [32400]={0.6};
  for(int i=0; i<3240; i+=3)
  {
    color_buffer_data[i]=color.r;
    color_buffer_data[i+1]=color.g;
    color_buffer_data[i+2]=color.b;
  }
  VAO *temp;
  temp = create3DObject(GL_TRIANGLES,360*3, vertex_buffer_data, color_buffer_data, GL_FILL);
  return temp;
}
/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */

void lasercreation()
{
  current_time1 = glfwGetTime();
  if(current_time1 - last_update_time1 >=1.0)
  {
  // cout<<"ganti"<<"\n";
  lasers[laser_count].ray=createCircle(0.1,black);
  lasers[laser_count].angle=gun_rotate;
  lasers[laser_count].c=-3.875;
  lasers[laser_count].c1=guny;
  lasers[laser_count].speed=0;
  circle_count++;
  laser_count++;
  last_update_time1 = current_time1;
  system("canberra-gtk-play -f ./smb_fireball.wav");

  }
}
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
     // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_C:
                rectangle_rot_status = !rectangle_rot_status;
                break;
            case GLFW_KEY_P:
                triangle_rot_status = !triangle_rot_status;
                break;
            case GLFW_KEY_LEFT:
                if(box1_status == true)
                box1_translate=0;
                if(box2_status==true)
                box2_translate=0;
                break;
            case GLFW_KEY_RIGHT:
                if(box1_status == true)
                box1_translate=0;
                if(box2_status == true)
                box2_translate=0;
                break;
            case GLFW_KEY_H:
                box2_translate=0;
                break;
            case GLFW_KEY_K:
                box2_translate=0;
                break;
            case GLFW_KEY_W:
                gun=0;
                break;
            case GLFW_KEY_S:
                gun=0;
                break;
            case GLFW_KEY_UP:
                rectangle_rot_status=false;
                rectangle_rot_dir=0;
                break;
            case GLFW_KEY_DOWN:
                rectangle_rot_status=true;
                rectangle_rot_dir=0;
                break;
            case GLFW_KEY_RIGHT_ALT:
                box1_status =false;
                break;
            case GLFW_KEY_RIGHT_CONTROL:
                box2_status =false;
                break;
            case GLFW_KEY_SPACE:
                flag=0;
                break;


            default:
                break;
        }
    }
    else if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                quit(window);
                break;
            case GLFW_KEY_SPACE:
                lasercreation();
                break;
            case GLFW_KEY_RIGHT_ALT:
                box1_status =true;
                break;
            case GLFW_KEY_RIGHT_CONTROL:
                box2_status =true;
                break;
            case GLFW_KEY_LEFT:
                if(box1_status == true)
                box1_translate=-0.1;
                if(box2_status == true)
                box2_translate=-0.1;
                break;
            case GLFW_KEY_RIGHT:
                if(box1_status == true)
                box1_translate=0.1;
                if(box2_status == true)
                box2_translate=0.1;
                break;
            case GLFW_KEY_H:
                box2_translate=-0.1;
                break;
            case GLFW_KEY_K:
                box2_translate=0.1;
                break;
            case GLFW_KEY_W:
                gun=0.1;
                break;
            case GLFW_KEY_S:
                gun=-0.1;
                break;
            case GLFW_KEY_UP:
                rectangle_rot_status=true;
                rectangle_rot_dir=+1;
                break;
            case GLFW_KEY_DOWN:
                rectangle_rot_status=true;
                rectangle_rot_dir=-1;
                break;
            case GLFW_KEY_KP_ADD:
                if(zoom<3)
                {
                  zoom++;
                  flag_z=1;
                }
              break;
            case GLFW_KEY_KP_SUBTRACT:
                if(zoom)
                {
                  zoom--;
                  flag_z=1;
                }
              break;
            case GLFW_KEY_A:
                  if(zoom){
            			pan++;
            			flag_z=1;
            		}
              break;
            case GLFW_KEY_D:
              if(zoom){
            			pan--;
            			flag_z=1;
            		}
            case GLFW_KEY_N:
                if(speed>0)
                speed-=0.02;
                break;
            case GLFW_KEY_M:
                if(speed<0.08)
                speed+=0.02;
                break;
            default:
                break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
      switch (key) {
        case 'Q':
        case 'q':
        case 27: //ESC
            exit (0);
        default:
            break;

	}
}
int fbwidth=600,fbheight=600;
double mouse_x,mouse_y,m_click_x;
int m_flag0=0,m_flag1=0,m_flag2=0,m_flag3=0;
static void cursor_position(GLFWwindow* window, double xpos, double ypos)
{
	mouse_x= ((8*xpos)/fbwidth)-4;
	mouse_y=-((8*ypos)/fbheight)+4;
	if(m_flag0==1)
		box1 = mouse_x;
	if(m_flag1==1)
		box2 = mouse_x;
	if(m_flag2==1)
  {
    // cout<<"enter";
		guny = mouse_y;
  }
}

/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
  if(action == GLFW_RELEASE){
		if(button == GLFW_MOUSE_BUTTON_LEFT){
			m_flag0=0;
			m_flag1=0;
			m_flag2=0;
		}
		if(button == GLFW_MOUSE_BUTTON_RIGHT)
			m_flag3=0;
	}

	else if(action == GLFW_PRESS){
		if(button == GLFW_MOUSE_BUTTON_LEFT){
			if(mouse_x>=-4 && mouse_x<=-3 && mouse_y>=guny-0.5 && mouse_y<=guny+0.5)
				m_flag2=1;
			else if(mouse_x>=box1-0.5 && mouse_x<=0.5+box1 && mouse_y>=-4 && mouse_y<=-3)
				m_flag0=1;
			else if(mouse_x>=box2-0.5 && mouse_x<=box2+0.5 && mouse_y>=-4 && mouse_y<=-3)
				m_flag1=1;
			else if(mouse_x>=-3.875)
      {
				float slope = (mouse_y-guny)/(mouse_x+3.875);
				float anglee= (atan(slope)*180.0)/M_PI;
				if(anglee>=-60 && anglee<=60){
					// fire=glfwGetTime();
					 gun_rotate = anglee*M_PI/180.0;
           rectangle_rotation = anglee;
					 lasercreation();
				}
			}
		}
		if(button == GLFW_MOUSE_BUTTON_RIGHT){
			if(m_flag3==0)
				m_click_x = mouse_x;
			m_flag3=1;
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	zoom += yoffset;
	if(zoom>=8)
		zoom=7;
	if(zoom<0)
		zoom=0;
	if(pan>zoom)
		pan=zoom;
	if(pan<-zoom)
		pan=-zoom;
	Matrices.projection = glm::ortho(-4.0f+zoom-pan, 4.0f-zoom-pan, -4.0f+zoom, 4.0f-zoom, 0.1f, 500.0f);
}

/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
    int fbwidth=width, fbheight=height;
    /* With Retina display on Mac OS X, GLFW's FramebufferSize
     is different from WindowSize */
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

	GLfloat fov = 90.0f;

	// sets the viewport of openGL renderer
	glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

	// set the projection matrix as perspective
	/* glMatrixMode (GL_PROJECTION);
	   glLoadIdentity ();
	   gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
	// Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    // Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    Matrices.projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 0.1f, 500.0f);
}
// Creates the rectangle object used in this sample code
void createRectangle (GLfloat vx1, GLfloat vy1, GLfloat vx2, GLfloat vy2,GLfloat vx3,
                      GLfloat vy3, GLfloat vx4, GLfloat vy4, COLOR colorA, COLOR colorB,COLOR colorC, COLOR colorD,int choice)
{
  // GL3 accepts only Triangles. Quads are not supported
  GLfloat vertex_buffer_data [] = {

      vx1, vy1, 0,
      vx2, vy2, 0,
      vx3, vy3, 0,

      vx3, vy3, 0,
      vx4, vy4, 0,
      vx1, vy1, 0,
    };

  GLfloat color_buffer_data [] = {

      colorA.r,colorA.g,colorA.b, // color 1
      colorB.r,colorB.g,colorB.b, // color 2
      colorC.r,colorC.g,colorC.b, // color 3

      colorC.r,colorC.g,colorC.b, // color 4
      colorD.r,colorD.g,colorD.b, // color 5
      colorA.r,colorA.g,colorA.b // color 6
  };

  // create3DObject creates and returns a handle to a VAO that can be used later
    if(choice==1)
    mirror[mirror_count]= create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
    if(choice==0)
    {
    rectangle[rect_count] = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
    float xcor = rand()%7+(-3);
    rectangle[rect_count]->x=xcor;
    rectangle[rect_count]->y=4;
  }
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void reflection(float laser_x, float laser_y, float laser_angle, float mirror_x, float mirror_y, float mirror_angle)
{
  if(sin(laser_angle) == 1)
  return;
  float slope_laser = sin(laser_angle)/cos(laser_angle);
  float slope_mirror = sin(mirror_angle)/cos(laser_angle);
  if(slope_laser!=slope_mirror)
  {
    float c_mirror = mirror_y - slope_mirror*mirror_x;
    float c_laser = laser_y - slope_laser*laser_x;
    intersection_x = (c_laser - c_mirror)/(slope_mirror - slope_laser);
    intersection_y = slope_mirror*intersection_x + c_mirror;
  }
  else
  return;
}

int intersect_point(Point p1,Point p2,Point p4,Point p5){

  float x0=p1.x, y0=p1.y,x1=p2.x, y1=p2.y;
// Point p3;

    float s1_x, s1_y, s2_x, s2_y, x2=p4.x, y2=p4.y, x3=p5.x, y3=p5.y, q, p, r;

    s1_x = x1 - x0;
    s1_y = y1 - y0;
    s2_x = x3 - x2;
    s2_y = y3 - y2;

    r=s1_x*s2_y - s2_x*s1_y;
    if(r==0){
      // p3.va=0;
      return 0;
    }

    p = (s1_x*(y0-y2) - s1_y*(x0-x2))/(r*1.0f);
    q = (s2_x*(y0-y2) - s2_y*(x0-x2))/(r*1.0f);

    if (p>=0 && p<=1 && q>=0 && q<=1)
    {
        intersection_x = x0 + (q * s1_x);
        intersection_y = y0 + (q * s1_y);
        // p3.va=1;
        return 1;
    }
    // p3.va=0;
    return 0;
}
bool is_between(float mirror_x, float mirror_y, float x, float y)
{
  float distance;
  distance = sqrt((mirror_x - x)*(mirror_x - x) + (mirror_y - y)*(mirror_y - y));
  if(distance<=1.0)
  return true;
  else
  return false;
}
void draw ()
{
  // clear the color and depth in the frame buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // use the loaded shader program
  // Don't change unless you know what you are doing
  glUseProgram (programID);

  // Eye - Location of camera. Don't change unless you are sure!!
  glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
  // Target - Where is the camera looking at.  Don't change unless you are sure!!
  glm::vec3 target (0, 0, 0);
  // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
  glm::vec3 up (0, 1, 0);

  // Compute Camera matrix (view)
  // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
  //  Don't change unless you are sure!!
  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane

  // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
  //  Don't change unless you are sure!!
  glm::mat4 VP = Matrices.projection * Matrices.view;

  // Send our transformation to the currently bound shader, in the "MVP" uniform
  // For each model you render, since the MVP will be different (at least the M part)
  //  Don't change unless you are sure!!
  glm::mat4 MVP;	// MVP = Projection * View * Model

  // Load identity to model matrix
  Matrices.model = glm::mat4(1.0f);

  /* Render your scene */

  glm::mat4 translateTriangle = glm::translate (glm::vec3(-2.0f, 0.0f, 0.0f)); // glTranslatef
  glm::mat4 rotateTriangle = glm::rotate((float)(triangle_rotation*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
  glm::mat4 triangleTransform = translateTriangle * rotateTriangle;
  Matrices.model *= triangleTransform;
  MVP = VP * Matrices.model; // MVP = p * V * M

  //  Don't change unless you are sure!!
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

  // draw3DObject draws the VAO given to it using current MVP matrix
  // draw3DObject(triangle);

  // Pop matrix to undo transformations till last push matrix instead of recomputing model matrix
  // glPopMatrix ();
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translateMirror = glm::translate (glm::vec3(-1.5 , 3, 0));    // glTrmakanslatef
  glm::mat4 rotateMirror = glm::rotate((float)(95*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
  Matrices.model *= (translateMirror * rotateMirror);
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(mirror[0]);
  mirror[0]->x=-1.5;
  mirror[0]->y=3;
  mirror_angle[0]=95*M_PI/180.0;
  mirror_cor[0][0]=-1.5+0.5*cos(mirror_angle[0]);
  mirror_cor[0][1]=3+0.5*sin(mirror_angle[0]);
  mirror_cor[0][2]=-1.5-0.5*cos(mirror_angle[0]);
  mirror_cor[0][3]=3-0.5*sin(mirror_angle[0]);


  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translateMirror1 = glm::translate (glm::vec3(1.5 ,3, 0));    // glTrmakanslatef
  glm::mat4 rotateMirror1 = glm::rotate((float)(60*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
  Matrices.model *= (translateMirror1 * rotateMirror1);
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(mirror[1]);
  mirror[1]->x=1.5;
  mirror[1]->y=3;
  mirror_angle[1]=60*M_PI/180.0;
  mirror_cor[1][0]=1.5+0.5*cos(mirror_angle[1]);
  mirror_cor[1][1]=3+0.5*cos(mirror_angle[1]);
  mirror_cor[1][2]=1.5-0.5*cos(mirror_angle[1]);
  mirror_cor[1][3]=3-0.5*cos(mirror_angle[1]);


  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translateMirror2 = glm::translate (glm::vec3(1.5 , -2, 0));    // glTrmakanslatef
  glm::mat4 rotateMirror2 = glm::rotate((float)(120*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
  Matrices.model *= (translateMirror2 * rotateMirror2);
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(mirror[2]);
  mirror[2]->x=1.5;
  mirror[2]->y=-2;
  mirror_angle[2]=120*M_PI/180.0;
  mirror_cor[2][0]=1.5+0.5*cos(mirror_angle[2]);
  mirror_cor[2][1]=-2+0.5*sin(mirror_angle[2]);
  mirror_cor[2][2]=1.5-0.5*cos(mirror_angle[2]);
  mirror_cor[2][3]=-2-0.5*sin(mirror_angle[2]);



  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translateMirror3 = glm::translate (glm::vec3(0.5 , 0, 0));    // glTrmakanslatef
  glm::mat4 rotateMirror3 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
  Matrices.model *= (translateMirror3 * rotateMirror3);
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(mirror[3]);
  mirror[3]->x=0.5;
  mirror[3]->y=0;
  mirror_angle[3]=90*M_PI/180.0;
  mirror_cor[3][0]=0.5+0.5*cos(mirror_angle[3]);
  mirror_cor[3][1]=0+0.5*sin(mirror_angle[3]);
  mirror_cor[3][2]=0.5-0.5*cos(mirror_angle[3]);
  mirror_cor[3][3]=0-0.5*sin(mirror_angle[3]);


  Matrices.model = glm::mat4(1.0f);
  if( box1 >-3.35 && box1 < 3.35 )
  box1+=box1_translate;
  else if(box1<0)
  box1=-3.3;
  else
  box1=3.3;
  glm::mat4 translateRectangle = glm::translate (glm::vec3(box1, -3.5, 1));    // glTranslatef
  Matrices.model *= (translateRectangle);
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  rectangle[0]->x=box1;
  rectangle[0]->y=-3.5;
  // draw3DObject draws the VAO given to it using current MVP matrix
  draw3DObject(rectangle[0]);
  Matrices.model = glm::mat4(1.0f);
  if( box2 >-3.35 && box2 < 3.35 )
  box2+=box2_translate;
  else if(box2<0)
  box2=-3.3;
  else
  box2=3.3;
  glm::mat4 translateRectangle1 = glm::translate (glm::vec3(box2, -3.5, 1));    // glTranslatef
  Matrices.model *= (translateRectangle1);
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  rectangle[1]->x=box2;
  rectangle[1]->y=-3.5;
  // draw3DObject draws the VAO given to it using current MVP matrix
  draw3DObject(rectangle[1]);

  Matrices.model = glm::mat4(1.0f);
  if(guny<=3.5 && guny>=-2.5)
  guny+=gun;
  else if(guny>3.5)
  guny=3.5;
  else
  guny=-2.5;
  glm::mat4 translateRectangle2 = glm::translate (glm::vec3(-3.6, guny, 0));    // glTranslatef
  gun_rotate=rectangle_rotation*M_PI/180.0f;
  if(gun_rotate>1.0472/1.5 )//roatation not greater than **(some) degrees
  gun_rotate=1.0472/1.5;
  if(gun_rotate<-1.0472/1.5)
  gun_rotate=-1.0472/1.5;
  glm::mat4 rotateRectangle2 = glm::rotate((float)(gun_rotate), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
  Matrices.model *= (translateRectangle2 * rotateRectangle2);
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  // draw3DObject draws the VAO given to it using current MVP matrix
  rectangle[2]->x=-3.6;
  rectangle[2]->y=guny;
  draw3DObject(rectangle[2]);


  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translateCircle = glm::translate (glm::vec3(box1 , -3.25, 0));    // glTrmakanslatef
  glm::mat4 rotateCircle = glm::rotate((float)(75*M_PI/180.0f), glm::vec3(1,0,0)); // rotate about vector (-1,1,1)
  Matrices.model *= (translateCircle * rotateCircle);
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

  draw3DObject(circle[0]);

  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translateCircle1 = glm::translate (glm::vec3(box2 , -3.25, 0));    // glTranslatef
  glm::mat4 rotateCircle1 = glm::rotate((float)(75*M_PI/180.0f), glm::vec3(1,0,0)); // rotate about vector (-1,1,1)
  Matrices.model *= (translateCircle1 * rotateCircle1);
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

  draw3DObject(circle[1]);

  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translateCircle2 = glm::translate (glm::vec3(-4.0 , guny, 0));    // glTranslatef
  glm::mat4 rotateCircle2 = glm::rotate((float)(75*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
  Matrices.model *= (translateCircle2 * rotateCircle2);
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

  draw3DObject(circle[2]);

  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translateCircle3 = glm::translate (glm::vec3(-3.875 , guny, 0));    // glTranslatef
  glm::mat4 rotateCircle3 = glm::rotate((float)(75*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
  Matrices.model *= (translateCircle3 * rotateCircle3);
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

  draw3DObject(circle[3]);
  for(int i=0;i<laser_count;i++)
  {
    present=i;
    Matrices.model = glm::mat4(1.0f);
    // glm::mat4 translateCircle4 = glm::translate (glm::vec3(-3.875,guny, 0));    // glTranslatef
    lasers[i].x= lasers[i].c+(lasers[i].speed*cos(lasers[i].angle));
    lasers[i].y= (lasers[i].speed*sin(lasers[i].angle))+lasers[i].c1;
    glm::mat4 translateCircle4 = glm::translate (glm::vec3(lasers[i].x,lasers[i].y, 0));    // glTranslatef
    // glm::mat4 rotateCircle3 = glm::rotate((float)(75*M_PI/180.0f), glm::vec3(0,0,0)); // rotate about vector (-1,1,1)
    Matrices.model *= (translateCircle4);
    // present=i;
    // current_time=glfwGetTime();
    lasers[i].speed+=0.09;
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    if(lasers[i].flag==0)
    draw3DObject(lasers[i].ray);
    //Collision with block
    for(int j=0;j<4;j++)
    {
      if(lasers[i].flag==0)
      {
        Point p1,p2,p3,p4;
        p1.x=lasers[i].x+0.05*cos(lasers[i].angle);
        p1.y=lasers[i].y+0.05*sin(lasers[i].angle);
        p2.x=lasers[i].x-0.05*cos(lasers[i].angle);
        p2.y=lasers[i].y-0.05*sin(lasers[i].angle);
        p3.x=mirror_cor[j][0];
        p3.y=mirror_cor[j][1];
        p4.x=mirror_cor[j][2];
        p4.y=mirror_cor[j][3];
        // cout<<p1.x<<" "<<p1.y<<" "<<p2.x<<" "<<p2.y<<" "<<p3.x<<" "<<p3.y<<" "<<p4.x<<" "<<p4.y<<endl;
        if(intersect_point(p1,p2,p3,p4)==1)
        {
          lasers[i].c=intersection_x;
          lasers[i].c1=intersection_y;
          lasers[i].angle=(2*mirror_angle[j])-lasers[i].angle;
          lasers[i].speed=0.15f;
        }
      }
  }
  if(lasers[i].flag==0)
  {
    for(int y=3;y<rect_count;y++)
    {
      // if(out==0)
      {
      if(rectangle[y]->y>=-4)
      {
        if((rectangle[y]->x-0.125)>lasers[i].x && (rectangle[y]->y+0.25+0.1)>lasers[i].y &&
            ((rectangle[y]->y)-0.25-0.1)<lasers[i].y && (rectangle[y]->x-0.125)-0.125/2<(lasers[i].x))
            {
              // out=1;
              rectangle[y]->y=-8;
              lasers[i].flag=1;
              if(brick_color[y]==2)
                total_score+=10;
              else
                misfire+=1;
              break;
            }
      }
      }
    }
  }
}
  for(int i=3;i<rect_count;i++)
  {
    flag_box1=0;
    flag_box2=0;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 brick = glm::translate (glm::vec3(rectangle[i]->x,rectangle[i]->y, 0));
    rectangle[i]->y-=speed;    // glTranslatef
    if(rectangle[i]->y<=-3.0 && rectangle[i]->y>=-3.1)
    {
      if(rectangle[0]->x-0.5<=rectangle[i]->x && rectangle[i]->x<=rectangle[0]->x+0.5)
      {
        flag_box1=1;
      }
      if(rectangle[1]->x-0.5<=rectangle[i]->x && rectangle[i]->x<=rectangle[1]->x+0.5)
        {
          flag_box2=1;
        }
        if(flag_box1*flag_box2==0 && (flag_box1==1 || flag_box2==1))
        {
          if(flag_box2==1)
          {
          rectangle[i]->y=-8;
          if(brick_color[i]==1)
          total_score+=10;
          if(brick_color[i]==0)
          total_score-=5;
          if(brick_color[i]==2)
          {
              cout<<"score"<<endl<<total_score<<"\n";
              system("canberra-gtk-play -f ./smb_mariodie.wav");

              exit(0);
          }
        }
        else
        {
          rectangle[i]->y=-8;
          if(brick_color[i]==0)
          total_score+=10;
          if(brick_color[i]==1)
          total_score-=5;
          if(brick_color[i]==2)
          {
              cout<<"score"<<endl<<total_score<<"\n";
              system("canberra-gtk-play -f ./smb_mariodie.wav");

              exit(0);
          }

        }
      }

    }
    Matrices.model *= (brick);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(rectangle[i]);
  }
  // Increment angles
  float increments = 1;

  //camera_rotation_angle++; // Simulating camera rotation
  triangle_rotation = triangle_rotation + increments*triangle_rot_dir*triangle_rot_status;
  rectangle_rotation = rectangle_rotation + increments*rectangle_rot_dir*rectangle_rot_status;
}

/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
GLFWwindow* initGLFW (int width, int height)
{
    GLFWwindow* window; // window desciptor/handle

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
//        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", NULL, NULL);

    if (!window) {
        glfwTerminate();
//        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval( 1 );

    /* --- register callbacks with GLFW --- */

    /* Register function to handle window resizes */
    /* With Retina display on Mac OS X GLFW's FramebufferSize
     is different from WindowSize */
    glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);

    /* Register function to handle keyboard input */
    /* Register function to handle window close */
    glfwSetWindowCloseCallback(window, quit);

    glfwSetKeyCallback(window, keyboard);      // general keyboard input
    glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

    /* Register function to handle mouse click */
    glfwSetCursorPosCallback(window, cursor_position);
    glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks
    glfwSetScrollCallback(window, scroll_callback);


    return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */

void initGL (GLFWwindow* window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
	// Create the models
	createRectangle (-0.65/2, -0.5/2, 0.65/2, -0.5/2, 1.0/2, 0.5/2,-1.0/2, 0.5/2,lightgreen,lightgreen,lightgreen,lightgreen,0 );
  rect_count++;
  createRectangle (-0.65/2, -0.5/2, 0.65/2, -0.5/2, 1.0/2, 0.5/2,-1.0/2, 0.5/2,lightpink,lightpink,lightpink,lightpink,0 );
  rect_count++;
  createRectangle (-0.5/2, -0.155/2, 0.5/2, -0.155/2, 0.5/2, 0.155/2,-0.5/2, 0.155/2,lightbrown,lightbrown,lightbrown,lightbrown,0 );
  rect_count++;
  circle[circle_count]=createCircle(1.0/2,darkgreen);
  circle_count++;
  circle[circle_count]=createCircle(1.0/2,red);
  circle_count++;
  circle[circle_count]=createCircle(0.35,grey);
  circle_count++;
  circle[circle_count]=createCircle(0.125,gold);
  circle_count++;
  createRectangle (-0.5,-0.125/2,0.5,-0.125/2,0.5,0.125/2,-0.5,0.125/2,skyblue,skyblue,skyblue,skyblue,1);
  mirror_count++;
  createRectangle (-0.5,-0.125/2,0.5,-0.125/2,0.5,0.125/2,-0.5,0.125/2,skyblue,skyblue,skyblue,skyblue,1);
  mirror_count++;
  createRectangle (-0.5,-0.125/2,0.5,-0.125/2,0.5,0.125/2,-0.5,0.125/2,skyblue,skyblue,skyblue,skyblue,1);
  mirror_count++;
  createRectangle (-0.5,-0.125/2,0.5,-0.125/2,0.5,0.125/2,-0.5,0.125/2,skyblue,skyblue,skyblue,skyblue,1);
  mirror_count++;
  programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

    // Background color of the scene
	glClearColor (229/255.0,255/255.0,255/255.0, 0.0f); // R, G, B, A
	glClearDepth (1.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main (int argc, char** argv)
{
	int width = 600;
	int height = 600;
  // playSound("/home/lostsymbol/Downloads/smb_world_clear.wav",NULL,SND_FILENAME);


    GLFWwindow* window = initGLFW(width, height);

	  initGL (window, width, height);
    // randnum();

    double last_update_time = glfwGetTime(), current_time;



    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {

        // OpenGL Draw commands
        draw();


        // Swap Frame Buffer in double buffering

        glfwSwapBuffers(window);
        if(flag_z)
        {
          if(pan>zoom)
            pan=zoom;
          if(pan<-zoom)
            pan=-zoom;
          Matrices.projection = glm::ortho(-4.0f+zoom-pan, 4.0f-zoom-pan, -4.0f+zoom,4.0f-zoom, 0.1f, 500.0f);
        }

        if(m_flag3 && zoom>0){
      		pan -= (m_click_x - mouse_x);
      		m_click_x = mouse_x;
      		if(pan>zoom)
      			pan=zoom;
      		if(pan<-zoom)
      			pan=-zoom;
      		Matrices.projection = glm::ortho(-4.0f+zoom-pan, 4.0f-zoom-pan, -4.0f+zoom, 4.0f-zoom, 0.1f, 500.0f);
      	}
        if(misfire>5)
        {
          cout<<"score"<<endl<<total_score<<"\n";
          system("canberra-gtk-play -f ./smb_mariodie.wav");
          exit(0);
        }
        // Poll for Keyboard and mouse events
        glfwPollEvents();

        // Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
        current_time = glfwGetTime();     // Time in seconds
        if ((current_time - last_update_time) >= 0.7) { // atleast 0.5s elapsed since last frame
            // do something every 0.5 seconds ..
            int k=rand()%3;
            brick_color[rect_count]=k;
            if(k==2)
            createRectangle (0.125, 0.25, 0.125, -0.25,-0.125, -0.25, -0.125, 0.25,black,black,black,black,0);
            if(k==1)
            createRectangle (0.125, 0.25, 0.125, -0.25,-0.125, -0.25, -0.125, 0.25,red,red,red,red,0);
            if(k==0)
            createRectangle (0.125, 0.25, 0.125, -0.25,-0.125, -0.25, -0.125, 0.25,lightgreen,lightgreen,lightgreen,lightgreen,0);
            rect_count++;
            last_update_time = current_time;
          }
  }
    glfwTerminate();
//    exit(EXIT_SUCCESS);
}
