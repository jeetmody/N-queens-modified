//
//  main.cpp
//  AI Assignment
//
//  Created by Jeet Mody on 8/30/17.
//  Copyright © 2017 Jeet Mody. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <ctime>
#include <fstream>
#include <utility>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_set>
#include <iterator>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <iterator>

using namespace std;

int n;
double dif;
int totalB = 0;
int tim = 1;
time_t start;
unordered_map<int,set <int> > blocks;
unordered_map<int, int> cStore;
pair<int,int> old;
pair<int,int> newer;
void findBlocks(int x, char **table, int n, set<int>& possible);
fstream ofile("output.txt", ios_base::out);

class bfsLevel
{
public:
    set<int> possible;
    int lPlaced;
    vector<pair<int,int> > place;
    
};



void fileprint(set<pair<int,int>> place, char** table)
{
    ofile << "OK"<<endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n;j++)
        {
            if(place.find(make_pair(i,j)) != place.end())
                ofile << "1";
            else if (table[i][j] == '2')
            {
                ofile<<"2";
            }
            else
                ofile << "0";
        }
        ofile<<endl;
    }
}



bool solveIt(int n, set<int> possible, int l, set<pair<int,int> > place, char **table, int* tree);


bool bfsHelper(vector <bfsLevel *> q, vector <vector <int>> bfsPreprocess, int l, char** table, int curLevel,set<int> possible, int curLiz, int *tree, int nd, set<int> possibled, int ld, set<pair<int,int> > placed, char **tabled, int* treed )
{
    int c = 0;
    
    for (auto level : bfsPreprocess)
    {
        if(c < curLevel)
        {
            c++;
            continue;
        }
        
        vector <bfsLevel *> q2;
        for (auto i : level)
        {
            if(n - i/n + tree[i/n] < curLiz )
            {
                return false;
            }
            for (auto qu : q)
            {
                
                if(qu->possible.find(i) != qu->possible.end())
                {
                    
                    bfsLevel *temp = new bfsLevel;
                    temp->lPlaced = qu->lPlaced+1;
                    temp->place = qu->place;
                    temp->place.push_back((make_pair(i/n,i%n)));
                    
                    if(temp->lPlaced == l)
                    {
                        auto place = temp->place;
                        
                        ofile << "OK"<<endl;
                        for (int i = 0; i < n; i++)
                        {
                            for (int j = 0; j < n;j++)
                            {
                                if ( find(place.begin(), place.end(), make_pair(i,j)) != place.end() )
                                    ofile << "1";
                                else if (table[i][j] == '2')
                                {
                                    ofile<<"2";
                                }
                                else
                                    ofile << "0";
                            }
                            ofile<<endl;
                        }
                        
                        return true;
                    }
                    
                    
                    if((time(nullptr) - start ) > 240)
                    {
                        if(!solveIt(nd, possibled, ld, placed, tabled, treed))
                        {
                            ofile<<"FAIL";
                        }
                        return true;
                    }
                    
                    
                    auto poss = qu->possible;
                    findBlocks(i, table, n, poss);
                    temp->possible = poss;
                    q2.push_back(temp);
                   
                    
                }
            }
        }
        
        
        c++;
        if(bfsHelper(q2,bfsPreprocess,l,table,c,possible,curLiz-1, tree, nd, possibled, ld, placed, tabled, treed))
            return true;
        
        int po = 0;
        bfsLevel *x = NULL;
        for(auto del : q2)
        {
            if(po!=0)
            {
                delete x;
            }
            x = del;
        }
        q2.clear();
        
    }
    
    return false;
    
}


bool bfs (vector <vector <int>> bfsPreprocess, int l, char** table,set<int> possible, int* tree, int nd, set<int> possibled, int ld, set<pair<int,int> > placed, char **tabled, int* treed )
{
    vector <bfsLevel *> q;
    int c = 0;
    for (auto level : bfsPreprocess)
    {
        for (auto i : level)
        {
            
            bfsLevel *temp = new bfsLevel;
            temp->place.push_back(make_pair(i/n, i%n));
            temp->lPlaced = 1;
            auto poss = possible;
            poss.erase(i);
            findBlocks(i, table, n, poss);
            temp->possible = poss;
            q.push_back(temp);
            
        }
        c++;
        if(bfsHelper(q,bfsPreprocess,l,table,c,possible,l-1, tree, nd, possibled, ld, placed, tabled, treed ))
            return true;
    }
    
    if(!solveIt(nd, possibled, ld, placed, tabled, treed))
    {
        ofile<<"FAIL";
    }
    
    return false;
}


int conf (pair<int,int> a, pair<int,int> b, char** table)
{
    int x1 = a.first, y1 = a.second;
    int x2 = b.first, y2 = b.second;
    if(x1 == x2)
    {
        int i = y1>y2?y2:y1;
        int j = y1>y2?y1:y2;
        for (; i<=j ;i++)
        {
            if(table[x1][i] == '2')
                return 0;
        }
        return 1;
    }
    if(y1 == y2)
    {
        int i = x1>x2?x2:x1;
        int j = x1>x2?x1:x2;
        for (; i<=j ;i++)
        {
            if(table[i][y1] == '2')
                return 0;
        }
        return 1;
    }
    
    int i = x1>x2?x2:x1;
    int k = x1>x2?x1:x2;
    int j = x1>x2?y2:y1;
    int l = x1>x2?y1:y2;
    
    if(i-k == j-l)
    {
        for (; i<=k && j<=l ;i++,j++)
        {
            if(table[i][j] == '2')
                return 0;
        }
        return 1;
    }
    if(i-k == l-j)
    {
        for (; i>=k && j<=l ;i--,j++)
        {
            if(table[i][j] == '2')
                return 0;
        }
        return 1;
    }
    return 0;
}

int conflicts(char **table,set<pair<int,int> >& place)
{
    int conflicts = 0;
    set <int> ret;
    for (auto i : place)
    {
        int x = i.first*n + i.second;
        for (auto j : place)
        {
            if (i == j)
                continue;
            int y = j.first*n + j.second;
            int f = x>y?x:y;
            int s = x>y?y:x;
            int t = f*n*n+s;
            if(cStore.find(t)==cStore.end())
            {
                cStore[t] = conf(i,j, table);
            }
            
            conflicts += cStore[t];
        }
    }
    return conflicts/2;
}

int conflict2(char **table,set<pair<int,int> >& place, pair<int,int> old, pair<int,int> ne, int c1)
{
    for (auto i : place)
    {
        int x = i.first*n + i.second;
        int y = old.first*n + old.second;
        int z = ne.first*n + ne.second;
        if(ne!=i)
        {
            int f = x>y?x:y;
            int s = x>y?y:x;
            int t = f*n*n+s;
            if(cStore.find(t)==cStore.end())
            {
                cStore[t] = conf(i,old, table);
            }
            c1-=cStore[t];
        }
        if(ne!=i)
        {
            int f = x>z?x:z;
            int s = x>z?z:x;
            int t = f*n*n+s;
            if(cStore.find(t)==cStore.end())
            {
                cStore[t] = conf(i,ne, table);
            }
            c1+=cStore[t];
        }
        
        
    }
    
    return c1;
}

bool randomize(char **table, int n, int l, int tree[], set<pair<int,int> >& place, map <int, vector<int> > rows)
{
    int x;
    int c = 0;
    for (auto i : rows)
    {
        if(l == 0)
            break;
        if(i.second.size()!=0)
            x = rand() % i.second.size();
        else
            continue;
        
        
        place.insert(make_pair(c,i.second[x]));
        rows[i.first].erase(rows[i.first].begin()+x);
        c++;
        l--;
        
    }
    int tree2[n];
    for (int v = 0; v < n; v++)
    {
        tree2[v] = tree[v];
    }
    if(l!=0)
    {
        c = 0;
        int f = 0;
        auto i = rows.begin();
        while(l!=0)
        {
            if(tree2[c]>0 && i->second.size()!=0)
            {
                tree2[c] = tree2[c]-1;
                x = rand() % i->second.size();
                place.insert(make_pair(c,i->second[x]));
                rows[i->first].erase(rows[i->first].begin()+x);
                f = 1;
                l--;
            }
            c++;
            i++;
            if(i==rows.end())
            {
                if(f == 0)
                {
                    ofile<<"FAIL";
                    return false;
                }
                i = rows.begin();
                c = 0;
                f = 0;
            }
        }
    }
    
    if(l == 0)
        return true;
    else
        ofile<<"FAIL";
    
    return false;
}

set<pair<int,int>> moveOne (set<pair<int,int> > place, char** table)
{
    
    int x = rand()%place.size();
    auto it = place.begin();
    advance(it, x);
    int f = it->first;
    int s = it->second;
    int b = 1;
    while (b<8)
    {
        int y = rand()%8;
        if(y == 0 || y == 9)
        {
            for(int g = f-1;g>=0;g--)
            {
                if(table[g][s] =='0' && place.find(make_pair(g,s)) == place.end())
                {
                    
                    old = *it;
                    newer = make_pair(g,s);
                    place.erase(*it);
                    place.insert(make_pair(g,s));
                    return place;
                }
            }
        }
        else if(y == 1 || y == 8)
        {
            for(int g = f+1;g<n;g++)
            {
                if(table[g][s] =='0' && place.find(make_pair(g,s)) == place.end())
                {
                    old = *it;
                    newer = make_pair(g,s);
                    place.erase(*it);
                    place.insert(make_pair(g,s));
                    return place;
                }
            }
        }
        else if(y == 2)
        {
            for (int g = s-1; g>=0; g--)
            {
                if(table[f][g] =='0' && place.find(make_pair(f,g)) == place.end())
                {
                    
                    old = *it;
                    newer = make_pair(f,g);
                    place.erase(*it);
                    place.insert(make_pair(f,g));
                    return place;
                }
            }
        }
        else if(y == 3)
        {
            for (int g = s+1; g<n; g++)
            {
                if(table[f][g] =='0' && place.find(make_pair(f,g)) == place.end())
                {
                    
                    old = *it;
                    newer = make_pair(f,g);
                    place.erase(*it);
                    place.insert(make_pair(f,g));
                    return place;
                }
            }
        }
        else if(y == 4)
        {
            for (int g = s-1, h = f-1; g>=0 && h>=0; g--,h--)
            {
                if(table[h][g] =='0' && place.find(make_pair(h,g)) == place.end())
                {
                    
                    old = *it;
                    newer = make_pair(h,g);
                    place.erase(*it);
                    place.insert(make_pair(h,g));
                    return place;
                }
            }
        }
        
        else if(y == 5)
        {
            for (int g = s+1, h = f+1; g<n && h<n; g++,h++)
            {
                if(table[h][g] =='0' && place.find(make_pair(h,g)) == place.end())
                {
                    
                    old = *it;
                    newer = make_pair(h,g);
                    place.erase(*it);
                    place.insert(make_pair(h,g));
                    return place;
                }
            }
        }
        
        else if(y == 6)
        {
            for (int g = s-1, h = f+1; g>=0 && h<n; g--,h++)
            {
                if(table[h][g] =='0' && place.find(make_pair(h,g)) == place.end())
                {
                    
                    old = *it;
                    newer = make_pair(h,g);
                    place.erase(*it);
                    place.insert(make_pair(h,g));
                    return place;
                }
            }
        }
        else if(y == 7)
        {
            for (int g = s+1, h = f-1; g<n && h>=0; g++,h--)
            {
                if(table[h][g] =='0' && place.find(make_pair(h,g)) == place.end())
                {
                    
                    old = *it;
                    newer = make_pair(h,g);
                    place.erase(*it);
                    place.insert(make_pair(h,g));
                    return place;
                }
            }
        }
        b++;
        
    }
    old.first = 0;
    old.second = 0;
    newer.first = 0;
    newer.second = 0;
    return place;
}


void sa(char **table, int n, int l, int *tree, set<pair<int,int> >& place, map <int, vector<int> > rows, int nd, set<int> possibled, int ld, set<pair<int,int> > placed, char **tabled, int *treed)
{
    
    int power = 1;
    while(1)
    {
        
        power++;
        int rate = 1;
        double delta = 0;
        place.clear();
        if(!randomize(table, n,  l, tree, place, rows))
        {
            return;
        }
        int c1 = conflicts(table, place);
        //fileprint(place,table);
        //cout<<c1;
        if(c1 == 0)
        {
            fileprint(place, table);
            return;
        }
        double t = 3/log(rate+10000);
        
        while((time(nullptr) - start)<20)
        {
            rate++;
            auto place2 = moveOne(place, table);
            if(old.first == newer.first && old.second == newer.second)
            {
                continue;
            }
            int c2 = conflict2(table, place2, old, newer,c1);
            
            delta = c1-c2;
            double random = (double)rand()/((double)RAND_MAX+1);
            double probability = exp((delta)/t);
            
            if(c2<c1)
            {
                place = place2;
                c1 = c2;
            }
            else if(random<=probability)
            {
                place = place2;
                c1 = c2;
            }
            
            t = 3/log(rate+10000);
            
            if(c1 == 0)
            {
                fileprint(place, table);
                return;
            }
        }
        
        if(!solveIt(nd, possibled, ld, placed, tabled, treed))
        {
            ofile<<"FAIL";
        }
        return;
        
    }
}

void findBlocks(int x, char **table, int n, set<int>& possible)
{
    
    set <int> ret;
    int i = x/n;
    int j = x%n;

    for (int k = i-1; k >= 0; k--)
    {
        if(table[k][j] == '2')
            break;
        else
        {
            ret.insert(k*n+j);
            if(possible.find(k*n+j) != possible.end())
            {
                possible.erase(k*n+j);
            }
        }
    }
    
    for (int k = i+1; k < n; k++)
    {
        if(table[k][j] == '2')
            break;
        else
        {
            ret.insert(k*n+j);
            if(possible.find(k*n+j) != possible.end())
            {
                possible.erase(k*n+j);
            }
        }
    }
    
    for (int k = j-1; k >= 0; k--)
    {
        if(table[i][k] == '2')
            break;
        else
        {
            ret.insert(i*n+k);
            if(possible.find(i*n+k) != possible.end())
            {
                possible.erase(i*n+k);
            }
        }
    }
    
    for (int k = j+1; k < n; k++)
    {
        if(table[i][k] == '2')
            break;
        else
        {
            ret.insert(i*n+k);
            if(possible.find(i*n+k) != possible.end())
            {
                possible.erase(i*n+k);
            }
        }
    }
    
    int k,l;
    
    for (k = i-1, l = j-1; k >= 0 && l >= 0; k--,l--)
    {
        if(table[k][l] == '2')
            break;
        else
        {
            ret.insert(k*n+l);
            if(possible.find(k*n+l) != possible.end())
            {
                possible.erase(k*n+l);
            }
        }
    }
    
    for (k = i+1, l = j+1; k < n && l < n; k++,l++)
    {
        if(table[k][l] == '2')
            break;
        else
        {
            ret.insert(k*n+l);
            if(possible.find(k*n+l) != possible.end())
            {
                possible.erase(k*n+l);
            }
        }
    }
    
    for (k = i+1, l = j-1; k < n && l >= 0; k++,l--)
    {
        if(table[k][l] == '2')
            break;
        else
        {
            ret.insert(k*n+l);
            if(possible.find(k*n+l) != possible.end())
            {
                possible.erase(k*n+l);
            }
        }
    }
    
    for (k = i-1, l = j+1; k >=0 && l < n; k--,l++)
    {
        if(table[k][l] == '2')
            break;
        else
        {
            ret.insert(k*n+l);
            if(possible.find(k*n+l) != possible.end())
            {
                possible.erase(k*n+l);
            }
        }
    }
    int temp = i*n + j;
    blocks.insert(make_pair(temp,ret));
}


bool solveIt(int n, set<int> possible, int l, set<pair<int,int> > place, char **table, int* tree)
{
    if (l == 0)
    {
        fileprint(place, table);
        return true;
    }
    
    else
    {
        if(possible.size() == 0)
        {
            return false;
        }
        int c = 0;
        auto tt = possible.begin();
        for ( auto poss = possible.begin(); poss != possible.end(); poss++ )
        {
            totalB++;
            int first = *poss/n;
            int second = *poss%n;
            int num = *poss;
            if(n - first + tree[first] < l)
            {
                return false;
            }
            
            if(c!=0)
            {
                
                possible.erase(*tt);
            }
            
            c++;
            tt = poss;
            place.insert(make_pair(first,second));
            int x = first;
            int y = second;
            int temp = n*x + y;
            set<int> newPossible = possible;
            newPossible.erase(*poss);
            int dc = 0;
            if((blocks.find(temp) == blocks.end()))
            {
                findBlocks(num, table, n,newPossible);
                dc = 1;
            }
            
            
            if(dc == 0)
            {
                
                if(newPossible.size()>blocks.find(temp)->second.size())
                {
                    for (auto np : blocks.find(temp)->second)
                    {
                        if (newPossible.find(np) != newPossible.end())
                        {
                            newPossible.erase(np);
                        }
                    }
                }
                
                else
                {
                    int h;
                    int g=0;
                    for (auto np : newPossible)
                    {
                        
                        if(g==1)
                        {
                            newPossible.erase(h);
                        }
                        
                        g = 0;
                        
                        if (blocks.find(temp)->second.find(np) != blocks.find(temp)->second.end())
                        {
                            g = 1;
                            h = np;
                        }
                        
                        
                    }
                    if(g==1)
                    {
                        newPossible.erase(h);
                    }
                }
            }
            
            if(solveIt(n, newPossible, l-1, place, table,tree))
            {
                return true;
            }
            
            place.erase(make_pair(first,second));
        }
    }
    return false;
}

int main()
{
    
    srand(time(0));
    fstream myfile("input.txt", ios_base::in);
    string algo;
    ofile<<"FAIL";
    ofile.clear();
    ofile.seekg(0, ios::beg);
    start = time(nullptr);
    int l;
    map <int, vector<int> > rows;
    myfile >> algo;
    dif = 0.01;
    myfile >> n;
    myfile >> l;
    char** table = new char*[n];
    set<int> cells;
    set<pair<int,int> > placed;
    int totTrees = 0;
    int treeArr[n];
    vector <vector <int>> bfsPreprocess;
    int saTree[n];
    for (int i = 0; i<n; i++)
    {
        saTree[i] = 0;
    }
    vector <int> bfsTemp;
    for (int i = 0; i < n; i++)
    {
        table[i] = new char[n];
        vector <int> temp;
        for (int j = 0; j < n;j++)
        {
            
            myfile>>table[i][j];
            if(table[i][j] == '0')
            {
                bfsTemp.push_back(i*n+j);
                cells.insert(i*n+j);
                temp.push_back(j);
            }
            else
            {
                saTree[i]++;
                
                if(bfsTemp.size()>0)
                {
                    
                    bfsPreprocess.push_back(bfsTemp);
                    bfsTemp.clear();
                }
                
            }
            treeArr[i] = 0;
            
        }
        if(bfsTemp.size()>0)
        {
            
            bfsPreprocess.push_back(bfsTemp);
            bfsTemp.clear();
        }
        rows[i] = temp;
    }
    
    for (int i = n-1; i >= 0; i--)
    {
        for (int j = 0; j < n;j++)
        {
            if(table[i][j] == '2')
            {
                totTrees++;
                treeArr[i] = totTrees;
            }
        }
        treeArr[i] = totTrees;
    }
    if(algo == "BFS")
        bfs(bfsPreprocess, l, table, cells, treeArr,n, cells, l,placed, table,treeArr);
    else if(algo == "DFS")
    {
        
        if(!solveIt(n, cells, l,placed, table,treeArr))
        {
            ofile<<"FAIL";
        }
    }
    
    else
        sa(table, n, l, saTree,placed,rows, n, cells, l,placed, table,treeArr);
    
    
    return 0;
}

