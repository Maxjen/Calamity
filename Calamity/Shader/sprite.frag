/* Fragment shader */

#version 130

in vec2 ex_TexCoord;
in vec4 ex_Color;
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
	if (bRenderBlack)
		out_Color = vec4(0.0, 0.0, 0.0, ex_Color.a) * texture(tex, ex_TexCoord);
	else {
		out_Color = ex_Color * texture(tex, ex_TexCoord);
		if (bAnaglyph) {
			float g = out_Color.g;
			out_Color.g = 0.25 + g * 0.25;
		}
	}
	//out_Color = ex_Color;
	//out_Color = vec4(ex_TexCoord.s, ex_TexCoord.s, ex_TexCoord.s, 1.0f);
}
