/* Fragment shader */

#version 130

in vec3 ex_Normal;
in vec2 ex_TexCoord;
in vec3 ex_WorldPos;

/*layout (location = 0) out vec3 out_Pos;
layout (location = 1) out vec3 out_Diffuse;
layout (location = 2) out vec3 out_Normal;
layout (location = 3) out vec3 out_TexCoord;*/

out vec3 out_Pos;
out vec3 out_Diffuse;
out vec3 out_Normal;
out vec3 out_TexCoord;

uniform sampler2D tex;
uniform int renderBlack;

void main() {
	out_Pos = ex_WorldPos;
	if (renderBlack == 0) {
		out_Diffuse = texture(tex, ex_TexCoord).xyz;
		//out_Normal = normalize(ex_Normal);
		out_Normal = ex_Normal;
	}
	else {
		out_Diffuse = vec3(1.0, 0.6, 0.0);
		out_Normal = vec3(0, 0, 0);
	}
	out_TexCoord = vec3(ex_TexCoord, 0);
}
