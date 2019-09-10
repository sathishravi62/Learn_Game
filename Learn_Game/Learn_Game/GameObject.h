#ifndef GAMEOBJECT_H

#define GAMEOBJECT_H

#include<GL/glew.h>
#include<glm/glm.hpp>

#include"Texture.h"
#include"SpriteRenderer.h"

class GameObject
{
public:
	glm::vec2 Position, Size, Velocity;
	glm::vec3 Color;
	GLfloat Rotation;
	GLboolean IsSoild;
	GLboolean Destroyed;

	Texture2D Sprite;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	virtual void Draw(SpriteRenderer &renderer);
};

#endif // !GAMEOBJECT_H


