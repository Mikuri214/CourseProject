
#define GLSL(version, shader) "#version " #version "\n" #shader

const char* vert = GLSL
(
	440 core,
	void main( void )
{
	vec4 ves[ 3 ] = vec4[ 3 ]
		(
			vec4( 0.0, -0.5, 0.5, 1.0 ),
			vec4( 0.0, 0.5, 0.5, 1.0 ),
			vec4( -0.5, -0.5, 0.6, 1.0 )
			);

	gl_Position = ves[ gl_VertexID ];
}
);

const char* frag = GLSL
(
	440 core,
	out vec4 color;
void main( void )
{
	color = vec4( 0.0, 1.0, 0.0, 1.0 );
}
);


