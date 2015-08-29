/* Fragment shader */

#version 130

in vec2 ex_TexCoord;
in vec4 ex_Color;
out vec4 out_Color;

uniform sampler2D tex;		// scene texture
uniform vec2 radialSize;	// texel size

uniform float radialBlur;	// blur factor
uniform float radialBright;	// bright factor

uniform vec2 radialOrigin;	// blur origin

void main()
{
	vec4 sumColor = vec4(0.0, 0.0, 0.0, 0.0);

	vec2 texCoord = ex_TexCoord + radialSize * 0.5 - radialOrigin;

	for (int i = 0; i < 82; i++ ) {
		float scale = 1.0 - radialBlur * (float(i) / 81.0);
		sumColor += texture(tex, texCoord * scale + radialOrigin);
	}
	
	//out_Color = ex_Color * texture(tex, ex_TexCoord);
	out_Color = sumColor / 12.0 * radialBright * ex_Color;
	//out_Color = vec4(ex_TexCoord.x, ex_TexCoord.y, 0.0f, 1.0f);
	//out_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);

	//out_Color = ex_Color * texture(tex, ex_TexCoord);
}
