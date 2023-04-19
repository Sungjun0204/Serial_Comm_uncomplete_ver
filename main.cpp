#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string.h>
#include <cstdio>
#include <iostream>



using namespace std;

volatile int i = 0;							// 패킷 전체에 대한 인덱스 변수
volatile int k = 0;							// n번째 센서 값 버퍼(구조체)에 대한 인덱스 변수
volatile int l = 0;							// n번째 센서 값의 x, y, z값에 대한 인덱스 변수
volatile int kk = 0;							// 센서 9개에 대한 인덱스 변수
int checkSum_here = 0;
int num = 0;
int mode = 1;
char checkSum_arr[10];				// 패킷에서 넘겨받은 checksum 배열 값 저장 변수
int checkSum;
int timerr = 0;
char header[3] = { 0, };


// 홀 센서 값을 저장하는 구조체 변수 선언
struct hallsensor_struct {
	char full_val[29] = "";			// 패킷에서 n번째 센서값을 뭉텅이로 먼저 저장하는 임시 버퍼
	char x_val[10] = "";				// n번째 센서 값의 x축 값
	char y_val[10] = "";				// n번째 센서 값의 y축 값
	char z_val[10] = "";				// n번째 센서 값의 z축 값 
										// (음수 부호와 null 까지 고려하여 크기를 6으로 설정)

	int x_val_int = 0;
	int y_val_int = 0;
	int z_val_int = 0;				// x, y, z_val에서 정수형으로 변환 후 저장할 변수 선언

	double x_val_real = 0;
	double y_val_real = 0;
	double z_val_real = 0;			// 정수형으로 변환 후, 손실된 소수점 복구한 최종 센서 값 저장

}hall_[9];

char large_cap = 'A';				// n번째 센서 값이 시작되는 것을 비교/확인하기 위한 변수
char small_cap = 'a';				// n번째 센서 값이 끝나는 것을 비교/확인하기 위한 변수

int input_flag = 0;
int loop_flag = 0;
int header_check = 0;


/////////////////////
/// 모드 설정 변수 ///
/////////////////////
int print_check_mode = 0;	// 1이면 디버깅 모드로 코드 출력값 확인



/////////////////////
/// 함수 헤더 선언 ///
/////////////////////
void reset_func();
void hall_buffer_func(char* serial_arr, char* buffer_arr);
void seperate_axis_val(hallsensor_struct *sensor);

//char incomingData[4096] = "";

//////////////////////////////////////////////////////////////////////////
////////////////////// P a c k e t   I n f o /////////////////////////////
//																		//
//     A  A  A  -  D  -  1 0 0 0  -  1 1 1 1  -  2 2 2 2  -  0 0 8 4    //
//																		//
//     header   packet    point      velocity     slope		  check		//
//               size	  data		   data		   data	       sum		//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



// application reads from the specified serial port and reports the collected data
int _tmain(int argc, _TCHAR* argv[]) {

	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("\\\\.\\COM3");    // adjust as needed

	if (SP->IsConnected())
		printf("We're connected\n\n");

		// don't forget to pre-allocate memory
	char one_buf[2] = "";


	char incomingData_pre[4096] = "";
	int dataLength = 4095;
	int readResult = 0;
	
	char incomingData[292] = "";
	int kkk = 0;

	while (SP->IsConnected())	// 연결되면 이제 출력문으로 진입
	{


		// 들어온 데이터를 크기 300짜리 배열 buffer에 저장
		readResult = SP->ReadData(incomingData_pre, dataLength);
		//incomingData[readResult] = 0;

		/*
		while (loop_flag == 0 && i < (dataLength - 292)) {
			if (incomingData_pre[i] == 'Z') {
				for (kkk = 0; kkk < 292; kkk++)
					incomingData[kkk] = incomingData_pre[i + kkk];
				loop_flag = 1;
				mode = 2;
			}

			else i++;
		}
		*/



		/*
		while (loop_flag == 0) {
			one_buf[0] = 0;
			readResult = SP->ReadData(one_buf, 1);

			if (one_buf[0] == 'Z' && input_flag == 0) input_flag = 1;
			// input_flag가 0이고 Z가 들어오기 시작하면 input_flag를 1로 바꿔 통신 값 저장 시작

			if (input_flag == 1) incomingData[i++] = one_buf[0];

			if (i == 292 && incomingData[291] == 'Y') {
				loop_flag = 1;
				i = 0; mode = 2;
			}
				// if (incomingData[291] != 'Y') { input_flag = 0; loop_flag = 0; mode = 1; i = 0; }

			else if(i == 292 && incomingData[291] != 'Y') {
				input_flag = 0; loop_flag = 0; mode = 1; i = 0;
			}
			

		}
	*/

	

		// && incomingData[291] == 'Y'

		//mode = 2;

		/*
		if (incomingData[0] == 'Z') {
			num++;
			if (incomingData[1] == 'Z') {
				num++;
				if (incomingData[2] == 'Z') {
					num++;
					if (num == 3) mode = 2;
				}
			}
		}
		*/


		/*
		else if (mode == 1) {


			if (incomingData[0] == 'Z') mode++;
			else mode = 0;

			//else if (incomingData[i++] == 0) { reset_func(); break; }

			if (i >= dataLength) i = 0;
			//if (num == 3) mode++;


		}
		*/







		// mode = 2일때: 센서별 값 분리
		if (mode == 2) {

			i = 3;

			if (print_check_mode == 1) printf("%s\n", incomingData);

			// 헤더 분리
			//for (kk = 0; kk < 3; kk++) header[kk] = incomingData[i++];

			// 센서마다 값 분리
			for (kk = 0; kk < 9; kk++) {
				hall_buffer_func(incomingData, hall_[kk].full_val);
				if (print_check_mode == 1) printf("%s\n", hall_[kk].full_val);
			}

			// checksum 값 분리
			for (k = 0; k < 9; k++) {	// checksum 값 10개만 딱 받기
				checkSum_arr[k] = incomingData[i++];	// 차례로 checksum 변수에 저장
			}
			k = 0;						// 다른 곳에서 사용될 것을 감안하여 초기화를 해 준다


			//if (header[0] == 'Z' && header[1] == 'Z' && header[2] == 'Z') mode = 3;
			//else { i = 0; mode = 2; }

			// if (incomingData[291] != 'Y') { i = 0; mode = 2; }

			mode++;
		}




		// mode = 3일때: 각 센서값을 x, y, z값으로 분해 후 정수형으로 변환
		else if (mode == 3) {

			// 축 별로 값 분리 후 정수형으로 변환
			if (print_check_mode == 1) printf("------------------------------------\n");
			for (kk = 0; kk < 9; kk++) {
				seperate_axis_val(&hall_[kk]);
				if (print_check_mode == 1) printf("%d %d %d\n", hall_[kk].x_val_int, hall_[kk].y_val_int, hall_[kk].z_val_int);
			}
			//printf("process complete4\n");

			// checksum 정수형으로 변환
			int sign = 0;
			if (checkSum_arr[0] == '1') { checkSum_arr[0] = '0'; sign = -1; }
			else sign = 1;
			checkSum = atoi(checkSum_arr) * sign;

			// 지금까지 분리했던 모든 센서 값들 전부 더하기
			for (kk = 0; kk < 9; kk++)
				checkSum_here += (hall_[kk].x_val_int) + (hall_[kk].y_val_int) + (hall_[kk].z_val_int);


			if (print_check_mode == 1) printf("%d, %d\n", checkSum, checkSum_here);

			// checksum 비교
			if (checkSum == checkSum_here) {
				//printf("Packet Comm Success!!\n");
				printf("%d %d\n", hall_[0].x_val_int, timerr++);

			}
			else {
				//printf("Try again...\n");
				//mode = 2;
			}	// 그러나 일치하지 않으면 다시 처음부터

			reset_func();	// 한 번 통신이 끝나면 지금까지 사용했던 변수들 전부 초기화
		}


	}
	//Sleep(100);

}



// 패킷 분리작업 때 사용한 변수들 초기화하는 함수
void reset_func() {
	mode = 1;
	num = 0;
	for (kk = 0; kk < 9; kk++) hall_[kk] = { 0 };
	//for (kk = 0; kk < 4096; kk++) incomingData[kk] = { 0 };
	//for (kk = 0; kk < 10; kk++) checkSum_arr[kk] = 0;
	for (kk = 0; kk < 3; kk++) header[kk] = 0;
	i = 0;
	k = 0;
	l = 0;
	kk = 0;
	checkSum = 0;
	checkSum_here = 0;


	large_cap = 'A';
	small_cap = 'a';


	input_flag = 0;
	loop_flag = 0;
}


// n번째 센서 값 별로 분리 후 저장해주는 함수
void hall_buffer_func(char* serial_arr, char* buffer_arr) {

	if (serial_arr[i++] == large_cap) {					// 패킷 헤더 다음 4번째부터 대문자가 나오면
		while (serial_arr[i] != small_cap) {			// 첫 번째 센서 값이므로 소문자 알파벳이 나올때 까지 
			if (serial_arr[i] == 0) {					// 만약 중간에 null 값이 나오면 처음부터 다시 시작
				reset_func(); break;
			}

			buffer_arr[k++] = serial_arr[i++];	// 센서값 통째로 버퍼에 저장
		}

		// n번째 센서 값을 버퍼에 다 저장했으면(= 소문자 알파벳이 나와서 반복문이 끝나고 여기로 넘어오면)
		large_cap++;	// n번째 센서 값의 시작을 알리는 변수에 +1
		small_cap++;	// n번째 센서 값의 끝을 알리는 변수에 +13
		i++;			// n+1번째 센서 값 분해 작업 시, serial_arr를 알파벳 대문자로 위치시키기 위한 연산
		k = 0;			// n+1번째 센서 값을 또 다른 버퍼에 담아야 하므로 인덱스 k값 초기화

	}

}


// n번째 센서 값을 x, y, z축별로 세부 분리 후 실수형으로 따로 저장해주는 함수
void seperate_axis_val(hallsensor_struct* sensor) {

	int sign = 0;
	l = 0;
	k = 0;

	while ((*sensor).full_val[k] != ',') { 	// n번째 센서 값의 x값 추출 
		(*sensor).x_val[l++] = (*sensor).full_val[k++];
		if (l == 10) break;			// 만약 x, y, z_val 최대 크기인 10을 넘어가 버리면 자동으로 반복문 탈출
	}
	if ((*sensor).x_val[0] == '1') { (*sensor).x_val[0] = '0'; sign = -1; }	// 패킷 맨 앞자리가 1이면 음수를 의미하므로, 0으로 바꾼 후 음수 연산자 설정
	else sign = 1;				// 그런데 0으로 시작하면 양수를 의미하므로, 양수 연산자로 설정
	(*sensor).x_val_int = atoi((*sensor).x_val) * sign;	// 추출한 값을 실수형으로 변환 + 부호 복원

	k++;		// 콤마 패스
	l = 0;		// 다음 축 값을 분리하기 위해 인덱스 값 초기화

	while ((*sensor).full_val[k] != ',') {	// n번째 센서 값의 y값 추출
		(*sensor).y_val[l++] = (*sensor).full_val[k++];
		if (l == 10) break;
	}
	if ((*sensor).y_val[0] == '1') { (*sensor).y_val[0] = '0'; sign = -1; }
	else sign = 1;
	(*sensor).y_val_int = atoi((*sensor).y_val) * sign;

	k++;
	l = 0;

	while ((*sensor).full_val[k] != '\0') {	// n번째 센서 값의 z값 추출
		(*sensor).z_val[l++] = (*sensor).full_val[k++];
		if (l == 9) break;
	}
	if ((*sensor).z_val[0] == '1') { (*sensor).z_val[0] = '0'; sign = -1; }
	else sign = 1;
	(*sensor).z_val_int = atoi((*sensor).z_val) * sign;

	k = 0;
	l = 0;		// n+1번째 센서 값의 세부 값 추출을 위해 인덱스 변수 초기화

}