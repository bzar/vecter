#include "common.h"

typedef struct Segment
{
  int ax, ay, bx, by;
} Segment;

Segment WALLS[] = {
  { 10,  -1, 10,  1 },
  { 100, -1, 100, 1 },
  { 1,   -1, 1,   1 },
  { 5,   -1, 5,   1 },
};
int NUM_WALLS = sizeof(WALLS) /  sizeof(Segment);

int main(int argc, char** argv)
{
  (void) argv;
  (void) argc;

  vecterWorld* world = vecterWorldNew();

  int i;
  for(i = 0; i < NUM_WALLS; ++i)
  {
    v2d base = {fix16_from_int(WALLS[i].ax), fix16_from_int(WALLS[i].ay)};
    v2d tip = {fix16_from_int(WALLS[i].bx), fix16_from_int(WALLS[i].by)};
    vecterSegmentAdd(world, &base, &tip);

  }

  vecterActorId actorId = vecterActorAdd(world);
  v2d vel0 = { fix16_from_int(200), 0 };
  v2d pos1 = { fix16_one, 0 };
  vecterActorVelocity(world, actorId, &vel0);

  vecterWorldUpdate(world, fix16_one);

  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos1) && "Invalid position");
  vecterWorldUpdate(world, fix16_one);

  vecterWorldFree(world);

  return EXIT_SUCCESS;
}

