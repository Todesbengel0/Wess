#sginclude "://shaders/commonlightmodels.inc"

// uniform passed textures
uniform float hasTexPattern = 0.0;
uniform sampler2D TexPattern;
uniform float hasTexNormal = 0.0;
uniform sampler2D TexNormal;

// uniform parameters
uniform vec3 colWoodDark = vec3(0.61, 0.44, 0.30);

// from VS, interpolated
in vec3 viewPosition;
in vec3 viewDirection;
in vec3 eyeNormal;
in float materialAlpha;

in vec2 texCoords;

out vec4 fragColor;

void main()
{
	// Interpolation zerstört Normierung!
	// muss neu normalized werden, sonst auch im FS kantig
	vec3 phongColor = phong(viewPosition, normalize(viewDirection), normalize(eyeNormal.xyz), vec3(1.0));

	vec3 patternColor = phongColor;
	if (hasTexPattern > 0.5)
	{
		vec4 patternFrag = texture(TexPattern, texCoords);
		//patternColor = patternFrag.xyz;

		float factor = 1.0 - ((patternFrag.x + patternFrag.y + patternFrag.z) / 3.0);

		patternColor = mix(phongColor, colWoodDark, factor);
	}

    fragColor = vec4(patternColor + material.emission.rgb, materialAlpha);
    //fragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
