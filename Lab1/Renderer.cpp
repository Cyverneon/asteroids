#include "Renderer.h"

void Renderer::init(DisplayFacade* display, Camera* cam)
{
	_display = display;
	_camera = cam;
}

void Renderer::setupUBOs()
{
	UBOManager::getInstance().createUBO("Matrices", sizeof(glm::mat4) * 3, 0);

	// Initialize with identity matrices to avoid garbage data
	glm::mat4 identity = glm::mat4(1.0f);
	UBOManager::getInstance().updateUBOData("Matrices", 0, glm::value_ptr(identity), sizeof(glm::mat4));
	UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4), glm::value_ptr(identity), sizeof(glm::mat4));
	UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4) * 2, glm::value_ptr(identity), sizeof(glm::mat4));
}

void Renderer::setActiveShader(const std::string& shaderTag)
{
	if (ShaderManager::getInstance().getShader(shaderTag))
	{
		_activeShaderTag = shaderTag;
		std::cout << "Shader switched to: " << shaderTag << std::endl;
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
	// track current shader to avoid redundant binds
	Shader* currentShader = nullptr;

	for (auto& gameObjectPair : GameObjectManager::getInstance().getGameObjects())
	{
		std::shared_ptr<GameObject> obj = gameObjectPair.second;
		if (obj->shader != currentShader)
		{
			currentShader = obj->shader;
			currentShader->Bind();
		}

		// Update UBO for this object's transform
		glm::mat4 model = obj->transform->GetModel();
		glm::mat4 view = _camera->getView();
		glm::mat4 projection = _camera->getProjection();

		UBOManager::getInstance().updateUBOData("Matrices", 0, glm::value_ptr(model), sizeof(glm::mat4));
		UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4), glm::value_ptr(view), sizeof(glm::mat4));
		UBOManager::getInstance().updateUBOData("Matrices", sizeof(glm::mat4) * 2, glm::value_ptr(projection), sizeof(glm::mat4));

		obj->mesh->draw();
	}
}

void Renderer::drawGame() {
	clearScreenBuffer();
	renderGameObjects();
	_display->swapBuffers();
}