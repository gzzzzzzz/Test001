#include <vector>
using namespace std;
enum BarType
{
	Error=0,
	M1=1,
	M5=5,
	M10=10,
	M15=15,
	M30=30,
	M60=60,
	M120=120,
	Day=240,
	Week=1200,
	Month=4800,
};
class Bar
{
	Bar();
	Bar(const Bar& other);
	BarType type;
	float o, h, l, c;
	float vol, amt, v5, v10;
	float dif, dea, macd;
	float m5, m10, m20, m60, m120, m250;
};
class BarArr
{
	vector<Bar*> arr;
	BarType type;
};
class BarGroup
{
	BarType base_type;
	vector<BarArr*> arr;
};