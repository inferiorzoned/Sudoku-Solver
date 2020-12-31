#include <bits/stdc++.h>
#include "util_funcs.h"
using namespace std;

long long int nodeCount;
long long int backtrackCount;

bool backtracking(int grid[N][N], bool isValid, int typeOfBacktrack, int varHeuristicType, int valHeuristicType)
{
	// if (nodeCount % 20000 == 0)
	// 	cout << nodeCount << endl;
	nodeCount++;
	int row, col;

	if (!isValid)
	{
		backtrackCount++;
		return false;
	}
	pair<int, int> pos = nextUnassignedPos(grid, varHeuristicType);
	row = pos.first;
	col = pos.second;
	if (row == -1 || col == -1)
		return true;

	sortDomain(grid, row, col, valHeuristicType);
	for (int i = 0; i < domains[row][col].size(); i++)
	{
		int num = domains[row][col][i];
		vector<int> oldDomains[N][N];
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < N; k++)
			{
				oldDomains[j][k] = domains[j][k];
			}
		}
		bool valid = isSafeToAssign(grid, row, col, num, typeOfBacktrack);
		// if (valid)
		// {
		// 	if (backtracking(grid, valid, typeOfBacktrack, varHeuristicType, valHeuristicType))
		// 		return true;
		// }
		if (backtracking(grid, valid, typeOfBacktrack, varHeuristicType, valHeuristicType))
			return true;
		grid[row][col] = UNASSIGNED;
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k < N; k++)
			{
				domains[j][k] = oldDomains[j][k];
			}
		}
	}
	backtrackCount++;
	return false;
}

int main()
{
	enum typeofBacktracks bt = SIMPLE;
	enum variableHeuristics varheu = DOMDDEG;
	enum valueHeuristics valheu = MOSTAPPEARANCE;
	int grid[N][N] = {{3, 0, 6, 5, 0, 8, 4, 0, 0},
					  {5, 2, 0, 0, 0, 0, 0, 0, 0},
					  {0, 8, 7, 0, 0, 0, 0, 3, 1},
					  {0, 0, 3, 0, 1, 0, 0, 8, 0},
					  {9, 0, 0, 8, 6, 3, 0, 0, 5},
					  {0, 5, 0, 0, 9, 0, 6, 0, 0},
					  {1, 3, 0, 0, 0, 0, 2, 5, 0},
					  {0, 0, 0, 0, 0, 0, 0, 7, 4},
					  {0, 0, 5, 2, 0, 6, 3, 0, 0}};

	cout << "input grid:" << endl << endl;
	printGrid(grid);

	for (int r = 0; r < N; r++)
		for (int c = 0; c < N; c++)
			degs[r][c] = calcDeg(grid, r, c);

	for (int r = 0; r < N; r++)
		for (int c = 0; c < N; c++)
			domains[r][c] = calcDomain(grid, r, c);

	if (backtracking(grid, true, bt, varheu, valheu) == true) {
		cout << "output grid: " << endl<< endl;
		printGrid(grid);
	}
	else
		cout << "No solution exists" << endl;

	// cout << bt << " " << varheu << " " << valheu << endl;
	// cout << nodeCount << " " << backtrackCount << endl;
	if (isConflicting(grid))
		cout << "output grid is conflicting" << endl;
	if (!isComplete(grid))
		cout << "output grid is incomplete" << endl;
	return 0;
}
