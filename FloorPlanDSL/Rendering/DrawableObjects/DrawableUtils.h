#pragma once
#ifndef _DRAWABLEUTILS_
#define _DRAWABLEUTILS_

#include "../../Objects/Object.h"
#include <corecrt_math_defines.h>


static Position getAdjacentPos(Position pos, float width, float alpha) {
	Position adjacentPos;

	float beta = M_PI / 2 - alpha;

	adjacentPos.x = pos.x - cos(beta) * width;
	adjacentPos.y = pos.y + sin(beta) * width;

	return adjacentPos;
}

static Position getSegmentIntersection(Position a, Position b, Position c, Position d) {
	double A1 = b.y - a.y;
	double B1 = a.x - b.x;
	double C1 = A1 * b.x + B1 * b.y;

	double A2 = d.y - c.y;
	double B2 = c.x - d.x;
	double C2 = A2 * d.x + B2 * d.y;


	// compute intersection
	double det = A1 * B2 - A2 * B1;

	double x = (B2 * C1 - B1 * C2) / det;
	double y = (A1 * C2 - A2 * C1) / det;

	Position pos(x, y);
	return pos;
}

static float determinant(Position A, Position B, Position C) {
	return (A.x * B.y + B.x * C.y + C.x * A.y - B.y * C.x - C.y * A.x - A.y * B.x);
}

#endif