#version 150 core

in vec4 FragmentColor;
in vec4 FragmentWorldPosition;
in vec3 FragmentNormal;

out vec4 FragColor;

void main()
{
	if(FragmentNormal != vec3(0.0, 0.0, 0.0))
	{
		vec3 LightPosition = vec3(0.25, 0.5, 1);
		vec3 L = normalize(FragmentWorldPosition.xyz - LightPosition);
		vec3 N = normalize(FragmentNormal);

		float ambient = 0.5;
		float diffuse = max(dot(L, N), 0.0);
		FragColor = FragmentColor*ambient + FragmentColor*diffuse;
		//FragColor = vec4(FragmentNormal, 1.0);
	}
	else
	{
		FragColor = FragmentColor;
	}
}
