#include"function.h"
void main() {//Khanh
	Engine search;
	TrieNode*** root, *stopword = search.getNode();
	search.Init(root, stopword);
	clock_t begin, end1, end2;
	cout << "             W               ,___________ __________,', " << endl;
	cout << "              E       T     / '-_._GR14-KNV_.______|__] " << endl;
	cout << "               L       O   / _ == o;;;;;;;;__ _____.:/" << endl;
	cout << "                C          ), -.(_(__)/                " << endl;
	cout << "                 O        //___///_),-             " << endl;
	cout << "                  M      //___//                .*. .*.  SEARCH  .*. .*.  " << endl;
	cout << "                   E    //___//      ______     |_| |_|   TOOL   |_| |_| " << endl;
	cout << "                                    //===//" << endl;
	cout << "                                   //===//" << endl;
	cout << "                                  //===//" << endl;
	cout << "_________________________________********" << endl;
	while (1) {
		int NumOfRes = 0;
		cout << "0. Exit" << endl << "1. Input query from keyboard" << endl << "2. Clear history" << endl << "OPTION:   ";
		int choice; cin >> choice; cin.ignore();
		if (!choice) break;
		if (choice == 2) {
			clearHistory();
			cout << "History has been cleared..............." << endl << endl;
			continue;
		}
		string query;
		cout << "Query:"; getline(cin, query);
		cout << "0. Search" << endl << "1. Show history suggestion" << endl << "OPTION:   ";
		cin >> choice; cin.ignore();
		if (choice) {
			vector<string> res;
			History(res, query);
			cin >> choice; cin.ignore();
			if (choice > res.size() - 1) {
				cout << "Invalid" << endl; choice = 0;
			}
			query = res[choice];
		}
		else {
			ofstream output; output.open("history.txt", ios::app);
			output << query << endl;
			output.close();
		}
		begin = clock();
		vector<string> syno;
		if (query[0] == '~') syno = search.getSyno(query.substr(1));
		bool check = search.checkOperator(query);
		vector<Store> prior, result;
		for (int i = 1; i <= 15; ++i) {
			if (i == 10 || i == 14)
				for (int j = 1; j <= 100; ++j) {
					Store File;
					File.score = 0;
					if (query[0] == '~') {
						if (search.searchSyno(root[i - 1][j - 1], query.substr(1), File.pos,syno)) {
							++NumOfRes;
							File.filename = search.OpenFile(i, j);
							prior.push_back(File);
						}
						continue;
					}
					if (check) {
						string exact = '"' + query + '"';
						if (search.searchRoot(root[i - 1][j - 1], exact, stopword, File.pos, File.score)) {
							++NumOfRes;
							File.filename = search.OpenFile(i, j);
							prior.push_back(File);
							continue;
						}
					}
					bool res = search.searchRoot(root[i - 1][j - 1], query, stopword, File.pos, File.score);
					if (res) {
						++NumOfRes;
						File.filename = search.OpenFile(i, j);
						result.push_back(File);
					}
				}
			else if (i != 15)
				for (int j = 1; j <= 50; ++j) {
					Store File;
					File.score = 0;
					if (query[0] == '~') {
						if (search.searchSyno(root[i - 1][j - 1], query.substr(1), File.pos, syno)) {
							++NumOfRes;
							File.filename = search.OpenFile(i, j);
							prior.push_back(File);
						}
						continue;
					}
					if (check) {
						string exact = '"' + query + '"';
						if (search.searchRoot(root[i - 1][j - 1], exact, stopword, File.pos, File.score)) {
							++NumOfRes;
							File.filename = search.OpenFile(i, j);
							prior.push_back(File);
							continue;
						}
					}
					bool res = search.searchRoot(root[i - 1][j - 1], query, stopword, File.pos, File.score);
					if (res) {
						++NumOfRes;
						File.filename = search.OpenFile(i, j);
						result.push_back(File);
					}
				}
			else for (int j = 801; j <= 1600; ++j) {
					Store File;
					File.score = 0;
					if (query[0] == '~') {
						if (search.searchSyno(root[i - 1][j - 801], query.substr(1), File.pos, syno)) {
							++NumOfRes;
							File.filename = search.OpenFile(i, j);
							prior.push_back(File);
						}
						continue;
					}
					if (check) {
						string exact = '"' + query + '"';
						if (search.searchRoot(root[i - 1][j - 801], exact, stopword, File.pos, File.score)) {
							++NumOfRes;
							File.filename = search.OpenFile(i, j);
							prior.push_back(File);
							continue;
						}
					}
					bool res = search.searchRoot(root[i - 1][j - 801], query, stopword, File.pos, File.score);
					if (res) {
						++NumOfRes;
						File.filename = search.OpenFile(i, j);
						result.push_back(File);
					}
				}
			end1 = clock();
		}
		sort(prior.begin(), prior.end(), CompareByScore);
		sort(result.begin(), result.end(), CompareByScore);
		for (int i = 0; i < prior.size(); ++i) {
			cout << prior[i].filename << endl;
			search.OutputResult(prior[i].filename, prior[i].pos);
			cout << "Score:" << prior[i].score << endl;
			printf("------------------------------------------------------------\n");
		}
		printf("==============================================================\n");
		for (int i = 0; i < result.size(); ++i) {
			cout << result[i].filename << endl;
			search.OutputResult(result[i].filename, result[i].pos);
			cout << "Score:" << result[i].score << endl;
			printf("------------------------------------------------------------\n");
		}
		end2 = clock();
		cout << "Total result(s): " << NumOfRes << endl;
		cout << "Search time: " << (float)(end1 - begin) / CLOCKS_PER_SEC << "s" << endl;
		cout << "Output time: " << (float)(end2 - begin) / CLOCKS_PER_SEC << "s" << endl << endl;
	}
	cout << "Destructor is running..." << endl;
	search.destructor(root, stopword);
}