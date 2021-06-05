#include<iostream>
#include "FloorPlan.h"

int main() {
	FloorPlan floorPlan("res/sample.txt");

	floorPlan.init();
	floorPlan.build();
}