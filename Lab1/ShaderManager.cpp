#include "ShaderManager.h"
#include <iostream>

ShaderManager& ShaderManager::getInstance()
{
    static ShaderManager instance;
    return instance;
}

std::shared_ptr<Shader> ShaderManager::loadShader(const std::string& tag, const std::string& vertFile, const std::string& fragFile)
{
    if (_shaders.find(tag) != _shaders.end())
    {
        std::cerr << "Shader already exists with tag: " << tag << std::endl;
        return _shaders[tag];
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>(); // declare shared pointer on the heap. 
    shader->init(vertFile, fragFile);
    _shaders[tag] = shader;

    return shader;
}

std::shared_ptr<Shader> ShaderManager::getShader(const std::string& tag)
{
    if (_shaders.find(tag) == _shaders.end())
    {
        std::cerr << "Shader not found: " << tag << std::endl;
        return nullptr;
    }
    return _shaders[tag];
}

void ShaderManager::clear()
{
    _shaders.clear();
}
