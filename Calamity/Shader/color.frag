/* Fragment shader */

#version 130

in vec4 ex_Color;
out vec4 out_Color;

uniform bool bAnaglyph;

void main()
{
	out_Color = ex_Color;
	if (bAnaglyph) {
		float g = out_Color.g;
		out_Color.g = 0.25 + g * 0.25;
	}
}
