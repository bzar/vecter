#include "vecter.h"
#include "pool.h"
#include <assert.h>
#include <stdio.h>

typedef struct _vecterWorld
{
  chckPool* segments;
  chckPool* actors;
} _vecterWorld;

typedef struct vecterSegment
{
  v2d base;
  v2d tip;
  void* userData;
} vecterSegment;

typedef struct vecterActor
{
  v2d position;
  v2d velocity;
  void* userData;
  vecterCollisionCallback collisionCallback;
} vecterActor;

static vecterSegment* getSegment(const vecterWorld* world, vecterSegmentId segmentId);
static vecterActor* getActor(const vecterWorld* world, vecterActorId actorId);
static void updateActor(vecterWorld* world, vecterActor* actor, vecterActorId actorId, fix16_t delta);
static bool segmentIntersection(const v2d* aBase, const v2d* aTip, const v2d* bBase, const v2d* bTip, v2d* intersectionPoint);
static fix16_t v2d_cross(const v2d* a, const v2d* b);
static void v2d_projection(v2d* dest, const v2d* a, const v2d* b);

vecterWorld* vecterWorldNew()
{
  vecterWorld* world = calloc(1, sizeof(vecterWorld));
  world->segments = chckPoolNew(64, 64, sizeof(vecterSegment));
  world->actors = chckPoolNew(64, 64, sizeof(vecterActor));
  return world;
}

void vecterWorldFree(vecterWorld* world)
{
  chckPoolFree(world->segments);
  chckPoolFree(world->actors);
}



void vecterWorldUpdate(vecterWorld* world, fix16_t delta)
{
  chckPoolIndex iter = 0;
  vecterActor* actor;
  while((actor = chckPoolIter(world->actors, &iter)))
  {
    vecterActorId actorId = iter - 1;
    updateActor(world, actor, actorId, delta);
  }
}


vecterSegmentId vecterSegmentAdd(vecterWorld* world, const v2d* base, const v2d* tip)
{
  vecterSegment segment = {*base, *tip, NULL};
  chckPoolIndex index;
  assert(chckPoolAdd(world->segments, &segment, &index) && "VECTER: Could not add segment");
  return index;
}


void vecterSegmentRemove(vecterWorld* world, vecterSegmentId segmentId)
{
  chckPoolRemove(world->segments, segmentId);
}

void vecterSegmentBase(vecterWorld* world, vecterSegmentId segmentId, const v2d* base)
{
  getSegment(world, segmentId)->base = *base;
}

const v2d* vecterSegmentGetBase(const vecterWorld* world, vecterSegmentId segmentId)
{
  return &(getSegment(world, segmentId)->base);
}

void vecterSegmentTip(vecterWorld* world, vecterSegmentId segmentId, const v2d* tip)
{
  getSegment(world, segmentId)->tip = *tip;
}

const v2d* vecterSegmentGetTip(const vecterWorld* world, vecterSegmentId segmentId)
{
  return &(getSegment(world, segmentId)->tip);
}


void vecterSegmentUserData(vecterWorld* world, vecterSegmentId segmentId, void* userData)
{
  getSegment(world, segmentId)->userData = userData;
}


void* vecterSegmentGetUserData(const vecterWorld* world, vecterSegmentId segmentId)
{
  return getSegment(world, segmentId)->userData;
}


vecterActorId vecterActorAdd(vecterWorld* world)
{
  v2d zeroVector = {fix16_from_int(0), fix16_from_int(0)};
  vecterActor actor = {
    zeroVector,
    zeroVector,
    NULL,
    NULL
  };

  chckPoolIndex index;
  assert(chckPoolAdd(world->actors, &actor, &index) && "VECTER: Could not add actor");
  return index;
}


void vecterActorRemove(vecterWorld* world, vecterActorId actorId)
{
  chckPoolRemove(world->actors, actorId);
}


void vecterActorPosition(vecterWorld* world, vecterActorId actorId, const v2d* position)
{
  getActor(world, actorId)->position = *position;
}


const v2d* vecterActorGetPosition(const vecterWorld* world, vecterActorId actorId)
{
  return &(getActor(world, actorId)->position);
}


void vecterActorVelocity(vecterWorld* world, vecterActorId actorId, const v2d* velocity)
{
  getActor(world, actorId)->velocity = *velocity;
}


const v2d* vecterActorGetVelocity(const vecterWorld* world, vecterActorId actorId)
{
  return &(getActor(world, actorId)->velocity);
}


void vecterActorUserData(vecterWorld* world, vecterActorId actorId, void* userData)
{
  getActor(world, actorId)->userData = userData;
}


void* vecterActorGetUserData(const vecterWorld* world, vecterActorId actorId)
{
  return getActor(world, actorId)->userData;
}


void vecterActorCollisionCallback(vecterWorld* world, vecterActorId actorId, vecterCollisionCallback collisionCallback)
{
  getActor(world, actorId)->collisionCallback = collisionCallback;
}

void vecterCollisionStop(vecterWorld* world, vecterActorId actorId, vecterSegmentId segmentId)
{
  (void) segmentId;
  v2d newVelocity = {0, 0};
  vecterActorVelocity(world, actorId, &newVelocity);
}

void vecterCollisionSlide(vecterWorld* world, vecterActorId actorId, vecterSegmentId segmentId)
{
  v2d const* base = vecterSegmentGetBase(world, segmentId);
  v2d const* tip = vecterSegmentGetTip(world, segmentId);
  v2d const* velocity = vecterActorGetVelocity(world, actorId);

  v2d offset = { fix16_sub(tip->y, base->y), -fix16_sub(tip->x, base->x) };
  v2d_mul_s(&offset, &offset, -1);
  v2d newVelocity;
  v2d_sub(&newVelocity, tip, base);
  v2d_normalize(&newVelocity, &newVelocity);
  fix16_t dot = v2d_dot(&newVelocity, velocity);
  fix16_t sign = dot > 0 ? 1 : dot < 0 ? -1 : 0;
  v2d_mul_s(&newVelocity, &newVelocity, sign * v2d_norm(velocity));
  v2d_add(&newVelocity, &newVelocity, &offset);
  vecterActorVelocity(world, actorId, &newVelocity);
}

void vecterCollisionProjection(vecterWorld* world, vecterActorId actorId, vecterSegmentId segmentId)
{
  v2d const* base = vecterSegmentGetBase(world, segmentId);
  v2d const* tip = vecterSegmentGetTip(world, segmentId);
  v2d const* velocity = vecterActorGetVelocity(world, actorId);

  v2d offset = { fix16_sub(tip->y, base->y), -fix16_sub(tip->x, base->x) };
  v2d_mul_s(&offset, &offset, -1);
  v2d newVelocity;
  v2d_sub(&newVelocity, tip, base);
  v2d_projection(&newVelocity, velocity, &newVelocity);
  v2d_add(&newVelocity, &newVelocity, &offset);
  vecterActorVelocity(world, actorId, &newVelocity);
}

void vecterCollisionReflection(vecterWorld* world, vecterActorId actorId, vecterSegmentId segmentId)
{
  v2d const* base = vecterSegmentGetBase(world, segmentId);
  v2d const* tip = vecterSegmentGetTip(world, segmentId);
  v2d const* velocity = vecterActorGetVelocity(world, actorId);

  v2d normal = { fix16_sub(tip->y, base->y), -fix16_sub(tip->x, base->x) };
  v2d_normalize(&normal, &normal);
  v2d_mul_s(&normal, &normal, -2 * v2d_dot(&normal, velocity));
  v2d newVelocity;
  v2d_add(&newVelocity, &normal, velocity);
  vecterActorVelocity(world, actorId, &newVelocity);
}


/* PRIVATE */

vecterSegment* getSegment(const vecterWorld* world, vecterSegmentId segmentId)
{
  vecterSegment* segment = chckPoolGet(world->segments, segmentId);
  assert(segment && "VECTER: No such segment");
  return segment;
}

vecterActor* getActor(const vecterWorld* world, vecterActorId actorId)
{
  vecterActor* actor = chckPoolGet(world->actors, actorId);
  assert(actor && "VECTER: No such segment");
  return actor;
}

void updateActor(vecterWorld* world, vecterActor* actor, vecterActorId actorId, fix16_t delta)
{
  int repeat = 0;
  fix16_t deltaLeft = delta;
  while(deltaLeft > 0 && repeat++ < 10)
  {
    v2d positionDelta, destination;
    v2d_mul_s(&positionDelta, &actor->velocity, deltaLeft);

    if(positionDelta.x == 0 && positionDelta.y == 0)
    {
      break;
    }
    v2d_add(&destination, &actor->position, &positionDelta);

    bool collided = false;
    fix16_t distance = 0;
    v2d collisionPoint;
    vecterSegmentId segmentId;

    /* TODO: Query segment tree to find potential collisions instead of iterating all */
    chckPoolIndex iter = 0;
    vecterSegment* segment;
    while((segment = chckPoolIter(world->segments, &iter)))
    {
      v2d segmentDelta;
      v2d_sub(&segmentDelta, &segment->tip, &segment->base);
      if(v2d_cross(&segmentDelta, &positionDelta) > 0)
        continue;

      v2d cp;
      if(segmentIntersection(&actor->position, &destination, &segment->base, &segment->tip, &cp))
      {
        v2d collisionDelta;
        v2d_sub(&collisionDelta, &cp, &actor->position);

        fix16_t d = v2d_norm(&collisionDelta);
        if(!collided || d < distance)
        {
          distance = d;
          collisionPoint = cp;
          segmentId = iter - 1;
          collided = true;
        }
      }
    }

    if(!collided)
    {
      actor->position = destination;
      deltaLeft = 0;
    }
    else if(!actor->collisionCallback)
    {
      actor->position = collisionPoint;
      deltaLeft = 0;
    }
    else
    {
      actor->position = collisionPoint;

      v2d oldVelocity = actor->velocity;
      actor->collisionCallback(world, actorId, segmentId);
      bool velocityChanged = oldVelocity.x != actor->velocity.x || oldVelocity.y != actor->velocity.y;
      fix16_t totalDistance = v2d_norm(&positionDelta);
      deltaLeft = velocityChanged
          ? fix16_sub(deltaLeft, fix16_mul(deltaLeft, fix16_div(distance, totalDistance)))
          : 0;
    }
  }
}

bool segmentIntersection(const v2d* aBase, const v2d* aTip, const v2d* bBase, const v2d* bTip, v2d* intersectionPoint)
{
  v2d aDelta, bDelta;
  v2d_sub(&aDelta, aTip, aBase);
  v2d_sub(&bDelta, bTip, bBase);

  if(v2d_cross(&aDelta, &bDelta) != 0)
  {
    v2d abDelta;
    v2d_sub(&abDelta, bBase, aBase);
    fix16_t aCrossB = v2d_cross(&aDelta, &bDelta);
    fix16_t t = fix16_div(v2d_cross(&abDelta, &bDelta), aCrossB);
    fix16_t u = fix16_div(v2d_cross(&abDelta, &aDelta), aCrossB);

    if(t >= 0 && t <= fix16_one && u >= 0 && u <= fix16_one)
    {
      v2d_mul_s(intersectionPoint, &bDelta, u);
      v2d_add(intersectionPoint, intersectionPoint, bBase);
      return true;
    }
  }

  return false;
}

fix16_t v2d_cross(const v2d* a, const v2d* b)
{
  return fix16_sub(fix16_mul(a->x, b->y), fix16_mul(a->y, b->x));
}

void v2d_projection(v2d* dest, const v2d* a, const v2d* b)
{
  v2d result;
  v2d_normalize(&result, b);
  v2d_mul_s(&result, &result, v2d_dot(a, &result));
  *dest = result;
}

