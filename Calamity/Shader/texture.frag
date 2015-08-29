/* Fragment shader */

#version 130

in vec2 ex_TexCoord;
in vec4 ex_Color;
in vec2 ex_TexOrigin;
in vec2 ex_TexDimensions;
out vec4 out_Color;

uniform sampler2D tex;
uniform bool bRenderBlack;
uniform bool bAnaglyph;
//uniform int useTexture;

void main()
{
	/*if (useTexture == 1) {
		out_Color = ex_Color * texture(tex, ex_TexCoord);
	}
	else {
		out_Color = ex_Color;
	}*/
	vec2 texCoord = ex_TexCoord;
	texCoord *= ex_TexDimensions;
	texCoord = mod(texCoord, ex_TexDimensions);
	texCoord.y *= -1;
	texCoord += ex_TexOrigin;
	if (bRenderBlack)
		out_Color = vec4(0.0, 0.0, 0.0, ex_Color.a) * texture(tex, texCoord);
	else {
		out_Color = ex_Color * texture(tex, texCoord);
		if (bAnaglyph) {
			float g = out_Color.g;
			out_Color.g = 0.25 + g * 0.25;
		}
	}

	//out_Color = ex_Color;
	//out_Color = ex_Color * texture(tex, ex_TexCoord) + vec4(ex_TexOrigin, 0, 0) * 0.001;

	//out_Color = out_Color * 0.0001 + vec4(1, 1, 1, 1);
	//out_Color = vec4(ex_TexCoord.s, ex_TexCoord.s, ex_TexCoord.s, 1.0f);
}
