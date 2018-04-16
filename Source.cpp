#include "Header.h"
#include <iostream>
#include <queue>
#include <vector>
#include <string.h>
#include <cmath>
#include <algorithm>
using namespace std;
int W, n;
int choose = 0;


vector<bool> resultData;
struct thing {
	int v;
	int w;
	float v_w;
};

struct nodeKS {
	int level;
	int profit;
	int weight;
	float bound;
	vector<bool> data;
};

bool compareThing(const thing& a, const thing& b) {
	return a.v_w > b.v_w;
}

class CompareKS
{
	bool reverse;
public:
	CompareKS(const bool& revparam = false)
	{
		reverse = revparam;
	}
	bool operator() (const nodeKS& lhs, const nodeKS&rhs) const
	{
		if (reverse) return (lhs.bound < rhs.bound);
		else return (lhs.bound>rhs.bound);
	}
};

vector<thing> arr;

float Bound0(const nodeKS& u){
	if (u.weight >= W) return 0;
	float p = u.profit;
	int j = u.level + 1;
	if( j < n)
		p += (W*1.0 - u.weight*1.0) * arr[j].v_w;
	return ceil(p);
}
float Bound1(const nodeKS& u) {
	if (u.weight >= W) return 0;
	float p = u.profit;
	int j = u.level + 1;
	float accWeight = u.weight;
	while (j < n && accWeight + arr[j].w <= W) {
		accWeight += arr[j].w;
		p += arr[j].v;
		j++;
	}
	if (j < n) {
		p += (float(W) - accWeight)* (arr[j].v*1.0) / (arr[j].w*1.0);
	}
	return ceil(p);
}


int Knapsack_BB1() {
	typedef priority_queue <nodeKS, vector<nodeKS>, CompareKS> my_pq;
	my_pq pq(true);

	nodeKS u,v;
	u.level = -1;
	u.profit = u.weight = 0;
	u.bound = Bound1(u);
	u.data.resize(n, false);
	pq.push(u);

	int best = 0;
	
	while (!pq.empty()) {
		u = pq.top();
		pq.pop();
		if (u.bound > best) {
			v.data = u.data;
			v.data[u.level+1] = true;
			v.level = u.level + 1;
			v.weight = u.weight + arr[v.level].w;
			v.profit = u.profit + arr[v.level].v;
			if (v.weight <= W && v.profit > best) {
				best = v.profit;
				resultData = v.data;
			}
			v.bound = Bound1(v);
			if (v.bound > best)
				pq.push(v);

			v.weight = u.weight;
			v.profit = u.profit; 
			v.data[u.level+1] = false;
			v.bound = Bound1(v);
			if (v.bound > best)
				pq.push(v);
		}
	}
	return best;
}

int Knapsack_BB0() {
	typedef priority_queue <nodeKS, vector<nodeKS>, CompareKS> my_pq;
	my_pq pq(true);

	nodeKS u, v;
	u.level = -1;
	u.profit = u.weight = 0;
	u.bound = Bound0(u);
	u.data.resize(n, false);
	pq.push(u);

	int best = 0;

	while (!pq.empty()) {
		u = pq.top();
		pq.pop();
		if (u.bound > best) {
			v.data = u.data;
			v.data[u.level + 1] = true;
			v.level = u.level + 1;
			v.weight = u.weight + arr[v.level].w;
			v.profit = u.profit + arr[v.level].v;
			if (v.weight <= W && v.profit > best) {
				best = v.profit;
				resultData = v.data;
			}
			v.bound = Bound0(v);
			if (v.bound > best)
				pq.push(v);

			v.weight = u.weight;
			v.profit = u.profit;
			v.data[u.level + 1] = false;
			v.bound = Bound0(v);
			if (v.bound > best)
				pq.push(v);
		}
	}
	return best;
}


int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	cin >> W >> n;
	int v, w;
	for (int i = 0; i < n; i++) {
		cin >> v >> w;
		thing temp = { v, w, (float)(v*1.0) / (w*1.0) };
		arr.push_back(temp);
	}

	sort(arr.begin(), arr.end(), compareThing);
	int ret = Knapsack_BB0();
	for (int i = 0; i < n; i++) {
		if (resultData[i]) {
			cout << arr[i].v << " " << arr[i].w << endl;
		}
	}
	return 0;
}