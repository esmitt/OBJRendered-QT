#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

out vec3 vertexPosition_M;
out vec3 vertexNormal_MV;
out vec3 eyeDir_MV;
out vec3 lightDir_MV;
out vec3 lightPos_MV;

uniform vec3 light_PosDir;
uniform mat4 ModelViewProyection_Matrix;
uniform mat4 Model_Matrix;
uniform mat4 View_Matrix;
uniform mat4 Normal_Matrix;
uniform bool point_light;

void main()
{
    gl_Position = ModelViewProyection_Matrix * vec4(vertexPosition, 1.0);

    vertexPosition_M = (Model_Matrix * vec4(vertexPosition, 1)).xyz;

    vec3 vertexPosition_MV = (View_Matrix * Model_Matrix * vec4(vertexPosition, 1)).xyz;

    eyeDir_MV = vec3(0, 0, 2) - vertexPosition_MV;

    lightPos_MV = (View_Matrix * vec4(light_PosDir, 1.0)).xyz;
    lightDir_MV = -light_PosDir;

    if(point_light)
    {
        lightDir_MV = lightPos_MV - vertexPosition_MV;
    }

    vertexNormal_MV = (Normal_Matrix * vec4(vertexNormal, 0)).xyz;
};
