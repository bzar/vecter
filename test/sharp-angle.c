#include "common.h"

typedef struct Segment
{
  int ax, ay, bx, by;
} Segment;

Segment WALLS[] = {
  { -1, -1, 1, -1 },
  { 1, -1, -10000, 0 }
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
  v2d vel0 = { 0, fix16_from_int(-2) };
  vecterActorVelocity(world, actorId, &vel0);

  vecterWorldUpdate(world, fix16_one);

  v2d vel1 = { fix16_from_int(20), 0 };
  v2d pos2 = { fix16_one, -fix16_one };
  vecterActorVelocity(world, actorId, &vel1);
  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos2) && "Invalid position after first corner collision");
  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos2) && "Invalid position after second corner collision");

  v2d vel2 = { fix16_from_int(20), -fix16_one };
  v2d pos3 = { fix16_one, -fix16_one };
  vecterActorVelocity(world, actorId, &vel2);
  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos3) && "Invalid position after third corner collision");

  v2d vel3 = { fix16_from_int(20), fix16_one };
  v2d pos4 = { fix16_one, -fix16_one };
  vecterActorVelocity(world, actorId, &vel3);
  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos4) && "Invalid position after fourth corner collision");
  vecterWorldFree(world);

  return EXIT_SUCCESS;
}

