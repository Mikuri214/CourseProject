#include <GL/glew.h>
#include <GL/freeglut.h>

#include "learnopengl/model.h"
#include "learnopengl/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SOIL.h>

#include <iostream>
#include <stdlib.h>
#include <string>
#include "helper.h"
#include "LoadFileDlg.h"
using namespace glm;
using namespace std;

GLuint screenWidth = 800, screenHeight = 600;
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Xaxis;
GLfloat Theta[ NumAxes ] = { 0.0, 0.0, 0.0 };

vec2 lastPosition( 0.0, 0.0 );
GLfloat aspect = (GLfloat)screenWidth/screenHeight;
GLfloat fov = 45;

GLuint VAO;
GLuint VBO;
GLuint EBO;
GLuint program;
GLint modelLoc, viewLoc, projLoc, viewPosLoc;// uniform location
Shader* shader;
Model* model;
//camera position
vec3 cameraPos( 0, 0, 2 );

int main( int argc, char** argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitContextVersion( 4, 4 );
	glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize( screenWidth, screenHeight );
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( "opengl" );
	glewExperimental = GL_TRUE;
	if(glewInit( )!=  GLEW_OK) cout<<"init glew failed.";
	init( );

	glutDisplayFunc( display );
	glutMouseFunc( mouse );
	glutMotionFunc( motion );
	glutMouseWheelFunc( wheel );
	glutKeyboardFunc( keyboard );
	glutReshapeFunc( reshape );
	glutIdleFunc( idle );

	glutMainLoop( );
	return 0;
}


void init( )
{
	glShadeModel( GL_FLAT );
	glEnable( GL_DEPTH_TEST );
	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );

	Theta[ Xaxis ] = 45;
	Theta[ Yaxis ] = 45;

	shader = new Shader( "shaders/shader.vs", "shaders.frag" );
	shader->Use( );
	program = shader->Program;

	modelLoc = glGetUniformLocation( program, "model" );
	viewLoc = glGetUniformLocation( program, "view" );
	projLoc = glGetUniformLocation( program, "projection" );
	viewPosLoc = glGetUniformLocation( program, "viewPos" );

	//set parallel Light
	glUniform3f( glGetUniformLocation( program, "dirLight.direction" ), 1.0f,-1.0f,1.0f );
	glUniform3f( glGetUniformLocation( program, "dirLight.ambient" ), 0.2f, 0.2f, 0.2f );
	glUniform3f( glGetUniformLocation( program, "dirLight.diffuse" ), 1.0f, 1.f, 1.f );
	glUniform3f( glGetUniformLocation( program, "dirLight.specular" ), 1.0f, 1.0f, 1.0f );

	//view position
	glUniform3f( viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z );

	model = new Model( "G:\\Project\\Visual C++ ÏîÄ¿\\opengl\\OpenGLDemo\\Model\\bridge.obj" );
}


void display( )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	////view matrix
	//mat4 view;
	//view = ( rotate( view, Theta[ Xaxis ], vec3( 1.0f, 0.0f, 0.0f ) )*
	//		 rotate( view, Theta[ Yaxis ], vec3( 0.0f, 1.0f, 0.0f ) )*
	//		 rotate( view, Theta[ Zaxis ], vec3( 0.0f, 0.0f, 1.0f ) ) );

	//glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );

	////projection matrix
	//mat4 projection = glm::perspective( fov, aspect, 0.1f, 100.0f );
	//glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );

	////model matrix
	//mat4 model = mat4( 1.0f );
	//model = glm::scale( model, vec3( 0.5, 0.5, 0.5 ) );
	////	model = glm::rotate( model, 45.0f, vec3(1.0f, 0.0f, 0.0f));
	////	model = glm::rotate( model, 45.0f, vec3( 0.0f, 1.0f, 0.0f ) );
	//glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );

	//glBindVertexArray( VAO );
	//glDrawArrays( GL_TRIANGLES, 0, 36 );
	//glBindVertexArray( 0 );
	if ( model )
	{
		model->Draw( *shader );
	}
	
	glutSwapBuffers( );
}


void motion( int x, int y ) {
	Theta[ Xaxis ] += ( y - lastPosition.y ) / 10;
	Theta[ Xaxis ] = Theta[ Xaxis ] < 0 ? 360 : ( Theta[ Xaxis ] > 360 ? 0 : Theta[ Xaxis ] );
	Theta[ Yaxis ] += ( x - lastPosition.x ) / 10;
	Theta[ Yaxis ] = Theta[ Yaxis ] < 0 ? 360 : ( Theta[ Yaxis ] > 360 ? 0 : Theta[ Yaxis ] );

	lastPosition = vec2( x, y );
	glutPostRedisplay( );
}


void mouse( int button, int state, int x, int y ) {
	if ( state == GLUT_DOWN ) {
		lastPosition = vec2( x, y );
		switch ( button ) {
		case GLUT_LEFT_BUTTON: Axis = Xaxis; break;
		case GLUT_MIDDLE_BUTTON: Axis = Yaxis; break;
		case GLUT_RIGHT_BUTTON: Axis = Zaxis; break;
		}
	}
}


void idle( void )
{
	glutPostRedisplay( );
}


void reshape( int width, int height ) {
	glViewport( 0, 0, width, height );

	aspect = GLfloat( width ) / height;
	mat4 projection = perspective( fov, aspect, 0.5f, 3.0f );

	glUniformMatrix4fv( projLoc, 1, GL_TRUE, glm::value_ptr( projection ) );
}


void keyboard( unsigned char key, int x, int y )
{
	char file[ 128 ]="",title[128]="";


	switch ( key )
	{
	case '+':
	{
		fov += -5;
		fov = fov > 180 ? 180 : ( fov < 15 ? 15 : fov );
		mat4 projection = perspective( fov, aspect, 0.5f, 3.0f );

		glUniformMatrix4fv( projLoc, 1, GL_TRUE, value_ptr( projection ) );
		break;
	}
	case '-':
	{
		fov += 5;
		fov = fov > 180 ? 180 : ( fov < 15 ? 15 : fov );
		mat4 projection = perspective( fov, aspect, 0.5f, 3.0f );

		glUniformMatrix4fv( projLoc, 1, GL_TRUE, value_ptr( projection ) );
		break;
	}
	default:
		break;
	}

	glutPostRedisplay( );
}


void wheel( int button, int dir, int x, int y ) {
	fov += ( dir * 5 );
	fov = fov > 180 ? 180 : ( fov < 15 ? 15 : fov );
	mat4 projection = perspective( fov, aspect, 0.5f, 3.0f );

	glUniformMatrix4fv( projLoc, 1, GL_TRUE, value_ptr(projection) );
	glutPostRedisplay( );
}
