#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <vector>
#define fps 60

using namespace std;

struct cell
{
	bool dead;
	bool buff_dead;
};

int main(int argc, char *argv[])
{
	float fps_ms = 1000.0 / fps; //60
	float scale = 3;

	int screen_length = 300;
	int cell_size = 1; //5
	int chance = 2;	   //20
	int i, j, alive;
	int grid_size = screen_length / cell_size;

	bool run = true;
	bool pause = 0;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *screen = SDL_CreateWindow("Conway", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_length, screen_length, SDL_WINDOW_FULLSCREEN);
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;
	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, render_flags);
	//SDL_RenderSetScale(renderer, scale, scale);
	SDL_RenderSetLogicalSize(renderer, screen_length, screen_length);

	srand(time(nullptr));

	Uint32 totalFrameTicks = 0;
	Uint32 totalFrames = 0;
	Uint32 endTicks;
	Uint64 endPerf;
	Uint64 framePerf;
	Uint32 startTicks;
	Uint64 startPerf;

	vector<vector<cell>> cells(grid_size);

	for (i = 0; i < grid_size; i++)
	{
		cells[i] = vector<cell>(grid_size);
		for (j = 0; j < grid_size; j++)
		{
			cells[i][j].dead = rand() % chance;
			cells[i][j].buff_dead = cells[i][j].dead;
		}
	}

	while (run)
	{
		totalFrames++;
		startTicks = SDL_GetTicks();
		startPerf = SDL_GetPerformanceCounter();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				run = false;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE:
					pause = not pause;
					break;

				case SDLK_RETURN:
					for (i = 0; i < grid_size; i++)
					{
						for (j = 0; j < grid_size; j++)
						{
							cells[i][j].dead = rand() % chance;
							cells[i][j].buff_dead = cells[i][j].dead;
						}
					}
					break;

				default:
					break;
				}
				break;

			default:
				break;
			}
		}

		//draw
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		for (i = 0; i < grid_size; i++)
		{
			for (j = 0; j < grid_size; j++)
			{
				cells[i][j].dead = cells[i][j].buff_dead;
			}
		}

		for (i = 0; i < grid_size; i++)
		{
			for (j = 0; j < grid_size; j++)
			{
				if (not cells[i][j].dead)
				{
					SDL_RenderDrawPoint(renderer, i, j);
				}
				if (not pause)
				{
					if (i > 0 and i < grid_size - 1 and j > 0 and j < grid_size - 1)
					{
						alive = 8 - cells[i - 1][j - 1].dead -
								cells[i][j - 1].dead -
								cells[i + 1][j - 1].dead -
								cells[i - 1][j].dead -
								cells[i + 1][j].dead -
								cells[i - 1][j + 1].dead -
								cells[i][j + 1].dead -
								cells[i + 1][j + 1].dead;

						if (not cells[i][j].dead)
						{
							if (not(alive > 1 and alive < 4))
							{
								cells[i][j].buff_dead = true;
							}
						}
						else
						{
							if (alive == 3)
							{
								cells[i][j].buff_dead = false;
							}
						}
					}

					else
					{
						if (not cells[i][j].dead)
						{
							cells[i][j].buff_dead = true;
						}
					}
				}
			}
		}

		SDL_RenderPresent(renderer);

		endTicks = SDL_GetTicks();
		endPerf = SDL_GetPerformanceCounter();
		framePerf = (endPerf - startPerf) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
		cout << (1.0f / ((endPerf - startPerf) / (float)SDL_GetPerformanceFrequency())) << endl;
		SDL_Delay(floor(fps_ms - framePerf));
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);
	SDL_Quit();
	return 0;
}
