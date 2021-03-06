#pragma once

#include "sfwdraw.h"
#include "vec2.h"

#include "Transform.h"

void drawVecLine(vec2 p1,vec2 p2,unsigned int tint = WHITE);
void drawVecLine3(vec2 p1, vec2 p2, unsigned int tint = WHITE, const Transform &T = Transform(nullptr));
void drawVecLine3(vec2 p1, vec2 p2, unsigned int tint = WHITE, const mat3x3 &M = mat3x3::identity());

void drawVecCircle(vec2 p, float radius, unsigned int steps = 12, unsigned int tint = WHITE);