#include <cstring>
#include <cmath>
#include <cassert>

#include "obj.h"

Obj::Obj(){
}

Obj::~Obj()
{
    Materials.clear();
}

bool Obj::loadObj(const char *path, const char* name)
{
    std::string fileName = path;
    fileName = fileName + "/" + name;

    printf("Loading obj: %s/%s\n", path,name);

    std::ifstream file;
    file.open(fileName.c_str(), std::ios::in);

    if (!file.is_open())
    {
        printf("File not found: %s/%s\n", path, name);
        return false;
    }

    std::string  line;

    Mesh *newMesh = NULL;
    MaterialGroup *newGroup = NULL;

    int numberCoordPerVertex = 0;

    bool first = true;

    unsigned int vertexSize = 0;
    unsigned int normalsSize = 0;
    unsigned int uvsSize = 0;

    do
    {
        std::getline(file, line);

        if (!file.eof())
        {
            //Ignored # Comments, Ignored , s Shading
            if (line[0] == 'm') //Read Mtl
            {
                readMtl(line, path);
            }
            else if (line[0] == 'o') //Read Object Name
            {
                std::strtok((char*)line.c_str(), " ");
                Mesh m;
                m.setName(std::strtok(NULL, " "));
                Meshes.push_back(m);

                if (newMesh != NULL)
                {
                    vertexSize += newMesh->getVertexsSize();
                    normalsSize += newMesh->getNormalsize();
                    uvsSize += newMesh->getUvsSize();
                }

                newMesh = &Meshes.back();
            }
            else if (line[0] == 'v') //Read vertex
            {
                if (newMesh == NULL) {
                    Mesh m;
                    m.setName("UNNAMED");
                    Meshes.push_back(m);
                    newMesh = &Meshes.back();
                }
                readVertex(line, newMesh, first);
            }
            else if (line[0] == 'u') //Read begin off material group
            {
                if (newMesh == NULL) {
                    Mesh m;
                    m.setName("UNNAMED");
                    Meshes.push_back(m);
                    newMesh = &Meshes.back();
                }

                std::strtok((char*)line.c_str(), " ");

                std::string materialName = ::strtok(NULL, " ");

                MaterialGroup m;

                int i = 0;
                for (i = 0; Materials[i].getName()->compare(materialName) != 0; i++);

                m.setBaseMaterial(&Materials[i]);

                newMesh->AddMaterialGroup(m);
                newGroup = newMesh->getLastMaterialGroup();
            }
            else if (line[0] == 'f') //Read face of material group
            {
                if (newMesh == NULL) {
                    Mesh m;
                    m.setName("UNNAMED");
                    Meshes.push_back(m);
                    newMesh = &Meshes.back();
                }

                readTriangle(line, newGroup, newMesh, vertexSize + 1, normalsSize + 1, uvsSize + 1);
            }
        }
    }while (!file.eof());

    printf("Successfully loaded: %s/%s\n", path, name);

    Print();

    return true;
}

void Obj::normalize()
{
    double distance = 1.f / glm::distance(MinCoord, MaxCoord);

    Center.x = ((MinCoord.x + MaxCoord.x) / 2);
    Center.y = ((MinCoord.y + MaxCoord.y) / 2);
    Center.z = ((MinCoord.z + MaxCoord.z) / 2);


    for (int i = 0; i < Meshes.size(); i++)
    {
        std::vector<glm::vec3>* vertex = Meshes[i].getVertexs();
        unsigned long size = vertex->size();

        for (int j = 0; j < size; j++)
        {
            (*vertex)[j].x = ((*vertex)[j].x - Center.x) * distance;
            (*vertex)[j].y = ((*vertex)[j].y - Center.y) *  distance;
            (*vertex)[j].z = ((*vertex)[j].z - Center.z) *  distance;

        }
    }

    MinCoord.x = (MinCoord.x - Center.x) * distance;
    MinCoord.y = (MinCoord.y - Center.y) * distance;
    MinCoord.z = (MinCoord.z - Center.z) * distance;

    MaxCoord.x = (MaxCoord.x - Center.x) * distance;
    MaxCoord.y = (MaxCoord.y - Center.y) * distance;
    MaxCoord.z = (MaxCoord.z - Center.z) * distance;


    Center.x = 0;
    Center.y = 0;
    Center.z = 0;
}

void Obj::generateBuffers()
{
    int size = Meshes.size();

    for (int i = 0; i <size; i++)
        Meshes[i].GenBuffers();

    generateBoundingBox();
}

void Obj::render()
{
    int size = Meshes.size();
    for (int i = 0; i < size; i++)
        Meshes[i].render(MaterialAmbientID, MaterialDiffuseID, MaterialSpecularID, ShinninesID);
}

void Obj::renderBoundingBox()
{
    glBindVertexArray(VertexArrayBoundingBoxID);

    glDrawArrays(GL_LINES, 0, 24);
}

void Obj::setUniforms(GLuint materialAmbientID, GLuint materialDiffuseID, GLuint materialSpecularID, GLuint shinninesID)
{
    MaterialAmbientID = materialAmbientID;
    MaterialDiffuseID = materialDiffuseID;
    MaterialSpecularID = materialSpecularID;
    ShinninesID = shinninesID;
}

void Obj::readVertex(std::string line, Mesh* mesh, bool &first)
{
    if (line[1] == 'n')
    {
        float x, y, z;
        std::sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
        mesh->AddNormal(x, y, z);
    }
    else if (line[1] == 't')
    {
        float u, v;
        std::sscanf(line.c_str(), "vt %f %f", &u, &v);
        mesh->AddUv(u, v);
    }
    else if (line[1] != 'p' && line[1] != 'c')
    {
        float x, y, z;
        std::sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);

        if (first)
        {
            MinCoord.x = x; MinCoord.y = y; MinCoord.z = z;
            MaxCoord.x = x; MaxCoord.y = y; MaxCoord.z = z;
            first = false;
        }
        else
        {
            MinCoord.x = fmin(MinCoord.x, x); MinCoord.y = fmin(MinCoord.y, y); MinCoord.z = fmin(MinCoord.z, z);
            MaxCoord.x = fmax(MaxCoord.x, x); MaxCoord.y = fmax(MaxCoord.y, y); MaxCoord.z = fmax(MaxCoord.z, z);
        }

        mesh->AddVertex(x, y, z);
    }
}

bool Obj::readTriangle(std::string line, MaterialGroup* group, Mesh* mesh, unsigned int vertexOffset, unsigned int normalOffset, unsigned int uvOffset)
{
    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

    if (mesh->getNormalsize() == 0 && mesh->getUvsSize() == 0)
    {
        int matches = sscanf(line.c_str(), "f %d %d %d", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);

        if (matches != 3)
        {
            printf("Corrupted file\n");
            return false;
        }

        for (int i = 0; i < 3; i++)
            vertexIndex[i] -= vertexOffset;

        group->addTriangle(vertexIndex);
    }
    else if (mesh->getNormalsize() == 0 && mesh->getUvsSize() != 0)
    {
        int matches = sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
        if (matches != 6)
        {
            printf("Corrupted file\n");
            return false;
        }

        for (int i = 0; i < 3; i++)
        {
            vertexIndex[i] -= vertexOffset;
            uvIndex[i] -= uvOffset;
        }

        group->addTriangle(vertexIndex, uvIndex);
    }
    else if (mesh->getNormalsize() != 0 && mesh->getUvsSize() == 0)
    {
        int matches = sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
        if (matches != 6)
        {
            printf("Corrupted file\n");
            return false;
        }

        for (int i = 0; i < 3; i++)
        {
            vertexIndex[i] -= vertexOffset;
            normalIndex[i] -= normalOffset;
        }

        group->addTriangle(vertexIndex, normalIndex);
    }
    else
    {
        int matches = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
        if (matches != 9)
        {
            printf("Corrupted file\n");
            return false;
        }

        for (int i = 0; i < 3; i++)
        {
            vertexIndex[i] -= vertexOffset;
            normalIndex[i] -= normalOffset;
            uvIndex[i] -= uvOffset;
        }

        group->addTriangle(vertexIndex, normalIndex, uvIndex);
    }

    return true;
}

bool Obj::readMtl(std::string line, const char* path)
{
    std::strtok((char*)line.c_str(), " ");
    std::string fileName = path;
    fileName = fileName + "/" + std::strtok(NULL, " ");

    int i = fileName.length() - 1;
    while (fileName[i] == '\r' || fileName[i] == '\n') {
        fileName[i] = '\0';
        i--;
    }
    printf("Loading MtlFile: %s\n", fileName.c_str());

    std::ifstream file;
    file.open(fileName.c_str(), std::ios::in);

    if (!file.is_open())
    {
        printf("File not found: %s\n", fileName.c_str());
        return false;
    }

    bool newMaterial = false;
    do
    {
        if(!newMaterial) std::getline(file, line);

        newMaterial = false;

        if (!file.eof())
        {
            if (line[0] == 'n') //New Material
            {
                newMaterial = true;
                Materials.push_back(readMaterial(line, file));
            }
        }
    } while (!file.eof());

    printf("Successfully loaded: %s\n", fileName.c_str());

    return true;
}

Material Obj::readMaterial(std::string &line, std::ifstream &file)
{
    Material newMaterial;

    std::strtok((char*)line.c_str(), " ");

    newMaterial.setName(std::strtok(NULL, " "));

    do
    {
        std::getline(file, line);

        if (!file.eof() && line[0] != 'n')
        {
            if (line[0] == 'K') //Material Color
            {
                float r, g, b;

                if (line[1] == 'a') //Ambiental
                {
                    sscanf(line.c_str(), "Ka %f %f %f", &r, &g, &b);
                    newMaterial.setKa(r, g, b);
                }
                else if (line[1] == 'd') //Diffuse
                {
                    sscanf(line.c_str(), "Kd %f %f %f", &r, &g, &b);
                    newMaterial.setKd(r, g, b);
                }
                else if (line[1] == 's') //Especular
                {
                    sscanf(line.c_str(), "Ks %f %f %f", &r, &g, &b);
                    newMaterial.setKs(r, g, b);
                }
            }
            else if (line[0] == 'N') //Ns Shininess
            {
                float ns;
                sscanf(line.c_str(), "Ns %f", &ns);
                newMaterial.setNs(ns);
            }
            else if (line[0] == 'i') //Illum model
            {
                unsigned short i;
                sscanf(line.c_str(), "illum %hd", &i);
                newMaterial.setIllum(i);
            }
            else if (line[0] == 'T') // Transparency
            {
                float t;
                sscanf(line.c_str(), "Tr %f", &t);
                newMaterial.setTr(t);
            }
            else if (line[0] == 'd')
            {
                /** TODO d can be Tr or disp (displacementMap), decal(decalMap)**/
            }
            else if (line[0] == 'm')
            {
                /** TODO map_Ka, map_Ns, map_d, map_bump **/
            }
            else if (line[0] == 'b')
            {
                /** TODO bump or map_bump **/
            }
        }

    } while (line[0] != 'n' && !file.eof());

    return newMaterial;
}

void Obj::Print()
{
    printf("Number of Materials: %d\n", Materials.size());

    for (int i = 0; i < Materials.size(); i++) Materials[i].Print();

    printf("Number of Objects: %d\n", Meshes.size());

    for (int i = 0; i < Meshes.size(); i++) Meshes[i].Print();
}

void Obj::generateBoundingBox()
{
    float bbox[]=
    {
        MinCoord.x, MaxCoord.y, MinCoord.z, MinCoord.x, MinCoord.y, MinCoord.z, //Linea Izquierda frontal
        MinCoord.x, MinCoord.y, MinCoord.z, MaxCoord.x, MinCoord.y, MinCoord.z, //Linea Inferior frontal
        MaxCoord.x, MinCoord.y, MinCoord.z, MaxCoord.x, MaxCoord.y, MinCoord.z, //Linea Derecha frontal
        MaxCoord.x, MaxCoord.y, MinCoord.z, MinCoord.x, MaxCoord.y, MinCoord.z, //Linea Superior frontal

        MinCoord.x, MaxCoord.y, MaxCoord.z, MinCoord.x, MinCoord.y, MaxCoord.z, //Linea Izquierda frontal
        MinCoord.x, MinCoord.y, MaxCoord.z, MaxCoord.x, MinCoord.y, MaxCoord.z, //Linea Inferior frontal
        MaxCoord.x, MinCoord.y, MaxCoord.z, MaxCoord.x, MaxCoord.y, MaxCoord.z, //Linea Derecha frontal
        MaxCoord.x, MaxCoord.y, MaxCoord.z, MinCoord.x, MaxCoord.y, MaxCoord.z, //Linea Superior frontal

        MinCoord.x, MinCoord.y, MinCoord.z, MinCoord.x, MinCoord.y, MaxCoord.z, //Linea inferior izquierda
        MinCoord.x, MaxCoord.y, MinCoord.z, MinCoord.x, MaxCoord.y, MaxCoord.z, //Linea superior izquierda

        MaxCoord.x, MinCoord.y, MinCoord.z, MaxCoord.x, MinCoord.y, MaxCoord.z, //Linea inferior derecha
        MaxCoord.x, MaxCoord.y, MinCoord.z, MaxCoord.x, MaxCoord.y, MaxCoord.z, //Linea superior derecha
    };

    glGenVertexArrays(1, &VertexArrayBoundingBoxID);
    glBindVertexArray(VertexArrayBoundingBoxID);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (72) * sizeof(float), bbox, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

std::vector<Material>* Obj::getMaterials()
{
    return &Materials;
}
