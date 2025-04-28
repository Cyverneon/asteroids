#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Mesh.h"

class MeshManager
{
public:
    static MeshManager& getInstance(); // Singleton instance

    std::shared_ptr<Mesh> loadMesh(const std::string& tag, const std::string& file);
    std::shared_ptr<Mesh> getMesh(const std::string& tag);

    void clear();

private:
    MeshManager() = default;  // Private constructor (Singleton)
    ~MeshManager() = default;
    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<Mesh>> _meshes;
};

