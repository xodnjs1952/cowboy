// cowboy_q3.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
//2013112044 강태원;
#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double reculsive(int *c_pi,int* c_life,int*last,int CN,int c_shoot_index);
int main()
{
	int CN;
	printf("cn=");
	scanf_s("%d", &CN);

	int *c_pi= (int*)malloc(sizeof(int) * (CN));//각 카우보이들이 맞출 확률
	int *c_life= (int*)malloc(sizeof(int) * (CN));//카우보이의 생존여부
	int c_q_count = 0;//생존확률이 얼마만큼 차있나 체크하는 변수 
	double *c_q = (double*)malloc(sizeof(double) * (CN));//카우 보이가 살아남을 확률 (최종 결과물)
	int *last; //재귀 함수의 형태로 구현할것이기 때문에 가장 마지막에 살아남는 카우보이가 누구이지를 
	           //콜 바이 레퍼런스로 가져온다.
	int lastcowboy;//last라는 포이터의 주소를 할당하기위해서
	last = &lastcowboy;//last 포인터를 lastcowboy의 연결한다.
	double life_p;//생존 확률
	int p0_100_check = 0;//첫번째 카우보이가 100퍼센트의 실력을 가지고 있다면 
	                     //반드시 한 놈이 죽기 때문에 그것을 예외처리 할 예정이다.
	                     // 그러기위해서 무한루프를 탈출하기위해 임의로 설정된 변수
	for (int i = 0; i < CN; i++) {//카우보이들의 맞출 확률을 설정한다.
		scanf_s("%d", &c_pi[i]);
	}
	
	for (int i = 0; i < CN; i++) {//초기화 시킨다.
		c_q[i] = NULL;
		c_life[i] = 1;
		//인덱스번호가 카우보이의 번호라고 생각하고 1이면 산경우 0이면 죽는 경우로 생각하자.
		//일단 전부 살려야지 게임이 진행된다.
	}
	
	while (1)
	{
		if (c_pi[0]==100) {//제일먼저 쏘는 카우보이가 100의 확률을 가졌다면 한사람이 반드시 죽기 때문에
						   //예외 처리했다.
			for (int i = 0; i < CN; i++) {
				if (c_q[i] == NULL) {
					p0_100_check++;//0.0의 확률을 가진카우보이는 시작하자 마자 죽는 카우보이 이기때문에
				}                  //다른 카우보이들은 0의 확률을 가질수 없다.
			}
			if (p0_100_check == 1) {//시작하자마자 죽은 카우보이를 제외하고는 생존확률 탐색이 끝났다.
				break;//탈출
			}
			else {
				p0_100_check = 0;//찰출하지 못하면 체크변수를 초기화 해준다.
			}

			life_p = reculsive(c_pi, c_life, last, CN, 0);//리컬시브로 리턴한다.
			c_q[*last] = life_p;//한사람이 생존했을때 라스트 포인터가 작동해서 그값을 가져온다
		   // 함수의 리턴은 하나의 값뿐이 안되기에 콜 바이 레퍼런스를 이용했다.
		}
		else {//제일 처음 쏘는놈이 100의 확률을 가지지 않았을 경우에 실시한다.

			for (int i = 0; i < CN; i++) { //맞출 확률이 0인 놈은 있을수가 없기에
				if (c_q[i] == NULL) { // 반드시 단 1퍼센트의 생존 확률을 가져야한다.
					c_q_count++;// 즉 0퍼센트의 생존확률이 있다면 무한루프를 탈출할수 없다.
				}
			}
			if (c_q_count == 0) {//생존확률이 다구해진 경우 무한 루프를 탈출한다.
				break;
			}
			else {
				c_q_count = 0;
			}
			//확률을 계산해서 저장한다.
			life_p = reculsive(c_pi, c_life, last, CN, 0);
			c_q[*last] = life_p;
		}
	}

	//출력
	for (int i = 0; i < CN; i++) {
		printf("%d 번째 카우보이의 생존 확률 : %f 맞출 확률:%f \n", i + 1, c_q[i], (double)c_pi[i] / 100);
	}
	
}
 //파라미터 설명  맞출확률   생존여부  마지막생존자 명수    /*쏘는 사람*/      
double reculsive(int *c_pi, int* c_life, int*last, int CN, int c_shoot_index) {
	srand(time(NULL));//총을 쏘는것을 란드 함수로 계산하기에 그때 그때 이것을 설정 안해주면 항상 동일하게 된다.
	                  //다시말해 이게 없으면 무한루프를 탈출할수가 없다.
	int c_p_max = 0; //카우보이는 항상 제일 잘쏘는 놈을 겨냥한다. 제일 잘쏘는 카우보이의 맞출 확률이다.
	int c_p_max_index; //가장 잘쏘는 카우보이의
	int c_count = 0;// 살아 남은 카우 보이의 숫자가 몇명이지 확인.
	int c_last_index = 0;//마지막에 살아 남는 카우보이를 저장한다
	int shoot;// 총을 쏜다
	
	if (c_shoot_index >= CN) {//가장 마지막놈이 쏘면 제일 처음 으로 돌아온다.
		return reculsive(c_pi, c_life, last, CN, 0);
	}
	else {
		if (c_life[c_shoot_index] == 0)//쏘려는 사람이 죽은경우 다음 사람이 쏘게한다.
		{
			return reculsive(c_pi, c_life, last, CN, c_shoot_index + 1);
		}
		else {// 쏘려는 사람이 죽지 않았을경우 쏘게 된다.
			for (int i = 0; i < CN; i++) {//탐색을 위한 용도일 뿐 이걸로 쏘는 순서가 변경되지는 않는다.
				if (c_life[i] == 1) {//살아 있는 사람을 탐색
					c_count++; // 남아있는 사람이 몇명인가를 탐색
					c_last_index = i;//마지막 남아 있는 한명이 저장된다.
					if (c_shoot_index != i) {//자기 자신을 겨냥할수는 없다.
						if (c_p_max < c_pi[i]) {//제일 잘쏘는 카우보이를 탐색
							c_p_max = c_pi[i];
							c_p_max_index = i;//제일 잘쏘는 카우보이!
						}
					}
				}
			}
			if (c_count == 1) {// 한명의 카우보이만 살아 남는경우
				for (int i = 0; i < CN; i++) {//카우보이의 생존을 리셋한다.
					c_life[i] = 1;
				}
				*last = c_last_index;// 제일 마지막에 살아남는 카우보이를 last포인터로 지명
				return 1.0;//즉 이곳이 작동하는 경우는 제일 마지막 사람이 남아 있는 경우 이므로
			}              //뒤로 이어지지 않는다. 그래서 1을 리턴
			else {//여러명의 카우보이가 있다면 총을쏜다.
				shoot = rand() % 100 + 1;// 총쏘기-이때 겨냥하는 사람은 가장 잘쏘는 사람!
				if (c_pi[c_shoot_index] >= shoot) {//총에 맞는경우
					c_life[c_p_max_index] = 0;// 가장 잘쏘는 사람이 총에 맞아서 죽는다.
					return (((double)c_pi[c_shoot_index] / 100.0)*reculsive(c_pi, c_life, last, CN, c_shoot_index + 1));
					//여러명이 남았으므로 다음 게임을 진행한다.
				}
				else {// 안맞는경우!
					return ((1.0 - ((double)c_pi[c_shoot_index] / 100.0))*reculsive(c_pi, c_life, last, CN, c_shoot_index + 1));
					//여러명이 남아 있으므로 다음게임을 진행한다.
				}
			}
		}

	}
}
// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
