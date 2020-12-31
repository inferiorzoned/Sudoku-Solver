#include <bits/stdc++.h>
#include "constants.h"
using namespace std;

int degs[100][100];
vector<int> domains[100][100];

pair<int, int> nextUnassignedPos(int grid[N][N], int type);
pair<int, int> firstUnassignedLocation(int grid[N][N]);
vector<pair<int, int>> sdfMinCalc(int grid[N][N]);
pair<int, int> ddegMaxCalc(int grid[N][N]);
pair<int, int> domdegCalc(int grid[N][N]);
pair<int, int> domDdegCalc(int grid[N][N]);
pair<int, int> brelazCalc(int grid[N][N]);

void sortDomain(int grid[N][N], int row, int col, int type);
vector<int> calcDomain(int grid[N][N], int row, int col);
int calcDeg(int grid[N][N], int row, int col);

bool isSafeToAssign(int grid[N][N], int row, int col, int num, int typeOfBacktrack);
bool fcAssign(int grid[N][N], int row, int col, int num);
bool macAssign(int grid[N][N], int row, int col, int num);
bool btAssign(int grid[N][N], int row, int col, int num);

bool isSafe(int grid[N][N], int row, int col, int num);
bool UsedInRow(int grid[N][N], int row, int num);
bool UsedInCol(int grid[N][N], int col, int num);

bool isConflicting(int grid[N][N]);
bool isComplete(int grid[N][N]);
void printGrid(int grid[N][N]);

bool UsedInRow(int grid[N][N], int row, int num)
{
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}

bool UsedInCol(int grid[N][N], int col, int num)
{
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}

bool isSafe(int grid[N][N], int row, int col, int num)
{
    return !UsedInRow(grid, row, num) && !UsedInCol(grid, col, num) &&
           grid[row][col] == UNASSIGNED;
}

bool isSafeToAssign(int grid[N][N], int row, int col, int num, int typeOfBacktrack)
{
    if (typeOfBacktrack == SIMPLE)
        return btAssign(grid, row, col, num);
    else if (typeOfBacktrack == FORWARDCHECK)
        return fcAssign(grid, row, col, num);
    else
        return false;
}

bool fcAssign(int grid[N][N], int row, int col, int num)
{
    bool safetoAssign = isSafe(grid, row, col, num);
    // bool safetoAssign = true;
    grid[row][col] = num;
    domains[row][col].erase(remove(domains[row][col].begin(), domains[row][col].end(), num), domains[row][col].end());
    // explore the row
    for (int c = 0; c < N; c++)
    {
        if (grid[row][c] == UNASSIGNED)
        {
            domains[row][c].erase(remove(domains[row][c].begin(), domains[row][c].end(), num), domains[row][c].end());
            if (domains[row][c].size() == 0)
                safetoAssign = false;
        }
    }
    // explore the column
    for (int r = 0; r < N; r++)
    {
        if (grid[r][col] == UNASSIGNED)
        {
            domains[r][col].erase(remove(domains[r][col].begin(), domains[r][col].end(), num), domains[r][col].end());
            if (domains[r][col].size() == 0)
                safetoAssign = false;
        }
    }
    return safetoAssign;
}

bool btAssign(int grid[N][N], int row, int col, int num)
{
    bool safetoAssign = isSafe(grid, row, col, num);
    grid[row][col] = num;
    domains[row][col].erase(remove(domains[row][col].begin(), domains[row][col].end(), num), domains[row][col].end());
    return safetoAssign;
}

void sortDomain(int grid[N][N], int row, int col, int type)
{
    if (type == RANDOM)
        return;
    else if (type == MOSTAPPEARANCE)
    {
        sort(domains[row][col].begin(), domains[row][col].end(),
             [grid](const auto &lhs, const auto &rhs) {
                 int countd1, countd2;
                 countd1 = 0;
                 countd2 = 0;
                 for (int row = 0; row < N; row++)
                 {
                     for (int col = 0; col < N; col++)
                     {
                         if (grid[row][col] == lhs)
                             countd1++;
                         else if (grid[row][col] == rhs)
                             countd2++;
                     }
                 }
                 return countd1 > countd2;
             });
        // cout << "here";
        return;
    }
    else if (type == DOMAINREDUCTION)
    {
        sort(domains[row][col].begin(), domains[row][col].end(),
             [grid, row, col](const auto &lhs, const auto &rhs) {
                 int countd1, countd2;
                 countd1 = 0;
                 countd2 = 0;
                 // explore the row
                 for (int c = 0; c < N; c++)
                 {
                     if (count(domains[row][c].begin(), domains[row][c].end(),
                               lhs) > 0)
                         countd1++;
                     if (count(domains[row][c].begin(), domains[row][c].end(),
                               rhs) > 0)
                         countd2++;
                 }
                 // explore the column
                 for (int r = 0; r < N; r++)
                 {
                     if (count(domains[r][col].begin(), domains[r][col].end(),
                               lhs) > 0)
                         countd1++;
                     if (count(domains[r][col].begin(), domains[r][col].end(),
                               rhs) > 0)
                         countd2++;
                 }
                 return countd1 > countd2;
             });
        // cout << "here";
        return;
    }
}

pair<int, int> nextUnassignedPos(int grid[N][N], int type)
{
    pair<int, int> pos(-1, -1);

    if (type == FIRSTAVAILABLE)
        pos = firstUnassignedLocation(grid);
    else if (type == SDF)
        pos = sdfMinCalc(grid).back();
    // if (type == MAXDDEG)
    // 	pos = ddegMaxCalc(grid);
    else if (type == DOMDEG)
        pos = domdegCalc(grid);
    else if (type == DOMDDEG)
        pos = domDdegCalc(grid);
    else if (type == BRELAZ)
        pos = brelazCalc(grid);
    return pos;
}

pair<int, int> firstUnassignedLocation(int grid[N][N])
{
    pair<int, int> pos(-1, -1);
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            if (grid[row][col] == UNASSIGNED)
                pos = make_pair(row, col);
    return pos;
}

// pair<int, int> sdfMinCalc(int grid[N][N])
vector<pair<int, int>> sdfMinCalc(int grid[N][N])
{
    vector<pair<int, int>> vec;
    vec.push_back(make_pair(-1, -1));
    pair<int, int> pos(-1, -1);
    int domainSize;
    int min = 100;
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            if (grid[row][col] == UNASSIGNED)
            {
                // domainSize = domains[row][col].size();
                domainSize = calcDomain(grid, row, col).size();
                if (domainSize == min)
                    vec.push_back(make_pair(row, col));
                if (domainSize < min)
                {
                    vec.clear();
                    min = domainSize;
                    // pos.first = row;
                    // pos.second = col;
                    vec.push_back(make_pair(row, col));
                }
            }
        }
    }
    return vec;
    // return pos;
}

pair<int, int> ddegMaxCalc(int grid[N][N])
{
    pair<int, int> pos(-1, -1);
    int degree;
    int max = -1;
    // int min = 100;
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            if (grid[row][col] == UNASSIGNED)
            {
                degree = calcDeg(grid, row, col);
                if (degree > max)
                {
                    max = degree;
                    pos.first = row;
                    pos.second = col;
                }
                // if (degree < min)
                // {
                // 	min = degree;
                // 	pos.first = row;
                // 	pos.second = col;
                // }
            }
        }
    }
    return pos;
}

pair<int, int> brelazCalc(int grid[N][N])
{
    // return maxBrelazDegree(grid, sdfMinCalc(grid));
    vector<pair<int, int>> sdfTie = sdfMinCalc(grid);
    pair<int, int> pos(-1, -1);
    int degree;
    int max = -1;
    // int min = 100;

    for (int i = 0; i < sdfTie.size(); i++)
    {
        degree = calcDeg(grid, sdfTie[i].first, sdfTie[i].second);
        if (degree > max)
        {
            max = degree;
            pos = sdfTie[i];
        }
    }
    return pos;
}

pair<int, int> domdegCalc(int grid[N][N])
{
    pair<int, int> pos(-1, -1);
    int domainSize;
    double domdegRatio;
    double min = 100.0;
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            if (grid[row][col] == UNASSIGNED)
            {
                domainSize = calcDomain(grid, row, col).size();
                domdegRatio = (1.0 * domainSize) / degs[row][col];
                if (domdegRatio < min)
                {
                    min = domdegRatio;
                    pos.first = row;
                    pos.second = col;
                }
            }
        }
    }
    return pos;
}

pair<int, int> domDdegCalc(int grid[N][N])
{
    pair<int, int> pos(-1, -1);
    int domainSize;
    int degree;
    double domdegRatio;
    double min = 100.0;
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            if (grid[row][col] == UNASSIGNED)
            {
                domainSize = calcDomain(grid, row, col).size();
                degree = calcDeg(grid, row, col);
                if (degree != 0)
                {
                    domdegRatio = (1.0 * domainSize) / degree;
                    if (domdegRatio < min)
                    {
                        min = domdegRatio;
                        pos.first = row;
                        pos.second = col;
                    }
                }
            }
        }
    }
    return pos;
}

int calcDeg(int grid[N][N], int row, int col)
{
    int degree = 0;
    // explore the row
    for (int c = 0; c < N; c++)
        if (grid[row][c] == UNASSIGNED)
            degree++;
    // explore the column
    for (int r = 0; r < N; r++)
        if (grid[r][col] == UNASSIGNED)
            degree++;
    if (grid[row][col] == UNASSIGNED)
        degree -= 1;
    return degree;
}

vector<int> calcDomain(int grid[N][N], int row, int col)
{
    int domainSize = N;
    set<int> s;
    vector<int> domains;
    if (grid[row][col] != UNASSIGNED)
    {
        domains.push_back(grid[row][col]);
        return domains;
    }
    // explore the row
    for (int c = 0; c < N; c++)
        if (grid[row][c] != UNASSIGNED)
            s.insert(grid[row][c]);
    // explore the column
    for (int r = 0; r < N; r++)
        if (grid[r][col] != UNASSIGNED)
            s.insert(grid[r][col]);
    // domainSize = domainSize - s.size();
    for (int i = 1; i <= N; i++)
        if (s.find(i) == s.end())
            domains.push_back(i);
    return domains;
}

bool isConflicting(int grid[N][N])
{
    /* Check if the board is
    conflicting at any element */
    for (int i = 1; i <= N; i++)
    {
        // check row-wise
        for (int row = 0; row < N; row++)
        {
            int count = 0;
            for (int col = 0; col < N; col++)
            {
                if (i == grid[row][col])
                    count++;
            }
            if (count > 1)
                return true;
        }
        // check column-wise
        for (int col = 0; col < N; col++)
        {
            int count = 0;
            for (int row = 0; row < N; row++)
            {
                if (i == grid[row][col])
                    count++;
            }
            if (count > 1)
                return true;
        }
    }
    return false;
}

bool isComplete(int grid[N][N])
{
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            if (grid[row][col] == 0)
                return false;
    return true;
}

void printGrid(int grid[N][N])
{
    for (int row = 0; row < N; row++)
    {
        if ((row) % int(sqrt(N)) == 0 && row != 0)
        {
            for (int i = 0; i < N; i++) cout << "--";
            cout << endl;
        }
        for (int col = 0; col < N; col++)
        {
            if((col) % int(sqrt(N)) == 0 && col != 0) cout << "|";
            // if(col!=N-1) cout << grid[row][col] << " ";
            // else cout << grid[row][col];
            cout << grid[row][col] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
