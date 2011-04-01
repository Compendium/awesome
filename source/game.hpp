#ifndef _GAME_HPP
#define _GAME_HPP

#include "state.hpp"
#include "motor/graphics/image.hpp"
#include "motor/graphics/shader.hpp"
#include "motor/graphics/camera.hpp"
#include "motor/utility/log.hpp"

#include <motor/math/glm/glm.hpp>
#include <motor/math/glm/gtc/matrix_transform.hpp>
#include <motor/math/glm/gtx/projection.hpp>
#include <motor/math/glm/gtc/type_ptr.hpp>
#include <GL/glut.h>
#include <cmath>

#include "motor/graphics/chunk.hpp"
#include "motor/graphics/world.hpp"

using namespace std;

namespace motor
{
	class Game : public State
	{
		public:
			Game();
			int main(Window*, Input*);
			void init();
			void load();
			void unload();
			void update();
			void draw();

		private:
			Image *tileset;

			Input *input;
			Window *window;
			Camera *cam;

			bool loop;

			glm::mat4 projectionMatrix;
			glm::mat4 viewMatrix;
			glm::mat4 modelMatrix;
	};
}
#endif
