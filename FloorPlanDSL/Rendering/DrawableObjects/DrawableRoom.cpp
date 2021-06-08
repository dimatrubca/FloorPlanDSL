#pragma once
#include "DrawableRoom.h"
#include "DrawableUtils.h"
#include <glm/vec3.hpp> // glm::vec3
#include <iomanip>      // std::setprecision

DrawableRoom::DrawableRoom(Room* room) : room(room) {
	Position currPos = room->startPosition;
	float prevAngle = 0;

	float width = room->border->width->value;

	std::vector<Position> points;
	std::vector<Position> borderPoints;
	std::vector<Position> adjBorderPoints;

	for (int i = 0; i + 1 < room->vertices.size(); i++) {
		points.push_back(room->vertices[i]);
		points.push_back(room->vertices[i + 1]);
	}

	points.push_back(points[0]);
	points.push_back(points[1]);

	for (int i = 0; i + 1 < points.size(); i+=2) {
		auto currVertex = points[i];
		auto nextVertex = points[i + 1];

		float angle = atan2(nextVertex.y - currVertex.y, nextVertex.x - currVertex.x);
		
		Position currAdjacent = getAdjacentPos(currVertex, width, angle);
		Position nextAdjacent = getAdjacentPos(nextVertex, width, angle);

		borderPoints.push_back(currAdjacent);
		borderPoints.push_back(nextAdjacent);
	}

	Position prevIntersection = borderPoints[0];

	for (int i = 0; i + 3 < borderPoints.size(); i+=2) {
		Position intersection = getSegmentIntersection(borderPoints[i], borderPoints[i + 1],
			borderPoints[i + 2], borderPoints[i + 3]);

		adjBorderPoints.push_back(prevIntersection);
		adjBorderPoints.push_back(intersection);
		prevIntersection = intersection;
	}

	adjBorderPoints.push_back(adjBorderPoints.back());
	adjBorderPoints.push_back(borderPoints.back());

	for (int i = 0; i + 1 < points.size() && i + 1  < adjBorderPoints.size(); i+=2) {
		vertices.push_back(points[i].x);
		vertices.push_back(points[i].y);
		vertices.push_back(0);

		vertices.push_back(adjBorderPoints[i].x);
		vertices.push_back(adjBorderPoints[i].y);
		vertices.push_back(0);

		vertices.push_back(points[i + 1].x);
		vertices.push_back(points[i + 1].y);
		vertices.push_back(0);


		vertices.push_back(adjBorderPoints[i + 1].x);
		vertices.push_back(adjBorderPoints[i + 1].y);
		vertices.push_back(0);
	}

	this->color = glm::vec3(room->border->color->rgb[0], room->border->color->rgb[1], room->border->color->rgb[2]);
	this->center = findCenter();
	this->area = calculateArea();
}

Position DrawableRoom::findCenter() {
	float sumX = 0, sumY = 0;
	int cntVertices = room->vertices.size();

	for (auto vertex : room->vertices) {
		sumX += vertex.x;
		sumY += vertex.y;
	}

	return Position(sumX / cntVertices, sumY / cntVertices);
};

float DrawableRoom::calculateArea() {
	float area = 0;
	Position origin = Position(0.0, 0.0);
	for (int i = 0; i + 1 < room->vertices.size(); i++) {
		area += determinant(origin, room->vertices[i], room->vertices[i+1]);
	}

	area = 0.5 * abs(area);

	// convert to m^2
	area = area / 1000000.0;

	return area;
}

void DrawableRoom::draw(Renderer* renderer, TextRenderer* textRenderer) {
	renderer->drawTriangleStrip(vertices, color);

	if (room->label != "") {
		std::stringstream ss; ss << " (" << std::fixed << std::setprecision(2) << this->area << " m^2)";
		textRenderer->RenderText(room->label + ss.str(), center.x - 10, center.y +10, 1);
	}
}

