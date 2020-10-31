layout (triangles) in;
layout (triangle_strip) out; //es wird kein echter
layout (max_vertices=4) out; //Strip erzeugt!

in vec4 teColor[];
out vec4 gcolor;

void main(void)
{
for(int i=0; i<gl_in.length(); ++i) {
    gl_Position=gl_in[i].gl_Position;
    gcolor = teColor[i];
    EmitVertex();
  }
  EndPrimitive();
}
