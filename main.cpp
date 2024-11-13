#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include "dxl.hpp"
using namespace std;

// Ctrl+C 신호를 처리할 전역 변수
bool ctrl_c_pressed = false;

// Ctrl+C 핸들러
void ctrlc_handler(int) {
    ctrl_c_pressed = true;  // Ctrl+C가 눌렸으면 true로 설정
}

int main(void)
{
    Dxl mx;  // Dxl 클래스 객체 생성
    struct timeval start, end1;  // 시간 측정을 위한 timeval 구조체
    double time1;  // 시간 변수
    int vel1 = 0, vel2 = 0;  // 왼쪽/오른쪽 모터 속도 변수 (초기값: 0)

    signal(SIGINT, ctrlc_handler);  // SIGINT (Ctrl+C) 신호 처리 함수 설정
    if (!mx.open()) {  // Dynamixel 장치 열기
        cout << "dynamixel open error" << endl;
        return -1;
    }

    while (true)  // 무한 루프
    {
        gettimeofday(&start, NULL);  // 루프 시작 시간 기록
        
        if (mx.kbhit())  // 키보드 입력 체크
        {
            char c = mx.getch();  // 입력된 키값을 가져옴
            switch (c)  // 키에 따라 모터 속도 결정
            {
            case 's': vel1 = 0; vel2 = 0; break;  // 's' 키: 정지
            case 'f': vel1 = 50; vel2 = -50; break;  // 'f' 키: 전진
            case 'b': vel1 = -50; vel2 = 50; break;  // 'b' 키: 후진
            case 'l': vel1 = -50; vel2 = -50; break;  // 'l' 키: 좌회전
            case 'r': vel1 = 50; vel2 = 50; break;  // 'r' 키: 우회전
            default: vel1 = 0; vel2 = 0; break;  // 그 외의 키: 정지
            }
            mx.setVelocity(vel1, vel2);  // 모터에 속도 명령 전송
        }

        if (ctrl_c_pressed) break;  // Ctrl+C 입력 시 루프 종료
        usleep(20 * 1000);  // 20ms 대기 (모터 제어 후 빠른 반복 방지)

        gettimeofday(&end1, NULL);  // 종료 시간 기록
        time1 = end1.tv_sec - start.tv_sec + (end1.tv_usec - start.tv_usec) / 1000000.0;
        // 실행 시간 계산

        cout << "vel1:" << vel1 << ", vel2:" << vel2 << ", time:" << time1 << endl;
        // 속도 및 실행 시간 출력
    }

    mx.close();  // Dynamixel 장치 닫기
    return 0;  // 프로그램 종료
}