//need to install this library
#include <LedControl.h>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define CAR_WIDTH 2
#define CAR_LENGTH 3
#define OBSTACLE_WIDTH 2
#define OBSTACLE_LENGTH 3
#define SPEED_EASY_MODE 1
#define SPEED_HARD_MODE 4
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

int gameMap[8][8];
int loopCounter = 0;
int last_counter = 0;
int gameSpeed = SPEED_EASY_MODE;
int endGame = 0;
int ran = 0;
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
struct Car car = {5, 3};

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
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  lc.shutdown(0,false);  // Wake up displays
  //lc.shutdown(1,false);
  //delay(1000);
  lc.setIntensity(0,4);  // Set intensity levels
  //lc.setIntensity(1,5);
  //delay(1000);
  lc.clearDisplay(0);  // Clear Displays
  //lc.clearDisplay(1);
  //delay(1000);
  init(obstacles);
}


int buttonState;
int joyX;
int leftValue;
int rightValue;
void(* resetFunc) (void) = 0; //declare reset function @ address 0
void loop() {
  // put your main code here, to run repeatedly:
  createRandomObstacles(obstacles);
  makeMapReady(&car, obstacles);
  
  buttonState = digitalRead(2);
  if (buttonState == HIGH) {
    changeDifficulty();
  }

  //joyX = analogRead(A0);
  leftValue = analogRead(A1);
  rightValue = analogRead(A2);
  
  //if (joyX < 250) {
  //  moveLeft(&car);
  //}
  //if (joyX > 750) {
  //  moveRight(&car);
  //}
  //Serial.print("joyX ");
  //Serial.print(joyX);
  //Serial.print("\n");
  Serial.print("left ");
  Serial.print(leftValue);
  Serial.print("\n");
  Serial.print("right ");
  Serial.print(rightValue);
  Serial.print("\n");
  if (leftValue > 1000 || rightValue > 300) {
    if (leftValue > rightValue + 700) {
      moveLeft(&car);
    } else {
      moveRight(&car);
    }
  }
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      Serial.print(gameMap[i][j]);
    }
    Serial.print("\n");
  }
  Serial.print("-----------------------------");
  byte gameMapByte[8];// = {129, 0, 0, 0, 0, 0, 0 , 0};
  changeToByte(gameMapByte);
  for(int i = 0; i < 8; i++) {
    Serial.print(gameMapByte[i]);
    Serial.print(", ");
  }
  Serial.print("\n");
  Serial.print("+++++++++++++++++++++++++++++\n");
  for (int i = 0; i < 8; i++){
    lc.setRow(0,i,gameMapByte[i]);
  }
  
  //for (int i = 8; i < 16; i++)
  //{
  //  lc.setRow(1,i - 8,gameMapByte[i]);
  //}
  for (int i = 0; i < COUNT_OF_OBSTACLE; ++i) {
      if (obstacles[i].isExist) {
          obstacles[i].x += 1;
          if (obstacles[i].x > 7) {
              obstacles[i].isExist = 0;
          }
      }
  }
  checkForCollision(&car, obstacles);
  loopCounter += 1;
  delay(1500 / gameSpeed);
  if (endGame == 1) {
      for(int i = 0; i < 8; i++) {
        lc.setRow(0,i, power(i) + power(7 - i));
      }
      delay(200);
      resetFunc();  //call reset
  }
}

void changeToByte(byte game[]) {
  for(int i = 0; i < 8; i++) {
    game[i] = 0;
    for(int j = 0; j < 8; j++) {
      game[i] += gameMap[i][j] * power(7 - j);
    }
  }
}
int power(int i) {
  int k = 1;
  for(int j = 0; j < i; j++) {
    k *= 2;
  }
  return k;
}
void drawLED(int x, int y) {
    gameMap[x][y] = 1;
}

void createRandomObstacles(struct Obstacle *obstacles) {
    if (loopCounter - 6 > last_counter) {
        last_counter = loopCounter;
        for (int i = 0; i < 5; ++i) {
          srand(time(NULL));
            int y = 2*(random() % 3) + 1;
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
            if (obstacle_x > 2) {
                if (obstacle_y <= car_y + 1 && obstacle_y >= car_y - 1) {
                    endGame = 1;
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
            if (obstacles[i].isExist && points[j][0] < 8) {
                drawLED(points[j][0], points[j][1]);
            }
        }
    }
}

void makeMapReady(struct Car *car, struct Obstacle obstacles[]) {
    for (int i = 0; i < 8; ++i) {
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
        gameMap[carPoints[k][0]][carPoints[k][1]] = 1;
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
