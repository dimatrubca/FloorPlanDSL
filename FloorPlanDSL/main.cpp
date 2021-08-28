#include<iostream>
#include "Rendering/FloorPlan.h"

int main() {
	FloorPlan floorPlan("res/sample_0.txt");

	floorPlan.init();
	floorPlan.build();
}