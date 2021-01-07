#include<iostream>
#include <time.h>
#include <chrono>
using namespace std;

class BTreeNode{ 
public:
	int* keys; 
	int minimum;	 
	BTreeNode** potomci;
	int keynum=0;	
	bool leaf;

	BTreeNode(int nminimum, bool _leaf){
		minimum =nminimum;
		leaf = _leaf;
		keys = new int[2 * minimum - 1];
		potomci = new BTreeNode *[2 * minimum];
	}

	void traverse()	{	
		for (int i = 0; i < keynum; i++)		{
			if (leaf == false) potomci[i]->traverse();
			cout << " " << keys[i];
		}
		if (leaf == false) 	potomci[keynum]->traverse(); 
	}

	void insertVolno(int k) {
		int i = keynum - 1;
		if (leaf == true) {                    
			while (i >= 0 && keys[i] > k){
				keys[i + 1] = keys[i];
				i--;
			}
			keys[i + 1] = k;	
			keynum = keynum + 1;
		}
		else {
			while (i >= 0 && keys[i] > k) {
				i--;
			}

			if (potomci[i + 1]->keynum == 2 * minimum - 1) { 
				splitChild(i + 1, potomci[i + 1]);
				if (keys[i + 1] < k) i++;
			}
			potomci[i + 1]->insertVolno(k); 
		}
	}
	
	void splitChild(int i, BTreeNode* old) {
		BTreeNode* now = new BTreeNode(old->minimum, old->leaf);
		now->keynum = minimum - 1;		

		if (old->leaf == false) {                               
			for (int j = 0; j < minimum; j++)
				now->potomci[j] = old->potomci[j + minimum];
		}
		
		for (int j = 0; j < minimum - 1; j++) {
			now->keys[j] = old->keys[j + minimum];
		}
		old->keynum = minimum - 1;
		for (int j = keynum; j >= i + 1; j--)  potomci[j + 1] = potomci[j];
		potomci[i + 1] = now; 

		for (int j = keynum - 1; j >= i; j--) keys[j + 1] = keys[j];

		keys[i] = old->keys[minimum - 1]; 
		keynum = keynum + 1;
	}
	BTreeNode* search(int k){
		int i = 0;
		while (i < keynum && k > keys[i]) i++;

		if (keys[i] == k) return this;

		if (leaf == true) return NULL;
		return potomci[i]->search(k);
	}	
};


class BTree{
public:
	BTreeNode* root; 
	int minimum; 

	BTree(int aminimum) {
		root = NULL; minimum = aminimum;
	}


	void traverse()	{
		if (root != NULL) root->traverse();
	}


	void insert(int k) {
		if (root == NULL) {                          
			root = new BTreeNode(minimum, true);
			root->keys[0] = k;
			root->keynum = 1;
		}
		else {
			if (root->keynum == 2 * minimum - 1) {   
				BTreeNode* s = new BTreeNode(minimum, false);
				s->potomci[0] = root;
				s->splitChild(0, root);
				int i = 0;                             
				if (s->keys[0] < k)	i++;
				s->potomci[i]->insertVolno(k);
				root = s;                 
			}
			else root->insertVolno(k);   
		}
	}

	void vyhledavani(int k)	{
		BTreeNode* vysledek = root->search(k);
		if (vysledek == NULL) {
			cout << endl << "prvek nenalezen" << endl;
		}
		else {
			cout <<endl << "prvek nalezen"<<endl;
		}
	}
};


inline void PrintTimeDifference(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end){
	long long t = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	printf("%lld [microseconds]\n", t);
}

void main(){
	BTree tree(2);
	tree.insert(5);
	tree.insert(9);
	tree.insert(8);
	tree.insert(10);
	tree.insert(15);
	tree.insert(9);
	tree.traverse();
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	tree.vyhledavani(9);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	PrintTimeDifference(begin, end);
};



