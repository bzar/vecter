#include "common.h"

typedef struct Segment
{
  int ax, ay, bx, by;
} Segment;

Segment WALLS[4] = {
  { 1, -1, 1, 1 },
  { 1, 1, -1, 1 },
  { -1, 1, -1, -1},
  { -1, -1, 1, -1}
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
  v2d vel0 = { fix16_from_int(2), 0 };
  v2d pos1 = { fix16_one, 0 };
  vecterActorVelocity(world, actorId, &vel0);

  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos1) && "Invalid position after +x wall collision");

  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos1) && "Invalid position after second +x wall collision");

  v2d vel1 = { 0, fix16_from_int(-2) };
  v2d pos2 = { fix16_one, -fix16_one };
  vecterActorVelocity(world, actorId, &vel1);

  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos2) && "Invalid position after -y wall collision");

  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos2) && "Invalid position after second -y wall collision");

  v2d vel2 = { fix16_from_int(-4), 0 };
  v2d pos3 = { -fix16_one, -fix16_one };
  vecterActorVelocity(world, actorId, &vel2);

  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos3) && "Invalid position after -x wall collision");

  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos3) && "Invalid position after second -x wall collision");

  v2d vel3 = { 0, fix16_from_int(40) };
  v2d pos4 = { -fix16_one, fix16_one };
  vecterActorVelocity(world, actorId, &vel3);

  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos4) && "Invalid position after +y wall collision");

  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos4) && "Invalid position after second +y wall collision");

  v2d vel4 = { fix16_from_int(4000), 0 };
  v2d pos5 = { fix16_one, fix16_one };
  vecterActorVelocity(world, actorId, &vel4);

  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos5) && "Invalid position after +x2 wall collision");

  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos5) && "Invalid position after second +x2 wall collision");

  vecterWorldFree(world);

  return EXIT_SUCCESS;
}

