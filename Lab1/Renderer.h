#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <vector>
#include <windows.h>
#include "DisplayFacade.h" 
#include "UBOManager.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "GameObjectManager.h"
#include "TransformManager.h"
#include <glm/gtc/type_ptr.hpp>  

class Renderer
{
public:
	void init(DisplayFacade* display, Camera* cam);
	void setupUBOs();
	void setActiveShader(const std::string& shaderTag);
	void clearScreenBuffer();
	void renderGameObjects();
	void drawGame();

private:
	DisplayFacade* _display;

	Camera* _camera;

	std::string _activeShaderTag;
};

