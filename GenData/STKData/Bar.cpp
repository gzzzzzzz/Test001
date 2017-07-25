#include "Bar.h"
Bar::Bar() :type(BarType::Error), year(0), month(0), day(0)
, o(0), h(0), l(0), c(0), vol(0), amt(0), turnover(0)
, m5(0), m10(0), m20(0), m60(0), m120(0), m250(0)
, dif(0), dea(0), macd(0), bol_top(0), bol_btm(0), bol_mid(0), pb(0), pe(0){}
Bar::Bar(const Bar& b) : o(b.o), h(b.h), l(b.l), c(b.c), vol(b.vol), amt(b.amt), dif(b.dif), dea(b.dea), macd(b.macd)
, m5(b.m5), m10(b.m10), m20(b.m20), m60(b.m60), m120(b.m120), m250(b.m250), type(b.type),year(b.year),month(b.month),day(b.day){}
void Bar::SetYMD(string& str, char split)
{
	istringstream f(str);
	string sy, sm, sd;
	getline(f, sy, split);
	getline(f, sm, split);
	getline(f, sd, split);

	sscanf(sy.c_str(), "%d", &year);
	sscanf(sm.c_str(), "%d", &month);
	sscanf(sd.c_str(), "%d", &day);
}
void Bar::SetVal(string s[])
{
	SetYMD(s[2], '-');

	c = atof(s[9].c_str());
	float inv_c = 1.0f / c;
	o = (atof(s[6].c_str()) * inv_c - 1) * 100.f;
	h = (atof(s[7].c_str()) * inv_c - 1) * 100.f;
	l = (atof(s[8].c_str()) * inv_c - 1) * 100.f;
	bq = atof(s[10].c_str());//后复权
	fq = atof(s[11].c_str());//前复权

	chg = atof(s[12].c_str()) * 100.0f;//涨跌
	vol = atof(s[13].c_str());//手
	amt = atof(s[14].c_str());//量
	turnover = atof(s[15].c_str()) * 100.0f;//换手率
	amp = atof(s[46].c_str()) * 100.0f;//振幅
	qr = atof(s[47].c_str());//量比

	pe = atof(s[20].c_str());
	pb = atof(s[23].c_str());

	float inv_fq = 1.0f / fq;
	m5 = atof(s[24].c_str()) * inv_fq;
	m10 = atof(s[25].c_str()) * inv_fq;
	m20 = atof(s[26].c_str()) * inv_fq;
	m30 = atof(s[27].c_str()) * inv_fq;
	m60 = atof(s[28].c_str()) * inv_fq;

	dif = atof(s[30].c_str());
	dea = atof(s[31].c_str());
	macd = atof(s[32].c_str());

	kdj_k = atof(s[34].c_str()) * 0.01f;
	kdj_d = atof(s[35].c_str()) * 0.01f;
	kdj_j = atof(s[36].c_str()) * 0.01f;

	bol_mid = atof(s[38].c_str()) * inv_fq - 1;
	bol_top = atof(s[39].c_str()) * inv_fq - 1;
	bol_btm = atof(s[40].c_str()) * inv_fq - 1;

	psy = atof(s[41].c_str()) * 0.01f;
	psyma = atof(s[42].c_str()) * 0.01f;
	rsi1 = atof(s[43].c_str()) * 0.01f;
	rsi2 = atof(s[44].c_str()) * 0.01f;
	rsi3 = atof(s[45].c_str()) * 0.01f;
}
int Bar::WriteData(fstream& f_img)
{
	int sof = sizeof(float);
	int ret = 0;
	f_img.write((char*)&chg, sof); ret++;
	f_img.write((char*)&turnover, sof); ret++;
	f_img.write((char*)&amp, sof); ret++;
	f_img.write((char*)&qr, sof); ret++;
	f_img.write((char*)&pe, sof); ret++;
	f_img.write((char*)&pb, sof); ret++;

	f_img.write((char*)&o, sof); ret++;
	f_img.write((char*)&h, sof); ret++;
	f_img.write((char*)&l, sof); ret++;
	f_img.write((char*)&m5, sof); ret++;
	f_img.write((char*)&m10, sof); ret++;
	f_img.write((char*)&m20, sof); ret++;
	f_img.write((char*)&m30, sof); ret++;
	f_img.write((char*)&m60, sof); ret++;

	f_img.write((char*)&dif, sof); ret++;
	f_img.write((char*)&dea, sof); ret++;
	f_img.write((char*)&macd, sof); ret++;
	f_img.write((char*)&kdj_k, sof); ret++;
	f_img.write((char*)&kdj_d, sof); ret++;
	f_img.write((char*)&kdj_j, sof); ret++;
	f_img.write((char*)&bol_mid, sof); ret++;
	f_img.write((char*)&bol_top, sof); ret++;
	f_img.write((char*)&bol_btm, sof); ret++;
	f_img.write((char*)&psy, sof); ret++;
	f_img.write((char*)&psyma, sof); ret++;
	f_img.write((char*)&rsi1, sof); ret++;
	f_img.write((char*)&rsi2, sof); ret++;
	f_img.write((char*)&rsi3, sof); ret++;
	return ret;
}
extern double lbl_dis[12];
int Bar::WriteLable(fstream& f_lbl, Bar* next)
{
	const int dnum = 11;
	float distr[dnum] = {-4, -2.5f, -1.5f, -1, -0.5f, 0, 0.5f, 1, 1.5f, 2.5f, 4};
	char lbl = 0;
	for (int i = dnum - 1; i >= 0; i--)
	{
		if (next->chg > distr[i])
		{
			lbl = i + 1;
			break;
		}
	}
	f_lbl.write(&lbl, sizeof(lbl));
	lbl_dis[lbl]++;
	lbl_dis[dnum + 1]++;
	return lbl;
}
//===================================================================//
BarArr::BarArr(BarType t) : type(t){}
BarArr::~BarArr()
{
	for (auto c : bar_vec)
		delete c;
}
//===================================================================//
BarGroup::BarGroup(string s) : symbol(s)
{
	bar_arr_map[M1] = new BarArr(M1);
	bar_arr_map[M5] = new BarArr(M5);
	bar_arr_map[M15] = new BarArr(M15);
	bar_arr_map[M30] = new BarArr(M30);
	bar_arr_map[M60] = new BarArr(M60);
	bar_arr_map[M60] = new BarArr(M60);
	bar_arr_map[Day] = new BarArr(Day);
	bar_arr_map[Week] = new BarArr(Week);
	bar_arr_map[Month] = new BarArr(Month);
}
BarGroup::~BarGroup()
{
	for (auto& c : bar_arr_map)
		delete c.second;
}
void BarGroup::Load5min()
{
	BarArr* bar_5min = bar_arr_map[M5];
	if (bar_5min == nullptr)
		return;

	for (int y = 1999; y < 2020; y++)
	{
		char path[260 + 1] = { 0 };
		sprintf(path, "Data/5min/Stk_5F_%d/%s.csv", y, symbol.c_str());
		fstream fin(path);
		if (fin.is_open())
		{
			string line;
			while (getline(fin, line))//无标题栏
				Add5minData(bar_5min, line);
		}
		fin.close();
	}
}
void BarGroup::LoadDay()
{
	BarArr* bar_day = bar_arr_map[Day];
	if (bar_day == nullptr)
		return;

	char path[260 + 1] = { 0 };
	sprintf(path, "Data/day/%s.csv", symbol.c_str());
	fstream fin(path);
	if (fin.is_open())
	{
		string line;
		getline(fin, line);//标题栏
		//cout << symbol<< " : " << line << endl;
		while (getline(fin, line))
			AddDayData(bar_day, line);
	}
	else
	{
		memset(path, 0, sizeof(path));
		sprintf(path, "Data/day/index/%s.csv", symbol.c_str());
		fin.open(path);
		if (fin.is_open())
		{
			string line;
			getline(fin, line);//标题栏
			//cout << symbol << " : " << line << endl;
			while (getline(fin, line))
				AddDayDataIndex(bar_day, line);
		}
		else
			printf("not find '%s.csv'\n", symbol.c_str());
	}
	fin.close();
	//bar_day->SortByTime();
	struct bar_cmp
	{
		static bool cmp_time(Bar* b0, Bar* b1)
		{
			return ((b0->year << 9) | (b0->month << 5) | b0->day)
				< ((b1->year << 9) | (b1->month << 5) | b1->day);
		}
	};
	std::sort(bar_day->bar_vec.begin(), bar_day->bar_vec.end(), bar_cmp::cmp_time);
}
void BarGroup::Add5minData(BarArr* arr, string& line)
{
	istringstream f(line);
	string s;
	while (getline(f, s, ','))
	{
		//cout << s << endl;
	}
}
void BarGroup::AddDayData(BarArr* arr, string& line)
{
	const int col_num = 48;
	istringstream f(line);//47column
	string s[col_num];
	int i = 0;
	while (getline(f, s[i++], ','))
		;// cout << s[i++] << endl;
	if ((i - 1) != col_num)
		cout << arr->type << " col num(" << (i - 1) << ") != " << col_num << endl;

	Bar* new_bar = new Bar();
	arr->bar_vec.push_back(new_bar);
	new_bar->SetVal(s);
	
}
void BarGroup::AddDayDataIndex(BarArr* arr, string& line)
{
	istringstream f(line);//9column
	string s;
	while (getline(f, s, ','))
	{
		cout << s << endl;
	}
}

void BarGroup::WriteData(fstream& f_train_img, fstream& f_train_lbl, fstream& f_test_img, fstream& f_test_lbl, int& train_cnt, int& test_cnt)
{
	BarArr* day = bar_arr_map[Day];
	if (day == nullptr || day->bar_vec.size() < 50) return;
	Bar* first_bar = day->bar_vec[0];
	Bar* last_bar = day->bar_vec[day->bar_vec.size() - 1];

	int total_cnt = day->bar_vec.size() - 50;
	int train_start = 44;
	int train_end = train_start + total_cnt * 0.9f;
	int test_start = train_end + 1;
	int test_end = day->bar_vec.size() - 3;
	printf("%s write day data[%d], train[%d,%d], test[%d,%d]. [%d-%02d-%02d->%d-%02d-%02d]\n"
		, symbol.c_str(), day->bar_vec.size(), train_start, train_end, test_start, test_end
		, first_bar->year, first_bar->month, first_bar->day, last_bar->year, last_bar->month, last_bar->day);
	for (int i = train_start; i <= train_end; i++)
	{
		if (train_cnt >= max_train_num)
			break;
		int tnum = day->bar_vec[i]->WriteData(f_train_img);
		day->bar_vec[i - 1]->WriteData(f_train_img);
		day->bar_vec[i - 2]->WriteData(f_train_img);
		day->bar_vec[i - 3]->WriteData(f_train_img);
		day->bar_vec[i - 4]->WriteData(f_train_img);
		int lbl_v = day->bar_vec[i]->WriteLable(f_train_lbl, day->bar_vec[i + 1]);

		train_cnt++;
		if (train_cnt  == max_train_num)
		{
			Bar* bb = day->bar_vec[i];
			printf("last train img : chg=%f, turn=%f, amp=%f, qr=%f, pe=%f, pb=%f\n lbl=%d\n",
				bb->chg, bb->turnover, bb->amp, bb->qr, bb->pe, bb->pb, lbl_v);
		}
	}
	for (int i = test_start; i <= test_end; i++)
	{
		if (test_cnt >= max_test_num)
			break;
		day->bar_vec[i]->WriteData(f_test_img);
		day->bar_vec[i - 1]->WriteData(f_test_img);
		day->bar_vec[i - 2]->WriteData(f_test_img);
		day->bar_vec[i - 3]->WriteData(f_test_img);
		day->bar_vec[i - 4]->WriteData(f_test_img);
		int lbl_v = day->bar_vec[i]->WriteLable(f_test_lbl, day->bar_vec[i + 1]);
		test_cnt++;
		if (test_cnt == max_test_num)
		{
			Bar* bb = day->bar_vec[i];
			printf("last test img : chg=%f, turn=%f, amp=%f, qr=%f, pe=%f, pb=%f\n lbl=%d\n",
				bb->chg, bb->turnover, bb->amp, bb->qr, bb->pe, bb->pb, lbl_v);
		}
	}
}