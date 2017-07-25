#include <iostream>
#include "Bar.h"
using namespace std;
std::vector<string> all_sym;
std::vector<string> all_sym_name;
double lbl_dis[13] = {0};
void LoadAllSymbol()
{
	char path[260 + 1] = "Data/symbol.csv";
	//sprintf(path, "Data/symbol.csv");
	fstream fin(path);
	if (fin.is_open())
	{
		string line;
		while (getline(fin, line))
		{
			istringstream f(line);
			string s[2];
			getline(f, s[0], ',');
			getline(f, s[1], ',');
			all_sym.push_back(s[0]);
			all_sym_name.push_back(s[1]);
		}
	}
	fin.close();
}
int main()
{
	LoadAllSymbol();
	int train_cnt = 0;
	int test_cnt = 0;
	unsigned int magic_num0 = 2051;
	unsigned int magic_num1 = 2049;
	fstream f_train_img("stk_train_img.bin", ios::binary | ios::out);
	fstream f_train_lbl("stk_train_lbl.bin", ios::binary | ios::out);
	fstream f_test_img("stk_test_img.bin", ios::binary | ios::out);
	fstream f_test_lbl("stk_test_lbl.bin", ios::binary | ios::out);
	f_train_img.write((char*)&magic_num0, sizeof(magic_num0));
	f_test_img.write((char*)&magic_num0, sizeof(magic_num0));
	f_train_lbl.write((char*)&magic_num1, sizeof(magic_num1));
	f_test_lbl.write((char*)&magic_num1, sizeof(magic_num1));
	f_train_img.write((char*)&train_cnt, sizeof(train_cnt));
	f_test_img.write((char*)&train_cnt, sizeof(train_cnt));
	f_train_lbl.write((char*)&train_cnt, sizeof(train_cnt));
	f_test_lbl.write((char*)&train_cnt, sizeof(train_cnt));
	for (int i = 0; i < all_sym.size(); i++)
	{
		BarGroup bg(all_sym[i]);// ("sh600000");
		//bg.Load5min();
		bg.LoadDay();
		bg.WriteData(f_train_img, f_train_lbl, f_test_img, f_test_lbl, train_cnt, test_cnt);
		if (train_cnt >= max_train_num && test_cnt >= max_test_num)
			break;
	}
	f_train_img.write((char*)&magic_num0, sizeof(magic_num0));
	f_test_img.write((char*)&magic_num0, sizeof(magic_num0));
	f_train_lbl.write((char*)&magic_num1, sizeof(magic_num1));
	f_test_lbl.write((char*)&magic_num1, sizeof(magic_num1));
	f_train_img.close();
	f_train_lbl.close();
	f_test_img.close();
	f_test_lbl.close();

	f_train_img.open("stk_train_img.bin", ios::binary | ios::out | ios::in);
	f_train_lbl.open("stk_train_lbl.bin", ios::binary | ios::out | ios::in);
	f_train_img.seekp(sizeof(int));
	f_train_img.write((char*)&train_cnt, sizeof(int));
	f_train_lbl.seekp(sizeof(int));
	f_train_lbl.write((char*)&train_cnt, sizeof(int));
	f_test_img.open("stk_test_img.bin", ios::binary | ios::out | ios::in);
	f_test_lbl.open("stk_test_lbl.bin", ios::binary | ios::out | ios::in);
	f_test_img.seekp(sizeof(int));
	f_test_img.write((char*)&test_cnt, sizeof(int));
	f_test_lbl.seekp(sizeof(int));
	f_test_lbl.write((char*)&test_cnt, sizeof(int));
	f_train_img.close();
	f_train_lbl.close();
	f_test_img.close();
	f_test_lbl.close();
	printf("done, train_cnt=%d, test_cnt=%d\n", train_cnt, test_cnt);
	for (int i = 0; i < 12; i++)
		printf("lbl_dis[%d] : %f\n", i, lbl_dis[i] / lbl_dis[12]);
	system("pause");
	return 0;
}