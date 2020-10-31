#define MAX_LIGHTS 5

struct Light{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec4 position;
  vec3 spotDirection;
  float spotCutoff;
};

uniform int numberOfLights;
uniform Light lights[MAX_LIGHTS];

void main()
{
    float factor = 1.0;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    vec4 eyePosition = gl_ModelViewMatrix * gl_Vertex;
    vec4 eyeLightPos = gl_LightSource[0].position;
    vec3 eyeNormalVec = normalize(gl_NormalMatrix * gl_Normal);
    vec3 eyeLightVec = normalize(eyeLightPos.xyz - eyePosition.xyz);
    vec3 eyeViewVec = -normalize(eyePosition.xyz);
    vec3 eyeReflectVec = -reflect(eyeLightVec, eyeNormalVec);
    float Kd = max(dot(eyeLightVec, eyeNormalVec), 0.0);
    float Ks = pow(max(dot(eyeViewVec, eyeReflectVec),0.0), factor*gl_FrontMaterial.shininess);
    float Ka = 1.0;

    gl_FrontColor = Kd * gl_FrontLightProduct[0].diffuse + Ks * gl_FrontLightProduct[0].specular;// + gl_FrontLightModelProduct.sceneColor;
	//gl_FrontLightProduct is Lightvalue*FrontMaterial
}
