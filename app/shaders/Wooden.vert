#sginclude "://shaders/commonlight.vert.inc"

// Phong pass to FS
out vec3 viewPosition;
out vec3 viewDirection;
out vec3 eyeNormal;
out float materialAlpha;

in vec2 textureCoords;
out vec2 texCoords;

void main()
{
	// Phong pass to FS
    viewPosition = eyePosition().xyz;
    viewDirection = -normalize(viewPosition);
	eyeNormal = eyeNormalVec().xyz;
	materialAlpha = computeMaterialAlpha();

	// Only do pos transformation
    gl_Position = vPos();

	// Pass texture coords
    texCoords = textureCoords;
}
