#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>

static GLboolean should_rotate = GL_TRUE;

#include<stdio.h>
#include<SDL/SDL.h>

#ifdef __EMSCRIPTEN__
#include<emscripten.h>
#endif

#define XLEN 512
#define YLEN 512

static float angle_y=0.0;
static float angle_z=0.0;

int forward;
int autodelay;

void setAngles(const double ucy,const double ucz){
	angle_y=ucy;
	angle_z=ucz;
}

void setDelay(const int delay){
	autodelay=delay;
}

void checkbox(){
	should_rotate = !should_rotate;
}

static void quit_tutorial( int code )
{
    SDL_Quit( );
    exit( code );
}

static void handle_key_down(SDL_keysym* keysym){
	switch( keysym->sym ) {
		case SDLK_ESCAPE:
			quit_tutorial( 0 );
		break;
		case SDLK_SPACE:
			should_rotate = !should_rotate;
		break;
		case SDLK_UP:
			angle_z++;
		break;
		case SDLK_DOWN:
			angle_z--;
		break;
		case SDLK_RIGHT:
			angle_y++;
		break;
		case SDLK_LEFT:
			angle_y--;
		break;
		default:
		break;
	}
}

static void process_events( void )
{
    /* Our SDL event placeholder. */
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
            quit_tutorial( 0 );
            break;
        }

    }

}

static void draw_screen( void ){
    /*
     * EXERCISE:
     * Replace this awful mess with vertex
     * arrays and a call to glDrawElements.
     *
     * EXERCISE:
     * After completing the above, change
     * it to use compiled vertex arrays.
     *
     * EXERCISE:
     * Verify my windings are correct here ;).
     */
    static GLfloat v0[] = { -1.0f, -1.0f,  1.0f };
    static GLfloat v1[] = {  1.0f, -1.0f,  1.0f };
    static GLfloat v2[] = {  1.0f,  1.0f,  1.0f };
    static GLfloat v3[] = { -1.0f,  1.0f,  1.0f };
    static GLfloat v4[] = { -1.0f, -1.0f, -1.0f };
    static GLfloat v5[] = {  1.0f, -1.0f, -1.0f };
    static GLfloat v6[] = {  1.0f,  1.0f, -1.0f };
    static GLfloat v7[] = { -1.0f,  1.0f, -1.0f };
    static GLubyte red[]    = { 255,   0,   0, 255 };
    static GLubyte green[]  = {   0, 255,   0, 255 };
    static GLubyte blue[]   = {   0,   0, 255, 255 };
    static GLubyte white[]  = { 255, 255, 255, 255 };
    static GLubyte yellow[] = {   0, 255, 255, 255 };
    static GLubyte black[]  = {   0,   0,   0, 255 };
    static GLubyte orange[] = { 255, 255,   0, 255 };
    static GLubyte purple[] = { 255,   0, 255,   0 };

    /* Clear the color and depth buffers. */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* We don't want to modify the projection matrix. */
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    /* Move down the z-axis. */
    glTranslatef( 0.0, 0.0, -5.0 );

    /* Rotate. */
    glRotatef(angle_y,0.0,1.0,0.0);
    glRotatef(angle_z,0.0,0.0,1.0);

    if( should_rotate ) {
        if( ++angle_y >= 360.0f ) {
            angle_y = 0.0f;
        }
    }

    /* Send our triangle data to the pipeline. */
    glBegin( GL_TRIANGLES );

    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( blue );
    glVertex3fv( v2 );

    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( blue );
    glVertex3fv( v2 );
    glColor4ubv( white );
    glVertex3fv( v3 );

    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( black );
    glVertex3fv( v5 );
    glColor4ubv( orange );
    glVertex3fv( v6 );

    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( orange );
    glVertex3fv( v6 );
    glColor4ubv( blue );
    glVertex3fv( v2 );

    glColor4ubv( black );
    glVertex3fv( v5 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( purple );
    glVertex3fv( v7 );

    glColor4ubv( black );
    glVertex3fv( v5 );
    glColor4ubv( purple );
    glVertex3fv( v7 );
    glColor4ubv( orange );
    glVertex3fv( v6 );

    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( white );
    glVertex3fv( v3 );

    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( purple );
    glVertex3fv( v7 );

    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( blue );
    glVertex3fv( v2 );
    glColor4ubv( orange );
    glVertex3fv( v6 );

    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( orange );
    glVertex3fv( v6 );
    glColor4ubv( purple );
    glVertex3fv( v7 );

    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );

    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( black );
    glVertex3fv( v5 );

    glEnd( );

    /*
     * Swap the buffers. This this tells the driver to
     * render the next frame from the contents of the
     * back-buffer, and to set all rendering operations
     * to occur on what was the front-buffer.
     *
     * Double buffering prevents nasty visual tearing
     * from the application drawing on areas of the
     * screen that are being updated at the same time.
     */
    SDL_GL_SwapBuffers( );
}

void main_loop(){
	process_events();
	draw_screen();
}

static void setup_opengl( int width, int height ){
    float ratio = (float) width / (float) height;
    /* Our shading model--Gouraud (smooth). */
    glShadeModel( GL_SMOOTH );
    /* Culling. */
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
    glClearColor( 0, 0, 0, 0 );
    glViewport( 0, 0, width, height );
    /*
     * Change to the projection matrix and set
     * our viewing volume.
     */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    /*
     * EXERCISE:
     * Replace this with a call to glFrustum.
     */
    gluPerspective( 60.0, ratio, 1.0, 1024.0 );
}

static void setup_sdl_window(int width,int height){
	const SDL_VideoInfo* info;
	int bpp = 0;	/* Color depth in bits of our window. */
	int flags = 0;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf("Video initialization failed: %s\n", SDL_GetError( ) );
		quit_tutorial( 1 );
	}

	info = SDL_GetVideoInfo( );

	if( !info ) {
		printf("Video query failed: %s\n", SDL_GetError( ) );
		quit_tutorial( 1 );
	}

	bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	/*
	 * EXERCISE:
	 * Make starting windowed an option, and
	 * handle the resize events properly with
	 * glViewport.
	 */
	//flags = SDL_OPENGL | SDL_FULLSCREEN;
	flags = SDL_OPENGL;
	if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) {
		// DISPLAY not set, the specified resolution not available?
		printf("Video mode set failed: %s\n",	 SDL_GetError( ) );
		quit_tutorial( 1 );
	}

}

int main(){
	int width = 0;
	int height = 0;

	width = XLEN;
	height = YLEN;
	setup_sdl_window(width,height);
	setup_opengl( width, height );

	emscripten_set_main_loop(main_loop,0,0);
//	emscripten_set_main_loop_timing(EM_TIMING_RAF,10);
	printf("Rotating 3D cube written in C using SDL and OpenGL.  Converted to Javascript through Emscripten.\n");
	printf("Press spacebar to pause rotation.\n");
	printf("Press up/down to rotate up/down.\n");
	printf("Press left/right to rotate left/right.\n");
	return 0;
}
