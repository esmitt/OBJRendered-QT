#pragma once
#ifndef OBJ_H
#define OBJ_H

#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "material.h"
#include "mesh.h"

class Obj
{
    public:
        Obj();

        ~Obj();

        bool loadObj(const char *path, const char *name); //Loads the file

        void normalize();

        void generateBuffers();

        void render();

        void renderBoundingBox();

        void setUniforms(GLuint materialAmbientID, GLuint materialDiffuseID, GLuint materialSpecularID, GLuint shinninesID);

        std::vector<Material>* getMaterials();

    private:
        std::vector<Material> Materials; //All the materials on the file
        std::vector<Mesh> Meshes; //All the objects defined on the file

        bool readMtl(std::string line, const char* path); //Reads the Material File

        Mesh readObject(std::string line, std::filebuf &file); //Reads an object from the file and returns the object
        Material readMaterial(std::string &line, std::ifstream &file); //Reads a Material and returns a material

        void readVertex(std::string line, Mesh* mesh, bool &first); //Reads a vertex information
        bool readTriangle(std::string line, MaterialGroup* group, Mesh* mesh, unsigned int vertexOffset, unsigned int nomalOffset, unsigned int uvOffset); //Reads a triangle information

        void generateBoundingBox();

        void Print();

        glm::vec3 MinCoord; // Min coordinate of the model
        glm::vec3 MaxCoord; // Max coordinate of the model
        glm::vec3 Center;   // Center coordinate of the model

        GLuint MaterialAmbientID, MaterialDiffuseID, MaterialSpecularID, ShinninesID;

        GLuint VertexArrayBoundingBoxID;
};

#endif // OBJ_H
