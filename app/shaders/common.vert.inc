uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

mat4 modelViewMatrix = viewMatrix*modelMatrix;

in vec4 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;

vec4 vPos()
{
    return projectionMatrix * modelViewMatrix * vertexPosition;
}
