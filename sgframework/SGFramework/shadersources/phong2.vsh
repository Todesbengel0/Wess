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
flat out int numOfLights;
smooth out vec2 vTexCoord;

void main()
{
  vTexCoord = texCoord;
  baseNormal = nMatrix * normal;
  normalMatrix = nMatrix;
  vec4 vVertex = mvMatrix * vertex;
  eyeVec = - vVertex.xyz;
  gl_Position = mvpMatrix * vertex;
  
  int i;
  for (i=0; i<numberOfLights; ++i)
    lightDir[i] = vec3(lights[i].position.xyz - vVertex.xyz);
  
  numOfLights = numberOfLights;
}
