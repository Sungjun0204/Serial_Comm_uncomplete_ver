#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string.h>
#include <cstdio>
#include <iostream>



using namespace std;

volatile int i = 0;							// ��Ŷ ��ü�� ���� �ε��� ����
volatile int k = 0;							// n��° ���� �� ����(����ü)�� ���� �ε��� ����
volatile int l = 0;							// n��° ���� ���� x, y, z���� ���� �ε��� ����
volatile int kk = 0;							// ���� 9���� ���� �ε��� ����
int checkSum_here = 0;
int num = 0;
int mode = 1;
char checkSum_arr[10];				// ��Ŷ���� �Ѱܹ��� checksum �迭 �� ���� ����
int checkSum;
int timerr = 0;
char header[3] = { 0, };


// Ȧ ���� ���� �����ϴ� ����ü ���� ����
struct hallsensor_struct {
	char full_val[29] = "";			// ��Ŷ���� n��° �������� �����̷� ���� �����ϴ� �ӽ� ����
	char x_val[10] = "";				// n��° ���� ���� x�� ��
	char y_val[10] = "";				// n��° ���� ���� y�� ��
	char z_val[10] = "";				// n��° ���� ���� z�� �� 
										// (���� ��ȣ�� null ���� ����Ͽ� ũ�⸦ 6���� ����)

	int x_val_int = 0;
	int y_val_int = 0;
	int z_val_int = 0;				// x, y, z_val���� ���������� ��ȯ �� ������ ���� ����

	double x_val_real = 0;
	double y_val_real = 0;
	double z_val_real = 0;			// ���������� ��ȯ ��, �սǵ� �Ҽ��� ������ ���� ���� �� ����

}hall_[9];

char large_cap = 'A';				// n��° ���� ���� ���۵Ǵ� ���� ��/Ȯ���ϱ� ���� ����
char small_cap = 'a';				// n��° ���� ���� ������ ���� ��/Ȯ���ϱ� ���� ����

int input_flag = 0;
int loop_flag = 0;
int header_check = 0;


/////////////////////
/// ��� ���� ���� ///
/////////////////////
int print_check_mode = 0;	// 1�̸� ����� ���� �ڵ� ��°� Ȯ��



/////////////////////
/// �Լ� ��� ���� ///
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

	while (SP->IsConnected())	// ����Ǹ� ���� ��¹����� ����
	{


		// ���� �����͸� ũ�� 300¥�� �迭 buffer�� ����
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
			// input_flag�� 0�̰� Z�� ������ �����ϸ� input_flag�� 1�� �ٲ� ��� �� ���� ����

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







		// mode = 2�϶�: ������ �� �и�
		if (mode == 2) {

			i = 3;

			if (print_check_mode == 1) printf("%s\n", incomingData);

			// ��� �и�
			//for (kk = 0; kk < 3; kk++) header[kk] = incomingData[i++];

			// �������� �� �и�
			for (kk = 0; kk < 9; kk++) {
				hall_buffer_func(incomingData, hall_[kk].full_val);
				if (print_check_mode == 1) printf("%s\n", hall_[kk].full_val);
			}

			// checksum �� �и�
			for (k = 0; k < 9; k++) {	// checksum �� 10���� �� �ޱ�
				checkSum_arr[k] = incomingData[i++];	// ���ʷ� checksum ������ ����
			}
			k = 0;						// �ٸ� ������ ���� ���� �����Ͽ� �ʱ�ȭ�� �� �ش�


			//if (header[0] == 'Z' && header[1] == 'Z' && header[2] == 'Z') mode = 3;
			//else { i = 0; mode = 2; }

			// if (incomingData[291] != 'Y') { i = 0; mode = 2; }

			mode++;
		}




		// mode = 3�϶�: �� �������� x, y, z������ ���� �� ���������� ��ȯ
		else if (mode == 3) {

			// �� ���� �� �и� �� ���������� ��ȯ
			if (print_check_mode == 1) printf("------------------------------------\n");
			for (kk = 0; kk < 9; kk++) {
				seperate_axis_val(&hall_[kk]);
				if (print_check_mode == 1) printf("%d %d %d\n", hall_[kk].x_val_int, hall_[kk].y_val_int, hall_[kk].z_val_int);
			}
			//printf("process complete4\n");

			// checksum ���������� ��ȯ
			int sign = 0;
			if (checkSum_arr[0] == '1') { checkSum_arr[0] = '0'; sign = -1; }
			else sign = 1;
			checkSum = atoi(checkSum_arr) * sign;

			// ���ݱ��� �и��ߴ� ��� ���� ���� ���� ���ϱ�
			for (kk = 0; kk < 9; kk++)
				checkSum_here += (hall_[kk].x_val_int) + (hall_[kk].y_val_int) + (hall_[kk].z_val_int);


			if (print_check_mode == 1) printf("%d, %d\n", checkSum, checkSum_here);

			// checksum ��
			if (checkSum == checkSum_here) {
				//printf("Packet Comm Success!!\n");
				printf("%d %d\n", hall_[0].x_val_int, timerr++);

			}
			else {
				//printf("Try again...\n");
				//mode = 2;
			}	// �׷��� ��ġ���� ������ �ٽ� ó������

			reset_func();	// �� �� ����� ������ ���ݱ��� ����ߴ� ������ ���� �ʱ�ȭ
		}


	}
	//Sleep(100);

}



// ��Ŷ �и��۾� �� ����� ������ �ʱ�ȭ�ϴ� �Լ�
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


// n��° ���� �� ���� �и� �� �������ִ� �Լ�
void hall_buffer_func(char* serial_arr, char* buffer_arr) {

	if (serial_arr[i++] == large_cap) {					// ��Ŷ ��� ���� 4��°���� �빮�ڰ� ������
		while (serial_arr[i] != small_cap) {			// ù ��° ���� ���̹Ƿ� �ҹ��� ���ĺ��� ���ö� ���� 
			if (serial_arr[i] == 0) {					// ���� �߰��� null ���� ������ ó������ �ٽ� ����
				reset_func(); break;
			}

			buffer_arr[k++] = serial_arr[i++];	// ������ ��°�� ���ۿ� ����
		}

		// n��° ���� ���� ���ۿ� �� ����������(= �ҹ��� ���ĺ��� ���ͼ� �ݺ����� ������ ����� �Ѿ����)
		large_cap++;	// n��° ���� ���� ������ �˸��� ������ +1
		small_cap++;	// n��° ���� ���� ���� �˸��� ������ +13
		i++;			// n+1��° ���� �� ���� �۾� ��, serial_arr�� ���ĺ� �빮�ڷ� ��ġ��Ű�� ���� ����
		k = 0;			// n+1��° ���� ���� �� �ٸ� ���ۿ� ��ƾ� �ϹǷ� �ε��� k�� �ʱ�ȭ

	}

}


// n��° ���� ���� x, y, z�ະ�� ���� �и� �� �Ǽ������� ���� �������ִ� �Լ�
void seperate_axis_val(hallsensor_struct* sensor) {

	int sign = 0;
	l = 0;
	k = 0;

	while ((*sensor).full_val[k] != ',') { 	// n��° ���� ���� x�� ���� 
		(*sensor).x_val[l++] = (*sensor).full_val[k++];
		if (l == 10) break;			// ���� x, y, z_val �ִ� ũ���� 10�� �Ѿ ������ �ڵ����� �ݺ��� Ż��
	}
	if ((*sensor).x_val[0] == '1') { (*sensor).x_val[0] = '0'; sign = -1; }	// ��Ŷ �� ���ڸ��� 1�̸� ������ �ǹ��ϹǷ�, 0���� �ٲ� �� ���� ������ ����
	else sign = 1;				// �׷��� 0���� �����ϸ� ����� �ǹ��ϹǷ�, ��� �����ڷ� ����
	(*sensor).x_val_int = atoi((*sensor).x_val) * sign;	// ������ ���� �Ǽ������� ��ȯ + ��ȣ ����

	k++;		// �޸� �н�
	l = 0;		// ���� �� ���� �и��ϱ� ���� �ε��� �� �ʱ�ȭ

	while ((*sensor).full_val[k] != ',') {	// n��° ���� ���� y�� ����
		(*sensor).y_val[l++] = (*sensor).full_val[k++];
		if (l == 10) break;
	}
	if ((*sensor).y_val[0] == '1') { (*sensor).y_val[0] = '0'; sign = -1; }
	else sign = 1;
	(*sensor).y_val_int = atoi((*sensor).y_val) * sign;

	k++;
	l = 0;

	while ((*sensor).full_val[k] != '\0') {	// n��° ���� ���� z�� ����
		(*sensor).z_val[l++] = (*sensor).full_val[k++];
		if (l == 9) break;
	}
	if ((*sensor).z_val[0] == '1') { (*sensor).z_val[0] = '0'; sign = -1; }
	else sign = 1;
	(*sensor).z_val_int = atoi((*sensor).z_val) * sign;

	k = 0;
	l = 0;		// n+1��° ���� ���� ���� �� ������ ���� �ε��� ���� �ʱ�ȭ

}