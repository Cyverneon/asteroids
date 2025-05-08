#include "Renderer.h"

void Renderer::init(DisplayFacade* display, Camera* cam)
{
	_display = display;
	_camera = cam;
}

void Renderer::setActiveShader(const std::string& shaderTag)
{
	if (ShaderManager::getInstance().getShader(shaderTag))
	{
		_activeShaderTag = shaderTag;
		ShaderManager::getInstance().getShader(_activeShaderTag)->Bind();
	}
	else
	{
		std::cerr << "Error: Shader not found - " << shaderTag << std::endl;
	}
}

void Renderer::clearScreenBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::renderGameObjects()
{
	for (auto& obj : GameObjectManager::getInstance().getGameObjects())
	{
		// obj.first is the tag
		// obj.second is the pointer to the actual GameObject

		// don't bother rebinding shader if it's the same
		if (obj.second->_shaderTag != _activeShaderTag)
			setActiveShader(obj.second->_shaderTag);

		for (int i = 0; i < obj.second->_textureTags.size(); i++)
		{
			TextureManager::getInstance().getTexture(obj.second->_textureTags[i])->Bind(i);
		}

		glm::mat4 model = obj.second->_transform->GetModel();
		UBOManager::getInstance().updateUBOData("Matrices", 0, glm::value_ptr(model), sizeof(glm::mat4));
		// view and projection matrices don't actually need updated for asteroids since the camera never moves
		//glm::mat4 view = _camera->getView();
		//glm::mat4 projection = _camera->getProjection();
		//UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4), glm::value_ptr(view), sizeof(glm::mat4));
		//UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4) * 2, glm::value_ptr(projection), sizeof(glm::mat4));
		
		// use the scale of the object for the uv multiplier so for example on smaller asteroids the texture will appear the same size
		UBOManager::getInstance().updateUBOData("MatInfo", 0, &obj.second->_transform->scale.x, sizeof(float));

		MeshManager::getInstance().getMesh(obj.second->_meshTag)->draw();

		// Very jank code to render collision spheres
		//glm::vec3 tempScale = obj.second->_transform->scale;
		//obj.second->_transform->scale = glm::vec3((obj.second->_physicsObject._collisionRadius));
		//model = obj.second->_transform->GetModel();
		//UBOManager::getInstance().updateUBOData("Matrices", 0, glm::value_ptr(model), sizeof(glm::mat4));
		//MeshManager::getInstance().getMesh("Sphere")->draw();
		//obj.second->_transform->scale = tempScale;
	}
}

void Renderer::drawGame()
{
	clearScreenBuffer();
	renderGameObjects();
	_display->swapBuffers();
}