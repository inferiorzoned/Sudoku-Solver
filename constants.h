#include <bits/stdc++.h>
using namespace std;

#define UNASSIGNED 0
#define N 9

enum typeofBacktracks
{
	SIMPLE,
	FORWARDCHECK
};
ostream &operator<<(ostream &out, const typeofBacktracks value)
{
	const char *s = 0;
#define PROCESS_VAL(p) \
	case (p):          \
		s = #p;        \
		break;
	switch (value)
	{
		PROCESS_VAL(SIMPLE);
		PROCESS_VAL(FORWARDCHECK);
	}
#undef PROCESS_VAL
	return out << s;
}

enum variableHeuristics
{
	FIRSTAVAILABLE,
	SDF,
	DOMDEG,
	DOMDDEG,
	BRELAZ
};
ostream &operator<<(ostream &out, const variableHeuristics value)
{
	const char *s = 0;
#define PROCESS_VAL(p) \
	case (p):          \
		s = #p;        \
		break;
	switch (value)
	{
		PROCESS_VAL(FIRSTAVAILABLE);
		PROCESS_VAL(SDF);
		PROCESS_VAL(DOMDEG);
		PROCESS_VAL(DOMDDEG);
		PROCESS_VAL(BRELAZ);
	}
#undef PROCESS_VAL
	return out << s;
}

enum valueHeuristics
{
	RANDOM,
	MOSTAPPEARANCE,
	DOMAINREDUCTION
};
ostream &operator<<(ostream &out, const valueHeuristics value)
{
	const char *s = 0;
#define PROCESS_VAL(p) \
	case (p):          \
		s = #p;        \
		break;
	switch (value)
	{
		PROCESS_VAL(RANDOM);
		PROCESS_VAL(MOSTAPPEARANCE);
		PROCESS_VAL(DOMAINREDUCTION);
	}
#undef PROCESS_VAL
	return out << s;
}