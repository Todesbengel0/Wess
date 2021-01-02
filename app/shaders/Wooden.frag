#sginclude "://shaders/commonlightmodels.inc"

// from VS, interpolated
in vec3 viewPosition;
in vec3 viewDirection;
in vec3 eyeNormal;
in float materialAlpha;

out vec4 fragColor;

void main()
{
	// Interpolation zerstört Normierung!
	// muss neu normalized werden, sonst auch im FS kantig
	vec3 vertexColor = phong(viewPosition, normalize(viewDirection), normalize(eyeNormal.xyz), vec3(1.0));

    fragColor = vec4(vertexColor + material.emission.rgb, materialAlpha);
    //fragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
