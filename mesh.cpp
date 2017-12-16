#include <cstdio>
#include "mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    Vertexs.clear();
    Normals.clear();
    Uvs.clear();
    Objects.clear();
}

void Mesh::setName(std::string name){ Name = name; }

std::string* Mesh::getName(){ return &Name; }

void Mesh::AddVertex(float x, float y, float z)
{
    glm::vec3 vertex(x, y, z);
    Vertexs.push_back(vertex);
}

void Mesh::AddNormal(float x, float y, float z)
{
    glm::vec3 normal(x, y, z);
    Normals.push_back(normal);
}

void Mesh::AddUv(float u, float v)
{
    glm::vec2 uv(u, v);
    Uvs.push_back(uv);
}

void Mesh::AddMaterialGroup(MaterialGroup newMG)
{
    Objects.push_back(newMG);
}

MaterialGroup* Mesh::getLastMaterialGroup()
{
    return &Objects.back();
}

unsigned long Mesh::getVertexsSize()
{
    return Vertexs.size();
}

unsigned long Mesh::getNormalsize()
{
    return Normals.size();
}

unsigned long Mesh::getUvsSize()
{
    return Uvs.size();
}

void Mesh::Print()
{
    printf("Object Name: %s\n  -Number of Vertex: %d\n  -Number of Normals: %d\n  -Number of Uvs: %d\n-Number of Materials Groups: %d\n", Name.c_str(), Vertexs.size(), Normals.size(), Uvs.size(), Objects.size());

    for (int i = 0; i < Objects.size(); i++) Objects[i].Print();
}

void Mesh::GenBuffers()
{
    int size = Objects.size();

    sizeFaces = 0;

    for (int i = 0; i < size; i++)
    {
        int numTriangle = Objects[i].getTriangles()->size() / 3;
        Objects[i].setOffsetandCount(sizeFaces, numTriangle);
        sizeFaces += numTriangle;
    }

    float* BufferData = new float [sizeFaces * (3+3+2)];

    int cont = 0;

    for (int i = 0; i < size; i++)
    {
        std::vector<unsigned int>* faces = Objects[i].getTriangles();

        int sizeFace = faces->size();

        for (int j = 0; j < sizeFace; j += 3, cont += 8)
        {
            BufferData[cont] = Vertexs[(*faces)[j]].x;
            BufferData[cont + 1] = Vertexs[(*faces)[j]].y;
            BufferData[cont + 2] = Vertexs[(*faces)[j]].z;

            BufferData[cont + 3] = Normals[(*faces)[j + 1]].x;
            BufferData[cont + 4] = Normals[(*faces)[j + 1]].y;
            BufferData[cont + 5] = Normals[(*faces)[j + 1]].z;

            BufferData[cont + 6] = Uvs[(*faces)[j + 2]].x;
            BufferData[cont + 7] = Uvs[(*faces)[j + 2]].y;
        }
    }

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (sizeFaces * 8) * sizeof(float), BufferData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

    delete BufferData;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::render(GLuint MaterialAmbientID, GLuint MaterialDiffuseID, GLuint MaterialSpecularID, GLuint ShinninesID)
{

    glBindVertexArray(VertexArrayID);

    unsigned int size = Objects.size();

    for (int i = 0; i < size; i++)
        Objects[i].render(MaterialAmbientID, MaterialDiffuseID, MaterialSpecularID, ShinninesID);
}
