#ifndef GAME_H
#define GAME_H
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<vector>
#include"GameLevel.h"
#include"BallObject.h"


enum Gamestate {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT	
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;


const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);

const glm::vec2 BALL_VELOCITY(100.0f, -350.0f);

const GLfloat BALL_RADIUS = 12.5f;

class Game
{
public:	

	Gamestate State;
	GLboolean Keys[1024];
	GLuint Width, Height;
	std::vector<GameLevel> Levels;
	std::vector<GameObject*> Bullets;
	GLuint	Level;

	Game(GLuint width, GLuint height);
	~Game();

	void Init();
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
	
	void DoCollision();
	
	
	void ResetLevel();
	void ResetPlayer();
	

};
#endif // !GAME_H



