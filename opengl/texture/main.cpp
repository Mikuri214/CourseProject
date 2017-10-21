//#include <iostream>
//#include <vector>
//
//#include <gl/glew.h>
//#include <gl/glut.h>
//#include <glm/glm/glm.hpp>
//#include <glm/glm/gtc/type_ptr.hpp>
//#include "LoadShaders.h"
//
//
//
//using namespace glm;
//using namespace std;
//
//GLfloat vertices[] = {
//	-0.5f, -0.5f, 0.5f,		0.5f, -0.5f, 0.5f,			0.5f, 0.5f, 0.5f,		-0.5f, 0.5f, 0.5f,
//	-0.5f,	-0.5f,	-0.5f,			0.5f, -0.5f, -0.5f,		0.5f, 0.5f, -0.5f,		-0.5f, 0.5f, -0.5f,  
//};
//
//GLint index[] = { 0, 1, 2,		2, 3, 0,	 3, 2, 6,		6, 7, 3,		2, 1, 5,		 5, 6, 2,		 4, 0, 3,		3, 7, 4,		0, 4, 5,		5, 1, 0, };
//
//GLuint VAO;
//GLuint VBO;
//GLuint EBO;
//
//void init (){
//	ShaderInfo shaders[] = { { GL_VERTEX_SHADER, "triangle.vert" }, { GL_FRAGMENT_SHADER, "triangle.frag" }, { GL_NONE, NULL } };
//	GLuint program = LoadShaders ( shaders );
//	glUseProgram ( program );
//
//
//	glGenVertexArrays ( 1, &VAO );
//	glGenBuffers ( 1, &VBO );
//	glGenBuffers ( 1, &EBO );
//
//	glBindVertexArray ( VAO );
//	// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
//	glBindBuffer ( GL_ARRAY_BUFFER, VBO );
//	glBufferData ( GL_ARRAY_BUFFER, sizeof ( vertices ), vertices, GL_STATIC_DRAW );
//	// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
//	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, EBO );
//	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, sizeof ( index ), index, GL_STATIC_DRAW );
//	// 3. 设定顶点属性指针
//	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof ( GLfloat ), ( GLvoid* ) 0 );
//	glEnableVertexAttribArray ( 0 );
//	// 4. 解绑VAO
//	glBindVertexArray ( 0 );
//}
//
//void mydisplay (){
//	glBindVertexArray ( VAO );
//	glDrawElements ( GL_TRIANGLES,sizeof(index)/sizeof(GLint),GL_INT,0);
//	glBindVertexArray ( 0 );
//
//	glutSwapBuffers (  );
//}
//
//
//int main ( int argc, char* argv[] )
//{
//	glutInit ( &argc, ( char** ) argv );
//	glutInitDisplayMode ( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
//	glutInitWindowPosition ( 100, 100 );
//	glutInitWindowSize ( 600, 600 );
//	
//	if ( glewInit () != GLEW_OK )
//	{
//		cout << "init fail. ";
//	}
//	init ();
//	glutCreateWindow ( "-by陈红宇" );
//	glutDisplayFunc ( mydisplay );
//	glutMainLoop ();
//	return 0;
//}
