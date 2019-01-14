#include "function.h"
#include "console.h"
//return the new TrieNode
//ASCII, 0:48, 9:57, a:97, z:122, A:65, Z:90
TrieNode* Engine::getNode() {//Nam
	TrieNode* pNode = NULL;
	pNode = new TrieNode;
	if (pNode) {
		pNode->isLeaf = false;
		pNode->isTitle = false;
		for (int i = 0; i < 42; ++i) pNode->children[i] = NULL;
	}
	return pNode;
}
string Engine::NumberToString(int num) {//Viet
	stringstream ss;
	ss << num;
	return ss.str();
}
string FloatToString(float num) {//Viet
	stringstream ss;
	ss << num;
	return ss.str();
}
void Engine::insertWord(TrieNode* &root, string key, int place, bool title) {//Nam
	TrieNode* pCrawl = root;
	int index, length = key.length();
	for (int i = 0; i < length; ++i) {
		index = convert(key[i]);
		if (index == -1) continue;
		if (!pCrawl->children[index]) pCrawl->children[index] = getNode();
		pCrawl = pCrawl->children[index];
	}
	pCrawl->isLeaf = true;
	pCrawl->order.push_back(place);
	if (title) pCrawl->isTitle = true;
}
void Engine::insertStopword(TrieNode* &root, string key) {//Nam
	TrieNode* pCrawl = root;
	int index, length = key.length();
	for (int i = 0; i < length; ++i) {
		index = convert(key[i]);
		if (index == -1) continue;
		if (!pCrawl->children[index]) pCrawl->children[index] = getNode();
		pCrawl = pCrawl->children[index];
	}
	pCrawl->isLeaf = true;
}
//return true if key presents in trie, else false
TrieNode* Engine::searchWord(TrieNode* root, string key, bool title){//Khanh
	TrieNode* pCrawl = root;
	int index, length = key.length();
	for (int i = 0; i < length; ++i) {
		index = convert(key[i]);
		if (index == -1) continue;//invalid
		if (!pCrawl->children[index]) return NULL;
		pCrawl = pCrawl->children[index];
	}
	if (pCrawl) {
		if (title && !pCrawl->isTitle) return NULL;
		if (pCrawl->isLeaf) return pCrawl;
	}
	return NULL;
}
string Engine::SenFilter(string sen) {//Khanh
	string res;
	int length = sen.length();
	for (int i = 0; i < length; ++i) {
		if (!accept(sen[i])) {
			if (sen[i] == 39 && sen[i + 1] == 's'&&sen[i + 2] == ' ') ++i;
			else if (i + 1 != length&&sen[i] == '.'&&isNumber(sen[i - 1]) && isNumber(sen[i + 1]))//so thap phan
				res.append(sen, i, 1);
		}
		else res.append(sen, i, 1);//get 1 
	}
	return res;
}

void Engine::InsertStopword(TrieNode*& root) {//trie of stopword, Nam
	ifstream file;
	string get;
	file.open("stopword.txt");
	if (!file.is_open()) return;
	while (!file.eof()) {
		getline(file, get);
		insertStopword(root, get);
	}
	file.close();
}
void Engine::InputListFile(TrieNode***& root) {//Khanh
	ifstream file;
	for (int i = 1; i <= 15; ++i) {
		if (i == 10 || i == 14) {
			for (int j = 1; j <= 100; ++j) {
				string filename = OpenFile(i, j);
				file.open(filename);
				if (!file.is_open()) { cout << "Cannot open file " << filename << endl; continue; }
				cout << i << " " << j << endl;
				InputFile(root[i - 1][j - 1], file);
				file.close();
			}
		}
		else if (i != 15) {
			for (int j = 1; j <= 50; ++j) {
				string filename = OpenFile(i, j);
				file.open(filename);
				if (!file.is_open()) { cout << "Cannot open file " << filename << endl; continue; }
				cout << i << " " << j << endl;
				InputFile(root[i - 1][j - 1], file);
				file.close();
			}
		}
		else {
			for (int j = 801; j <= 1600; ++j) {
				string filename = OpenFile(i, j);
				file.open(filename);
				if (!file.is_open()) { cout << "Cannot open file " << filename << endl; continue; }
				cout << i << " " << j << endl;
				InputFile(root[i - 1][j - 801], file);
				file.close();
			}
		}
	}
}
void Engine::InputFile(TrieNode*& root, ifstream& file) {//open each file,Khanh
	int start = 0;
	string sen; getline(file, sen);//get the title
	InputSen(root, sen, start, true);//a trie for the intitle:
	while (!file.eof()) {
		getline(file, sen,'.');//get a sentence
		if (sen.length()&&isNumber(sen.back())) {
			string next; getline(file, next, '.');
			if (next.length() && isNumber(next[0])) {
				sen = sen + '.' + next;
				InputSen(root, sen, start, false);
				continue;
			}
			else {
				InputSen(root, sen, start, false);
				InputSen(root, next, start, false);
				continue;
			}
		}
		InputSen(root, sen, start, false);
	}
}
void Engine::InputSen(TrieNode*& root, string sen, int& start, bool title) {//sen has been filted, start:place to start sentence
	sen = SenFilter(sen);//Nam
	if (!root) return;
	stringstream ss(sen);
	while (ss >> sen) {
		if (title) insertWord(root, sen, start, true);
		else insertWord(root, sen, start, false);
		++start;
	}
}
bool Engine::searchRoot(TrieNode* root, string query, TrieNode* stopword, vector<int>& pos, int& score) {//search a file, score=0 initially
	stringstream ss(query);//Khanh
	string tmp, get;
	while (ss >> tmp) {
		if (tmp[0] == '"') {//exact match and wildcard
			vector<int> res;
			TrieNode* res1 = searchWord(root, tmp, false), *res2;
			if (!res1) return false;
			if (tmp.back() == '"') {
				pos = getUnion(pos, res1->order); 
				score += res1->order.size(); 
				continue;
			}
			string tmp2;
			vector<int> tmp = res1->order;
			while (ss >> tmp2) {
				int cnt = 1;
				if (tmp2 == "") return true;
				if (tmp2 == "*") {
					++cnt;
					while (ss >> tmp2) {
						if (tmp2 == "*") ++cnt;
						else break;
					}
				}
				res2 = searchWord(root, tmp2, false);
				if (!res2) return false;
				vector<int> out1, out2;
				exact(tmp, res2->order, cnt, out1, out2);
				if (out1.empty()) return false;
				res = getUnion(res, getUnion(out1, out2));
				if (tmp2.back() == '"') {
					score += out1.size();
					break;
				}
				tmp = out2;
			}
			pos = getUnion(pos, res);
			continue;
		}
		if (tmp[0] == '-') {//minus sign
			if (searchWord(root, tmp.substr(1), false)) return false;
			continue;
		}
		get = tmp.substr(0, 8);//intitle:
		if (get == "intitle:") {
			get = tmp.substr(8);
			TrieNode* title = searchWord(root, get, true);
			if (!title) return false;
			score += title->order.size();
			pos = getUnion(pos, title->order);
			while (ss >> get) {
				title = searchWord(root, get, true);
				if (!title) return false;
				score += title->order.size();
				pos = getUnion(pos, title->order);
			}
			continue;
		}
		TrieNode* searchRes;
		if (tmp == "OR") {
			ss >> tmp;
			searchRes = searchWord(root, tmp, false);
			if (searchRes) {
				pos = getUnion(pos, searchRes->order);
				score += searchRes->order.size();
			}
			continue;
		}
		searchRes = searchWord(root, tmp, false);
		if (tmp[0] == '+') {
			if (!searchRes) return false;
			pos = getUnion(pos, searchRes->order);
			score += searchRes->order.size();
		}
		if (searchWord(stopword, tmp, false)) continue;
		if (tmp == "AND" || tmp == "filetype:txt") continue;
		if (tmp.back() == '*') {//bonus
			vector<int> res;
			searchIncomplete(root, tmp, res);
			if (res.empty()) return false;
			pos = getUnion(pos, res);
			continue;
		}
		if (isNumber(tmp[0]) || tmp[0] == '$') {//search range
			double lowbound, upbound;
			if (getRange(tmp, lowbound, upbound)) {
				string number;  vector<int> res;
				if (tmp[0] == '$') {
					if (!root->children[38]) return false;
					findNumber(root->children[38], number, lowbound, upbound, res);
					if (res.empty()) return false;
				}
				else findNumber(root, number, lowbound, upbound, res);
				if (res.empty()) return false;
				pos = getUnion(pos, res);
				continue;
			}
		}
		if (!searchRes) {
			bool check = false;
			while (ss >> tmp) {
				if (tmp == "OR") {
					ss >> tmp;
					searchRes = searchWord(root, tmp, false);
					if (searchRes) {
						pos = getUnion(pos, searchRes->order);
						score += searchRes->order.size();
						check = true;
						break;
					}
				}
				else return false;
			}
			if (!check) return false;
		}
		else {
			pos = getUnion(pos, searchRes->order);
			score += searchRes->order.size();
		}
	}
	return true;
}
void Engine::OutputResult(string filename, vector<int> pos) {//Khanh
	if (!pos.size()) return;
	ifstream file; file.open(filename);
	if (!file.is_open()) { cout << "Error" << endl; return; }
	string title; getline(file, title);
	cout << title << endl;
	file.clear();
	file.seekg(0, ios::beg);
	int cnt = 0, cur = 0, totalLength = -1;
	while (!file.eof()) {
		string tmp, res;
		getline(file, tmp, '.');
		if (tmp.length() && isNumber(tmp.back())) {//decimal number case
			string next; getline(file, next, '.');
			if (next.length() && isNumber(next[0])) {
				tmp = tmp + '.' + next;
			}
			else tmp = tmp + " " + next;
		}
		tmp = SenFilter(tmp);
		int NumWord = WordinSen(tmp);
		totalLength += NumWord;
		if (pos[cur] > totalLength) {
			cnt += NumWord;
			continue;
		}
		cout << "...";
		stringstream ss(tmp);
		while (ss >> tmp) {
			if (cur < pos.size() && pos[cur] == cnt) {
				++cur;
				for (int i = 0; i < tmp.length(); ++i) if (isWord(tmp[i])) tmp[i] -= 32;
				TextColor(10);
				cout << tmp << " ";
				TextColor(7);
			}
			else cout << tmp << " ";
			++cnt;
		}
		cout << "..." << endl;
		if (cur >= pos.size()) {
			file.close();
			return;
		}
	}
	file.close();
}
//HELPER UTILITY
int Engine::convert(char key) {//convert to index, Nam
	if (key >= 48 && key <= 57) return (key - '0');
	if (key >= 97 && key <= 122) return (int(key) - 87);
	if (key >= 65 && key <= 90) return (int(key) - 55);
	if (key == ' ') return 36;
	if (key == '.') return 37;
	if (key == '$') return 38;
	if (key == '%') return 39;
	if (key == '#') return 40;
	if (key == '-') return 41;
	return -1;
}
bool Engine::accept(char& key) {//Viet
	if (key >= 65 && key <= 90) {
		key = int(key) + 32;
		return true;
	}
	if ((key >= 48 && key <= 57) || (key >= 97 && key <= 122)) return true;
	if (key == '\n'){
		key = ' '; return true;
	}
	if (key == '—') {
		key = '-'; return true;
	}
	if (key == ' ' || key == '$' || key == '%' || key == '#'|| key == '-') return true;
	return false;
}
bool Engine::isNumber(char key) {//Viet
	if (key >= 48 && key <= 57) return true;
	return false;
}
bool Engine::isWord(char key){//Viet
	if ((key >= 65 && key <= 90) || (key >= 97 && key <= 122)) return true;
	return false;
}
void Engine::DeleteTries(TrieNode* & root) {//destructor, Nam
	if (!root) return;
	for (int i = 0; i < 38; ++i) {
		DeleteTries(root->children[i]);
	}
	delete root;
}
void Engine::exact(vector<int>& res1, vector<int>& res2, int cnt, vector<int>& out1, vector<int>& out2) {//Khanh
	int i = 0, j = 0, size1 = res1.size(), size2 = res2.size();
	while (i < size1&&j < size2) {
		if (res1[i] + cnt < res2[j]) ++i;
		else if (res2[j] < res1[i] + cnt) ++j;
		else {
			out1.push_back(res1[i++]);
			out2.push_back(res2[j++]);
		}
	}
}
string Engine::OpenFile(int i, int j) {//Viet
	string group = NumberToString(i), txt = NumberToString(j);
	if (i < 10 && j < 10) return ("Group0" + group + "_0" + txt + ".txt");
	else if (i < 10) return ("Group0" + group + "_" + txt + ".txt");
	else if (i >= 10 && j < 10 && i < 15) return ("Group" + group + "_0" + txt + ".txt");
	else if (i >= 10 && j >= 10 && i < 15) return ("Group" + group + "_" + txt + ".txt");
	else return ("Data" + txt + ".txt");
}
bool Engine::checkOperator(string query) {//Khanh
	stringstream ss(query);
	string tmp;
	while (ss >> tmp) {
		string get; get = tmp.substr(0, 8);
		if (get == "intitle:") return false;
		if (tmp == "AND" || tmp == "OR" || tmp[0] == '-' || tmp == "*" || tmp == "filetype:txt" || tmp[0] == '"' || tmp.back() == '*') return false;
		int length = tmp.length();
		for (int i = 0; i <= length - 3; ++i) {
			if (tmp[i] == '.'&&tmp[i + 1] == '.') return false;
		}
	}
	return true;
}
vector<int> Engine::getUnion(vector<int>& v1, vector<int>& v2) {//Khanh
	if (v1.empty()) return v2;
	if (v2.empty()) return v1;
	vector<int> res;
	int i = 0, j = 0, size1 = v1.size(), size2 = v2.size();
	while (i < size1&&j < size2) {
		if (v1[i] < v2[j]) res.push_back(v1[i++]);
		else if (v2[j] < v1[i]) res.push_back(v2[j++]);
		else {
			res.push_back(v2[j++]); ++i;
		}
	}
	while (i < size1) res.push_back(v1[i++]);
	while (j < size2) res.push_back(v2[j++]);
	return res;
}
void Engine::Init(TrieNode*** &root, TrieNode* &stopword) {//Khanh
	root = new TrieNode**[15];
	for (int i = 0; i < 15; ++i) {
		if (i == 9 || i == 13) root[i] = new TrieNode*[100];
		else if (i != 14) root[i] = new TrieNode*[50];
		else root[i] = new TrieNode*[800];
	}
	for (int i = 0; i < 15; ++i) {
		if (i == 9 || i == 13)
			for (int j = 0; j < 100; ++j) root[i][j] = getNode();
		else if (i != 14)
			for (int j = 0; j < 50; ++j) root[i][j] = getNode();
		else for (int j = 800; j < 1600; ++j) root[i][j - 800] = getNode();
	}
	clock_t load = clock();
	InputListFile(root);
	InsertStopword(stopword);
	clock_t het = clock();
	cout << "Loading time: " << (float)(het - load) / CLOCKS_PER_SEC << "s" << endl;
}
void Engine::searchIncomplete(TrieNode* root, string key, vector<int>& res) {//Khanh
	TrieNode* pCrawl = root;
	int index, length = key.length();
	for (int i = 0; i < length; ++i) {
		index = convert(key[i]);
		if (index == -1) continue;//invalid
		if (!pCrawl->children[index]) return;
		pCrawl = pCrawl->children[index];
	}
	if (pCrawl) findIncomplete(pCrawl, res);
}
void Engine::findIncomplete(TrieNode* root, vector<int>& res) {//Khanh
	if (root->isLeaf) res = getUnion(res, root->order);
	for (int i = 0; i <= 41; ++i)
		if (root->children[i]) findIncomplete(root->children[i], res);
}
void Engine::findNumber(TrieNode* root, string number, double lowbound, double upbound, vector<int>& pos) {//Khanh
	if (root->isLeaf) {
		double num = atof(number.c_str());
		if (num >= lowbound&&num <= upbound) {
			pos = getUnion(pos, root->order);
		}
	}
	for (int i = 0; i <= 37; ++i) {
		if (i == 10) {
			i += 26; continue;
		}
		if (root->children[i]) {
			char append;
			if (i != 37) append = i + '0';
			else append = '.';
			findNumber(root->children[i], number + append, lowbound, upbound, pos);
		}
	}
}
bool Engine::getRange(string tmp, double& lowbound, double& upbound) {//Khanh
	string low, up;
	int length = tmp.length();
	bool check = false;
	for (int i = 1; i <= length - 3; ++i) {
		if (tmp[i] == '.'&&tmp[i + 1] == '.') { check = true; break; }
	}
	if (!check) return false;
	int i;
	for (i = 0; i < length; ++i) {
		if (tmp[i] == '.'&&tmp[i + 1] == '.') break;
		low += tmp[i];
	}
	i += 2;
	for (i; i < length; ++i) {
		up += tmp[i];
	}
	if (tmp[0] == '$') {
		low = low.substr(1);
		up = up.substr(1);
	}
	lowbound = atof(low.c_str());
	upbound = atof(up.c_str());
}
bool CompareByScore(const Store&a, const Store&b) {//Viet
	return a.score > b.score;
}
void Engine::destructor(TrieNode*** &root, TrieNode* &stopword) {//Viet
	for (int i = 0; i < 15; ++i) {
		if (i == 9 || i == 13) {
			for (int j = 0; j < 100; ++j) DeleteTries(root[i][j]);
			delete[] root[i];
		}
		else if (i != 14) {
			for (int j = 0; j < 50; ++j) DeleteTries(root[i][j]);
			delete[] root[i];
		}
		else {
			for (int j = 0; j < 800; ++j) DeleteTries(root[i][j]);
			delete[] root[i];
		}
	}
	delete[]root;
	DeleteTries(stopword);
}
int WordinSen(string sen) {//Viet
	stringstream ss(sen);
	int cnt = 0;
	string tmp;
	while (ss >> tmp)++cnt;
	return cnt;
}
void History(vector<string>& res, string query) {//Nam
	res.push_back(query);
	cout << "Choose query:" << endl << "0. " << query << endl;
	int i = 1;
	ifstream input; input.open("history.txt");
	string tmp;
	while (!input.eof()) {
		getline(input, tmp);
		if (tmp == "" || query == tmp || tmp.find(query) == -1) continue;
		cout << i << ". " << tmp << endl;
		res.push_back(tmp);
		++i;
	}
	input.close();
	ofstream output; output.open("history.txt", ios::app);
	output << query << endl;
	output.close();
}
void clearHistory() {//Nam
	ifstream file; file.open("history.txt", ios::out | ios::trunc);
	file.close();
}
vector<string> Engine:: getSyno(string key) {//Khanh
	vector<string> res;
	res.push_back(key);
	if (key[0] >= 97 && key[0] <= 122) key[0] -= 32;
	for (int i = 1; i < key.length(); ++i)
		if (key[i] >= 65 && key[i] <= 90) key[i] += 32;
	ifstream file; file.open("synonym.txt");
	while (!file.eof()) {
		string tmp;
		getline(file, tmp);
		stringstream ss(tmp);
		ss >> tmp;
		if (tmp != "KEY:") continue;
		ss >> tmp;
		if (tmp != key) continue;
		if (tmp > key) return res;
		getline(file, tmp);
		ss.str(""); ss.clear(); ss << tmp;
		ss >> tmp;
		if (tmp != "SYN:") return res;
		while (ss >> tmp) {
			if (tmp[0] == '[' || tmp[0] == '{') continue;
			res.push_back(tmp);
		}
		file.close();
		return res;
	}
	file.close();
	return res;
}
bool Engine::searchSyno(TrieNode* root, string key, vector<int>& res,vector<string>& syno) {//Khanh
	bool check = false;
	for (int i = 0; i < syno.size(); ++i) {
		TrieNode* searchRes = searchWord(root, syno[i], false);
		if (searchRes) {
			check = true;
			res = getUnion(res, searchRes->order);
		}
	}
	return check;
}