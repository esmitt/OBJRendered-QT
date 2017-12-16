#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;


// This is our View-Projection matrix
uniform mat4 VP;
uniform mat4 M;
uniform mat4 MV;

// Output vertex color
//out vec3 fragmentColor;
//out vec3 fragmentPosition;
//out vec3 fragmentNormal;

void main()
{
	// Transforming vertex with MVP matrix
    vec4 v = position;
	gl_Position = VP * M * v;    
/*
	// Just output vertex color directly
	fragmentColor = vec3(0.6, 0.6, 0.6);

	// Also output world position and normal
	vec4 tp = M * position;
	mat3 normalMatrix = transpose(inverse(mat3(MV)));
	vec3 tn = normalMatrix * normal;
	
	fragmentPosition = vec3(tp.x, tp.y, tp.z);
	fragmentNormal = vec3(tn.x, tn.y, tn.z);
	*/
}