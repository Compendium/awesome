#include "game.hpp"

motor::Game::Game()
{
	loop = true;
}

glm::vec2 rotate(glm::vec2 point, float angleDeg)
{
	float angle = angleDeg / 180.f * M_PI; //convert from degrees (0-360) to radians (0-2*PI)
	glm::vec2 rotPoint;
	rotPoint.x = point.x * cos(angle) - point.y * sin(angle);
	rotPoint.y = point.x * sin(angle) + point.y * cos(angle);
	return rotPoint;
}

ostream& operator<<(ostream& out, const glm::vec3& v)
{
	return out << "v3(" << v.x << "|" << v.y << "|" << v.z << ")";
}

ostream& operator<<(ostream& out, const motor::AABB& bb)
{
	return out << "AABB(" << bb.min << " - " << bb.max << ")";
}

double round(double r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

bool falling()
{
	return false;
}

float floorGetRest(float toFloor)
{
	return toFloor - floor(toFloor);
}

motor::AABB getBbOfBlock(vec3 p)
{
	return motor::AABB
		(
		 vec3(floor(p.x), floor(p.y), floor(p.z)),
		 vec3(ceil(p.x), ceil(p.y), ceil(p.z))
		);
}

void motor::Game::handlePlayer()
{
	float multiplierMove = 0;
	float multiplierRotate = 0;

	if(input->isPressed(Key::LSHIFT))
	{
		multiplierMove = 1.5f;
		multiplierRotate = 1.5f;
	}
	else if(input->isPressed(Key::RCTRL))
	{
		multiplierMove = 0.1f;
	}
	else
	{
		multiplierMove = 5.4f;
		multiplierRotate = 5.0f;
	}

	multiplierMove *= time->getFrameTime();
	multiplierRotate *= time->getFrameTime() * 20;

	glm::vec3 deltaMove(0, 0, 0);

	if(input->isPressed(Key::COMMA) && !input->isPressed(Key::O))//w
	{
		glm::vec3 delta = glm::vec3(0, 0, -1); //forward vector
		glm::vec2 deltaXZ = rotate(glm::vec2(delta.x, delta.z), camera->rotation.y);
		delta = glm::vec3(deltaXZ.x, delta.y, deltaXZ.y);
		deltaMove += delta;
		deltaMove = glm::normalize(deltaMove);
	}
	if(input->isPressed(Key::O) && !input->isPressed(Key::COMMA))//S
	{
		glm::vec3 delta = glm::vec3(0, 0, 1); //backward vector
		glm::vec2 deltaXZ = rotate(glm::vec2(delta.x, delta.z), camera->rotation.y);
		delta = glm::vec3(deltaXZ.x, delta.y, deltaXZ.y);
		deltaMove += delta;
		deltaMove = glm::normalize(deltaMove);
	}
	if(input->isPressed(Key::A) && !input->isPressed(Key::E))//a
	{
		glm::vec3 delta = glm::vec3(-1, 0, 0); //left vector
		glm::vec2 deltaXZ = rotate(glm::vec2(delta.x, delta.z), camera->rotation.y);
		delta = glm::vec3(deltaXZ.x, delta.y, deltaXZ.y);
		deltaMove += delta;
		deltaMove = glm::normalize(deltaMove);
	}
	if(input->isPressed(Key::E) && !input->isPressed(Key::A))//d
	{
		glm::vec3 delta = glm::vec3(1, 0, 0); //right vector
		glm::vec2 deltaXZ = rotate(glm::vec2(delta.x, delta.z), camera->rotation.y);
		delta = glm::vec3(deltaXZ.x, delta.y, deltaXZ.y);
		deltaMove += delta;
		deltaMove = glm::normalize(deltaMove);
	}

	if(input->isPressed(Key::LEFT))
	{
		camera->moveRotation(glm::vec3(0, -multiplierRotate, 0));
	}
	if(input->isPressed(Key::RIGHT))
	{
		camera->moveRotation(glm::vec3(0, multiplierRotate, 0));
	}
	if(input->isPressed(Key::UP))
	{
		camera->moveRotation(glm::vec3(-multiplierRotate, 0, 0));
	}
	if(input->isPressed(Key::DOWN))
	{
		camera->moveRotation(glm::vec3(multiplierRotate, 0, 0));
	}
	//----------------------------------
	pos = camera->position;

	vec3 delta = (deltaMove * multiplierMove) + (vel * time->getFrameTime());
	vec3 newPos = pos + delta;
	vec3 oldPos = pos;

	float& nx = newPos.x; float& ny = newPos.y; float& nz = newPos.z;
	float& ox = oldPos.x; float& oy = oldPos.y; float& oz = oldPos.z;
	
	//  /z
	// o----x
	// |
	// |y
	float playerRadius = .35;
	float playerHeight = 1.6;

	AABB    playerBox(vec3(nx - playerRadius, ny - playerHeight, nz - playerRadius), vec3(nx + playerRadius, ny, nz + playerRadius));
	AABB oldPlayerBox(vec3(ox - playerRadius, oy - playerHeight, oz - playerRadius), vec3(ox + playerRadius, oy, oz + playerRadius));

	AABB blockBelowOldPlayerBox = getBbOfBlock(vec3(ox, oy - playerHeight - 1, oz)); //1 = Block size

	//ONLY Y!
	if(world.getBlock(vec3(newPos.x, newPos.y - playerHeight, newPos.z)).type != BLOCK_AIR)//if block at FEET is not air
	{
		newPos.y = getBbOfBlock(vec3(newPos.x, newPos.y - playerHeight, newPos.z)).max.y + playerHeight;
		camera->setPosition(newPos);
	}
	else
	{
		//cout << "apllying gravity" << endl;
		vel.y -= 13.666f * time->getFrameTime() * 10.f; // Physics is evil stuff!
		camera->setPosition(newPos);
	}

	//TODO mix in proper acceleration and velocity + use AABBs

	//STILL ONLY Y!
	if(world.getBlock(vec3(newPos.x, newPos.y + .123, newPos.z)).type != BLOCK_AIR) // if block at head is not air
	{
		//newPos.y = getBbOfBlock(vec3(newPos.x, newPos.y, newPos.z)).min.y - .25;
		newPos.y -= delta.y + .2;
		//newPos.y = getBbOfBlock(vec3(oldPos.x, oldPos.y, oldPos.z)).min.y;
		//newPos.y = oldPos.y;
		//camera->setPosition(oldPos - vec3(0, 0, 0));
		if(vel.y > 0)
			vel.y = 0;
			
		camera->setPosition(newPos - vec3(0, 0, 0));
	}

	if(input->isPressed(Key::SPACE) && input->getKeyDelay(Key::SPACE) > 0.41f && (world.getBlock(floor(pos.x), floor(pos.y - playerHeight) - 1, floor(pos.z)).type != BLOCK_AIR))
	{
		input->resetKeyDelay(Key::SPACE);
		//if(world.getBlock(vec3(pos.x, pos.y + .4, pos.z)).type == BLOCK_AIR)
			vel.y = 12.f;
		//else
			//vel.y = 2.f;
		//pos.y += 10;
	}
	else
	{
		vel.y *= 0.8f;
	}
	if(input->isPressed(Key::W) && input->getKeyDelay(Key::W) > .2f)
	{
		input->resetKeyDelay(Key::W);
		settings.printPosition = !settings.printPosition;
	}
	if(settings.printPosition)
	{
		cout << pos << "\n";
	}

	if(input->isPressed(Key::BACKSPACE))
	{
		world.setBlock(int(pos.x) + 1, int(pos.y), int(pos.z), BLOCK_AIR);
		world.setBlock(int(pos.x) - 1, int(pos.y), int(pos.z), BLOCK_AIR);
		world.setBlock(int(pos.x) + 1, int(pos.y) - 1, int(pos.z), BLOCK_AIR);
		world.setBlock(int(pos.x) - 1, int(pos.y) - 1, int(pos.z), BLOCK_AIR);

		world.setBlock(int(pos.x), int(pos.y), int(pos.z) + 1, BLOCK_AIR);
		world.setBlock(int(pos.x), int(pos.y), int(pos.z) - 1, BLOCK_AIR);
		world.setBlock(int(pos.x), int(pos.y) - 1, int(pos.z) + 1, BLOCK_AIR);
		world.setBlock(int(pos.x), int(pos.y) - 1, int(pos.z) - 1, BLOCK_AIR);

		//world.setBlock(int(pos.x), int(pos.y - 1.6) - 1, int(pos.z), BLOCK_AIR);

		world.recalculateChunck(int(fabs(pos.x)), int(fabs(pos.y)), int(fabs(pos.z)));
		world.recalculateChunck(int(fabs(pos.x)) - 16, int(fabs(pos.y)), int(fabs(pos.z)));
		world.recalculateChunck(int(fabs(pos.x)) + 16, int(fabs(pos.y)), int(fabs(pos.z)));
		world.recalculateChunck(int(fabs(pos.x)), int(fabs(pos.y)), int(fabs(pos.z)) + 16);
		world.recalculateChunck(int(fabs(pos.x)), int(fabs(pos.y)), int(fabs(pos.z)) - 16);

		world.recalculateChunck(int(fabs(pos.x)) - 16, int(fabs(pos.y)), int(fabs(pos.z)) + 16);
		world.recalculateChunck(int(fabs(pos.x)) - 16, int(fabs(pos.y)), int(fabs(pos.z)) - 16);
		world.recalculateChunck(int(fabs(pos.x)) + 16, int(fabs(pos.y)), int(fabs(pos.z)) + 16);
		world.recalculateChunck(int(fabs(pos.x)) + 16, int(fabs(pos.y)), int(fabs(pos.z)) - 16);
	}

}

int motor::Game::main(Window *wndw, Input *inp, Time *tt)
{
	window = wndw;
	input = inp;
	time = tt;

	cout << endl;

	float oldTime = time->get();
	world.load(8, 8, 8, 16, 16, 16); // 128
	world.generate();
	cout << "world generation took " << time->get() - oldTime << " seconds" << endl;
	cout << endl;

	glActiveTexture(GL_TEXTURE0);
	tileset = new Image("data/tileset2.png");

	baseShader = new motor::Shader();
	baseShader->init();

	baseShader->attachVertexShader("data/base.vert");
	baseShader->attachFragmentShader("data/base.frag");
	baseShader->compile();

	int texUniform;
	texUniform = baseShader->getUniformLocation("texture");

	int positionAttrib;
	int texcoordAttrib;
	positionAttrib = baseShader->getAttributeLocation("position");
	texcoordAttrib = baseShader->getAttributeLocation("texcoord");

	baseShader->activate();

	camera = new Camera(input, baseShader);
	camera->setPerspective(45.0f, float(window->width) / float(window->height), window->near, window->far);
	camera->position = glm::vec3(0, 0, 0);

	cout << endl;

	glUniform1i(texUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tileset->data);

	//	glPolygonMode(GL_FRONT, GL_LINE);

	pos = vel = acc = glm::vec3(0, 0, 0);

	pos = glm::vec3(0.5, 5, 0.5);
	camera->position = pos;
	settings.printPosition = false;

	while(loop)
	{
		time->update();
		input->update(time, window);//, time);


		if(input->quit())
			return 0;
		if(input->windowResized())
		{
			cout << "handled!" << window->width << " " << window->height << endl;
			camera->setPerspective(45.0f, float(window->width) / float(window->height), 0.3f, window->far); 
		}

		if(input->isPressed(Key::H) && input->getKeyDelay(Key::H) > .5f)
		{
			input->resetKeyDelay(Key::H);
			settings.holdPosition = !settings.holdPosition;
		}
		if(!settings.holdPosition)
			handlePlayer();

		if(input->isPressed(Key::R) && input->getKeyDelay(Key::R) > .5f)
		{
			input->resetKeyDelay(Key::R);
			world.generate();
			camera->position = glm::vec3(0.5, 20, 0.5);
			cout << "regenerating" << endl;
		}

		camera->think();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		world.draw(positionAttrib, texcoordAttrib);
		SDL_GL_SwapBuffers();
	}
	return 0;
}
void motor::Game::update()
{

}

void motor::Game::draw()
{

}

void motor::Game::init()
{

}

void motor::Game::load()
{

}

void motor::Game::unload()
{

}
