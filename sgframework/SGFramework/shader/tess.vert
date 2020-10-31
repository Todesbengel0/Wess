//layout (location = 0) in vec4 vertexPosition;
//layout (location = 2) in vec4 vertexColor;
in vec4 vertexPosition;
in vec4 vertexColor;

out vec4 vColor;
out vec4 vPosition;

void main()
{
    vColor = vertexColor;
    vPosition = vertexPosition;
}
