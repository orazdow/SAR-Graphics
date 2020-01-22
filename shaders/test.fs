#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

// uniform sampler2D eye;
// uniform sampler2D sap;
uniform sampler2DArray mysampler;
uniform float mixsig;
int layer = 0;

void main()
{

	FragColor = mix(  texture(mysampler, vec3(TexCoord.xy, 0)), texture(mysampler, vec3(TexCoord.xy, 1)), mixsig );
    // FragColor = texture(mysampler, vec3(TexCoord.xy, layer)); // + vec4(0,1,1,1);
    // FragColor = vec4(mixsig, 0, 1, 1);

}