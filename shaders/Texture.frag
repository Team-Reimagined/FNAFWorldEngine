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
	layout(offset = 80) vec2 uvOffset;
	layout(offset = 88) vec2 tileCount;
} PushConstants;

void main() 
{
	vec2 outUV;

	outUV.x = mod(inUV.x * PushConstants.tileCount.x, max(1., PushConstants.tileCount.x * PushConstants.uvScale.x) / PushConstants.tileCount.x);
	outUV.y = mod(inUV.y * PushConstants.tileCount.y, max(1., PushConstants.tileCount.y * PushConstants.uvScale.y) / PushConstants.tileCount.y);
	outFragColor = texture(displayTexture,outUV + PushConstants.uvOffset);
}