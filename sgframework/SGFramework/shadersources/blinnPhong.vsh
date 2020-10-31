#pragma optionNV unroll all

#define MAX_LIGHTS 5

struct Light{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 position;
  vec3 spotDirection;
  float spotCutoff;
};

uniform Light lights[MAX_LIGHTS];

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 nMatrix;
uniform int numberOfLights;

in vec4 vertex;
in vec3 normal;
in vec2 texCoord;

out vec3 baseNormal, eyeVec;
out mat3 normalMatrix;
out vec3 lightDir[MAX_LIGHTS];
flat out int numLights;
smooth out vec2 vTexCoord;

void main()
{
  //set texcoordinates for fragment shader
  vTexCoord = texCoord;
  
  //calculate normal
  baseNormal = nMatrix * normal;
  
  //set normalMatrix for fragment shader
  normalMatrix = nMatrix;
  
  //calculate pos
  vec4 vVertex = mvMatrix * vertex;
  
  
  eyeVec = - vVertex.xyz;
  
  //calc actual position of vertex
  gl_Position = mvpMatrix * vertex;
  
  //calculate lightDirection vectors
  int i;
  for (i=0; i<numberOfLights; ++i)
    lightDir[i] = vec3(lights[i].position.xyz - vVertex.xyz);
  
  //set the number of lights for fragment shader
  numLights = numberOfLights;
}
