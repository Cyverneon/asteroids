#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Shader.h"


class ShaderManager {
public:
    static ShaderManager& getInstance(); // Singleton instance

    std::shared_ptr<Shader> loadShader(const std::string& tag, bool geom, const std::string& vertFile, const std::string& fragFile, const std::string& geomFile = "");
    std::shared_ptr<Shader> getShader(const std::string& tag);

    void clear();

private:
    ShaderManager() = default;
    ~ShaderManager() = default;
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<Shader>> _shaders;
};
 