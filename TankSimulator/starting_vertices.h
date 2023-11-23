#pragma once
#define CRES 30
#define TARGETS_NUM 3
#define TARGET_RADIUS_PX 400 //Ovo je obrnuto proporcijalno velicini mete i ja nemam snage da racunam zasto
#define AMMUNITION_SIZE 6
#define CAGE_SIZE 5
#define MAX_VOLTAGE 30
#define TANK_SPEED_C 0.002
#define BASE_TANK_SPEED 0.001
#include <math.h>
#include <random>

float panel_surface_background_vert[16] =
{   //X    Y      S    T 
	-1.0, -1.0,  0.0, 0.0,
	1.0, -1.0,   1.0, 0.0,
   -1.0, 1.0,    0.0, 1.0,
	1.0, 1.0,    1.0, 1.0
};

float ammunition_status_vert[56] = {
	-0.5, 0.15,
	0.5, 0.15,
	-0.5, 0.0,
	0.5, 0.0,

	-0.5, 0.0,
	0.5, 0.0,
	-0.5, -0.15,
	0.5, -0.15,

	-0.5, -0.15,
	0.5, -0.15,
	-0.5, -0.3,
	0.5, -0.3,

	-0.5, -0.3,
	0.5, -0.3,
	-0.5, -0.45,
	0.5, -0.45,

	-0.5, -0.45,
	0.5, -0.45,
	-0.5, -0.6,
	0.5, -0.6,

	-0.5, -0.6,
	0.5, -0.6,
	-0.5, -0.75,
	0.5, -0.75,

	-0.5, -0.75,
	0.5, -0.75,
	-0.5, -0.9,
	0.5, -0.9
};

float ammunition_status_border_vert[56] = {
	-0.5, 0.15,
	0.5, 0.15,
	0.5, 0.0,
	-0.5, 0.0,

	-0.5, 0.0,
	0.5, 0.0,
	0.5, -0.15,
	-0.5, -0.15,

	-0.5, -0.15,
	0.5, -0.15,
	0.5, -0.3,
	-0.5, -0.3,

	-0.5, -0.3,
	0.5, -0.3,
	0.5, -0.45,
	-0.5, -0.45,

	-0.5, -0.45,
	0.5, -0.45,
	0.5, -0.6,
	-0.5, -0.6,

	-0.5, -0.6,
	0.5, -0.6,
	0.5, -0.75,
	-0.5, -0.75,

	-0.5, -0.75,
	0.5, -0.75,
	0.5, -0.9,
	-0.5, -0.9,
};

float fire_led_vert[8] = {
	-0.5, 0.8,
	0.5, 0.8,
	-0.5, 0.5,
	0.5, 0.5
};

float fire_led_cage[40] = {
	-0.5, 0.8,
	0.5, 0.8,
	0.5, 0.5,
	-0.5, 0.5,

	-0.5, 0.8,
	-0.25, 0.8,
	-0.25, 0.5,
	-0.5, 0.5,

	-0.25, 0.8,
	0.0, 0.8,
	0.0, 0.5,
	-0.25, 0.5,

	0.0, 0.8,
	0.25, 0.8,
	0.25, 0.5,
	0.0, 0.5,

	0.25, 0.8,
	0.5, 0.8,
	0.5, 0.5,
	0.25, 0.5,
};

float voltmeter_line_vert[4] = {
	0.0, -0.15,
	-0.65, -0.13,
};

bool target_hit[TARGETS_NUM] = {
	false,
	false,
	false
};

float calculateVerticalToHorizontalRatio(float vertical) {
	return 16 * vertical / 9;
}

float convertToGLRange(float pixels, bool height) {
	float maxNew;
	if (height) {
		maxNew = 1080.0;
	}
	else {
		maxNew = 1920.0;
	}
	return (((pixels - 0.0) * (1.0 - (-1.0))) / (maxNew - 0.0)) + (-1.0);
}


void generateVoltmeterVert(float* voltmeter_vert) {
	float r = 0.7;
	voltmeter_vert[0] = 0;
	voltmeter_vert[1] = 0;
	int i;
	for (i = 0; i <= CRES; i++)
	{

		voltmeter_vert[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 180 / CRES));
		voltmeter_vert[2 + 2 * i + 1] = r * sin((3.141592 / 180) * (i * 180 / CRES));
	}
}

void generateRandomTargetPositions(float* positions) {
	std::random_device rd1;
	std::random_device rd2;
	std::mt19937 gen1(rd1());
	std::mt19937 gen2(rd2());
	std::uniform_int_distribution<> distr_x(533, 1387);
	std::uniform_int_distribution<> distr_y(300, 780);

	for (int i = 0; i < TARGETS_NUM; i++) {
		positions[i * 2] = convertToGLRange(distr_x(gen1), false);
		positions[i * 2 + 1] = convertToGLRange(distr_y(gen2), true);
	}
}

void generateTargetVert(float* positions, float* target_vert) {
	for (int i = 0; i < TARGETS_NUM; i++) {
		target_vert[i * 16] = positions[i * 2];	//x1
		target_vert[i * 16 + 1] = positions[i * 2 + 1]; //y1
		target_vert[i * 16 + 2] = 0.0;
		target_vert[i * 16 + 3] = 0.0;

		target_vert[i * 16 + 4] = positions[i * 2] + abs(convertToGLRange(calculateVerticalToHorizontalRatio(TARGET_RADIUS_PX), false));	//x2
		target_vert[i * 16 + 5] = positions[i * 2 + 1]; //y2
		target_vert[i * 16 + 6] = 1.0;
		target_vert[i * 16 + 7] = 0.0;

		target_vert[i * 16 + 8] = positions[i * 2];	//x3
		target_vert[i * 16 + 9] = positions[i * 2 + 1] + abs(convertToGLRange(TARGET_RADIUS_PX, true)); //y3
		target_vert[i * 16 + 10] = 0.0;
		target_vert[i * 16 + 11] = 1.0;

		target_vert[i * 16 + 12] = positions[i * 2] + abs(convertToGLRange(calculateVerticalToHorizontalRatio(TARGET_RADIUS_PX), false)); //x4
		target_vert[i * 16 + 13] = positions[i * 2 + 1] + abs(convertToGLRange(TARGET_RADIUS_PX, true)); //y4
		target_vert[i * 16 + 14] = 1.0;
		target_vert[i * 16 + 15] = 1.0;
	}
}

