uniform vec4 color;
out vec4 fragColor;
uniform sampler2D textureMap;
uniform bool hasTexture; 

in vec3 normalsWorld;
in vec3 oMCVertex;
smooth in vec2 vTexCoord;

void main()
{
  vec3 vLightPosition = vec3(0.0,0.0,0.0);
  
  vec3 vLightVector=normalize(vLightPosition-oMCVertex);
  float vDiffuse=max(dot(normalsWorld,vLightVector),0.2);
  
  //if(hasTexture == false)
  //  fragColor = vec4(color.xyz * vDiffuse, color.a);
  //else
  //  fragColor = vec4(color.xyz * vDiffuse * vec3(texture(textureMap, vTexCoord)),color.a);
  fragColor = vec4(color.xyz * vDiffuse, color.a);
}
