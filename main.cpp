//
// Created by rez on 26.01.20.
//
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

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
};


struct Obstacle {
    int x;
    int y;
    int isExist;
};

int gameMap[16][8];
int loopCounter = 0;
int last_counter = 0;
int gameSpeed = SPEED_EASY_MODE;
int end = 0;


void init(struct Obstacle obstacles[]);

void drawLED(int x, int y);

void createRandomObstacles(struct Obstacle *obstacles);

void moveRight(struct Car *car);

void moveLeft(struct Car *car);

void checkForCollision(struct Car *car, struct Obstacle *obstacles);

void makeMapReady(struct Car *car, struct Obstacle obstacles[]);

void showMap();

void changeDifficulty();

void drawObstaclesOnLED(struct Obstacle obstacles[]);

//change to setup and loop
int main() {
    //setup
    struct Obstacle obstacles[COUNT_OF_OBSTACLE];
    struct Car car = {};
    init(obstacles);
    car.x = 13;
    car.y = 3;
    //end_setup
    // loop
    while (!end) {
        createRandomObstacles(obstacles);
        makeMapReady(&car, obstacles);
        showMap();
        int rol;
        scanf("%d", &rol);
        if (rol == 1) {
            moveLeft(&car);
        } else if (rol == 2) {
            moveRight(&car);
        } else if (rol == 3) {
            changeDifficulty();
        } else if (rol == 4) {
            end = 1;
        }
        for (int i = 0; i < COUNT_OF_OBSTACLE; ++i) {
            if (obstacles[i].isExist) {
                obstacles[i].x += gameSpeed;
                if (obstacles[i].x > 15) {
                    obstacles[i].isExist = 0;
                }
            }
        }
        checkForCollision(&car, obstacles);
        loopCounter += gameSpeed;
    }
    //end_loop

}

void drawLED(int x, int y) {
    gameMap[x][y] = 3;
}

void createRandomObstacles(struct Obstacle *obstacles) {
    if (loopCounter - 6 > last_counter) {
        last_counter = loopCounter;
        srand(time(0));
        for (int i = 0; i < 5; ++i) {
            int y = (rand() % 3) * 2 + 1;
            if (obstacles[i].isExist == 0) {
                struct Obstacle obstacle = {0, y, 1};
                obstacles[i] = obstacle;
                break;
            }
        }
    }
}

void moveRight(struct Car *car) {
    if ((*car).y < 5)
        (*car).y += 2;
}

void moveLeft(struct Car *car) {
    if ((*car).y > 2)
        (*car).y -= 2;
}

void checkForCollision(struct Car *car, struct Obstacle *obstacles) {
    int car_y = (*car).y;
    for (int i = 0; i < COUNT_OF_OBSTACLE; ++i) {
        if (obstacles[i].isExist) {
            int obstacle_x = obstacles[i].x;
            int obstacle_y = obstacles[i].y;
            if (obstacle_x > 10) {
                if (obstacle_y <= car_y + 1 && obstacle_y >= car_y - 1) {
                    end = 1;
                    return;
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

void getAllPointsFromFirstPoint(int x, int y, int allPoints[][2]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            allPoints[i * 2 + j][0] = x + i;
            allPoints[i * 2 + j][1] = y + j;
        }
    }
}

void drawObstaclesOnLED(struct Obstacle obstacles[]) {

    for (int i = 0; i < COUNT_OF_OBSTACLE; ++i) {
        int points[COUNT_OF_POINT_AN_OBJECT][2];
        getAllPointsFromFirstPoint(obstacles[i].x, obstacles[i].y, points);
        for (int j = 0; j < COUNT_OF_POINT_AN_OBJECT; ++j) {
            if (obstacles[i].isExist && points[j][0] < 16) {
                drawLED(points[j][0], points[j][1]);
            }
        }
    }
}

void makeMapReady(struct Car *car, struct Obstacle obstacles[]) {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (j == 0 || j == 7) {
                gameMap[i][j] = 1;
            } else {
                gameMap[i][j] = 0;
            }
        }
    }
    int carPoints[6][2];
    getAllPointsFromFirstPoint((*car).x, (*car).y, carPoints);
    for (int k = 0; k < 6; ++k) {
        gameMap[carPoints[k][0]][carPoints[k][1]] = 2;
    }
    drawObstaclesOnLED(obstacles);
}

// for test
void showMap() {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 8; ++j) {
            printf("%d ", gameMap[i][j]);
        }
        printf("\n");
    }

}

void init(struct Obstacle obstacles[]) {
    struct Obstacle obstacle = {0, 0, 0};
    struct Obstacle obstacle1 = {0, 0, 0};
    struct Obstacle obstacle2 = {0, 0, 0};
    struct Obstacle obstacle3 = {0, 0, 0};
    struct Obstacle obstacle4 = {0, 0, 0};
    obstacles[0] = obstacle;
    obstacles[1] = obstacle1;
    obstacles[2] = obstacle2;
    obstacles[3] = obstacle3;
    obstacles[4] = obstacle4;
}