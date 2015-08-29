/* Fragment shader */

#version 130

in vec3 ex_Normal;
in vec2 ex_TexCoord;
in vec3 ex_WorldPos;
//in vec4 ex_Color;
//out vec4 out_Color;

out vec3 out_Pos;
out vec3 out_Diffuse;
out vec3 out_Normal;
out vec3 out_TexCoord;
out vec4 out_Outline;

/*struct DirectionalLight {
	vec3 color;
	vec3 direction;
	float ambientIntensity;
	float diffuseIntensity;
};*/

uniform sampler2D tex;
//uniform int useTexture;
uniform bool bRenderBlack;
//uniform DirectionalLight directionalLight;

void main()
{
	out_Pos = ex_WorldPos;
	if (bRenderBlack) {
		out_Normal = vec3(1.0, 1.0, 1.0);
		out_Outline = vec4(1.0, 0.6, 0.0, 1.0);
	}
	else {
		//out_Normal = normalize(ex_Normal);
		out_Normal = ex_Normal;
		out_Outline = vec4(0, 0, 0, 0);
	}
	out_Diffuse = texture(tex, ex_TexCoord).xyz;
	out_TexCoord = vec3(ex_TexCoord, 0);

	/*if (renderBlack == 0) {
		vec3 ambientColor = directionalLight.color * directionalLight.ambientIntensity;
		float diffuseFactor = dot(normalize(ex_Normal), -directionalLight.direction);
		vec3 diffuseColor;
		if (diffuseFactor > 0.0) {
			diffuseColor = directionalLight.color * directionalLight.diffuseIntensity * diffuseFactor;
		}
		else
			diffuseColor = vec3(0, 0, 0);

		out_Color = texture(tex, ex_TexCoord) * vec4(ambientColor + diffuseColor, 1.0);
	}
	else
		out_Color = vec4(1.0, 0.6, 0.0, 1.0);*/
}
