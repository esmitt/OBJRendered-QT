#version 330 core

//uniform vec3 LightPos;

//in vec3 fragmentColor;
//in vec3 fragmentPosition;
//in vec3 fragmentNormal;

// Output color
out vec4 color;
 
//uniform sampler2D myTextureSampler;

void main() {
//vec4 newcolor;
//	vec3 lightDir = normalize(LightPos);
//	float intensity = dot(lightDir, normalize(fragmentNormal));
		
//	color = vec4(fragmentColor,1) * intensity;
	color = vec4(1, 0, 0, 1);
	//color = vec4(fragmentColor,1) * vec4(0.6, 0.6, 0.6 ,1) * intensity;
}