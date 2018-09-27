// CPP code for offline queries in 
// approx constant time. 
#include<bits/stdc++.h> 
using namespace std; 

int n1; 

// Structure to store decomposed data 
typedef struct
{ 
	vector<int> data; 
	vector<vector<int>> rdata; 
	int blocks; 
	int blk_sz; 
}sqrtD; 

vector<vector<sqrtD>> Sq3; 
vector<sqrtD> Sq2; 
sqrtD Sq1; 

// Square root Decomposition of 
// a given array 
sqrtD decompose(vector<int> arr) 
{ 
	sqrtD sq; 
	int n = arr.size(); 
	int blk_idx = -1; 
	sq.blk_sz = sqrt(n); 
	sq.data.resize((n/sq.blk_sz) + 1, 0); 
	
	// Calculation of data in blocks 
	for (int i = 0; i < n; i++) 
	{ 
		if (i % sq.blk_sz == 0) 
		{ 
			blk_idx++; 
		} 
		sq.data[blk_idx] += arr[i]; 
	} 
	
	int blocks = blk_idx + 1; 
	sq.blocks = blocks; 
	
	// Calculation of prefix data 
	int prefixData[blocks]; 
	prefixData[0] = sq.data[0]; 
	for(int i = 1; i < blocks; i++) 
	{ 
		prefixData[i] = 
		prefixData[i - 1] + sq.data[i]; 
	} 
	
	sq.rdata.resize(blocks + 1, 
			vector<int>(blocks + 1)); 
	
	// Calculation of data between blocks 
	for(int i = 0 ;i < blocks; i++) 
	{ 
		for(int j = i + 1; j < blocks; j++) 
		{ 
			sq.rdata[i][j] = sq.rdata[j][i] = 
			prefixData[j - 1] - prefixData[i]; 
		} 
	} 
	
	return sq; 
} 

// Sqaure root Decompostion at level3 
vector<vector<sqrtD>> tripleDecompose(sqrtD sq1, 
					sqrtD sq2,vector<int> &arr) 
{ 
	vector<vector<sqrtD>> sq(sq1.blocks, 
					vector<sqrtD>(sq1.blocks)); 
	
	int blk_idx1 = -1; 
	
	for(int i = 0; i < sq1.blocks; i++) 
	{ 
		int blk_ldx1 = blk_idx1 + 1; 
		blk_idx1 = (i + 1) * sq1.blk_sz - 1; 
		blk_idx1 = min(blk_idx1,n1 - 1); 

		int blk_idx2 = blk_ldx1 - 1; 
		
		for(int j = 0; j < sq2.blocks; ++j) 
		{ 
			int blk_ldx2 = blk_idx2 + 1; 
			blk_idx2 = blk_ldx1 + (j + 1) * 
					sq2.blk_sz - 1; 
			blk_idx2 = min(blk_idx2, blk_idx1); 
		
			vector<int> ::iterator it1 = 
						arr.begin() + blk_ldx2; 
			vector<int> ::iterator it2 = 
						arr.begin() + blk_idx2 + 1; 
			vector<int> vec(it1, it2); 
			sq[i][j] = decompose(vec);	 
		} 
	} 
	return sq;		 
} 

// Sqaure root Decompostion at level2 
vector<sqrtD> doubleDecompose(sqrtD sq1, 
							vector<int> &arr) 
{ 
	vector<sqrtD> sq(sq1.blocks); 
	int blk_idx = -1; 
	for(int i = 0; i < sq1.blocks; i++) 
	{ 
		int blk_ldx = blk_idx + 1; 
		blk_idx = (i + 1) * sq1.blk_sz - 1; 
		blk_idx = min(blk_idx, n1 - 1); 
		vector<int> ::iterator it1 = 
					arr.begin() + blk_ldx; 
		vector<int> ::iterator it2 = 
					arr.begin() + blk_idx + 1; 
		vector<int> vec(it1, it2); 
		sq[i] = decompose(vec); 
	} 
	
	return sq;	 
} 

// Sqaure root Decompostion at level1 
void singleDecompose(vector<int> &arr) 
{ 
	sqrtD sq1 = decompose(arr); 
	vector<sqrtD> sq2(sq1.blocks); 
	sq2 = doubleDecompose(sq1, arr); 
	
	vector<vector<sqrtD>> sq3(sq1.blocks, 
		vector<sqrtD>(sq2[0].blocks)); 
			
	sq3 = tripleDecompose(sq1, sq2[0],arr); 
		
	// ASSIGNMENT TO GLOBAL VARIABLES 
	Sq1 = sq1; 
	Sq2.resize(sq1.blocks); 
	Sq2 = sq2; 
	Sq3.resize(sq1.blocks, 
		vector<sqrtD>(sq2[0].blocks)); 
	Sq3 = sq3; 
} 

// Function for query at level 3 
int queryLevel3(int start,int end, int main_blk, 
				int sub_main_blk, vector<int> &arr) 
{ 
	int blk_sz= Sq3[0][0].blk_sz; 

	// Element Indexing at level2 decompostion 
	int nstart = start - main_blk * 
		Sq1.blk_sz - sub_main_blk * Sq2[0].blk_sz; 
	int nend = end - main_blk * 
		Sq1.blk_sz - sub_main_blk * Sq2[0].blk_sz; 

	// Block indexing at level3 decompostion 
	int st_blk = nstart / blk_sz; 
	int en_blk = nend / blk_sz; 
	
	int answer = 
		Sq3[main_blk][sub_main_blk].rdata[st_blk][en_blk]; 
	
	// If start and end point dont lie in same block 
	if(st_blk != en_blk) 
	{ 
		int left = 0, en_idx = main_blk * Sq1.blk_sz + 
					sub_main_blk * Sq2[0].blk_sz + 
					(st_blk + 1) * blk_sz -1; 
	
		for(int i = start; i <= en_idx; i++) 
		{ 
			left += arr[i]; 
		} 
		
		int right = 0, st_idx = main_blk * Sq1.blk_sz + 
					sub_main_blk * Sq2[0].blk_sz + 
					(en_blk) * blk_sz; 
		
		for(int i = st_idx; i <= end; i++) 
		{ 
			right += arr[i]; 
		} 
		
		answer += left; 
		answer += right; 
	} 
	else
	{ 
		for(int i = start; i <= end; i++) 
		{ 
			answer += arr[i]; 
	} 
} 
return answer;	 
} 

// Function for splitting query to level two 
int queryLevel2(int start, int end, int main_blk, 
				vector<int> &arr) 
{ 
	int blk_sz = Sq2[0].blk_sz; 

	// Element Indexing at level1 decompostion 
	int nstart = start - (main_blk * Sq1.blk_sz); 
	int nend = end - (main_blk * Sq1.blk_sz); 

	// Block indexing at level2 decompostion 
	int st_blk = nstart / blk_sz; 
	int en_blk = nend / blk_sz; 
	
	// Interblock data level2 decompostion 
	int answer = Sq2[main_blk].rdata[st_blk][en_blk]; 
	
	if(st_blk == en_blk) 
	{ 
		answer += queryLevel3(start, end, main_blk, 
							st_blk, arr); 
	} 
	else
	{ 
		answer += queryLevel3(start, (main_blk * 
				Sq1.blk_sz) + ((st_blk + 1) * 
				blk_sz) - 1, main_blk, st_blk, arr); 
		
		answer += queryLevel3((main_blk * Sq1.blk_sz) + 
				(en_blk * blk_sz), end, main_blk, en_blk, arr); 
	} 
	return answer;	 
} 

// Function to return answer according to query 
int Query(int start,int end,vector<int>& arr) 
{ 
	int blk_sz = Sq1.blk_sz; 
	int st_blk = start / blk_sz; 
	int en_blk = end / blk_sz; 
	
	// Interblock data level1 decompostion	 
	int answer = Sq1.rdata[st_blk][en_blk]; 

	if(st_blk == en_blk) 
	{ 
		answer += queryLevel2(start, end, st_blk, arr); 
	} 
	else
	{ 
		answer += queryLevel2(start, (st_blk + 1) * 
				blk_sz - 1, st_blk, arr); 
		answer += queryLevel2(en_blk * blk_sz, end, 
							en_blk, arr); 
	} 
	
	// returning final answer 
	return answer; 
} 

// Driver code 
int main() 
{	 
	n1 = 16; 
	
	vector<int> arr = {7, 2, 3, 0, 5, 10, 3, 12, 
					18, 1, 2, 3, 4, 5, 6, 7}; 

	singleDecompose(arr); 
	
	int q = 5; 
	pair<int, int> query[q] = {{6, 10}, {7, 12}, 
				{4, 13}, {4, 11}, {12, 16}}; 
	
	for(int i = 0; i < q; i++) 
	{ 
		int a = query[i].first, b = query[i].second; 
		printf("%d\n", Query(a - 1, b - 1, arr)); 
	} 
	
	return 0; 
} 
