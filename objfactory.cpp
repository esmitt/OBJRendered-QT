#include <iostream>
#include <string>
#include <map>
#include <memory>

#include "objfactory.h"
#include "obj.h"

#define GLEW_HACK_IMPLEMENTATION
#include "glew_hack.h"

static bool is_glew_initialized = false;
static std::map<unsigned int, std::shared_ptr<Obj>> models;
static std::map<std::string, unsigned int> model_cache;

unsigned int ObjFactory::last_free_id = 0;

unsigned int ObjFactory::loadObj(const char * path, const char *name) {
    if (!is_glew_initialized) {
        glew_hack();
        is_glew_initialized = true;
    }

    if (model_cache.count(std::string(path) + "/" + std::string(name)) == 0) {
        std::shared_ptr<Obj> obj = std::make_shared<Obj>();
        if (!obj->loadObj(path, name))
            throw std::string("Failed to load model ") + name;
        obj->normalize();
        obj->generateBuffers();
        model_cache.insert(std::pair<std::string, unsigned int>(std::string(path) + "/" + std::string(name), last_free_id));
        models.insert(std::pair<unsigned int, std::shared_ptr<Obj>>(last_free_id, obj));
        last_free_id++;
        return last_free_id - 1;
    } else {
        return model_cache.find(std::string(path) + "/" + std::string(name))->second;
    }
}

void ObjFactory::unloadObj(unsigned int id) {
    if (models.count(id) != 0) {
        auto obj = models.find(id);
        models.erase(obj);

        std::map<std::string, unsigned int>::iterator rem;
        for (auto it = model_cache.begin(); it != model_cache.end(); it++) {
            if (it->second == id) {
                rem = it;
                break;
            }
        }
        model_cache.erase(rem);
    }
}

void ObjFactory::renderObj(unsigned int id) {
    if (models.count(id) != 0) {
        models.find(id)->second->render();
    }
}

void ObjFactory::setUniforms(unsigned int id,
                             unsigned int materialAmbientID,
                             unsigned int materialDiffuseID,
                             unsigned int materialSpecularID,
                             unsigned int shinninesID)
{
    if (models.count(id) != 0) {
        models.find(id)->second->setUniforms(materialAmbientID, materialDiffuseID, materialSpecularID, shinninesID);
    }
}

