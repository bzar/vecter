#include "common.h"

int main(int argc, char** argv)
{
  (void) argv;
  (void) argc;

  vecterWorld* world = vecterWorldNew();
  vecterActorId actorId = vecterActorAdd(world);

  v2d pos0 = {fix16_one, fix16_one};
  vecterActorPosition(world, actorId, &pos0);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos0) && "Invalid position set");

  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos0) && "Invalid position after zero move");

  v2d vel0 = { fix16_one, 0 };
  vecterActorVelocity(world, actorId, &vel0);
  assert(equalV2d(*vecterActorGetVelocity(world, actorId), vel0) && "Invalid velocity set");

  v2d pos1 = { fix16_from_int(2), fix16_one };
  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos1) && "Invalid position after move 1");

  v2d vel1 = { fix16_one, fix16_one };
  v2d pos2 = { fix16_from_int(3), fix16_from_int(2) };
  vecterActorVelocity(world, actorId, &vel1);
  vecterWorldUpdate(world, fix16_one);
  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos2) && "Invalid position after move 2");

  v2d vel2 = { fix16_from_int(-3), fix16_from_int(-2) };
  v2d pos3 = { 0, 0 };
  vecterActorVelocity(world, actorId, &vel2);
  vecterWorldUpdate(world, fix16_one);

  assert(equalV2d(*vecterActorGetPosition(world, actorId), pos3) && "Invalid position after move 3");

  vecterWorldFree(world);

  return EXIT_SUCCESS;
}
