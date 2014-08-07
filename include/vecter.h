#ifndef VECTER_H
#define VECTER_H

#include "fixvector2d.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct _vecterWorld vecterWorld;
typedef size_t vecterSegmentId;
typedef size_t vecterActorId;

typedef void (*vecterCollisionCallback)(vecterWorld* world, vecterActorId actorId, vecterSegmentId segmentId);

vecterWorld* vecterWorldNew();
void vecterWorldFree(vecterWorld* world);
void vecterWorldUpdate(vecterWorld* world, fix16_t delta);

vecterSegmentId vecterSegmentAdd(vecterWorld* world, const v2d* base, const v2d* tip);
void vecterSegmentRemove(vecterWorld* world, vecterSegmentId segmentId);
void vecterSegmentBase(vecterWorld* world, vecterSegmentId segmentId, const v2d* base);
const v2d* vecterSegmentGetBase(const vecterWorld* world, vecterSegmentId segmentId);
void vecterSegmentTip(vecterWorld* world, vecterSegmentId segmentId, const v2d* tip);
const v2d* vecterSegmentGetTip(const vecterWorld* world, vecterSegmentId segmentId);
void vecterSegmentUserData(vecterWorld* world, vecterSegmentId segmentId, void* userData);
void* vecterSegmentGetUserData(const vecterWorld* world, vecterSegmentId segmentId);

vecterActorId vecterActorAdd(vecterWorld* world);
void vecterActorRemove(vecterWorld* world, vecterActorId actorId);
void vecterActorPosition(vecterWorld* world, vecterActorId actorId, const v2d* position);
const v2d* vecterActorGetPosition(const vecterWorld* world, vecterActorId actorId);
void vecterActorVelocity(vecterWorld* world, vecterActorId actorId, const v2d* velocity);
const v2d* vecterActorGetVelocity(const vecterWorld* world, vecterActorId actorId);
void vecterActorUserData(vecterWorld* world, vecterActorId actorId, void* userData);
void* vecterActorGetUserData(const vecterWorld* world, vecterActorId actorId);
void vecterActorCollisionCallback(vecterWorld* world, vecterActorId actorId, vecterCollisionCallback collisionCallback);

// Provided collision responses
void vecterCollisionStop(vecterWorld* world, vecterActorId actorId, vecterSegmentId segmentId);
void vecterCollisionSlide(vecterWorld* world, vecterActorId actorId, vecterSegmentId segmentId);
void vecterCollisionProjection(vecterWorld* world, vecterActorId actorId, vecterSegmentId segmentId);
void vecterCollisionReflection(vecterWorld* world, vecterActorId actorId, vecterSegmentId segmentId);

#endif
