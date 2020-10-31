struct Light{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 position;
  vec3 spotDirection;
  float spotCutoff;
};

uniform Light light[5];
uniform int step;
uniform int numLight;
uniform mat4 mvMatrix;
uniform vec4 vertex;

//out vec3 normal;
out vec3 pos;

void main(void)
{
  gl_Position = mvMatrix * vertex - light[numLight].position;
  
  float len = length(gl_Position.xyz);
  gl_Position /= -len ;
  if(step==1) 
    gl_Position.z *= -1.0;
  
  gl_Position.z += 1.0;
  gl_Position.xy /= gl_Position.z;
  
  if(gl_Position.z >= 0.01) {
    gl_Position.z = len / 15.0;
    gl_Position.w = 1.0;        //alpha  
  }
  else //render the other direction
  {
    gl_Position.z = -1.0;
    gl_Position.w = -1.0;
  }
  
  gl_Position.z = 2.0 * gl_Position.z - 1.0;
  pos = gl_Position.xyz;
  
}
