#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<ctime>
using namespace std;
struct TrieNode {
	TrieNode* children[42];//0-9:number,10-35:words,36:maybe :,37:.(for so thap phan), 38:$, 39:%, 40:#, 41:-
	bool isLeaf, isTitle;
	vector<int> order;//place it appears
};
struct Store {
	string filename;
	vector<int> pos;
	int score;
};
class Engine {
public:
	TrieNode* getNode();
	vector<int> getUnion(vector<int>& v1, vector<int>& v2);
	string NumberToString(int num);
	void insertWord(TrieNode* &root, string key, int place, bool isTitle);
	void insertStopword(TrieNode* &root, string key);
	TrieNode* searchWord(TrieNode* root, string key, bool title);
	void InputListFile(TrieNode***&root);
	void InputFile(TrieNode*& root, ifstream& file);
	void InsertStopword(TrieNode*& root);
	void InputSen(TrieNode*& root, string sen, int& start, bool title);
	bool searchRoot(TrieNode* root, string query, TrieNode* stopword, vector<int>& pos, int& score);
	int convert(char key);
	bool accept(char& key);
	bool isNumber(char key);
	bool isWord(char key);
	void DeleteTries(TrieNode* & root);
	void exact(vector<int>& v1, vector<int>& v2, int cnt, vector<int>& out1, vector<int>& out2);
	string SenFilter(string sen);
	string OpenFile(int i, int j);
	void OutputResult(string filename, vector<int> pos);
	void Init(TrieNode*** &root, TrieNode* &stopword);
	void searchIncomplete(TrieNode* root, string key, vector<int>& res);
	void findIncomplete(TrieNode* root, vector<int>& res);
	void findNumber(TrieNode* root, string number, double lowbound, double upbound, vector<int>& pos);
	bool getRange(string tmp, double& lowbound, double& upbound);
	bool checkOperator(string query);
	void destructor(TrieNode*** &root, TrieNode* &stopword);
	vector<string> getSyno(string key);
	bool searchSyno(TrieNode* root, string key, vector<int>& res, vector<string>& syno);
};
bool CompareByScore(const Store&a, const Store&b);
int WordinSen(string sen);
void History(vector<string>& res, string query);
void clearHistory();