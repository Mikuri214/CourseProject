#version 430 core
  
struct Light {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 color;

in vec3 Normal;
in vec4 FragPos;
in vec4 material;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Light light[3];
uniform sampler2D texturer;

vec3 CalcLight(Light light, vec3 normal, vec3 viewDir)
{
	
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.w);
    // Combine results
    vec3 ambient = light.ambient * material.xyz;
    vec3 diffuse = light.diffuse * diff *material.xyz;
    vec3 specular = light.specular * spec * material.xyz;
    return (ambient + diffuse + specular);
}


void main(void)
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - vec3(FragPos));

    vec3 result ;
	for(int i = 0; i < 3; i++){
		result+= CalcLight(light[i],norm,viewDir);
	}
	result *= vec3(texture( texturer , TexCoords ));
    color = vec4(result, 1.0);
}