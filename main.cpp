//
// Created by rez on 26.01.20.
//
#include <stdbool.h>

#define CAR_WIDTH 2
#define CAR_LENGTH 3
#define OBSTACLE_WIDTH 2
#define OBSTACLE_LENGTH 3
#define SPEED_EASY_MODE 1
#define SPEED_HARD_MODE 2
#define COUNT_OF_OBSTACLE 5
#define COUNT_OF_POINT_AN_OBJECT 6
struct Car {
    int x;
    int y;
    int velocity;
} car;


struct Obstacle {
    int x;
    int y;
};

struct Obstacle obstacles[COUNT_OF_OBSTACLE];
int gameMap[8][16];
int gameSpeed = SPEED_EASY_MODE;
bool end = false

void setup() {
    // put your setup code here, to run once:

}

void loop() {
    // put your main code here, to run repeatedly:

}


void createRandomObstacles() {
    // TODO ShAli
}

void moveRight() {
    // TODO ShAli
}

void moveLeft() {
    // TODO ShAli
}

void getAllPointsFromFirstPoint(int x, int y, int allPoints[][2]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            allPoints[i + j][0] = x + i;
            allPoints[i + j][1] = y + j;
        }
    }
}

void checkForCollision() {
    int car_x = car.x;
    int car_y = car.y;
    int carPoints[COUNT_OF_POINT_AN_OBJECT][2];
    getAllPointsFromFirstPoint(car_x, car_y, carPoints);
    for (int i = 0; i < COUNT_OF_OBSTACLE; ++i) {
        int obstacle_x = obstacles[i].x;
        int obstacle_y = obstacles[i].y;
        int obstaclePoints[COUNT_OF_POINT_AN_OBJECT][2];
        getAllPointsFromFirstPoint(obstacle_x, obstacle_y, obstaclePoints);
        for (int j = 0; j < COUNT_OF_POINT_AN_OBJECT; ++j) {
            for (int k = 0; k < COUNT_OF_POINT_AN_OBJECT; ++k) {
                if (carPoints[k][0] == obstaclePoints[k][0] && carPoints[j][1] == obstaclePoints[j][1]) {
                    end = true;
                }
            }
        }
    }

}


void changeDifficulty() {
    if (gameSpeed == SPEED_EASY_MODE) {
        gameSpeed = SPEED_HARD_MODE;
    } else {
        gameSpeed = SPEED_EASY_MODE;
    }
}

void drawLED(int x, int y) {
    //TODO Rez
}

void drawObstaclesOnLED() {
    for (int i = 0; i < COUNT_OF_OBSTACLE; ++i) {
        int points[COUNT_OF_POINT_AN_OBJECT][2];
        getAllPointsFromFirstPoint(obstacles[i].x, obstacles[i].y, points);
        for (int j = 0; j < COUNT_OF_POINT_AN_OBJECT; ++j) {
            drawLED(points[j][0], points[j][1]);
        }
    }
}

void drawCarOnLED() {
    int points[COUNT_OF_POINT_AN_OBJECT][2];
    getAllPointsFromFirstPoint(car.x, car.y, points);
    for (int j = 0; j < COUNT_OF_POINT_AN_OBJECT; ++j) {
        drawLED(points[j][0], points[j][1]);
    }
}




