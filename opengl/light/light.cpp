
// GLUT header
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LoadShaders.h"

using namespace glm;
using namespace std;

#define BUFFER_OFFSET( x )  ( (GLvoid*)x)

const int  NumTriangles = 12;
const int  NumVertices = 3 * NumTriangles;
const int  TextureSize = 64;

typedef vec4 point4;
typedef vec4 Material;

const GLuint WIDTH = 800, HEIGHT = 600;

GLuint r = 2;
GLfloat theta = 0,alpha = 0,zoom = 45.0f;

GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

GLuint VAO;
GLuint VBO;
GLuint program;
GLuint textures[ 2 ];
point4  points[ NumVertices ];
vec3  normals[ NumVertices ];
vec2    tex_coords[ NumVertices ];
GLubyte image[ TextureSize ][ TextureSize ][ 3 ];
GLubyte image2[ TextureSize ][ TextureSize ][ 3 ];
GLint modelLoc, viewLoc, projLoc,viewPosLoc;// uniform location


vec3 lightDir[3] = { vec3 ( 0.0f, 0.0f, -1.0f ), vec3 (0.0f,-1.0f,0.0f), vec3 (-1.0f,0.0f,0.0f) };
Material materials[ 6 ] = {
	Material( 1.0, 0.0, 0.0, 1.0 ),
	Material( 0.0, 1.0, 0.0, 1.0 ),
	Material( 0.0, 0.0, 1.0, 1.0 ),
	Material( 0.0, 0.0, 0.0, 1.0 ),
	Material( 0.0, 0.0, 0.0, 1.0 ),
	Material( 0.0, 0.0, 0.0, 1.0 ),
};

int Index = 0;

void quad( int a, int b, int c, int d )
{
	point4 vertices[ 8 ] = {
		point4( -0.5, -0.5,  0.5, 1.0 ),
		point4( -0.5,  0.5,  0.5, 1.0 ),
		point4( 0.5,  0.5,  0.5, 1.0 ),
		point4( 0.5, -0.5,  0.5, 1.0 ),
		point4( -0.5, -0.5, -0.5, 1.0 ),
		point4( -0.5,  0.5, -0.5, 1.0 ),
		point4( 0.5,  0.5, -0.5, 1.0 ),
		point4( 0.5, -0.5, -0.5, 1.0 )
	};



	vec3 normal = normalize(vec3( ( vertices[ a ] + vertices[ b ] ) + ( vertices[ c ] + vertices[ d ] ) ));


	points[ Index ] = vertices[ a ];
	normals[ Index ] = normal;
	tex_coords[ Index ] = vec2( 0.0, 0.0 );
	Index++;

	points[ Index ] = vertices[ b ];
	normals[ Index ] = normal;
	tex_coords[ Index ] = vec2( 0.0, 1.0 );
	Index++;

	points[ Index ] = vertices[ c ];
	normals[ Index ] = normal;
	tex_coords[ Index ] = vec2( 1.0, 1.0 );
	Index++;

	points[ Index ] = vertices[ a ];
	normals[ Index ] = normal;
	tex_coords[ Index ] = vec2( 0.0, 0.0 );
	Index++;

	points[ Index ] = vertices[ c ];
	normals[ Index ] = normal;
	tex_coords[ Index ] = vec2( 1.0, 1.0 );
	Index++;

	points[ Index ] = vertices[ d ];
	normals[ Index ] = normal;
	tex_coords[ Index ] = vec2( 1.0, 0.0 );
	Index++;
}

//----------------------------------------------------------------------------

void colorcube( )
{
	quad( 1, 0, 3, 2 );
	quad( 2, 3, 7, 6 );
	quad( 6, 5, 1, 2 );
	quad( 3, 0, 4, 7 );
	quad( 4, 5, 6, 7 );
	quad( 5, 4, 0, 1 );
}

//----------------------------------------------------------------------------



void init ()
{
	ShaderInfo shaders[] = { { GL_VERTEX_SHADER, "triangle.vert" }, { GL_FRAGMENT_SHADER, "triangle.frag" }, { GL_NONE, NULL } };
	program = LoadShaders ( shaders );
	glUseProgram ( program );

	colorcube( );
	//gen texture image
	for ( int i = 0; i < 64; i++ ) {
		for ( int j = 0; j < 64; j++ ) {
			GLubyte c = ( ( ( i & 0x8 ) == 0 ) ^ ( ( j & 0x8 ) == 0 ) ) * 255;
			image[ i ][ j ][ 0 ] = c;
			image[ i ][ j ][ 1 ] = c;
			image[ i ][ j ][ 2 ] = c;
			image2[ i ][ j ][ 0 ] = c;
			image2[ i ][ j ][ 1 ] = 0;
			image2[ i ][ j ][ 2 ] = c;
		}
	}
	//init texture
	glGenTextures( 2, textures );

	glBindTexture( GL_TEXTURE_2D, textures[ 0 ] );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0,
				  GL_RGB, GL_UNSIGNED_BYTE, image );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glBindTexture( GL_TEXTURE_2D, textures[ 1 ] );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0,
				  GL_RGB, GL_UNSIGNED_BYTE, image2 );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, textures[ 0 ] );
	//--------------------------------------------------------------
	glGenVertexArrays ( 1, &VAO );
	glGenBuffers ( 1, &VBO );

	glBindVertexArray ( VAO );
	
	glBindBuffer ( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( points ) + sizeof( materials ) + sizeof( tex_coords ) + sizeof( normals ),
				  NULL, GL_STATIC_DRAW );
	GLintptr offset = 0;
	glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof( points ), points );
	offset += sizeof( points );

	glBufferSubData( GL_ARRAY_BUFFER, offset,
					 sizeof( normals ), normals );
	offset += sizeof( normals );

	glBufferSubData( GL_ARRAY_BUFFER, offset,
					 sizeof( materials ), materials);
	offset += sizeof( materials );

	glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof( tex_coords ), tex_coords );
	//----------------------------------------------------------------------------------

	offset = 0;
	glVertexAttribPointer ( 0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( offset ) );
	glEnableVertexAttribArray ( 0 );
	offset += sizeof( points );

	glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET( offset ) );
	glEnableVertexAttribArray ( 1 );
	offset += sizeof( normals );

	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET( offset ) );
	glVertexAttribDivisor( 2, 0 );
	offset += sizeof( materials );

	glEnableVertexAttribArray( 3 );
	glVertexAttribPointer( 3, 2, GL_FLOAT, GL_FALSE, 0,
						   BUFFER_OFFSET( offset ) );
	glUniform1i( glGetUniformLocation( program, "texturer" ), 0 );

	glBindVertexArray ( 0 );

	//light 1   
	glUniform3f ( glGetUniformLocation ( program, "light[0].direction" ), lightDir[0].x, lightDir[0].y, lightDir[0].z );
	glUniform3f ( glGetUniformLocation ( program, "light[0].ambient" ), 0.0f, 0.0f, 0.0f );
	glUniform3f ( glGetUniformLocation ( program, "light[0].diffuse" ), 1.0f, 1.f, 1.f );
	glUniform3f ( glGetUniformLocation ( program, "light[0].specular" ), 1.0f, 0.0f, 0.0f );

	//light 2    
	glUniform3f ( glGetUniformLocation ( program, "light[1].direction" ), lightDir[1].x, lightDir[1].y, lightDir[1].z );
	glUniform3f ( glGetUniformLocation ( program, "light[1].ambient" ), 0.0f, 0.0f, 0.0f );
	glUniform3f ( glGetUniformLocation ( program, "light[1].diffuse" ), 1.0f, 1.0f, 1.0f );
	glUniform3f ( glGetUniformLocation ( program, "light[1].specular" ), 0.0f, 1.0f, 0.0f );

	//light 3
	glUniform3f ( glGetUniformLocation ( program, "light[2].direction" ), lightDir[2].x, lightDir[2].y, lightDir[2].z );
	glUniform3f ( glGetUniformLocation ( program, "light[2].ambient" ), 0.0f, 0.0f,0.0f );
	glUniform3f ( glGetUniformLocation ( program, "light[2].diffuse" ), 1.0f, 1.0f, 1.0f );
	glUniform3f ( glGetUniformLocation ( program, "light[2].specular" ), 0.0f, 0.0f,1.0f );
	

	modelLoc = glGetUniformLocation ( program, "model" );
	viewLoc = glGetUniformLocation ( program, "view" );
	projLoc = glGetUniformLocation ( program, "projection" );
	viewPosLoc = glGetUniformLocation ( program, "viewPos" );

	glEnable ( GL_DEPTH_TEST );
	glClearColor( 0.2, 0.3, 0.4, 1.0 );
}

void display ()
{
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	vec3 cameraPos( r*cos(alpha )*cos(theta),r*sin( alpha ) ,r*cos( alpha )*sin( theta ) );

	//camera position
	glUniform3f ( viewPosLoc,cameraPos.x,cameraPos.y,cameraPos.z );
	
	//view matrix
	mat4 view;
	view = lookAt( cameraPos, vec3( 0.0, 0.0, 0.0 ), vec3( 0.0, 1.0, 0.0 ) );
	glUniformMatrix4fv ( viewLoc, 1,GL_FALSE, glm::value_ptr ( view ) );

	//projection matrix
	mat4 projection = glm::perspective ( zoom, ( GLfloat ) WIDTH / ( GLfloat ) HEIGHT, 0.1f, 100.0f );
	glUniformMatrix4fv ( projLoc, 1, GL_FALSE, glm::value_ptr ( projection ) );

	//model matrix
	mat4 model = mat4(1.0f);
	model = scale(model,vec3(0.5,0.5,0.5) );
	glUniformMatrix4fv ( modelLoc, 1, GL_FALSE, glm::value_ptr ( model ) );

	glBindVertexArray ( VAO );
	glDrawArrays( GL_TRIANGLES,0,36);
	glBindVertexArray ( 0 );

	glutSwapBuffers ();
}

void idle( void )
{
	glutPostRedisplay( );
}

bool bLeftBntDown = false;
int OldX = 0, OldY = 0;
void mouse (int button,int state,int x,int y)
{
	if ( state == GLUT_DOWN && button == GLUT_LEFT_BUTTON )
	{
		OldX = x;
		OldY = y;
		bLeftBntDown = true;
	}
	else if ( state == GLUT_DOWN && button == GLUT_LEFT_BUTTON )
	{
		bLeftBntDown = false;
	}
}

void motion ( int x, int y )
{
	float xoffset = x - OldX;
	float yoffset = y -  OldY ;

	theta +=  xoffset * 0.005;
	alpha += yoffset * 0.005;

	OldX = x;
	OldY = y;
	glutPostRedisplay ();
}

void keyboard( unsigned char key, int mousex, int mousey )
{
	switch ( key ) {
	case 033:
	case 'q': case 'Q':
		exit( EXIT_SUCCESS );
		break;
	case '1':
		glBindTexture( GL_TEXTURE_2D, textures[ 0 ] );
		break;

	case '2':
		glBindTexture( GL_TEXTURE_2D, textures[ 1 ] );
		break;
	}

	glutPostRedisplay( );
}


int main(int argc, char* argv[])
{
	// Initialize the GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("201430610122-³ÂºìÓî-4°à");
	if (glewInit() != GLEW_OK)
	{
		cout << "glew init fail ;";
	}
	init();

	glutDisplayFunc(display);
	glutMouseFunc( mouse );
	glutMotionFunc ( motion );
	glutKeyboardFunc( keyboard );
	glutIdleFunc( idle );
	glutMainLoop();

	return 0;
}



