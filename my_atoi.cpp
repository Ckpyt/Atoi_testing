// my_atoi.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <math.h>
#include <ctime>
#include <cstdio>
#include <sstream>
#include <stdio.h>
#include <map>
#include <random>
//#include <cctype>

//#include "strtol_l.h"
//#include "strtol.h"
#include "atoi.h"



#define __MASSIVE_SIZE__ 10000000

std::string testData[__MASSIVE_SIZE__];
std::map<int, std::string, std::less<int>> NameAndDeltaTime;

void FillData() {
	/* initialize random seed: */
	srand((unsigned int)time(NULL));
	for (int i = 0; i < __MASSIVE_SIZE__; i++) {
		testData[i] = std::to_string(rand() - 1000000);
	}
}

void FillData(int max) {
	/* initialize random seed: */
	std::random_device rd;
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, max); // define the range
	for (int i = 0; i < __MASSIVE_SIZE__; i++) {
		testData[i] = std::to_string(1000000000 + distr(eng) /*- max / 2*/);
	}
}

int my_atoi_id(const char* str) {
	int res = 0;
	for (int i = 0; i < 10; i++)
		res = res * 10 + str[i] - '0';
	return res;
}

int my_idtoi_asm(const char* str) {
	int res = 0;
#ifdef X86
	__asm{
		pusha
		mov esi, [str]
		xor eax, eax
		xor ebx, ebx
		mov ebx, [esi]
		xor ecx,ecx
		xor edx,edx

		mov cl, 10
		sub bl, '0'
		mul ecx
		mov dl, bl
		add eax, edx
		mov ebx, [esi + 1]

		sub bl, '0'
		mul ecx
		mov dl, bl
		add eax, edx
		mov ebx, [esi + 2]


		sub bl, '0'
		mul ecx
		mov dl, bl
		add eax, edx
		mov ebx, [esi + 3]

		sub bl, '0'

		mul ecx
		mov dl, bl
		add eax, edx


		mov ebx, [esi + 4]

		sub bl, '0'
		mul ecx
		mov dl, bl
		add eax, edx
		mov ebx, [esi + 5]


		sub bl, '0'
		mul ecx
		mov dl, bl
		add eax, edx
		mov ebx, [esi + 6]

		sub bl, '0'
		mul ecx
		mov dl, bl
		add eax, edx
		mov ebx, [esi + 7]

		sub bl, '0'
		mul ecx
		mov dl, bl
		add eax, edx
		shr ebx, 8
		mov ebx, [esi + 8]

		sub bl, '0'
		mul ecx
		mov dl, bl
		add eax, edx
		mov ebx, [esi + 9]

		sub bl, '0'
		mul ecx
		mov dl, bl
		add eax, edx

		mov res, eax
		popa
		
	}
#endif
	return res;
}

int my_atoi_opt(const std::string& str) {
	int num = 0;
	int zn = 1;
	size_t len = str.length();
	const char* pos = str.c_str();

	if (*pos == '-') {
		pos++;
		zn = -1;
		len--;
	}
	switch (len) {
	case 10:
		num = num * 10 + (int)* pos - '0';
		pos++;
	case 9:
		num = num * 10 + (int)* pos - '0';
		pos++;
	case 8:
		num = num * 10 + (int)* pos - '0';
		pos++;
	case 7:
		num = num * 10 + (int)* pos - '0';
		pos++;
	case 6:
		num = num * 10 + (int)* pos - '0';
		pos++;
	case 5:
		num = num * 10 + (int)* pos - '0';
		pos++;
	case 4:
		num = num * 10 + (int)* pos - '0';
		pos++;
	case 3:
		num = num * 10 + (int)* pos - '0';
		pos++;
	case 2:
		num = num * 10 + (int)* pos - '0';
		pos++;
	case 1:
		num = num * 10 + (int)* pos - '0';
		pos++;
	case 0:
		break;
	}

	return num*zn;
}

int my_atoi(const std::string &str) {
	int num = 0;
	int zn = 1;
	const char* pos = str.c_str();

	if (*pos == '-') {
		pos++;
		zn = -1;
	}

	do {
		num = num * 10 + (int)*pos - '0';
	} while (*++pos != 0);

	return num * zn;
}



int test1() {
	const clock_t begin_time = clock();
	int summ = 0;


	for (int i = 0; i < __MASSIVE_SIZE__; i++) {
		summ += atoi(testData[i].c_str());
	}


	const std::string name("atoi");
	const clock_t end_time = clock();
	int testDeltaTime = end_time - begin_time;
	//I should use summ somewhere, the otherwise optimiser will remove whole cycle
	printf("%s delta time: %d, %d \n", name.c_str(), testDeltaTime, summ);

	NameAndDeltaTime.insert(std::pair<int, std::string>(testDeltaTime, name));

	return summ;
}


int test2() {
	const clock_t begin_time = clock();
	int summ = 0;

		for (int i = 0; i < __MASSIVE_SIZE__; i++) {
			summ += stoi(testData[i]);
		}

	const std::string name("stoi");
	const clock_t end_time = clock();
	int testDeltaTime = end_time - begin_time;
	printf("%s delta time: %d, %d \n", name.c_str(), testDeltaTime, summ);

	NameAndDeltaTime.insert(std::pair<int, std::string>(testDeltaTime, name));
	return summ;
}

int test3() {
	const clock_t begin_time = clock();
	int summ = 0;
	int num;

		for (int i = 0; i < __MASSIVE_SIZE__; i++) {
			if(sscanf(testData[i].c_str(), "%d", &num) > 0)
				summ += num;
		}

	const std::string name("sscanf");
	const clock_t end_time = clock();
	int testDeltaTime = end_time - begin_time;

	printf("%s delta time: %d, %d \n", name.c_str(), testDeltaTime, summ);

	NameAndDeltaTime.insert(std::pair<int, std::string>(testDeltaTime, name));
	return summ;
}

int test4() {
	int summ = 0;
	int num;
	//preparing data
	std::string data;
	
	for (int i = 0; i < __MASSIVE_SIZE__; i++) {
		data += testData[i] + " ";
	}
	std::istringstream dt(data);

	//testing
	const clock_t begin_time = clock();

	for (int i = 0; i < __MASSIVE_SIZE__; i++) {
		num = 0;
		dt >> num;
		summ += num;
	}

	const std::string name("istringstream");
	const clock_t end_time = clock();
	int testDeltaTime = end_time - begin_time;
	printf("%s delta time: %d, %d \n", name.c_str(), testDeltaTime, summ);

	NameAndDeltaTime.insert(std::pair<int, std::string>(testDeltaTime, name));

	return summ;
}

int test5() {
	const clock_t begin_time = clock();
	int summ = 0;


	for (int i = 0; i < __MASSIVE_SIZE__; i++) {
		summ += my_idtoi_asm(testData[i].c_str());
	}


	const std::string name("my_idtoi_asm");
	const clock_t end_time = clock();
	int testDeltaTime = end_time - begin_time;
	printf("%s delta time: %d, %d \n", name.c_str(), testDeltaTime, summ);

	NameAndDeltaTime.insert(std::pair<int, std::string>(testDeltaTime, name));

	return summ;
}

int test6() {
	const clock_t begin_time = clock();
	int summ = 0;


	for (int i = 0; i < __MASSIVE_SIZE__; i++) {
		summ += my_atoi_id(testData[i].c_str());
	}

	
	const std::string name("my_atoi_id");
	const clock_t end_time = clock();
	int testDeltaTime = end_time - begin_time;
	printf("%s delta time: %d, %d \n", name.c_str(), testDeltaTime, summ);

	NameAndDeltaTime.insert(std::pair<int, std::string>(testDeltaTime, name));

	return summ;
}

int testMyAtoi() {
	const clock_t begin_time = clock();
	int summ = 0;

	for (int i = 0; i < __MASSIVE_SIZE__; i++) {
		summ += my_atoi(testData[i]);
	}

	const std::string name("my_atoi");
	const clock_t end_time = clock();
	int testDeltaTime = end_time - begin_time;
	printf("%s delta time: %d, %d \n", name.c_str(), testDeltaTime, summ);

	NameAndDeltaTime.insert(std::pair<int, std::string>(testDeltaTime, name));
	return summ;
}

int testMyAtoiOpt() {
	const clock_t begin_time = clock();
	int summ = 0;

	for (int i = 0; i < __MASSIVE_SIZE__; i++) {
		summ += my_atoi_opt(testData[i]);
	}

	const std::string name("my_atoi_opt");
	const clock_t end_time = clock();
	int testDeltaTime = end_time - begin_time;
	printf("%s delta time: %d, %d \n", name.c_str(), testDeltaTime, summ);

	NameAndDeltaTime.insert(std::pair<int, std::string>(testDeltaTime, name));
	return summ;
}

void testing(int max) {
	NameAndDeltaTime.clear();
	std::cout << "testing: \n";
	std::cout << "maximum number is:" << max << "\n";
	FillData(max);
	testMyAtoiOpt();
	testMyAtoi();

	test1();
	test2();
	test3();
	test4();
#ifdef x86
	test5();
#endif
	if(max == 100000000)
		test6();
	std::cout << "\n comparison:\n";

	std::pair<int, std::string> faster = *NameAndDeltaTime.begin();
	int FasterTime = faster.first;

	for(auto var : NameAndDeltaTime)
	{
		printf("%s finished for %d slower in %.2f times \n", 
			var.second.c_str(), var.first, (float)((float)var.first / (float)FasterTime ));
		//cout << var.second << " finished in " << var.first << " "
		//	<< "slower in" << (float)FasterTime / (float)var.first << "times \n";

	}
	std::cout << "\n";
}

int main()
{

	testing(10);
	testing(100);
	testing(1000);
	testing(10000);
	testing(100000);
	testing(1000000);
	testing(10000000);
	testing(100000000);
}
