#include <stdlib.h>

#include "vecter.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

int main(int argc, char** argv)
{
  (void) argc;
  (void) argv;

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow("vecter mouse-follow example", 0, 0, 800, 480, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_Point vertices[] = {
    {0, 360}, {150, 360}, {50, 240}, {200, 280}, {250, 440}, {300, 280}, {400, 200}, {450, 80}, {500, 200}, {600, 200}, {550, 80}, {800, 80}
  };
  int num_vertices = sizeof(vertices) / sizeof(SDL_Point);

  vecterWorld* world = vecterWorldNew();

  int i;
  for(i = 1; i < num_vertices; ++i)
  {
    /* Invert segment directions to compensate SDL and vecter having inverted y-axes */
    v2d base = {fix16_from_int(vertices[i].x), fix16_from_int(vertices[i].y)};
    v2d tip = {fix16_from_int(vertices[i-1].x), fix16_from_int(vertices[i-1].y)};
    vecterSegmentAdd(world, &base, &tip);
  }

  vecterActorId actorId = vecterActorAdd(world);
  v2d startPosition = {F16(400), F16(240)};
  vecterActorPosition(world, actorId, &startPosition);
  vecterActorCollisionCallback(world, actorId, vecterCollisionProjection);

  bool running = true;
  while(running)
  {
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
      if(e.type == SDL_QUIT)
      {
        running = false;
      }
    }

    int mousex, mousey;
    Uint32 buttons = SDL_GetMouseState(&mousex, &mousey);
    if(buttons & SDL_BUTTON_LEFT)
    {
      const v2d* actorPos = vecterActorGetPosition(world, actorId);
      v2d mousePos = {fix16_from_int(mousex), fix16_from_int(mousey)};
      v2d velocity;
      v2d_sub(&velocity, &mousePos, actorPos);
      v2d_mul_s(&velocity, &velocity, F16(2));
      vecterActorVelocity(world, actorId, &velocity);
    }

    vecterWorldUpdate(world, F16(16.0/1000.0));

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const v2d* position = vecterActorGetPosition(world, actorId);

    SDL_SetRenderDrawColor(renderer, 64, 0, 0, 255);
    SDL_Rect rect = {
      fix16_to_int(position->x) - 16, fix16_to_int(position->y) - 16,
      32, 32
    };
    SDL_RenderDrawRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);
    SDL_RenderDrawLines(renderer, vertices, num_vertices);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(renderer, fix16_to_int(position->x), fix16_to_int(position->y));
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
