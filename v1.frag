#version 330 core

    in vec3 vertexPosition_M;
    in vec3 vertexNormal_MV;
    in vec3 eyeDir_MV;
    in vec3 lightDir_MV;

    uniform vec3 AmbientColor_Material;
    uniform vec3 DiffuseColor_Material;
    uniform vec3 SpecularColor_Material;
    uniform float Shinnines_Material;

    uniform vec3 Ambient_Light;
    uniform vec3 Diffuse_Light;
    uniform vec3 Specular_Light;

    uniform bool UseMaterial;
    uniform bool UseLight;
    uniform vec3 BaseColor;
    uniform vec3 light_PosDir;
    uniform bool point_light;

    uniform bool UseGrayscale;
    
    out vec3 color;

vec3 crosshatch(float t, vec3 texColor, float t1, float t2, float t3, float t4) {
  float lum =t;
  vec3 color = vec3(0.9);
  if (lum < t1) {
      if (mod(gl_FragCoord.x + gl_FragCoord.y, 10.0) == 0.0) {
          color = vec3(0.0);
      }
  }
  if (lum < t2) {
      if (mod(gl_FragCoord.x - gl_FragCoord.y, 10.0) == 0.0) {
          color = vec3(0.0);
      }
  }
  if (lum < t3) {
      if (mod(gl_FragCoord.x + gl_FragCoord.y - 5.0, 10.0) == 0.0) {
          color = vec3(0.0);
      }
  }
  if (lum < t4) {
      if (mod(gl_FragCoord.x - gl_FragCoord.y - 5.0, 10.0) == 0.0) {
          color = vec3(0.0);
      }
  }
  return color;
}

vec3 crosshatch(float a, vec3 texColor) {
  return crosshatch(a, texColor, 1.0, 0.75, 0.5, 0.3);
}
    void main()
    {
        if(UseLight)
        {
            float lightPower = 1.0f;

            float attenuation = 1;

            if(point_light)
            {
                lightPower = 8.0f;
                float distance = length( light_PosDir - vertexPosition_M );

                attenuation = 1.0f / (2 + distance*1.0 + distance*distance*0.5);
            }

            vec3 n = normalize( vertexNormal_MV );
            vec3 l = normalize( lightDir_MV );

            float cosTheta = clamp( dot( n,l ), 0,1 );

            vec3 E = normalize(eyeDir_MV);

            vec3 R = reflect(-l,n);

            float cosAlpha = clamp( dot( E,R ), 0,1 );

            color = Ambient_Light * AmbientColor_Material +
                    DiffuseColor_Material * lightPower * cosTheta * attenuation +
                    SpecularColor_Material * Specular_Light * lightPower * pow(cosAlpha, Shinnines_Material) * attenuation;
            
            if(UseMaterial)
            {
                    color *= crosshatch(cosTheta, color.rgb);
            }
            /*else
            {
                color = BaseColor;
            }*/
        }
        else
        {
            if(UseMaterial)
            {
                color = DiffuseColor_Material;
            }
            else
            {
                color = BaseColor;
            }
            color = vec3(1,0,0);
        }
        
        if (UseGrayscale) {
            float gray = (0.3 * color.r) + (0.59 * color.g) + (0.11 * color.b);
            color = vec3(gray, gray, gray);
        }
    };
