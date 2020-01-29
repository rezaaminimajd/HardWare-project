#include <LedControl.h>

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

LedControl lc = LedControl(12, 10, 11, 2); // Pins: DIN,CLK,CS, number of Display connected

struct Obstacle obstacles[COUNT_OF_OBSTACLE];
struct Car car = {13, 3};

void setup() {
  // put your setup code here, to run once:
  Serial. begin(9600);
  for (byte i = 2; i <= 7; i++)
        pinMode(i, INPUT);
  // 2 -> for key
  for (byte i = 8; i <= 13; i++)
        pinMode(i, OUTPUT);
  // 8 -> for leftWhite
  // 9 -> for rightWhite
  // 10 -> for CLK
  // 11 -> for CS
  // 12 -> for DIN
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  // A0 -> joyX
  // A1 -> leftBlack
  // A2 -> rightBlack
  lc.shutdown(0,false);  // Wake up displays
  lc.shutdown(1,false);
  lc.setIntensity(0,5);  // Set intensity levels
  lc.setIntensity(1,5);
  lc.clearDisplay(0);  // Clear Displays
  lc.clearDisplay(1);
  init(obstacles);
}


int buttonState;
int joyX;
int leftValue;
int rightValue;

void loop() {
  // put your main code here, to run repeatedly:
  createRandomObstacles(obstacles);
  makeMapReady(&car, obstacles);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  buttonState = digitalRead(2);
  if (buttonState == HIGH) {
    changeDifficulty();
  }

  joyX = analogRead(A0);
  leftValue = analogRead(A1);
  rightValue = analogRead(A2);
  
  if (A0 < 250) {
    moveLeft(&car);
  }
  if (A0 > 750) {
    moveRight(&car);
  }

  if (A1 > 300 || A2 > 300) {
    if (A1 > A2) {
      moveLeft(&car);
    } else {
      moveRight(&car);
    }
  }
  byte gameMapByte[8];
  changeToByte(gameMapByte);
  for (int i = 0; i < 8; i++){
    lc.setRow(0,i,gameMapByte[i]);
  }
  
  for (int i = 8; i < 16; i++)
  {
    lc.setRow(1,i - 8,gameMapByte[i]);
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

void changeToByte(byte game[]) {
  for(int i = 0; i < 16; i++) {
    for(int j = 0; j < 8; j++) {
      game[i] += gameMap[i][j] * ((int) pow(2, 7 - j));
    }
  }
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
