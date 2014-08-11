#include <stdlib.h>

#include "vecter.h"
#include "pool.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 480

int main(int argc, char** argv)
{
  (void) argc;
  (void) argv;

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow("vecter mouse-follow example", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_Point vertices[] = {
    {WIDTH, HEIGHT}, {0, HEIGHT}, {0, 0}, {WIDTH, 0}, {WIDTH, HEIGHT}
  };
  int num_vertices = sizeof(vertices) / sizeof(SDL_Point);

  vecterWorld* world = vecterWorldNew();
  chckIterPool* segmentIds = chckIterPoolNew(16, 16, sizeof(vecterSegmentId));

  int i;
  for(i = 1; i < num_vertices; ++i)
  {
    v2d base = {fix16_from_int(vertices[i-1].x), fix16_from_int(vertices[i-1].y)};
    v2d tip = {fix16_from_int(vertices[i].x), fix16_from_int(vertices[i].y)};
    vecterSegmentId segmentId = vecterSegmentAdd(world, &base, &tip);
    chckIterPoolAdd(segmentIds, &segmentId, NULL);
  }

  vecterActorId actorId = vecterActorAdd(world);
  v2d startPosition = {F16(WIDTH/2), F16(HEIGHT/2)};
  vecterActorPosition(world, actorId, &startPosition);
  vecterActorCollisionCallback(world, actorId, vecterCollisionProjection);

  v2d gravity = {0, F16(-98.1)};

  v2d segmentBase = {0, 0};
  v2d segmentTip = {0, 0};
  bool drawing = false;

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
    v2d mousePos = {fix16_from_int(mousex), fix16_from_int(HEIGHT - mousey)};
    if(buttons & SDL_BUTTON_RMASK)
    {
      vecterActorPosition(world, actorId, &mousePos);
    }
    if(buttons & SDL_BUTTON_LMASK)
    {
      if(!drawing)
      {
        segmentBase = mousePos;
      }
      segmentTip = mousePos;
      segmentTip.x = segmentBase.x + fix16_max(fix16_min(F16(128), segmentTip.x - segmentBase.x), F16(-128));
      segmentTip.y = segmentBase.y + fix16_max(fix16_min(F16(128), segmentTip.y - segmentBase.y), F16(-128));
      drawing = true;
    }
    else if(drawing)
    {
      vecterSegmentId segmentId = vecterSegmentAdd(world, &segmentBase, &segmentTip);
      chckIterPoolAdd(segmentIds, &segmentId, NULL);
      drawing = false;
    }

    fix16_t dt = F16(16.0/1000.0);
    v2d newVelocity;
    v2d frameGravity;
    v2d_mul_s(&frameGravity, &gravity, dt);
    v2d_add(&newVelocity, vecterActorGetVelocity(world, actorId), &frameGravity);
    vecterActorVelocity(world, actorId, &newVelocity);

    vecterWorldUpdate(world, dt);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const v2d* position = vecterActorGetPosition(world, actorId);

    SDL_SetRenderDrawColor(renderer, 64, 0, 0, 255);
    SDL_Rect rect = {
      fix16_to_int(position->x) - 16, HEIGHT - fix16_to_int(position->y) - 16,
      32, 32
    };
    SDL_RenderDrawRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 192, 192, 192, 255);

    chckPoolIndex iter = 0;
    vecterSegmentId* segmentId;
    while((segmentId = chckIterPoolIter(segmentIds, &iter)))
    {
      const v2d* base = vecterSegmentGetBase(world, *segmentId);
      const v2d* tip = vecterSegmentGetTip(world, *segmentId);
      SDL_RenderDrawLine(renderer,
                         fix16_to_int(base->x), HEIGHT - fix16_to_int(base->y),
                         fix16_to_int(tip->x), HEIGHT - fix16_to_int(tip->y));
    }

    if(drawing)
    {
      SDL_RenderDrawLine(renderer,
                         fix16_to_int(segmentBase.x), HEIGHT - fix16_to_int(segmentBase.y),
                         fix16_to_int(segmentTip.x), HEIGHT - fix16_to_int(segmentTip.y));
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(renderer, fix16_to_int(position->x), HEIGHT - fix16_to_int(position->y));
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
