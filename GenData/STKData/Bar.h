#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#define max_train_num 550000
#define max_test_num 50000
#define dis_num 5

//float distr[dis_num-1] = { -4, -2.5f, -1.5f, -1, -0.5f, 0, 0.5f, 1, 1.5f, 2.5f, 4 };
using namespace std;
enum BarType
{
	Error=0,
	M1=1,
	M5=5,
	M15=15,
	M30=30,
	M60=60,
	//M120=120,
	Day=240,
	Week=1200,
	Month=4800,
};
class Bar
{
public:
	Bar();
	Bar(const Bar& other);
	BarType type;
	int year, month, day, hour, minute;
	float o, h, l, c, chg, amp, fq, bq;
	float m5, m10, m20, m30, m60, m120, m250;
	float vol, amt, turnover, qr;
	float dif, dea, macd, kdj_k, kdj_d, kdj_j, bol_top, bol_btm, bol_mid, psy, psyma, rsi1, rsi2, rsi3;
	float pe, pb;
	void SetYMD(string& str, char split);
	void SetVal(string str[]);
	int WriteData(fstream& f_img);
	int WriteLable(fstream& f_lbl, Bar* next, double dis[]);
};
class BarArr
{
public:
	BarArr(BarType t);
	~BarArr();
	vector<Bar*> bar_vec;
	BarType type;
};
class BarGroup
{
	string symbol;
	map<BarType, BarArr*> bar_arr_map;
public:
	BarGroup(string symbol);
	~BarGroup();
	void Load5min();
	void LoadDay();
	void Add5minData(BarArr* arr, string& line);
	void AddDayData(BarArr* arr, string& line);
	void AddDayDataIndex(BarArr* arr, string& line);
	void WriteData(fstream& f_train_img, fstream& f_train_lbl, fstream& f_test_img, fstream& f_test_lbl, int& train_cnt, int& test_cnt);
};