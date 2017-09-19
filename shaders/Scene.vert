#version 150 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Color;
uniform mat4 ModelViewProjection;

out vec4 FragmentColor;

void main()
{
	gl_Position = ModelViewProjection * vec4(Position, 1.0);
	
	FragmentColor = vec4(Color, 1.0);
}
