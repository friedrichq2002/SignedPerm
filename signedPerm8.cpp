#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <functional>
#include <algorithm>
#include <numeric>

using namespace std;

void visit_print(vector<int> perm, int val, string change_type) {
	// print a signed permutation
	for (int i : perm) {
		if (i > 0)
			printf("%c%d", '+', i);
		else
			printf("%d", i);
	}
	if (change_type == "sl")
		printf("%s%d%s", " Swap ", val, " left");
	else if (change_type == "sr")
		printf("%s%d%s", " Swap ", val, " right");
	else if (change_type == "t1")
		printf("%s%d", " Flip ", val);
	else if (change_type == "t2l")
		printf("%s%d%s", " Twist ", val, " left");
	else if (change_type == "t2r")
		printf("%s%d%s", " Twist ", val, " right");
	else
		printf("%s", " Last permutation");
	printf("\n");
}

void signed_perm_8(int n, function<void(vector<int>, int, string)> visit) {
	vector<int> cur_perm(n);
	vector<int> inverse(n + 1);
	iota(cur_perm.begin(), cur_perm.end(), 1);
	iota(inverse.begin(), inverse.end(), -1);
	vector<int> bases; // bases = [n,n-1,...,2,1,2,2,...,2,2]
	for (int i = n; i > 0; i--) bases.push_back(i);
	for (int i = 0; i < n; i++) bases.push_back(2);
	vector<int> word(bases.size(), 0);
	vector<int> dir(bases.size(), 1);
	vector<int> focus(bases.size() + 1);
	iota(focus.begin(), focus.end(), 0);
	while (focus[0] < bases.size()) {
		int index = focus[0];
		focus[0] = 0;
		word[index] += dir[index];
		int change = dir[index] * (index + 1);
		if (word[index] == 0 || word[index] == bases[index] - 1) {
			dir[index] = -dir[index];
			int extra = (index < bases.size()-1 && bases[index+1] == 1 ?1:0);
			focus[index] = focus[index + extra + 1];
			focus[index + extra + 1] = index + extra + 1;
		}
		int val = -1; int change_type = -1;
		if (abs(change) > n) { val = 2*n+1 - abs(change); change_type = 0; }
		else { val = n+1 - abs(change); change_type = (change > 0 ?1:2);}
		int pos = inverse[abs(val)];
		switch (change_type){
		case 0: {
			visit(cur_perm, val, "t1"); // flip sign of value
			cur_perm[pos] *= -1;
			break;}
		case 1: {   
			visit(cur_perm, val, "t2l"); // twist value to the left
			int other = cur_perm[pos - 1];
			swap(cur_perm[pos - 1], cur_perm[pos]);
			cur_perm[pos] *= -1;
			cur_perm[pos - 1] *= -1;
			inverse[abs(val)] = pos - 1;
			inverse[abs(other)] = pos;
			break;}
		case 2: {
			visit(cur_perm, val, "t2r"); // twist value to the right
			int other = cur_perm[pos + 1];
			swap(cur_perm[pos + 1], cur_perm[pos]);
			cur_perm[pos] *= -1;
			cur_perm[pos + 1] *= -1;
			inverse[abs(val)] = pos + 1;
			inverse[abs(other)] = pos;
			break;}
		}
	}
	visit(cur_perm, 0, "o"); // the last permutation has no change
}

int main()
{
	int n;
	scanf("%d", &n);
	signed_perm_8(n, visit_print);
	return 0;
}