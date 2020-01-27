#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2DArray mysampler;
uniform float mixsig;
uniform int idx;
int layer = 0;

void main()
{

	FragColor = texture(mysampler, vec3(((0.5+mixsig*mixsig)*TexCoord.xy-(mixsig/2.0)), idx)) * vec4(1,1,1,mixsig);
	// FragColor = mix(  texture(mysampler, vec3(TexCoord.xy, idx)), texture(mysampler, vec3(TexCoord.xy, idx+1)), mixsig );
	// FragColor = texture(mysampler, vec3(TexCoord.xy-0.08, idx)) * vec4(1,1,1,mixsig);


}