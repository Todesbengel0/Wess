#pragma optionNV unroll all

#define MAX_LIGHTS 5

struct Material{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 emission;
  float shininess;
};

struct Light{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 position;
  vec3 spotDirection;
  float spotCutoff;
};

uniform Light lights[MAX_LIGHTS];

uniform Material material;

uniform vec4 color;
uniform vec4 globalAmbient;
uniform bool hasTexture; 
uniform sampler2D textureMap;

in vec3 lightDir[MAX_LIGHTS];
in vec3 baseNormal;
in vec3 eyeVec;
in mat3 normalMatrix;
flat in int numLights;
smooth in vec2 vTexCoord;
out vec4 fragColor;

void main()
{
  vec4 white = vec4(1.0,1.0,1.0,1.0);
  vec4 final_color = globalAmbient * material.ambient;
  vec3 N = normalize(baseNormal);
  vec3 E = normalize(eyeVec);
  int i;
  for (i=0; i<numLights; ++i)
  {  
    final_color += lights[i].ambient * material.ambient;
  
    vec3 L = normalize(lightDir[i]);
    
    float diffuse = clamp(dot(N, L), 0.0, 1.0);
    final_color += lights[i].diffuse * material.diffuse * diffuse;	
      
    
    vec3 halfVec = normalize(lightDir[i] + eyeVec);
      
    float specular = pow(clamp(dot(N, halfVec), 0.0, 1.0), material.shininess);
    final_color += lights[i].specular * material.specular * specular;	
  }
  
   
  
  
  if(hasTexture == false)
    fragColor = final_color;
  
  else
  {
    vec4 textureFrag = texture(textureMap, vTexCoord);
    fragColor = vec4(final_color.xyz * textureFrag.xyz, textureFrag.a);
  } 
  
  fragColor.xyz += material.emission.xyz;
}


