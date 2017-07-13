#include "Bar.h"
Bar::Bar() :o(0), h(0), l(0), c(0), vol(0), amt(0), v5(0), v10(0), dif(0), dea(0), macd(0)
, m5(0), m10(0), m20(0), m60(0), m120(0), m250(0), type(BarType::Error){}
Bar::Bar(const Bar& b) : o(b.o), h(b.h), l(b.l), c(b.c), vol(b.vol), amt(b.amt), v5(b.v5), v10(b.v10), dif(b.dif), dea(b.dea), macd(b.macd)
, m5(b.m5), m10(b.m10), m20(b.m20), m60(b.m60), m120(b.m120), m250(b.m250), type(b.type){}