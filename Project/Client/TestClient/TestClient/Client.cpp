#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

using namespace std;

const int MAP_SIZE = 12;
const char EMPTY = '.';
const char CHARACTER = '@';

int characterX, characterY;

// 커서 위치 설정 함수
void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, pos);
}

// 맵을 초기화
void initializeMap(char map[MAP_SIZE][MAP_SIZE]) {
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            map[i][j] = EMPTY;
        }
    }
}

// 캐릭터를 무작위 위치에 배치
void placeCharacter(char map[MAP_SIZE][MAP_SIZE]) {
    srand(static_cast<unsigned int>(time(0)));
    characterX = rand() % MAP_SIZE;
    characterY = rand() % MAP_SIZE;
    map[characterY][characterX] = CHARACTER;
}

// 맵을 처음 한 번만 그리는 함수
void printMap(char map[MAP_SIZE][MAP_SIZE]) {
    setCursorPosition(0, 0);  // 화면의 맨 위로 커서 이동
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            cout << map[i][j] << ' ';
        }
        cout << endl;
    }
}

// 캐릭터 위치 업데이트 함수
void updateCharacterPosition(char map[MAP_SIZE][MAP_SIZE], int oldX, int oldY) {
    setCursorPosition(oldX * 2, oldY);
    cout << EMPTY;  // 이전 캐릭터 위치를 지움

    setCursorPosition(characterX * 2, characterY);
    cout << CHARACTER;  // 새로운 캐릭터 위치 출력
}

// 캐릭터 이동 함수
void moveCharacter(char map[MAP_SIZE][MAP_SIZE], char direction) {
    int oldX = characterX;
    int oldY = characterY;

    switch (direction) {
    case 'w': if (characterY > 0) characterY--; break;
    case 'a': if (characterX > 0) characterX--; break;
    case 's': if (characterY < MAP_SIZE - 1) characterY++; break;
    case 'd': if (characterX < MAP_SIZE - 1) characterX++; break;
    default: return;  // 유효하지 않은 입력
    }

    // 이전 위치와 새로운 위치를 업데이트
    map[oldY][oldX] = EMPTY;
    map[characterY][characterX] = CHARACTER;
    updateCharacterPosition(map, oldX, oldY);
}

int main() {
    char map[MAP_SIZE][MAP_SIZE];
    char input;

    initializeMap(map);
    placeCharacter(map);
    printMap(map);

    while (true) {
        if (_kbhit()) {  // 키 입력이 있는지 확인
            input = _getch();
            if (input == 'q') {
                break;
            }
            moveCharacter(map, input);
        }
    }

    setCursorPosition(0, MAP_SIZE + 1);  // 게임 종료 메시지 위치 조정
    cout << "Game Over!\n";
    return 0;
}
