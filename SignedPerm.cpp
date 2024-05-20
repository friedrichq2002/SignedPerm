#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <functional>
#include <set>
#include <algorithm>
#include <numeric>

using namespace std;

vector<int> two_dec(int n){
	vector<int> ans(n, 2);
	for (int i = n; i > 0; i--) ans.push_back(i);
	return ans;
}

vector<int> dec_two(int n){
	vector<int> ans = {};
	for (int i = n; i > 0; i--) ans.push_back(i);
	for (int i = 0; i < n; i++) ans.push_back(2);
	return ans;
}

vector<int> two_dec_zigzag(int n){
	vector<int> ans = {};
	for (int i = n; i > 0; i--){
		ans.push_back(2);
		ans.push_back(i);
	}
	return ans;
}

vector<int> dec_two_zigzag(int n){
	vector<int> ans = {};
	for (int i = n; i > 0; i--){
		ans.push_back(i);
		ans.push_back(2);
	}
	return ans;
}

void visit(vector<int> perm, int val, string change_type){
	// print a signed permutation
	for (int i : perm){
		if (i > 0)
			printf("%c%d", '+', i); 
		else
			printf("%d", i);
	}
	if (change_type == "sl")
		printf("%s%d%s", " Swap ", val, " left");
	else if(change_type == "sr")
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

void loopless_signed_perm(int n, int base_type, 
	function<pair<int, int>(int)> algorithm){
	vector<int> cur_perm(n);
	vector<int> inverse(n+1);
	iota(cur_perm.begin(), cur_perm.end(), 1);
	iota(inverse.begin(), inverse.end(), -1);
	vector<int> bases;
	switch(base_type){
		case 0:
			bases = two_dec(n);
			break;
		case 1:
			bases = dec_two(n);
			break;
		case 2:
			bases = two_dec_zigzag(n);
			break;
		case 3:
			bases = dec_two_zigzag(n);
			break;
	}
	vector<int> word(bases.size(), 0);
	vector<int> dir(bases.size(), 1);
	vector<int> focus(bases.size() + 1);
	iota(focus.begin(), focus.end(), 0);
	int first = (bases[0] > 1 ? 0 : 1);
	while(focus[first] < bases.size()){
		int index = focus[first];
		focus[first] = first;
		word[index] += dir[index];
		int change = dir[index] * (index + 1);
		if (word[index] == 0 || word[index] == bases[index] - 1){
			dir[index] = -dir[index];
			int extra = (index < bases.size() - 1 && bases[index+1] == 1 ?1:0);
			focus[index] = focus[index + extra + 1];
			focus[index + extra + 1] = index + extra + 1;
		}
		pair<int, int> op = algorithm(change);
		int val = op.first;
		int pos = inverse[abs(val)];
		switch (op.second){
			case 0: {
				visit(cur_perm, val, "sl"); // swap value to the left
				int other = cur_perm[pos - 1];
				swap(cur_perm[pos - 1], cur_perm[pos]);
				inverse[abs(val)] = pos - 1;
				inverse[abs(other)] = pos;
				break;
			}
			case 1: {
				visit(cur_perm, val, "sr"); // swap value to the right
				int other = cur_perm[pos + 1];
				swap(cur_perm[pos + 1], cur_perm[pos]);
				inverse[abs(val)] = pos + 1;
				inverse[abs(other)] = pos;
				break;
			}
			case 2: {
				visit(cur_perm, cur_perm[pos], "t1"); // flip sign at index 
				pos = abs(val)-1;
				cur_perm[pos] *= -1;		
				break;
			}
			case 3: {
				visit(cur_perm, val, "t1"); // flip sign of value
				cur_perm[pos] *= -1;
				break;
			}
			case 4:  {
				visit(cur_perm, val, "t2l"); // twist value to the left
				int other = cur_perm[pos - 1];
				swap(cur_perm[pos - 1], cur_perm[pos]);
				cur_perm[pos] *= -1;
				cur_perm[pos - 1] *= -1;
				inverse[abs(val)] = pos - 1;
				inverse[abs(other)] = pos;	
				break;
			}
			case 5:  {
				visit(cur_perm, val, "t2r"); // twist value to the right
				int other = cur_perm[pos + 1];
				swap(cur_perm[pos + 1], cur_perm[pos]);
				cur_perm[pos] *= -1;
				cur_perm[pos + 1] *= -1;
				inverse[abs(val)] = pos + 1;
				inverse[abs(other)] = pos;
				break;
			}
		}
	}
	visit(cur_perm, 0, "o"); // the last permutation has no change
}
	
int main(){
	int n;
	scanf("%d", &n);

	auto algorithm1 = [n](int change){
		if(abs(change) <= n) return make_pair(n+1-abs(change), 3);
		else return make_pair(2*n+1-abs(change), (change > 0 ? 0 : 1));
	};
	loopless_signed_perm(n, 0, algorithm1);

	auto algorithm2 = [n](int change){
		if (abs(change) > n) return make_pair(2*n+1-abs(change), 3);
		else return make_pair(n+1-abs(change), (change > 0 ? 0 : 1));
	};
	loopless_signed_perm(n, 1, algorithm2);

	auto algorithm3 = [n](int change){
		if (abs(change)%2 == 1) return make_pair(n+1-(abs(change)+1)/2, 3);
		else return make_pair(n+1-(abs(change)+1)/2, (change > 0 ? 0 : 1));
	};
	loopless_signed_perm(n, 2, algorithm3);

	auto algorithm4 = [n](int change){
		if (abs(change)%2 == 0) return make_pair(n+1-(abs(change)+1)/2, 3);
		else return make_pair(n+1-(abs(change)+1)/2, (change > 0 ? 0 : 1));
	};
	loopless_signed_perm(n, 3, algorithm4);

	auto algorithm5 = [n](int change){
		if (abs(change) <= n) return make_pair(n+1-abs(change), 2);
		else return make_pair(2*n+1-abs(change), (change > 0 ? 0 : 1));
	};
	loopless_signed_perm(n, 0, algorithm5);

	auto algorithm6 = [n](int change){
		if (abs(change) > n) return make_pair(2*n+1-abs(change), 2);
		else return make_pair(n+1-abs(change), (change > 0 ? 0 : 1));
	};
	loopless_signed_perm(n, 1, algorithm6);

	auto algorithm7 = [n](int change){
		if (abs(change) <= n) return make_pair(n+1-abs(change), 3);
		else return make_pair(2*n+1-abs(change), (change > 0 ? 4 : 5));
	};
	loopless_signed_perm(n, 0, algorithm7);

	auto algorithm8 = [n](int change){
		if (abs(change) > n) return make_pair(2*n+1-abs(change), 3);
		else return make_pair(n+1-abs(change), (change > 0 ? 4 : 5));
	};
	loopless_signed_perm(n, 1, algorithm8);

	auto algorithm9 = [n](int change){
		if (abs(change) <= n) return make_pair(n+1-abs(change), 2);
		else return make_pair(2*n+1-abs(change), (change > 0 ? 4 : 5));
	};
	loopless_signed_perm(n, 0, algorithm9);

	auto algorithm10 = [n](int change){
		if (abs(change) > n) return make_pair(2*n+1-abs(change), 2);
		else return make_pair(n+1-abs(change), (change > 0 ? 4 : 5));
	};
	loopless_signed_perm(n, 1, algorithm10);

	auto algorithm11 = [n](int change){
		if (abs(change)%2 == 1) return make_pair(n+1-(abs(change)+1)/2, 3);
		else return make_pair(n+1-(abs(change)+1)/2, (change > 0 ? 4 : 5));
	};
	loopless_signed_perm(n, 2, algorithm11);

	auto algorithm12 = [n](int change){
		if (abs(change)%2 == 0) return make_pair(n+1-(abs(change)+1)/2, 3);
		else return make_pair(n+1-(abs(change)+1)/2, (change > 0 ? 4 : 5));
	};
	loopless_signed_perm(n, 3, algorithm12);
	return 0;
}
