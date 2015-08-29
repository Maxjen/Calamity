/* Fragment shader */

#version 130

in vec2 ex_TexCoord;
in vec4 ex_Color;
out vec4 out_Color;

uniform sampler2D tex;
//uniform int useTexture;

void main()
{
	/*if (useTexture == 1) {
		out_Color = ex_Color * texture(tex, ex_TexCoord);
	}
	else {
		out_Color = ex_Color;
	}*/
	//out_Color = vec4(ex_Color.xyz, texture(tex, ex_TexCoord).r);
	out_Color = vec4(ex_Color.rgb, texture(tex, ex_TexCoord).r * ex_Color.a);
	//out_Color = ex_Color;
	//out_Color = vec4(ex_TexCoord.s, ex_TexCoord.s, ex_TexCoord.s, 1.0f);
}
