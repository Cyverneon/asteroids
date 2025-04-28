#include "MeshManager.h"

MeshManager& MeshManager::getInstance()
{
    static MeshManager instance;
    return instance;
}

std::shared_ptr<Mesh> MeshManager::loadMesh(const std::string& tag, const std::string& file)
{
    if (_meshes.find(tag) != _meshes.end())
    {
        std::cerr << "Mesh already exists with tag: " << tag << std::endl;
        return _meshes[tag];
    }

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    mesh->loadModel(file);
    _meshes[tag] = mesh;

    return mesh;
}

std::shared_ptr<Mesh> MeshManager::getMesh(const std::string& tag)
{
    if (_meshes.find(tag) == _meshes.end())
    {
        std::cerr << "Mesh not found: " << tag << std::endl;
        return nullptr;
    }

    return _meshes[tag];
}

void MeshManager::clear()
{
    _meshes.clear();
}