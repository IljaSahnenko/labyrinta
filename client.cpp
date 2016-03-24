#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rwops.h>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/render.h>
#include <SDL2/SDL_video.h>

#include <unistd.h>


SDL_DisplayMode displayMode;

int request = SDL_GetDesktopDisplayMode(0,&displayMode);
const int player_WIGHT = 333;
const int player_HEIGH = 227;
double TESTtexture_SCALE = 1.0;
const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

void move_UP   (SDL_Renderer* render, SDL_Texture* texture, SDL_Rect &destrect, int offset = 5)
{
	destrect.y -= offset;
	SDL_RenderClear(render);
	SDL_RenderCopy(render, texture,NULL,&destrect);
}
void move_DOWN (SDL_Renderer* render, SDL_Texture* texture, SDL_Rect &destrect, int offset = 5)
{
	destrect.y += offset;
	SDL_RenderClear(render);
	SDL_RenderCopy(render, texture,NULL,&destrect);
}
void move_LEFT (SDL_Renderer* render, SDL_Texture* texture, SDL_Rect &destrect, int offset = 5)
{
	destrect.x -= offset;
	SDL_RenderClear(render);
	SDL_RenderCopy(render, texture,NULL,&destrect);
}
void move_RIGHT(SDL_Renderer* render, SDL_Texture* texture, SDL_Rect &destrect, int offset = 5)
{
	destrect.x += offset;
	SDL_RenderClear(render);
	SDL_RenderCopy(render, texture,NULL,&destrect);
}

void render_UPDATE(SDL_Renderer* render, SDL_Texture* texture[], SDL_Rect* destrect[], int states[])
{
	SDL_RenderClear(render);
	if(states[0]) SDL_RenderCopy(render, texture[0],NULL,destrect[0]);
	if(states[1]) SDL_RenderCopy(render, texture[1],NULL,destrect[1]);
}

int main()
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}


	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == NULL)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL)
	{
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Rect player_RECT;
	player_RECT.x = 0;   //Смещение полотна по Х
	player_RECT.y = 0;   //Смещение полотна по Y
	player_RECT.w = 333; //Ширина полотна
	player_RECT.h = 227; //Высота полотна

	SDL_Rect background_RECT;

	background_RECT.x = 0;
	background_RECT.y = 0;
	background_RECT.w = displayMode.w;
	background_RECT.h = displayMode.h;

	SDL_Surface *BMP_background = SDL_LoadBMP("res/hello.bmp");
	if (BMP_background == NULL)
	{
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Delay(20000);
	SDL_Texture *background = SDL_CreateTextureFromSurface(ren, BMP_background);
	SDL_FreeSurface(BMP_background); //Очищение памяти поверхности

	SDL_Texture *player =  IMG_LoadTexture(ren, "res/kosmos-81.jpg");
	if (player == NULL)
	{
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		return 1;
		/*
			}
						SDL_Event event;
						bool quit = false;
						while(!quit)
							while(SDL_PollEvent(&event))
							{
								SDL_PumpEvents(); // обработчик событий.
							}

		*/
		SDL_RenderClear(ren); //Очистка рендера
		SDL_RenderCopy(ren, background, NULL, &background_RECT); //Копируем в рендер фон
		SDL_RenderCopy(ren, player, NULL, &player_RECT); //Копируем в рендер персонажа
		SDL_RenderPresent(ren); //Погнали!!
		/*
			if((keyboardState[SDL_SCANCODE_UP])||(keyboardState[SDL_SCANCODE_W]))
				move_UP(ren,player,player_RECT);

			if((keyboardState[SDL_SCANCODE_DOWN])||(keyboardState[SDL_SCANCODE_S]))
				move_DOWN(ren,player,player_RECT);

			if((keyboardState[SDL_SCANCODE_LEFT])||(keyboardState[SDL_SCANCODE_A]))
				move_LEFT(ren,player,player_RECT);

			if((keyboardState[SDL_SCANCODE_RIGHT])||(keyboardState[SDL_SCANCODE_D]))
				move_RIGHT(ren,player,player_RECT);


		//ZOOM----------------------------------------------------------------
			if(keyboardState[SDL_SCANCODE_KP_PLUS])
			{
				TESTtexture_SCALE += 0.02;
				player_RECT.h = player_HEIGH * TESTtexture_SCALE;
				player_RECT.w = player_WIGHT * TESTtexture_SCALE;
			}
			if(keyboardState[SDL_SCANCODE_KP_MINUS])
			{
				TESTtexture_SCALE -= 0.02;
				player_RECT.h = player_HEIGH * TESTtexture_SCALE;
				player_RECT.w = player_WIGHT * TESTtexture_SCALE;
			}
		*/
	}
	SDL_RenderPresent(ren);

	SDL_DestroyTexture(player);
	SDL_DestroyTexture(background);

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
// g++ client.cpp -O2 -Wall -I/usr/include/SDL2 -D_REENTRANT -L/usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2_image -o client
