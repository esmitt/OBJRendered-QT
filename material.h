#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <cstdio>

#include <glm/glm.hpp>

/************************************************************************************
********************* Ilumination modes *********************************************
*	0. Color on and Ambient off														*
*	1. Color on and Ambient on														*
*	2. Highlight																	*
*	3. Reflection on and Ray trace on												*
*	4. Transparency: Glass on, Reflection: Ray trace on								*
*	5. Reflection: Fresnel on and Ray trace on										*
*	6. Transparency: Refraction on, Reflection: Fresnel off and Ray trace on		*
*	7. Transparency: Refraction on, Reflection: Fresnel on and Ray trace on			*
*	8. Reflection on and Ray trace off												*
*	9. Transparency: Glass on, Reflection: Ray trace off							*
*	10. Casts shadows onto invisible surfaces										*
*************************************************************************************
************************************************************************************/

class Material
{
public:

    Material()
    {
        Ka.r = 1; Ka.g = 1; Ka.b = 1;
        Kd.r = 1; Kd.g = 1; Kd.b = 1;
        Ks.r = 1; Ks.g = 1; Ks.b = 1;
        Ns = .5;
        Tr = 0;
        Illum = 2;
    }

    ~Material(){}

    void setName(std::string name){ Name = name; }
    std::string* getName(){ return &Name; }

    void setKa(float r, float g, float b){ Ka.r = 0.01; Ka.g = 0.01; Ka.b = 0.01; }
    glm::vec3* getKa(){ return &Ka; }

    void setKd(float r, float g, float b){ Kd.r = r; Kd.g = g; Kd.b = b; }
    glm::vec3* getKd(){ return &Kd; }

    void setKs(float r, float g, float b){ Ks.r = r; Ks.g = g; Ks.b = b; }
    glm::vec3* getKs(){ return &Ks; }

    void setNs(float ns){ Ns = ns; }
    float* getNs(){ return &Ns; }

    void setTr(float tr){ Tr = tr; }
    float* getTr(){ return &Tr; }

    void setIllum(unsigned short illum){ Illum = illum; }
    unsigned short* getIllum(){ return &Illum; }

    void Print()
    {
        printf("Name: %s\n -Kd: %f %f %f\n -Ka: %f %f %f\n -Ks: %f %f %f\n Ns: %f, Tr: %f, Illum: %d\n", Name.c_str(), Kd.r, Kd.g, Kd.b, Ka.r, Ka.g, Ka.b, Ks.r, Ks.g, Ks.b, Ns, Tr, Illum);
    }

private:
    std::string Name;
    glm::vec3 Ka; //Ambient Color
    glm::vec3 Kd; //Difusse Color
    glm::vec3 Ks; //Specular Color
    float Ns; //Specular Exponent
    float Tr; //Disolve or Transparency

    unsigned short Illum; //Ilumination Model

    /*** TO DO ***
        mapKa //Ambient texture map
        mapKd //Diffuse texture map
        mapKs //Especular texture map
        mapD  //Alpha texture map
        mapBump //Bump Map
        mapDisp //DisplacementMap
        mapDecal //DecalMap
    **/
};

#endif // MATERIAL_H
