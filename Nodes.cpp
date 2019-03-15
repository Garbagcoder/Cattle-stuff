#include "Nodes.h"
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

Nodes::Nodes() {};

Nodes::Nodes(int s, int i, int j) {
	Area = s;
	Current = i;
	Next = j;
};

