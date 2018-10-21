//
//  main.c
//  2048
//  表示 为了练习C语言 在这先写着一个小游戏吧
//  “听说以后上课要用？”
//  调试代码已注释 目前游戏所有功能均实现
//  唯一不足的是在苹果系统环境无getch函数
//  且不会自行实现 在其他系统直接include <conio.h>
//  把getchar改为getch即可畅玩2048
//  Created by licsber on 2018/10/11.
//  Copyright © 2018 licsber. All rights reserved.
//

#include <stdio.h>//emmmm
#include <stdbool.h> //c里没有布尔型变量 c99标准才加的
#include <time.h>//用于产生随机数
#include <stdlib.h>
//#include <ncurses.h>//图形界面库 等后来更新printMap函数再引用

void gameStart(void);//游戏开始
void randomAdd(void);//在空格随机加2和4 其中2为十分之九概率
void printMap(void);//用printf打印地图
void getDirection(void);//读取键盘输入
void moveUp(void);//上
void moveDown(void);//下
void moveLeft(void);//左
void moveRight(void);//右
void moveForward(void);//推进
void moveMerge(void);//合并
bool checkIfGameOver(void);//检测游戏是否over 即无空位同时相邻数组元素各不相等
void restart(void);//重新开始
void loop(void);//循环游戏
//int countZero(void);//还有几个是0
//int returnZero(int line);

int score = 0;//当前得分
int map[4][4]={
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};//这是地图 用二维数组实现
int tmp[4] = {0,0,0,0};

void gameStart(void){
    printf("Welcome to play 2048 game! by licsber\n");
    randomAdd();
    printMap();
    printf("[w][s][a][d]move [r]restart [q]quit\n");
}

void randomAdd(void){
    srand((unsigned int)time(NULL));//撒随机数种子
    //printf("%d",rand());
    int i,j,time = 0;//行 列 循环次数(debug用)
    do{
        i = rand() % 4;
        j = rand() % 4;
        time++;
    }while (map[i][j] != 0);
    if (rand() % 10 == 0 ) {
        map[i][j]=4;
    }
    else{
        map[i][j]=2;
    }
    //printf("%d",time);
    //上面的代码在格子快满之后也有几率循环N次都不停
    //todo改进方案：先找到所有空格子
    //下面的代码是有问题的 有几率无限循环
    /*
     bool finish = 0;//本轮是否已放置方块
     for (int i = 0; i < 4; i++) {
     for (int j = 0; j < 4; j++) {
     if (!map[i][j]) {
     if (rand() % 16 == 0 && finish == 0) {
     // 十分之一的概率放4
     if (rand() % 10 == 0 ) {
     map[i][j]=4;
     }
     else{
     map[i][j]=2;
     }
     finish = 1;
     }
     }
     }
     }
     if (finish == 0) {
     randomAdd();
     }
     else{
     return;
     }
     */
}

void printMap(void){
    for (int i = 0; i < 4; i++) {
        printf("          ");
        for (int j = 0; j < 4; j++) {
            printf("%4d ",map[i][j]);
        }
        printf("\n");
    }
    return;
}

void getDirection(void){
    tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
    char in = getchar();
    switch (in) {
        case 'w':
            moveUp();
            break;
        case 's':
            moveDown();
            break;
        case 'a':
            moveLeft();
            break;
        case 'd':
            moveRight();
            break;
        case 'r':
            restart();
            break;
        case 'q':
            exit(0);
            break;
        default:
            getDirection();
            break;
    }
}

void moveUp(void){
    for (int line = 0; line < 4; line++) {
        for (int i = 0; i < 4; i++) {
            tmp[i] = map[i][line];
        }
        moveMerge();
        for (int i = 0; i < 4; i++) {
            map[i][line] = tmp[i];
        }
    }
}

void moveDown(void){
    for (int line = 0; line < 4; line++) {
        for (int i = 0; i < 4; i++) {
            tmp[3 - i] = map[i][line];
        }
        moveMerge();
        for (int i = 0; i < 4; i++) {
            map[i][line] = tmp[3 - i];
        }
    }
}

void moveLeft(void){
    for (int line = 0; line < 4; line++) {
        for (int i = 0; i < 4; i++) {
            tmp[i] = map[line][i];
        }
        moveMerge();
        for (int i = 0; i < 4; i++) {
            map[line][i] = tmp[i];
        }
    }
}

void moveRight(void){
    for (int line = 0; line < 4; line++) {
        for (int i = 3; i >= 0; i--) {
            tmp[3 - i] = map[line][i];
        }
        moveMerge();
        for (int i = 3; i >= 0; i--) {
            map[line][i] = tmp[3 - i];
        }
    }
}

bool checkIfGameOver(void){
    //先检查格子是不是都满了
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (!map[i][j]) {
                //只要有空的格子 游戏就没结束
                return false;
            }
        }
    }
    //格子满了之后
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (map[i][j] == map[i+1][j] || map[i][j] == map[i][j+1]) {
                //如果一个格子和它右边或下边相等 游戏也没结束
                //这里数组如果越界访问会返回0 所以不影响结果
                return false;
            }
        }
    }
    return true;
}

void moveForward(void){
    //i为0的话就把最先一个非零的j挪到i的位置
    int j = 0;
    for (int i = 0; i < 4; i++) {
        j++;
        if (tmp[i] == 0) {
            for (; j < 4; j++) {
                if (tmp[j] != 0) {
                    tmp[i] = tmp[j];
                    tmp[j] = 0;
                    break;
                }
            }
        }
    }
}

void moveMerge(void){
    //处理合并 每合并一次就推进一次
    moveForward();
    for (int i = 0; i < 3; i++) {
        if (tmp[i] == tmp[i + 1]) {
            tmp[i] *=2;
            tmp[i + 1] = 0;
            moveForward();
        }
    }
}


/*
int countZero(void){
    int zero = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (!map[i][j]) {
                zero++;
            }
        }
    }
    return zero;
}
*/

void restart(void){
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            map[i][j] = 0;
        }
    }
    gameStart();
    loop();
}



void loop(void){
    for(;;) {
        getDirection();
        printMap();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (!map[i][j]) {
                    //只要有空的格子 就加
                    randomAdd();
                    goto go;
                }
            }
        }
    go:
        if (checkIfGameOver() ==  true){
            printf("You are lose!");
            exit(0);
        }
    }
}

int main() {
    gameStart();
    loop();
    return 0;
}
