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
#include "stdafx.h"
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


	const int Area = 4;
	int MaxSupCap[Area] = { 10000,20000,20000,INFINITY };//
	int MinSupCap[Area] = { 1000,2000,2000,INFINITY };//Ideas for randomising to discuss with Paul
	int MaxHoldCost[Area] = { 100,200,200,0 };
	int MinHoldCost[Area] = { 50,100,100,0 };//Hold Cost is 0 at max area because abbatoir
	const int TotalMaxLocs = 10;
	int MaxNumLocs[Area] = { 3,2,4,1 };

	srand(time(0));

	vector <int> BreedCost;
	for (int i = 0; i < MaxNumLocs[0]; i++) {
		BreedCost.push_back(i);
		BreedCost[i] = 1 + (rand() % 99);
		cout << BreedCost[i] << endl;
	}


	int MaxAge[Area] = { 2,3,5,5 };
	int Amax = 24;

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
	for (int i = 0; i < Area; i++) {
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
	for (int i = 0; i < Area - 1; i++) {
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

	const int Horizon = 3;
	double Demand[Horizon];
	int counter = 0;

	//From here create transcost matrices
	//Generate size for [t][N[s]]

	vector<TimeLoc> TimeLoc;
	for (int t = 0; t < Horizon; t++) {
		for (int s = 0; s < Area; s++) {
			for (int i = 0; i < MaxNumLocs[s]; i++) {
				TimeLoc.emplace_back(t, s, i);

			}//i
		}//s
	}//t

	cout << "TimeLocSize" << TimeLoc.size() << endl;

	for (int counter = 0; counter < TimeLoc.size(); counter++) {
		cout << TimeLoc[counter].Time << " " << TimeLoc[counter].Area << " " << TimeLoc[counter].Current << endl;
	}

	vector <AllNodes> NodeInfo;
	for (int s = 0; s < Area; s++) {
		for (int i = 0; i < MaxNumLocs[s]; i++) {
			for (int j = 0; j < MaxNumLocs[s + 1]; j++) {

				if (s == 3) {
					j = 0;
				}
				NodeInfo.emplace_back(s, i, j);
			}//j
		}//i
	}//s

	//create vector of candidate nodes at different times
	vector<NodeTime> All_Nodes;
	for (int t = 0; t < Horizon; t++) {
		for (int s = 0; s < Area; s++) {
			for (int i = 0; i < MaxNumLocs[s]; i++) {
				for (int j = 0; j < MaxNumLocs[s + 1]; j++) {

					if (s == 3) {
						j = 0;
					}
					//p1 = new Nodes(s,i,j);
					All_Nodes.emplace_back(t, s, i, j);//Need CS[t][n]

				}//j
			}//i
		}//s
	}//t

	cout << All_Nodes.size() << endl;

	//create vector of candidate ages at different timelocs
	vector<AgeTime> Age_Time;

	int g = 0;
	for (int t = 0; t < Horizon; t++) {
		for (int s = 0; s < Area; s++) {
			for (int i = 0; i < MaxNumLocs[s]; i++) {
				for (int g = 0; g < MaxAge[s]; g++) {
					Age_Time.emplace_back(t, s, i, g);
				}
			}
		}
	}

	vector <int> TransCost;
	for (int i = 0; i < NodeInfo.size(); i++) {
		TransCost.push_back(i);
		TransCost[i] = 1 + (rand() % 99);
	}

	cout << "Age Size " << Age_Time.size() << endl;

	cout << "t" << " " << "s" << " " << "i" << " " << "g" << endl;
	for (int counter = 0; counter < Age_Time.size(); counter++) {
		cout << Age_Time[counter].Time << " " << Age_Time[counter].Area << " " << Age_Time[counter].Current << " " << Age_Time[counter].Age << endl;
	}
	cout << "t" << " " << "s" << " " << "i" << " " << "j" << endl;
	for (int counter = 0; counter < All_Nodes.size(); counter++) {
		cout << All_Nodes[counter].Time << " " << All_Nodes[counter].Area << " " << All_Nodes[counter].Current << " " << All_Nodes[counter].Next << endl;
	}//Check that the valid combinations are being outputted

	for (int i = 0; i < Horizon; i++) {
		Demand[i] = 1 + (rand() % 50000);
		cout << "The demand for time period " << i << " is " << Demand[i] << " cattle" << endl;
	}

	vector<vector<double>> Cattle_Supply;
	for (int n = 1; n <= All_Nodes.size(); n++) {
		vector<double> Cattle_Sup_Sub;
		for (int t = 0; t < Horizon; t++) {
			Cattle_Sup_Sub.push_back(n);
		};
		Cattle_Supply.push_back(Cattle_Sup_Sub);
	};

	cout << "Size " << Cattle_Supply.size() << endl;
	double BreedMax = 1500;

//BIGM INITIALISATION

	double BigMNC = LocationSupCap[0][0];
	double BigMSC = 0;
	vector<int> LocSumVec;
	for (int s = 0; s < Area; s++) {
		for (int i = 0; i < MaxNumLocs[s]; i++) {
			LocSumVec.push_back(s);
			LocSumVec[s] += LocationSupCap[s][i];
			cout << LocationSupCap[s][i] << endl;
			if (LocationSupCap[s][i] > BigMNC) {
				BigMNC = LocationSupCap[s][i];
			}
			if (LocSumVec[s] > BigMSC) {
				BigMSC = LocSumVec[s];
			}
		}
	}


	cout << "Max Number is " << BigMNC << endl;
	cout << "Max Supply is " << BigMSC << endl;

	
	IloEnv ilenv;
	IloModel ilmod(ilenv);

	// DVARS //


	typedef IloArray <IloNumVarArray> NumVarMatrix;
	typedef IloArray <NumVarMatrix> NumVarMatrix3D;
	typedef IloArray <NumVarMatrix3D> NumVarMatrix4D;

	//Boolean Tracker Setup
	NumVarMatrix3D TransToTrack(ilenv, Horizon);
	NumVarMatrix3D TransFromTrack(ilenv, Horizon);
	NumVarMatrix3D InvTracker(ilenv, Horizon);
	NumVarMatrix3D Cat_Inv(ilenv, Horizon);
	NumVarMatrix4D AgeTracker(ilenv, Horizon);
	NumVarMatrix Cat_Sup(ilenv, Horizon);

	for (int t = 0; t < Horizon; t++) {
		TransToTrack[t] = NumVarMatrix(ilenv, Area);
		TransFromTrack[t] = NumVarMatrix(ilenv, Area);
		InvTracker[t] = NumVarMatrix(ilenv, Area);
		Cat_Inv[t] = NumVarMatrix(ilenv, Area);
		AgeTracker[t] = NumVarMatrix3D(ilenv, Area);

		for (int area = 0; area < Area; area++) {
			TransToTrack[t][area] = IloNumVarArray(ilenv, MaxNumLocs[area]);
			TransFromTrack[t][area] = IloNumVarArray(ilenv, MaxNumLocs[area]);
			InvTracker[t][area] = IloNumVarArray(ilenv, MaxNumLocs[area]);
			Cat_Inv[t][area] = IloNumVarArray(ilenv, MaxNumLocs[area]);
			AgeTracker[t][area] = NumVarMatrix(ilenv, MaxNumLocs[area]);

			for (int current = 0; current < MaxNumLocs[area]; current++) {
				TransToTrack[t][area][current] = IloNumVar(ilenv, 0, 1, ILOINT);
				TransFromTrack[t][area][current] = IloNumVar(ilenv, 0, 1, ILOINT);
				InvTracker[t][area][current] = IloNumVar(ilenv, 0, 1, ILOINT);
				Cat_Inv[t][area][current] = IloNumVar(ilenv, 0, IloInfinity, ILOINT);
				AgeTracker[t][area][current] = IloNumVarArray(ilenv, Amax);

				for (int age = 0; age < Amax; age++) {
					AgeTracker[t][area][current][age] = IloNumVar(ilenv, 0, 1, ILOINT);
				}
			}
		}
	}
	
	for (int t = 0; t < Horizon; t++) {
		Cat_Sup[t] = IloNumVarArray(ilenv, NodeInfo.size());
		for (int counter = 0; counter < NodeInfo.size(); counter++) {
			Cat_Sup[t][counter] = IloNumVar(ilenv, 0, IloInfinity, ILOINT);
		}
	}

	
	// OBJECTIVE FUNCTION //
	IloExpr ilobj(ilenv);

	IloExpr Total_Sum(ilenv);
for (int t = 0; t < Horizon; t++) {

	for (int area = 0; area < Area; area++) {

		IloExpr Cattle_Store_Sum(ilenv);

		for (int current = 0; current < MaxNumLocs[area]; current++) {

		IloExpr Cattle_Trans_Sum(ilenv);
		IloExpr Cattle_Breed_Sum(ilenv);

			Cattle_Store_Sum += LocationHoldCost[area][current]*Cat_Inv[t][area][current];

				for (counter = 0; counter < NodeInfo.size(); counter++) {

				Cattle_Trans_Sum += TransCost[counter]*Cat_Sup[t][counter];

					if(NodeInfo[counter].Area == 0 && NodeInfo[counter].Current == current && t > 0 ) {

					Cattle_Breed_Sum += BreedCost[current]*(Cat_Inv[t][0][current]-Cat_Inv[t-1][0][current]+Cat_Sup[t][counter]);

					}//if
					Total_Sum += Cattle_Trans_Sum + Cattle_Breed_Sum + Cattle_Store_Sum;
				}//counter
				Cattle_Trans_Sum.end();
				Cattle_Breed_Sum.end();
			}//i
		Cattle_Store_Sum.end();
		}//s
	int current = current;
	}//t

	ilmod.add(IloMinimize(ilenv,Total_Sum));

	// CONSTRAINTS //

	// Cattle supply Trans From

	for (int t = 0; t < Horizon; t++) {
		for (int area = 0; area < Area; area++) {
			for (int current = 0; current < MaxNumLocs[area]; current++) {
			IloExpr Cattle_Out_Sum(ilenv);
					for (double counter = 0; counter < NodeInfo.size(); counter++) {
						if (NodeInfo[counter].Area == area && NodeInfo[counter].Current == current) {
						Cattle_Out_Sum += Cat_Sup[t][counter];
						}//if
					}//counter
			ilmod.add(Cattle_Out_Sum <= BigMSC*TransFromTrack[t][area][current]);
			ilmod.add(Cattle_Out_Sum - TransFromTrack[t][area][current] >= 0);
			Cattle_Out_Sum.end();
			}//current
		}//area
	}//t

	//Demand and sending capacity constraints

	for (int t = 0; t < Horizon; t++) {
		for (int area = 0; area < Area; area++) {
			for (int current = 0; current < MaxNumLocs[area]; current++) {
			IloExpr Demand_Sum(ilenv);
				if (area == 0) {
					ilmod.add(Cat_Inv[t][0][current] <= BreedMax);
				}
				for (double counter = 0; counter < NodeInfo.size(); counter++) {
					if (NodeInfo[counter].Area == Area-1 && NodeInfo[counter].Current == current) {
						Demand_Sum += Cat_Sup[t][counter];
					} else if (NodeInfo[counter].Area != Area && NodeInfo[counter].Current == current && t > 0) {
					ilmod.add (Cat_Sup[t][counter] <= Cat_Inv[t-1][area][current]);
					}
					ilmod.add(Demand_Sum == Demand[t]);
					Demand_Sum.end();
				}//counter
			}//current
		}//area
	}//t


	//CattleSupplyTransFrom
	for (int t = 0; t < Horizon; t++) {
		for (int area = 0; area < Area; area++) {
			for (int current = 0; current < MaxNumLocs[area]; current ++) {
				for (int next = 0; next < MaxNumLocs[area+1]; next++) {
					if (area == 3) {
						next = 0;
					}
					IloExpr Cattle_In_Sum(ilenv);
						for (double counter = 0; counter < NodeInfo.size(); counter++) {
							if (NodeInfo[counter].Area == area-1 && NodeInfo[counter].Current == next) {
							Cattle_In_Sum += Cat_Sup[t][counter];
							}//if
						}//counter
					ilmod.add(Cattle_In_Sum <= BigMSC*TransToTrack[t][area][current]);
					ilmod.add(Cattle_In_Sum - TransToTrack[t][area][current] >= 0);
					Cattle_In_Sum.end();
					}//next
				}//current
			}//area
		}//t
	
	//Location Specific Constraints
//Check these values
for (int t = 0; t < Horizon; t++) {
	for (int area = 0; area < Area; area++) {
		for (int current = 0; current < MaxNumLocs[area]; current ++) {
			if (area != Area) {
			ilmod.add(Cat_Inv[t][area][current] <= LocationSupCap[area][current]);
			} else if (t == 0) {
			ilmod.add(Cat_Inv[area][current][0] == 0);
			} else if (t > 0) {
			ilmod.add(Cat_Inv[t][area][current] <= InvTracker[t][area][current]*BigMNC);
			ilmod.add(Cat_Inv[t][area][current] - InvTracker[t][area][current] >= 0);
			}
		}
	}
}

//Age Constraints

for (int t = 0; t < Horizon; t++) {
	for (int area = 0; area < Area; area++) {
		for (int current = 0; current < MaxNumLocs[area]; current ++) {
			IloExpr AgeSum(ilenv);
			for (int g = 0; g < Amax; g++) {
				AgeSum += AgeTracker[t][area][current][g];
					if (area != Area && t != Horizon) {
					ilmod.add(AgeTracker[t][area][current][MaxAge[area]] == TransFromTrack[t][area][current]);
						if(g != 0) {
							ilmod.add(AgeTracker[t][area][current][g]-InvTracker[area][current][t]+1 >= AgeTracker[t-1][area][current][g-1]);
						}
					} else if (area == 0) {
						ilmod.add(AgeTracker[t][area][current][MaxAge[area-1]+1] == TransToTrack[t][area][current]);
					}//if
				}//g
				ilmod.add(AgeSum == InvTracker[t][area][current]);
				AgeSum.end();
			}//i
		}//s
	}//t

for (int t = 0; t < Horizon; t++) {
	for (int area = 0; area < Area; area++) {
		for (int current = 0; current < MaxNumLocs[area]; current++) {
			IloExpr Cattle_Out_Sum(ilenv);
			IloExpr Cattle_In_Sum(ilenv);
				for (double counter = 0; counter < NodeInfo.size(); counter++) {
					if (NodeInfo[counter].Area == area && NodeInfo[counter].Current == current) { //If AllNodes[counter].Area = s && AllNodes[counter].current = i perform
						Cattle_Out_Sum += Cat_Sup[t][counter];
					} else if (NodeInfo[counter].Area == area - 1 && NodeInfo[counter].Next == current) {
						Cattle_In_Sum += Cat_Sup[t][counter];
					}//if
				}//counter
				ilmod.add(Cat_Inv[t][area][current] == Cat_Inv[t - 1][area][current] + Cattle_In_Sum - Cattle_Out_Sum);
				Cattle_In_Sum.end();
				Cattle_Out_Sum.end();//should change these to different vars just in case
		}//current
	}//area
}//time

//Don't need next forloop - IF the "next" location at the counter is equal to the current location, we'll be moving stuff to that location
//Therefore, we don't need to loop through all "next" locations. Create model similar to CPLEX to gauge if correct. Also check to make sure summations
//and loops are correct before doing so. Include outputs and other stuff. 

ilenv.end();

int i = 1;//breakpoint checker 
}
