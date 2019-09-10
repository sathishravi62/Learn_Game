#ifndef PARTICLE_GENERATOR
#define PARTICLE_GENERATOR


#include<vector>

#include<GL/glew.h>
#include<glm/glm.hpp>

#include"Shader.h"
#include"Texture.h"
#include"GameObject.h"
#include<iostream>

struct Particle
{
	glm::vec2 Positon, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle() :Positon(0.0f), Velocity(0.0f), Color(0.0f), Life(0.0f) {}
};


class ParticleGenerator
{
public:
	ParticleGenerator(Shader shader, Texture2D texture, GLuint anount);

	void Update(GLfloat dt, GameObject &object, GLuint newParticle, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void Draw();

private:
	std::vector<Particle> particles;
	GLuint amount;
	Shader shader;
	Texture2D texture;
	GLuint VAO;

	void init();
	GLuint firstUnunsedParticle();

	void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif // !PARTICLE_GENERATOR

