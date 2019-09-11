#include "Game.h"
#include"ResourceManager.h"
#include"SpriteRenderer.h"
#include"GameObject.h"
#include"BallObject.h"
#include"ParticleGenerator.h"

SpriteRenderer *Renderer;
GameObject	   *Player;
BallObject	   *Ball;
ParticleGenerator *Particles;
glm::vec2 playerPos;

Game::Game(GLuint width, GLuint height) :State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
}

void Game::Init()
{
	ResourceManager::LoadShader("shaders/Vertex.vert", "shaders/Fragment.frag", "sprite");
	ResourceManager::LoadShader("shaders/particleVs.vert", "shaders/particleFS.frag", "particle");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").Use().SetMatrix4("projection",projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
	// Load Texture
	ResourceManager::LoadTexture("texture/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("texture/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("texture/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("texture/paddle.png", GL_TRUE, "paddle");
	ResourceManager::LoadTexture("texture/Ball.png", GL_TRUE, "ball");
	ResourceManager::LoadTexture("texture/particle.png", GL_TRUE, "particle");

	// set Render specific controls
	Shader myShader;
	myShader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(myShader);
	// Creating Particle
	Particles = new ParticleGenerator(
		ResourceManager::GetShader("particle"),
		ResourceManager::GetTexture("particle"), 500);
	// load level
	GameLevel one; one.Load("levels/one.txt", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("levels/two.txt", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("levels/three.txt", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("levels/four.txt", this->Width, this->Height * 0.5);
	GameLevel test; test.Load("levels/test.txt", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Levels.push_back(test);
	this->Level = 0;

	// creating Player
    playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

	// Creating Player;
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, BALL_VELOCITY, ResourceManager::GetTexture("ball"));

	
}

void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// Move playerboard
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0)
			{
				Player->Position.x -= velocity;

				if (Ball->Stuck)
				{
					Ball->Position.x -= velocity;
				}
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;

				if (Ball->Stuck)
				{
					Ball->Position.x += velocity;
				}
			}

		}

		if (this->Keys[GLFW_KEY_SPACE])
		{
			Ball->Stuck = false;
		}
		if (this->Keys[GLFW_KEY_V])
		{
			glm::vec2 bPos = Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
			
		}
	}
}

void Game::Update(GLfloat dt)
{
	Ball->Move(dt, this->Width);
	this->DoCollision();
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));

	if (Ball->Position.y >= this->Height) // Did ball reach bottom edge?
	{
		this->ResetLevel();
		this->ResetPlayer();
	}
}

void Game::Render()
{
	
	if (this->State == GAME_ACTIVE)
	{
		// Draw background
		Texture2D myTexture;
		myTexture = ResourceManager::GetTexture("background");
		Renderer->DrawSprite(myTexture, glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		// Draw level
		this->Levels[this->Level].Draw(*Renderer);
		// Draw player
		Player->Draw(*Renderer);
		Ball->Draw(*Renderer);
		Particles->Draw();
	}
}




void Game::ResetLevel()
{
	if (this->Level == 0)this->Levels[0].Load("levels/one.txt", this->Width, this->Height * 0.5f);
	else if (this->Level == 1)
		this->Levels[1].Load("levels/two.txt", this->Width, this->Height * 0.5f);
	else if (this->Level == 2)
		this->Levels[2].Load("levels/three.txt", this->Width, this->Height * 0.5f);
	else if (this->Level == 3)
		this->Levels[3].Load("levels/four.txt", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), BALL_VELOCITY);
}

GLboolean CheckCollisionCircle(GameObject &one, GameObject &two);
Collision CheckCollisionCircle(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 closest);

void Game::DoCollision()
{
	for (GameObject &box : this->Levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = CheckCollisionCircle(*Ball, box);
			if (std::get<0>(collision)) // If collision is true
			{
				// Destroy block if not solid
				if (!box.IsSoild)
					box.Destroyed = GL_TRUE;
				// Collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT) // Horizontal collision
				{
					Ball->Velocity.x = -Ball->Velocity.x; // Reverse horizontal velocity
					// Relocate
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
					if (dir == LEFT)
						Ball->Position.x += penetration; // Move ball to right
					else
						Ball->Position.x -= penetration; // Move ball to left;
				}
				else // Vertical collision
				{
					Ball->Velocity.y = -Ball->Velocity.y; // Reverse vertical velocity
					// Relocate
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
					if (dir == UP)
						Ball->Position.y -= penetration; // Move ball bback up
					else
						Ball->Position.y += penetration; // Move ball back down
				}
			}
		}
	}

	// Also check collisions for player pad (unless stuck)
	Collision result = CheckCollisionCircle(*Ball, *Player);
	if (!Ball->Stuck && std::get<0>(result))
	{
		// Check where it hit the board, and change velocity based on where it hit the board
		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
		GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
		GLfloat percentage = distance / (Player->Size.x / 2);
		// Then move accordingly
		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = BALL_VELOCITY.x * percentage * strength;
		//Ball->Velocity.y = -Ball->Velocity.y;
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); // Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
		// Fix sticky paddle
		Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
	}
}

GLboolean CheckCollisionCircle(GameObject & one, GameObject & two)
{
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// Collision y-axis?
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// Collision only if on both axes
	return collisionX && collisionY;
}



Collision CheckCollisionCircle(BallObject & one, GameObject & two)
{
	// Get center point circle first 
	glm::vec2 center(one.Position + one.Radius);
	// Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
	// Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Now that we know the the clamped values, add this to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// Now retrieve vector between center circle and closest point AABB and check if length < radius
	difference = closest - center;

	if (glm::length(difference) < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	else
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

Direction VectorDirection(glm::vec2 closest)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(closest), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}
