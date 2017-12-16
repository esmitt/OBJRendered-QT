#ifndef OBJFACTORY_H
#define OBJFACTORY_H

/**
 * @brief The ObjFactory Static class used to load and manage OBJ models.
 */
class ObjFactory
{
public:
    /*
     * All constructors, copy constructors, desctructors and assingment
     * operators are deleted to ensure this class is only accessed statically.
     */
    ObjFactory() = delete;
    ObjFactory(const ObjFactory &) = delete;
    ObjFactory(const ObjFactory *) = delete;
    ~ObjFactory() = delete;
    void operator=(const ObjFactory &) = delete;
    void operator=(const ObjFactory *) = delete;

    /**
     * @brief loadObj Loads an OBJ model from a file.
     * @param path The path to the model without the file name.
     * @param name The file name.
     * @return A model ID number that can be used to render the model.
     */
    static unsigned int loadObj(const char * path, const char * name);

    /**
     * @brief unloadObj Unloads an OBJ model from memory.
     * @param id The ID of the model to unload.
     */
    static void unloadObj(unsigned int id);

    /**
     * @brief renderObj Renders the given OBJ model using modern OpenGL. The calling function must set the shaders.
     * @param id The ID of the model to render.
     */
    static void renderObj(unsigned int id);

    /**
     * @brief setUniforms Sets the location of the uniform variables needed to render the model.
     * @param id The ID of the model
     * @param materialAmbientID The location of the ambient term uniform.
     * @param materialDiffuseID The location of the diffuse term uniform.
     * @param materialSpecularID The location of the specular term uniform.
     * @param shinninesID The location of the shinniness term uniform.
     */
    static void setUniforms(unsigned int id,
                            unsigned int materialAmbientID,
                            unsigned int materialDiffuseID,
                            unsigned int materialSpecularID,
                            unsigned int shinninesID);

private:
    /**
     * @brief last_free_id Next free ID available for an OBJ model.
     */
    static unsigned int last_free_id;
};

#endif // OBJFACTORY_H
