#include "common.h"

int main(int argc, char** argv)
{
  (void) argv;
  (void) argc;

  vecterWorld* world = vecterWorldNew();
  vecterActorId actorId = vecterActorAdd(world);
  v2d wall1Data[3] = {{fix16_one, -fix16_one}, {fix16_one, fix16_one}, {F16(-2), 0}};
  vecterSegmentId wall1 = vecterSegmentAdd(world, wall1Data, wall1Data +  1);
  vecterSegmentVelocity(world, wall1, wall1Data + 2);

  vecterWorldUpdate(world, fix16_one);

  v2d wall1pos1[2] = {{-fix16_one, -fix16_one}, {-fix16_one, fix16_one}};
  assert(equalV2d(wall1pos1[0], *vecterSegmentGetBase(world, wall1)) && "Wall 1 base did not move");
  assert(equalV2d(wall1pos1[1], *vecterSegmentGetTip(world, wall1)) && "Wall 1 tip did not move");

  v2d pos1 = {-fix16_one, 0};
  assert(equalV2d(pos1, *vecterActorGetPosition(world, actorId)) && "Actor was not pushed by wall 1");

  vecterWorldUpdate(world, fix16_one);

  v2d wall1pos2[2] = {{F16(-3), -fix16_one}, {F16(-3), fix16_one}};
  assert(equalV2d(wall1pos2[0], *vecterSegmentGetBase(world, wall1)) && "Wall 1 base did not move the second time");
  assert(equalV2d(wall1pos2[1], *vecterSegmentGetTip(world, wall1)) && "Wall 1 tip did not move the second time");

  v2d pos2 = {F16(-3), 0};
  assert(equalV2d(pos2, *vecterActorGetPosition(world, actorId)) && "Actor was not pushed by wall 1 the second time");

  v2d vel2 = {F16(2), 0};
  vecterActorVelocity(world, actorId, &vel2);

  vecterWorldUpdate(world, fix16_one);

  v2d pos3 = {F16(-5), 0};
  assert(equalV2d(pos3, *vecterActorGetPosition(world, actorId)) && "Actor was not pushed by wall 1 the third time");
  vecterWorldFree(world);

  return EXIT_SUCCESS;
}

