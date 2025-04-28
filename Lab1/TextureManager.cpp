#include "TextureManager.h"

TextureManager& TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

std::shared_ptr<Texture> TextureManager::loadTexture(const std::string& tag, const std::string& file)
{
    if (_textures.find(tag) != _textures.end())
    {
        std::cerr << "Texture already exists with tag: " << tag << std::endl;
        return _textures[tag];
    }

    std::shared_ptr<Texture> texture = std::make_shared<Texture>();
    texture->init(file);
    _textures[tag] = texture;

    return texture;
}

std::shared_ptr<Texture> TextureManager::getTexture(const std::string& tag)
{
    if (_textures.find(tag) == _textures.end())
    {
        std::cerr << "Texture not found: " << tag << std::endl;
        return nullptr;
    }

    return _textures[tag];
}

void TextureManager::clear()
{
    _textures.clear();
}