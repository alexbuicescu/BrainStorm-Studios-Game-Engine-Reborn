#include "BS_Game_Demo.h"
#include "BS_Engine.h"
#include "BSTexture.h"
#include "BSOpenGL.h"
#include "BS_Object.h"

long BS_Game_Demo::current_time;
int BS_Game_Demo::FPS = 60;
BSTexture *gOpenGLTexture2 = new BSTexture;
BSObstacles *obstacle2 = new BSObstacles();
BSLevelMenu *theLevels2 = new BSLevelMenu;


BS_Object *my_first_object;

void BS_Game_Demo::update_input()
{
    SDL_PollEvent(&BS_Engine::_sdl_event);

    if(BS_Engine::_sdl_event.type == SDL_QUIT)
    {
        BS_Engine::set_quit_game_status(true);
    }
}

void BS_Game_Demo::update_physics()
{

}

void BS_Game_Demo::update_game()
{
    update_input();
    update_physics();
    _my_camera->update_camera();

	_my_shader_program->set_uniform(BS_Available_Shaders::time(), SDL_GetTicks() / 10000.0);
}

void BS_Game_Demo::render_game()
{
    my_first_object->draw();
}

void BS_Game_Demo::run_main_loop()
{
    while(BS_Engine::get_quit_game_status() == false)
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			current_time = SDL_GetTicks();

			update_game();
			render_game();

			current_time = SDL_GetTicks() - current_time;

			if(current_time < (double)(1000 / FPS))
			{
				SDL_Delay((double)(1000 / FPS) - current_time);
			}
		}
		SDL_GL_SwapBuffers();
    }
}

bool BS_Game_Demo::initialize_media()
{
	my_first_object = new BS_Object(0, 0, 5, 5, "textures.png");

	return true;
}

void BS_Game_Demo::loadCoinsAndDeaths()
{
	std::ifstream coinsAndDeaths("coinsAndDeaths");
	coinsAndDeaths >> coinsTotalNo >> deathsTotalNo;
}
