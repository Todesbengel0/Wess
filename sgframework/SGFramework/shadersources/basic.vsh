uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 nMatrix;

in vec4 vertex;
in vec3 normal;
in vec2 texCoord;

out vec3 normalsWorld;
out vec3 oMCVertex;
smooth out vec2 vTexCoord; 

void main()
{
  vTexCoord = texCoord;
  oMCVertex = vec3(mvMatrix*vertex);//vec3(mvMatrix*vec4(normal,0.0));
  normalsWorld = vec3(mvMatrix*vec4(normal,0.0));// nMatrix * normal;
  gl_Position = mvpMatrix * vertex;
}
