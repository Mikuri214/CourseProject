#version 430 core

layout( location = 0 ) in vec4 position;
layout( location = 1 ) in vec3 normal;
layout( location = 2 ) in vec4 vmaterial;
layout( location = 3 ) in vec2 texCoords;

out vec4 material;
out vec2 TexCoords;
out vec3 Normal;
out vec4 FragPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

 void main( )
{
	gl_Position =  projection * view *  model * position;
    FragPos = model * position;
    Normal = mat3(transpose(inverse(model))) * normal;  
	material = vmaterial;
	TexCoords = texCoords;
}