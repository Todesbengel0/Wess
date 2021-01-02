#sginclude "://shaders/commonlightmodels.inc"

// uniform passed textures
uniform float hasPatternMap = 0.0;
uniform sampler2D PatternMap;
uniform float hasNormalMap = 0.0;
uniform sampler2D NormalMap;

// uniform passed animation
uniform float Time;
uniform const float Pi = 3.141;

// uniform passed others
uniform float Highlighted = 0.0;
uniform vec3 HighlightCol = vec3(1.0, 0.0, 0.0);
uniform vec2 HighlightFactor = vec2(0.2, 0.5);

// uniform parameters
uniform vec3 colWoodDark = vec3(0.61, 0.44, 0.30);

// from VS, interpolated
in vec3 viewPosition;
in vec3 viewDirection;
in vec3 eyeNormal;
in float materialAlpha;

in vec2 texCoords;

out vec4 fragColor;

//Fuer das Verständnis der hier angewandten Berechnung des TBN-Systems sind folgende Quellen zu studieren:
//http://www.geeks3d.com/20130122/normal-mapping-without-precomputed-tangent-space-vectors/ [1]
//und als Verbesserung/follow-up:
//http://www.thetenthplanet.de/archives/1180 [2]
//Noch lesenswert: Eigentlich müsste die Inverse einer Matrix berechnet werden
//Die Inverse kann generell durch die adjugierte Matrix/det berechnet werden
//Unter http://www.emptyloop.com/technotes/A%20tutorial%20on%20inverting%203%20by%203%20matrices%20with%20cross%20products.pdf
//ist die Berechnung der adjugierten Matrix durch Kreuzprodukte dargestellt
//Daraus laesst sich (siehe [2]) dann die ug. Berechnung des contangent_frame ableiten

//Berechnet die Matrix, die den gestörten Normalenvektor aus dem TBN-Koordinantensystem ins WKS umrechnet!
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );

    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

    // construct a scale-invariant frame
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and
    // V, the view vector (vertex to eye)
   vec3 map = texture(NormalMap, texcoord).xyz;
   //siehe sign expansion in [2]
   //Nächste Zeile auskommentieren für VZ-behaftete Maps
   map = map * 255./127. - 128./127.;
   //Nächste Zeile auskommentieren für Maps, die schon mit 3 Kanälen kommen - selten!
   map.z = sqrt( 1. - dot( map.xy, map.xy ) );
   mat3 TBN = cotangent_frame(N, -V, texcoord);
   return normalize(TBN * map);
}

void main()
{
	// Normal mapping
	vec3 normal = normalize(eyeNormal.xyz);
	if (hasNormalMap > 0.5)
	{
		normal = perturb_normal(normal, viewPosition, texCoords);
	}

	// Lighting
	vec3 phongColor = phong(viewPosition, normalize(viewDirection), normal, vec3(1.0));

	// Wood pattern
	vec3 patternColor = phongColor;
	if (hasPatternMap > 0.5)
	{
		// sample pattern map texture
		vec4 patternFrag = texture(PatternMap, texCoords);
		//patternColor = patternFrag.xyz;

		// contrast = lerp factor
		float factor = 1.0 - ((patternFrag.x + patternFrag.y + patternFrag.z) / 3.0);

		patternColor = mix(phongColor, colWoodDark, factor);
	}

	// Highlight
	vec3 highlightColor = patternColor;
	if (Highlighted > 0.5)
	{
		highlightColor = mix(patternColor, HighlightCol, mix(HighlightFactor.x, HighlightFactor.y, sin(Time * Pi)));
	}

    fragColor = vec4(highlightColor + material.emission.rgb, materialAlpha);
    //fragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
