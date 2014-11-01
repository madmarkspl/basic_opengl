#version 400
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D texFramebuffer;

const float offset = 1.0 / 300;
void main()
{
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),  // top-left
		vec2(0.0f, offset),  // top-center
		vec2(offset, offset),  // top-right
		vec2(-offset, 0.0f),    // center-left
		vec2(0.0f, 0.0f),    // center-center
		vec2(offset, 0.0f),    // center-right
		vec2(-offset, -offset), // bottom-left
		vec2(0.0f, -offset), // bottom-center
		vec2(offset, -offset)  // bottom-right    
		);

	float kernel[9] = float[](
		0,0,0,
		0,1,0,
		0,0,0
		);

	vec3 samples[9];
	for (int i = 0; i < 9; i++)
	{
		samples[i] = vec3(texture(texFramebuffer, Texcoord.st + offsets[i]));
	}
	vec3 col;
	for (int i = 0; i < 9; i++)
		col += samples[i] * kernel[i];

	outColor = vec4(col, 1.0);
}