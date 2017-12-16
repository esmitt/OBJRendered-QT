#pragma once
#ifndef MATERIALGROUP
#define MATERIALGROUP

#include <vector>
#include <cstdio>

#include "glew_hack.h"
#include "material.h"

class MaterialGroup
{
public:
    MaterialGroup(){}
    ~MaterialGroup(){ Triangles.clear(); }

    Material** getBaseMaterial() { return &BaseMaterial; }

    void setBaseMaterial(Material *material) { BaseMaterial = material; }

    void addTriangle(unsigned int vertex[3])
    {
        CoordsPerVertex = 3;
        for (int i = 0; i < 3; i++)
            Triangles.push_back(vertex[i]);
    }

    void addTriangle(unsigned int vertex[3], unsigned int normals_uvs[3])
    {
        CoordsPerVertex = 6;
        for (int i = 0; i < 3; i++)
        {
            Triangles.push_back(vertex[i]);
            Triangles.push_back(normals_uvs[i]);
        }
    }

    void addTriangle(unsigned int vertex[3], unsigned int normals[3], unsigned int uvs[3])
    {
        CoordsPerVertex = 9;
        for (int i = 0; i < 3; i++)
        {
            Triangles.push_back(vertex[i]);
            Triangles.push_back(normals[i]);
            Triangles.push_back(uvs[i]);
        }
    }

    void Print()
    {
        printf("Base Material: %s\n -Number of triangles: %d\n -Coordinates per Vertex: %d\n", BaseMaterial->getName()->c_str(), Triangles.size(), CoordsPerVertex);
    }

    void render(GLuint MaterialAmbientID, GLuint MaterialDiffuseID, GLuint MaterialSpecularID, GLuint ShinninesID)
    {
        glUniform3f(MaterialAmbientID, BaseMaterial->getKa()->r, BaseMaterial->getKa()->g, BaseMaterial->getKa()->b);
        glUniform3f(MaterialDiffuseID, BaseMaterial->getKd()->r, BaseMaterial->getKd()->g, BaseMaterial->getKd()->b);
        glUniform3f(MaterialSpecularID, BaseMaterial->getKs()->r, BaseMaterial->getKs()->g, BaseMaterial->getKs()->b);
        glUniform1f(ShinninesID, *BaseMaterial->getNs());


        glDrawArrays(GL_TRIANGLES, begin, count);
    }

    std::vector<unsigned int>* getTriangles()
    {
        return &Triangles;
    }

    unsigned int getCoordsPerVertex()
    {
        return CoordsPerVertex;
    }

    void setOffsetandCount(int ofsset, int count)
    {
        begin = ofsset;
        this->count = count;
    }

private:
    unsigned short CoordsPerVertex;
    Material *BaseMaterial;
    std::vector<unsigned int> Triangles;

    unsigned int begin;
    unsigned int count;
};

#endif // MATERIALGROUP
