layout (location = 0) in vec4 vertexPosition;

out vec4 outputColor;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

mat4 modelViewMatrix = viewMatrix*modelMatrix;

uniform vec4 color;

#define COLOR_STEP 18

void main()
{
    // Pass through the color to the fragment shader
    if (color.a == 0.0)
    {
        // falback color, just to see something. calculated out of the index of the vertex so that every vertex gets a diferent color
        int i = (gl_VertexID / 3) % COLOR_STEP;
        outputColor = vec4(float(i) / float(COLOR_STEP), float(COLOR_STEP - i) / float(COLOR_STEP), 1.0, 1.0);
    }
    else
    {
         outputColor = color;
    }

    // Calculate the vertex position
    gl_Position = projectionMatrix * modelViewMatrix * vertexPosition;
}
