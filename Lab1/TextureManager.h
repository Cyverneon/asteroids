#pragma once
#include <unordered_map>
#include <memory>
#include <iostream>
#include "Texture.h"

class TextureManager
{
public:
    static TextureManager& getInstance(); // Singleton instance

    std::shared_ptr<Texture> loadTexture(const std::string& tag, const std::string& file);
    std::shared_ptr<Texture> getTexture(const std::string& tag);

    void clear();

private:
    TextureManager() = default;  // Private constructor (Singleton)
    ~TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
};

