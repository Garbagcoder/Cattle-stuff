#ifndef NODES_H
#define NODES_H
#include <ilcplex/ilocplex.h>
#include <math.h>
#include <conio.h>
#include <fstream>
#include <algorithm>
#include <functional>
#include <queue>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <ostream>

using namespace std;

class Nodes {
public:
	int Area;
	int Current;
	int Next;
	
		Nodes(int s, int i, int j);
		Nodes();
		
};

#endif // !1Nodes
