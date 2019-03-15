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
#include "Nodes.h"

#define IL_STD
using namespace std;
ILOSTLBEGIN
//define max number of locations at each area


//TODO: Create random number of locations for each cattle area
int main() {

	const int M = 4;
	int MaxSupCap[M] = { 10000,20000,20000,INFINITY };//
	int MinSupCap[M] = { 1000,2000,2000,INFINITY };//Ideas for randomising to discuss with Paul
	int MaxHoldCost[M] = { 100,200,200,0 };
	int MinHoldCost[M] = { 50,100,100,0 };//Hold Cost is 0 at max area because abbatoir
	const int TotalMaxLocs = 10;
	int MaxNumLocs[M] = { 3,2,4,1 };
	srand(time(0));
	///////////////////////////////////////////////below code randomises max number of locations for a given area//////////////////////////////////////////////
	//for (int i = 0; i <= M - 2; i++) {
	//	MaxNumLocs[i] = 1 + (rand() % TotalMaxLocs);
	//	MaxNumLocs[3] = 1;
	//}
	//Visual of Code
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	vector<vector<int>> LocationSupCap;
	vector<vector<int>> LocationHoldCost;
	vector<vector<int>> AreaProperty;
	for (int i = 0; i < M; i++) {
		vector<int> Bad;
		vector<int> ElementHoldCost;
		vector<int> LocationProperty;
		cout << "Area" << " " << i << " " << "Has" << " " << MaxNumLocs[i] << " " << "Locations" << endl;
		for (int j = 1; j <= MaxNumLocs[i]; j++) {
			Bad.push_back(i);
			ElementHoldCost.push_back(i);
			LocationProperty.push_back(i);
		};
		LocationSupCap.push_back(Bad);
		LocationHoldCost.push_back(ElementHoldCost);
		AreaProperty.push_back(LocationProperty);
	};
	for (int i = 0; i < LocationSupCap.size(); i++) { //The size of the vectors will be the same due to parameters having same indices; therefore only two loops required
		for (int j = 0; j < LocationSupCap[i].size(); j++) {
			LocationSupCap[i][j] = (rand() % (MaxSupCap[i] - MinSupCap[i] + 1)) + MinSupCap[i];
			LocationSupCap[3][0] = INFINITY;
			LocationHoldCost[i][j] = (rand() % (MaxHoldCost[i] - MinHoldCost[i] + 1)) + MinHoldCost[i];
			AreaProperty[i][j] = j;
			cout << "The Capacity Of Area " << i << " Location " << j << " Is " << LocationSupCap[i][j] << " Cattle" << endl;
			cout << "The Cost To Hold One Cattle Per Period At Area " << i << " Location " << j << " is " << LocationHoldCost[i][j] << "AUD" << endl;
			cout << AreaProperty[i][j] << endl;
		}
		cout << endl;
	}//TODO: create Vector with all different combinations rather than using a tuple, Breedcost, demand, transcost, target age, 
	vector<tuple<int, int, int>> XavierPurcell;
	//vector<vector<vector<int>>> PatrickOGrady;
	int Counter = 0;
	for (int i = 0; i < M - 1; i++) {
		for (int z = 0; z < MaxNumLocs[i]; z++) {
			for (int j = 0; j < MaxNumLocs[i + 1]; j++) {
				tuple <int, int, int> Nodes;
				XavierPurcell.push_back(make_tuple(i, z, j));
				Nodes = make_tuple(i, z, j);
				XavierPurcell[Counter] = Nodes;
				//cout << get<0>(XavierPurcell[Counter]) << get<1>(XavierPurcell[Counter]) << get<2>(XavierPurcell[Counter]) << endl;
				Counter = Counter + 1; //if this is not the case I can make a vector super easily anyway
				//PatrickOGrady[i][z][j] = (rand() % 100);
			}
			//Hold.push_back(Pleb);
		}
		//PatrickOGrady.push_back(Hold);
	}
	cout << endl;
	vector<vector<vector<int>>> PatrickOGrady;
	for (int i = 0; i < M - 1; i++) {
		vector<vector<int>> Hold;
		for (int z = 0; z < MaxNumLocs[i]; z++) {
			vector<int> Pleb;
			for (int j = 0; j < MaxNumLocs[i + 1]; j++) {
				Pleb.push_back(i);
			}
			Hold.push_back(Pleb);
		}
		PatrickOGrady.push_back(Hold);
	}
	vector<Nodes> All_Nodes;
	Nodes *p1 = NULL; //???
	vector<vector<Nodes*>> N_s;
	int counter = 0;
	for (int s = 0; s < PatrickOGrady.size(); s++) {
		for (int i = 0; i < PatrickOGrady[s].size(); i++) {
			for (int j = 0; j < PatrickOGrady[s][i].size(); j++) {
				//p1 = new Nodes(s,i,j);
				All_Nodes.push_back(Nodes(s, i, j));
				All_Nodes[counter] = Nodes(s, i, j);
				cout << All_Nodes[counter].Area << All_Nodes[counter].Current << All_Nodes[counter].Next << endl; //correct
				counter++;

				//TODO; have pointer to row of objects in the class, perform pushback(All_Nodes) to add to stack for accessing and pointer usage. 
				//cout << "The Cost To Transport Cattle From Area " << s << " From Location " << i << " To Location " << j << " Is " << PatrickOGrady[s][i][j] << " Dollarydoos" << endl;

			}
		}
	}

	int n = 0; //Consideration for formatting of accessing N[s]. 
	vector<double> Plushie;
	for (int s = 0; s < M; s++) {
		for (int i = 0; i < MaxNumLocs[s]; i++) {
			Plushie.push_back(n);
			Plushie[n] = LocationSupCap[s][i] * LocationHoldCost[s][i];
			cout << Plushie[n] << endl;
			n++;

		}

	}


	const int Horizon = 38;
	double Demand[Horizon];

	for (int i = 0; i < Horizon; i++) {
		Demand[i] = 1 + (rand() % 50000);
		cout << "The demand for time period " << i << " is " << Demand[i] << " cattle" << endl;
	}

	vector<vector<double>> Cattle_Supply; //create this vector of cattle supply??

	for (int t = 0; t < Horizon; t++) {
		vector<double> Cattle_Sup_Sub;
		for (int n = 0; n < All_Nodes.size(); n++) {
			Cattle_Sup_Sub.push_back(t);
		};
		Cattle_Supply.push_back(Cattle_Sup_Sub);
	};

	cout << "Size " << Cattle_Supply.size() << endl;


	IloEnv ilenv;
	IloModel ilmod(ilenv);
	IloNumVarArray SupCap(ilenv, Cattle_Supply.size(), 0.0, IloInfinity, ILOFLOAT);
	for (int t = 0; t < Horizon; t++) {
		for (int s = 0; s < M - 1; s++) {
			for (int counter = 0; counter < All_Nodes.size(); counter++) {
				for (int current = 0; current < MaxNumLocs[s - 1]; current++) {
					for (int next = 0; next < MaxNumLocs[s]; next++) {
						if (s = All_Nodes[counter].Area & All_Nodes[counter].Current = current) //howdo?
							Cattle_Supply[counter][t] = 1


					}
				}
			}
		}
	}
	ilenv.end();
		
}


	
	
		





//TODO: Start Randomly Assigning Values to Area A, Location I:  For A [tab] For i [tab] random assign between x-y, cout. Separate into Diff Classes for visualisation as well. 

//Create Classes of Variables For Ease - ONLY FOR ARRAYS AND SCALARS 
// Create Sets As Well 
