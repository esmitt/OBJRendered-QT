#pragma once
#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "glew_hack.h"
#include "material.h"
#include "materialgroup.h"

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void setName(std::string name);
    std::string* getName();

    void AddVertex(float x, float y, float z);

    void AddNormal(float x, float y, float z);

    void AddUv(float u, float v);

    void AddMaterialGroup(MaterialGroup newMG);

    MaterialGroup* getLastMaterialGroup();

    unsigned long getVertexsSize();
    unsigned long getNormalsize();
    unsigned long getUvsSize();

    void Print();

    std::vector<glm::vec3>* getVertexs(){ return &Vertexs; }
    std::vector<glm::vec3>* getNormals(){ return &Normals; }
    std::vector<glm::vec2>* getUvs(){ return &Uvs; }

    void GenBuffers();

    void render(GLuint MaterialAmbientID, GLuint MaterialDiffuseID, GLuint MaterialSpecularID, GLuint ShinninesID);

private:
    std::string Name; //Name of the object

    std::vector<glm::vec3> Vertexs; //Vertexs of the object
    std::vector<glm::vec3> Normals; //Normals of the object
    std::vector<glm::vec2> Uvs; //Uvs Coordinates of the object

    std::vector<MaterialGroup> Objects; //Objects agroupped by materials

    GLuint VertexArrayID;

    unsigned int sizeFaces;
};

#endif
