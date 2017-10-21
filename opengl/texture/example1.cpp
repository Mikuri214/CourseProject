#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/glut.h"
#include "LoadShaders.h"
using namespace glm;

#define BUFFER_OFFSET( x ) ( (GLvoid*)( 0 + x))

const int  NumTriangles = 12;
const int  NumVertices  = 3 * NumTriangles;
const int  TextureSize  = 64;

typedef vec4 point4;
typedef vec4 color4;

GLuint textures[2];

GLubyte image[TextureSize][TextureSize][3];
GLubyte image2[TextureSize][TextureSize][3];


point4  points[NumVertices];
color4  quad_colors[NumVertices];
vec2    tex_coords[NumVertices];

enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
GLuint   theta;

//----------------------------------------------------------------------------

int Index = 0;

void
quad( int a, int b, int c, int d )
{
    point4 vertices[8] = {
	point4( -0.5, -0.5,  0.5, 1.0 ),
	point4( -0.5,  0.5,  0.5, 1.0 ),
	point4(  0.5,  0.5,  0.5, 1.0 ),
	point4(  0.5, -0.5,  0.5, 1.0 ),
	point4( -0.5, -0.5, -0.5, 1.0 ),
	point4( -0.5,  0.5, -0.5, 1.0 ),
	point4(  0.5,  0.5, -0.5, 1.0 ),
	point4(  0.5, -0.5, -0.5, 1.0 )
    };

    color4 colors[8] = {
	color4( 0.0, 0.0, 0.0, 1.0 ),  // black
	color4( 1.0, 0.0, 0.0, 1.0 ),  // red
	color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
	color4( 0.0, 1.0, 0.0, 1.0 ),  // green
	color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
	color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
	color4( 1.0, 1.0, 1.0, 1.0 ),  // white
	color4( 1.0, 1.0, 1.0, 1.0 )   // cyan
    };

    quad_colors[Index] = colors[a];
    points[Index] = vertices[a];
    tex_coords[Index] = vec2( 0.0, 0.0 );
    Index++;
    
    quad_colors[Index] = colors[a];
    points[Index] = vertices[b];
    tex_coords[Index] = vec2( 0.0, 1.0 );
    Index++;
    
    quad_colors[Index] = colors[a];
    points[Index] = vertices[c];
    tex_coords[Index] = vec2( 1.0, 1.0 );
    Index++;
    
    quad_colors[Index] = colors[a];
    points[Index] = vertices[a];
    tex_coords[Index] = vec2( 0.0, 0.0 );
    Index++;
    
    quad_colors[Index] = colors[a];
    points[Index] = vertices[c];
    tex_coords[Index] = vec2( 1.0, 1.0 );
    Index++;
    
    quad_colors[Index] = colors[a];
    points[Index] = vertices[d];
    tex_coords[Index] = vec2( 1.0, 0.0 );
    Index++;
}

//----------------------------------------------------------------------------

void
colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

//----------------------------------------------------------------------------

void
init()
{
    colorcube();

    for ( int i = 0; i < 64; i++ ) {
        for ( int j = 0; j < 64; j++ ) {
            GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8)  == 0)) * 255;
            image[i][j][0]  = c;
            image[i][j][1]  = c;
            image[i][j][2]  = c;
            image2[i][j][0] = c;
            image2[i][j][1] = 0;
            image2[i][j][2] = c;
        }
    }

    glGenTextures( 2, textures );

    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0,
		  GL_RGB, GL_UNSIGNED_BYTE, image );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glBindTexture( GL_TEXTURE_2D, textures[1] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0,
		  GL_RGB, GL_UNSIGNED_BYTE, image2 );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, textures[0] );

    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER,
		  sizeof(points) + sizeof(quad_colors) + sizeof(tex_coords),
		  NULL, GL_STATIC_DRAW );

    GLintptr offset = 0;
    glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof(points), points );
    offset += sizeof(points);

    glBufferSubData( GL_ARRAY_BUFFER, offset,
		     sizeof(quad_colors), quad_colors );
    offset += sizeof(quad_colors);
    
    glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof(tex_coords), tex_coords );

	ShaderInfo shaders[] = { {GL_VERTEX_SHADER,"vshader.glsl"},{GL_FRAGMENT_SHADER,"fshader.glsl"},{GL_NONE,NULL} };
    GLuint program = LoadShaders(shaders );
    glUseProgram( program );


    offset = 0;
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(offset) );
    offset += sizeof(points);

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(offset) );
    offset += sizeof(quad_colors);

    GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(offset) );

    glUniform1i( glGetUniformLocation(program, "texture"), 0 );

    theta = glGetUniformLocation( program, "theta" );
    
    glEnable( GL_DEPTH_TEST );
    
    glClearColor( 0.2, 0.3,0.4, 1.0 );
}

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUniform3fv( theta, 1, Theta );

    glDrawArrays( GL_TRIANGLES, 0, NumVertices );

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
	switch( button ) {
	    case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
	    case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
	    case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
	}
    }
}

//----------------------------------------------------------------------------

void idle( void )
{
    Theta[Axis] += 1;
	
    if ( Theta[Axis] > 360.0 ) {
	Theta[Axis] -= 360.0;
    }
    
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int mousex, int mousey )
{
    switch( key ) {
	case 033: 
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
	case '1':
	    glBindTexture( GL_TEXTURE_2D, textures[0] );
	    break;

	case '2':
	    glBindTexture( GL_TEXTURE_2D, textures[1] );
	    break;
    }

    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutCreateWindow( "Color Cube" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );

    glutMainLoop();
    return 0;
}
