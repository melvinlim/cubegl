#include<GL/glew.h>
//#include<GL/gl.h>
//#include<GL/glu.h>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/string_cast.hpp>
#include<glm/ext.hpp>
#include"shader.hpp"

#include<iostream>

#include <stdlib.h>
static GLboolean autoRotate=GL_TRUE;

#include<stdio.h>
#include<SDL/SDL.h>
SDL_Window *window;

#ifdef __EMSCRIPTEN__
#include<emscripten.h>
#endif

#define XLEN 512
#define YLEN 512
using namespace glm;

static float reqAngleY=0.0;
static float reqAngleZ=0.0;

static float currentAngleY=0.0;
static float currentAngleZ=0.0;

int forward;
int autodelay;

extern "C"{
	void setAngles(const double ucy,const double ucz){
		reqAngleY=ucy;
		reqAngleZ=ucz;
	}

	void setDelay(const int delay){
		autodelay=delay;
	}

	void checkbox(){
		autoRotate = !autoRotate;
	}
}

static const GLfloat vertexPositions[]={
	-1,-1,-1,
	+1,-1,-1,
	+1,+1,-1,
	-1,+1,-1,
	-1,-1,+1,
	-1,+1,+1,
	+1,+1,+1,
	+1,-1,+1,
};

static GLuint elements[]={
	0,1,2,
	2,3,0,
	0,3,4,
	4,3,5,
	5,6,4,
	6,7,4,
	4,7,0,
	0,7,1,
	1,7,6,
	6,2,1,
	3,2,6,
	6,5,3,
};

static const GLfloat vertexColors[]={
	1,0,0,1,
	0,1,0,1,
	0,0,1,1,
	0,1,1,1,
	1,1,0,1,
	1,0,1,1,
	1,0,0,1,
	0,1,1,1,
};

GLuint elementsBuf;
GLuint vertexBuf;
GLuint colorBuffer;
GLuint program;
int width=XLEN;
int height=YLEN;
glm::mat4 mvp;
glm::mat4 Projection;
glm::mat4 View;
mat4 identityMat;
mat4 cameraRotation;
vec3 cameraPos;
void initScene(){
//FOV,aspect ratio,znear,zfar
Projection = glm::perspective(glm::radians(45.0f),(float)width/(float)height,0.1f,100.0f);
//ortho camera :
//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
identityMat=mat4(1.0f);
cameraRotation=identityMat;
//cameraPos=vec3(4,3,3); 
cameraPos=vec3(0,0,-4); 
 
// Camera matrix
View = glm::lookAt(
    cameraPos,
    glm::vec3(0,0,0), // and looks at the origin
    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
  
// Model matrix : an identity matrix (model will be at the origin)
mat4 Model = glm::mat4(1.0f);
mvp = Projection * View * Model;
}
void updateScene(){
	GLuint MatrixID;


	if(autoRotate){
		if(reqAngleY!=currentAngleY){
			cameraRotation=rotate(cameraRotation,radians(reqAngleY-currentAngleY),vec3(0,1,0));
			currentAngleY=reqAngleY;
		}
		if(reqAngleZ!=currentAngleZ){
			cameraRotation=rotate(cameraRotation,radians(reqAngleZ-currentAngleZ),vec3(0,0,1));
			currentAngleZ=reqAngleZ;
		}
		cameraRotation=rotate(cameraRotation,radians(1.0f),vec3(1,0,0));
		//cameraRotation=rotate(cameraRotation,radians(5.0f),vec3(0,0,1));
//		std::cout<<glm::to_string(cameraRotation)<<std::endl;

		MatrixID = glGetUniformLocation(program,"cameraRot");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, value_ptr(cameraRotation));
	}
//	MatrixID = glGetUniformLocation(program,"cameraRot");
//	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, value_ptr(cameraRotation));

	MatrixID = glGetUniformLocation(program,"viewPort");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, value_ptr(mvp));
}

void initGL(){
	program=glCreateProgram();

	glGenBuffers(1,&elementsBuf);
	glGenBuffers(1,&vertexBuf);
	glGenBuffers(1,&colorBuffer);

	initScene();
	loadShaders(program);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementsBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements),elements,GL_STATIC_DRAW);

	glEnableVertexAttribArray(glGetAttribLocation(program,"vertexBuf"));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuf);
	//glBindAttribLocation(program,0,"vertexBuf");
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(
		glGetAttribLocation(program,"vertexBuf"),
		3,						//coordinates per vertex
		GL_FLOAT,			//type
		GL_FALSE,			//normalized?
		0,						//stride
		0							//buffer offset
	);

	glEnableVertexAttribArray(glGetAttribLocation(program,"colorBuffer"));

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	//glBindAttribLocation(program,1,"colorBuffer");
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
	glVertexAttribPointer(
		glGetAttribLocation(program,"colorBuffer"),
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		0 			                          // array buffer offset
	);

//	glDisableVertexAttribArray(0);
}

void draw2(){
	glClearColor(0,0,0.5,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	updateScene();
	glUseProgram(program);
	//glDrawArrays(GL_TRIANGLES,0,3);
	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
	//SDL_GL_SwapWindow(window);
	SDL_GL_SwapBuffers();
}

static void handle_key_down(SDL_keysym* keysym){
	switch( keysym->sym ) {
		case SDLK_ESCAPE:
			SDL_Quit( );
		break;
		case SDLK_SPACE:
			autoRotate = !autoRotate;
		break;
		case SDLK_UP:
			reqAngleZ++;
		break;
		case SDLK_DOWN:
			reqAngleZ--;
		break;
		case SDLK_RIGHT:
			reqAngleY++;
		break;
		case SDLK_LEFT:
			reqAngleY--;
		break;
		default:
		break;
	}
}

static void process_events( void )
{
    SDL_Event event;

    /* Grab all the events off the queue. */
    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
        case SDL_KEYDOWN:
            /* Handle key presses. */
            handle_key_down( &event.key.keysym );
            break;
        case SDL_QUIT:
            /* Handle quit requests (like Ctrl-c). */
						SDL_Quit( );
            break;
        }
    }
}

void main_loop(){
	process_events();
//	draw_screen();
	draw2();
}

static void setup_opengl( int width, int height ){
    float ratio = (float) width / (float) height;
    //glShadeModel( GL_SMOOTH );
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
    glClearColor( 0, 0, 0, 0 );
    glViewport( 0, 0, width, height );
    /*
     * Change to the projection matrix and set
     * our viewing volume.
     */
    //glMatrixMode( GL_PROJECTION );
    //glLoadIdentity( );
    /*
     * EXERCISE:
     * Replace this with a call to glFrustum.
     */
//    gluPerspective( 60.0, ratio, 1.0, 1024.0 );
}

static void setup_sdl_window(int width,int height){
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf("Video initialization failed: %s\n", SDL_GetError( ) );
	}
/*
//SDL2 initialization.  requireds -s USE_SDL=2 in makefile.
	window = SDL_CreateWindow("OpenGL",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		XLEN, YLEN,
		SDL_WINDOW_OPENGL);
	//	SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_GL_CreateContext(window);
*/
	const SDL_VideoInfo* info;
	int bpp = 0;	/* Color depth in bits of our window. */
	int flags = 0;
	info = SDL_GetVideoInfo( );

	if( !info ) {
		printf("Video query failed: %s\n", SDL_GetError( ) );
	}
	bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	//flags = SDL_OPENGL | SDL_FULLSCREEN;
	flags = SDL_OPENGL;
	if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) {
		// DISPLAY not set, the specified resolution not available?
		printf("Video mode set failed: %s\n",	 SDL_GetError( ) );
	}
/*
GLEW init.  only required for GLEW extensions.
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		printf("Error: glewInit: %s\n",glewGetErrorString(glew_status));
	}
*/
}

int main(){
	int width;
	int height;

	width = XLEN;
	height = YLEN;
	setup_sdl_window(width,height);
	setup_opengl(width,height);
	initGL();

	emscripten_set_main_loop(main_loop,0,0);
//	emscripten_set_main_loop_timing(EM_TIMING_RAF,10);
	printf("Rotating 3D cube written in C using SDL and OpenGL.  Converted to Javascript through Emscripten.\n");
	printf("Press spacebar to pause rotation.\n");
	printf("Press up/down to rotate up/down.\n");
	printf("Press left/right to rotate left/right.\n");
	return 0;
}
