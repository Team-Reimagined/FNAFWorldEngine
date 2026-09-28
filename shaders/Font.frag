//glsl version 4.5
#version 450

//shader input
layout (location = 0) in vec3 inColor;
layout (location = 1) in vec2 inUV;
//output write
layout (location = 0) out vec4 outFragColor;

//texture to access
layout(set =0, binding = 0) uniform sampler2D displayTexture;

layout( push_constant ) uniform constants
{	
	layout(offset = 72) vec2 uvScale;
	vec2 uvOffset;
	uint colorPacked;
	float scale;
} PushConstants;

float median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}

float screenPxRange(vec2 uvCoord)
{
	vec2 unitRange = vec2(2.0) / vec2(textureSize(displayTexture, 0));
	vec2 screenTexSize = vec2(1.0) / fwidth(uvCoord);
	return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void main() 
{
	vec2 outUV;
	outUV.x = mod(inUV.x, max(1., PushConstants.uvScale.x));
	outUV.y = mod(inUV.y, max(1., PushConstants.uvScale.y));

	vec4 tex = texture(displayTexture, outUV + PushConstants.uvOffset);
	float dist;
	if(PushConstants.scale < 1)
	{
		dist = tex.a;
	}
	else
	{
		dist = median(tex.r, tex.g, tex.b);
	}
	float pixelDist = screenPxRange(outUV + PushConstants.uvOffset) * (dist - 0.5);
	float opacity = clamp(pixelDist + 0.5, 0.0, 1.0);

    vec4 color = unpackUnorm4x8(PushConstants.colorPacked);
// 	outFragColor = texture(displayTexture, outUV + PushConstants.uvOffset) * colorMod;

	outFragColor = vec4(color.r, color.b, color.g, opacity * color.a);
}
