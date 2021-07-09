#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include<numeric>
#include<sstream>
#include<algorithm>
using namespace std;

struct node {
	int year, month, day, deaths, cummulativeDeaths, confirmed, cummulativeConfirmed;
	string countryCode, countryName, region;
	node* next;
};

struct AdjacencyList {
	node* head;
};

class Graph {
	AdjacencyList* countryList;
	AdjacencyList* dateList;
	int sizeCountryList;
	int sizeDateList;
	int Budget = 300;
	vector<pair<string, int>>CountryCost; // acting as weight acc to knapsack prob and keeps track of cost of each country
	vector<pair<string, int>>LastDayConfirmed;// stores last day confirmed cases for each country
public:
	Graph() {
		countryList = NULL;
		sizeCountryList = 0;
		dateList = NULL;
		sizeDateList = 0;
	}

	void CountryList(int n) {
		countryList = new AdjacencyList[n];
		sizeCountryList = n;
		for (int i = 0; i < sizeCountryList; i++)
			countryList[i].head = NULL;
	}

	void DateList(int n) {
		dateList = new AdjacencyList[n];
		sizeDateList = n;
		for (int i = 0; i < sizeDateList; i++)
			dateList[i].head = NULL;
	}

	void createDateList() {
		int counter = 1;
		bool flag = true;
		for (int i = 0; i < sizeCountryList; i++) {
			node* cur1 = countryList[i].head;
			while (cur1 != NULL) {
				for (int j = 0; j < sizeDateList; j++) {

					flag = true;

					if (dateList[j].head == NULL) {
						node* temp = new node;
						temp->year = cur1->year;
						temp->month = cur1->month;
						temp->day = cur1->day;
						temp->countryCode = cur1->countryCode;
						temp->countryName = cur1->countryName;
						temp->region = cur1->region;
						temp->deaths = cur1->deaths;
						temp->cummulativeDeaths = cur1->cummulativeDeaths;
						temp->confirmed = cur1->confirmed;
						temp->cummulativeConfirmed = cur1->cummulativeConfirmed;
						temp->next = NULL;
						dateList[j].head = temp;
						counter++;
						break;
					}

					else {
						node* cur2 = dateList[j].head;
						while (cur2 != NULL) {

							if (cur1->day == cur2->day && cur1->month == cur2->month && cur1->year == cur2->year) {
								node* temp = new node;
								temp->year = cur1->year;
								temp->month = cur1->month;
								temp->day = cur1->day;
								temp->countryCode = cur1->countryCode;
								temp->countryName = cur1->countryName;
								temp->region = cur1->region;
								temp->deaths = cur1->deaths;
								temp->cummulativeDeaths = cur1->cummulativeDeaths;
								temp->confirmed = cur1->confirmed;
								temp->cummulativeConfirmed = cur1->cummulativeConfirmed;
								temp->next = NULL;

								while (cur2->next != NULL) {
									cur2 = cur2->next;
								}
								cur2->next = temp;
								flag = false;
								counter++;
								break;
							}
							if (flag != false)
								cur2 = cur2->next;
						}
						if (flag == false)
							break;
					}

				}
				cur1 = cur1->next;
			}
		}
	}

	void createCountryList(int y, int m, int d, string countrycode, string countryname, string reg, int death, int cumdeath, int conf, int cumconfirmed, int ind) {
		node* temp = new node;
		temp->year = y;
		temp->month = m;
		temp->day = d;
		temp->countryCode = countrycode;
		temp->countryName = countryname;
		temp->region = reg;
		temp->deaths = death;
		temp->cummulativeDeaths = cumdeath;
		temp->confirmed = conf;
		temp->cummulativeConfirmed = cumconfirmed;
		temp->next = NULL;

		if (countryList[ind].head == NULL) {
			countryList[ind].head = temp;
		}
		else {
			node* cur = countryList[ind].head;
			while (cur->next != NULL)
				cur = cur->next;
			cur->next = temp;
		}
	}

	struct compare1 {
		bool operator()(const pair<string, int>& a, const pair<string, int>& b) {
			return (a.second < b.second);
		}
	};

	void createCountryCost(vector<string>countries, vector<int>Cost) {
		for (int i = 0; i < Cost.size(); i++)
			CountryCost.push_back(make_pair(countries.at(i), Cost.at(i)));

		//sorting in order of increasing weights
		sort(CountryCost.begin(), CountryCost.end(), compare1());

		//finding number of confirmed cases on last day for each country
		vector<pair<string, int>>temp;
		for (int i = 0; i < sizeCountryList; i++) {
			node* cur = countryList[i].head;
			while (cur->next != NULL) {
				cur = cur->next;
			}
			if (cur->next == NULL)
				temp.push_back(make_pair(cur->countryName, cur->confirmed));
		}

		// sorting last day deaths in same order as country cost vector
		for (int i = 0; i < CountryCost.size(); i++) {
			for (int j = 0; j < temp.size(); j++) {
				if (CountryCost.at(i).first == temp.at(j).first)
					LastDayConfirmed.push_back(temp.at(j));
			}
		}

	}

	void Top20Confirmed(int d, int m, int y) {
		int ind = -1;
		for (int i = 0; i < sizeDateList; i++) {
			if ((dateList[i].head->day == d) && (dateList[i].head->month == m) && (dateList[i].head->year == y)) {
				ind = i;
			}
		}

		if (ind != -1) {
			node* cur1 = dateList[ind].head;
			node* temp;

			while (cur1 != NULL) {
				node* cur2 = dateList[ind].head;
				while (cur2 != NULL) {

					if (cur1->confirmed > cur2->confirmed) {
						temp = new node;

						temp->year = cur1->year;
						temp->month = cur1->month;
						temp->day = cur1->day;
						temp->countryCode = cur1->countryCode;
						temp->countryName = cur1->countryName;
						temp->region = cur1->region;
						temp->deaths = cur1->deaths;
						temp->cummulativeDeaths = cur1->cummulativeDeaths;
						temp->confirmed = cur1->confirmed;
						temp->cummulativeConfirmed = cur1->cummulativeConfirmed;

						cur1->year = cur2->year;
						cur1->month = cur2->month;
						cur1->day = cur2->day;
						cur1->countryCode = cur2->countryCode;
						cur1->countryName = cur2->countryName;
						cur1->region = cur2->region;
						cur1->deaths = cur2->deaths;
						cur1->cummulativeDeaths = cur2->cummulativeDeaths;
						cur1->confirmed = cur2->confirmed;
						cur1->cummulativeConfirmed = cur2->cummulativeConfirmed;

						cur2->year = temp->year;
						cur2->month = temp->month;
						cur2->day = temp->day;
						cur2->countryCode = temp->countryCode;
						cur2->countryName = temp->countryName;
						cur2->region = temp->region;
						cur2->deaths = temp->deaths;
						cur2->cummulativeDeaths = temp->cummulativeDeaths;
						cur2->confirmed = temp->confirmed;
						cur2->cummulativeConfirmed = temp->cummulativeConfirmed;

					}
					cur2 = cur2->next;
				}
				cur1 = cur1->next;
			}

			cout << endl << "Top 20 countries with the most confirmed cases are:" << endl << endl;
			cout << "Confirmed Cases			" << "Country Name" << endl << endl;
			node* cur = dateList[ind].head;
			int counter = 1;
			while (cur != NULL && counter <= 20) {
				cout << cur->confirmed << " 				" << cur->countryName << endl;
				counter++;
				cur = cur->next;
			}

		}

		else
			cout << endl << "Error: Date does not exist..." << endl;

	}

	struct compare {
		bool operator()(const pair<string, int>& a, const pair<string, int>& b) {
			return (a.second > b.second);
		}
	};

	void HighestNewCases(int d1, int m1, int y1, int d2, int m2, int y2) {

		int ind1 = -1;
		int ind2 = -1;

		for (int i = 0; i < sizeDateList; i++) {
			if ((dateList[i].head->day == d1) && (dateList[i].head->month == m1) && (dateList[i].head->year == y1)) {
				ind1 = i;
			}
		}

		for (int i = 0; i < sizeDateList; i++) {
			if ((dateList[i].head->day == d2) && (dateList[i].head->month == m2) && (dateList[i].head->year == y2)) {
				ind2 = i;
			}
		}

		if (ind1 != -1 && ind2 != -1) {
			vector<pair<string, int>> List;

			for (int i = 0; i < sizeCountryList; i++) {
				List.push_back(make_pair(countryList[i].head->countryName, 0));
			}

			for (int i = 0; i < sizeCountryList; i++) {
				node* cur = countryList[i].head;
				while (cur != NULL) {

					if (m1 == m2) {
						if (cur->day >= d1 && cur->day <= d2 && cur->month == m1)
							List.at(i).second += cur->confirmed;
					}

					else if (m1 < m2) {

						if (cur->month == m1) {
							if (cur->day >= d1)
								List.at(i).second += cur->confirmed;
						}

						else if (cur->month > m1 && cur->month < m2)
							List.at(i).second += cur->confirmed;

						else if (cur->month == m2) {
							if (cur->day <= d2)
								List.at(i).second += cur->confirmed;
						}

					}
					cur = cur->next;
				}
			}

			sort(List.begin(), List.end(), compare());
			cout << endl;
			for (int i = 0; i < List.size(); i++) {
				if (i == 0)
					cout << List.at(i).second << "			" << List.at(i).first << endl;
				else if (i > 0) {
					if (List.at(i).second < List.at(i - 1).second)
						break;
					else if (List.at(i).second == List.at(i - 1).second)
						cout << List.at(i).second << "			" << List.at(i).first << endl;
				}
			}
		}
		else
			cout << endl << "Error: One of the dates you entered does not exist..." << endl;
	}

	int findHighest(vector<int>V) {
		int highest = -1;
		for (int i = 0; i < V.size(); i++) {
			if (V.at(i) > highest)
				highest = V.at(i);
		}
		return highest;
	}

	void findLongestSpread(string cname) {
		int ind = -1;
		for (int i = 0; i < sizeCountryList; i++) {
			if (countryList[i].head->countryName == cname) {
				ind = i;
				break;
			}
		}

		if (ind != -1) {
			int startday = 0;
			int startmonth = 0;
			int startyear = 0;

			int endday = 0;
			int endmonth = 0;
			int endyear = 0;

			vector<int>CasesByDay;

			int spreadDays = 0;

			node* cur1 = countryList[ind].head;
			while (cur1 != NULL) {

				node* cur2 = cur1;

				int tempendday = 0;
				int tempendmonth = 0;
				int tempendyear = 0;

				int tempstartday = 0;
				int tempstartmonth = 0;
				int tempstartyear = 0;

				tempstartday = cur1->day;
				tempstartmonth = cur1->month;
				tempstartyear = cur1->year;

				bool flag = true;

				int tempSpreadDays = 1;
				vector<int>tempCasesByDay;

				while (cur2 != NULL && flag == true) {

					if (tempCasesByDay.size() == 0) {
						tempCasesByDay.push_back(cur2->confirmed);
						tempSpreadDays++;

						tempstartday = cur2->day;
						tempstartmonth = cur2->month;
						tempstartyear = cur2->year;

					}

					else if (cur2->confirmed > findHighest(tempCasesByDay)) {
						tempCasesByDay.push_back(cur2->confirmed);
						tempSpreadDays++;

						tempendday = cur2->day;
						tempendmonth = cur2->month;
						tempendyear = cur2->year;
					}

					else if (cur2->confirmed < findHighest(tempCasesByDay)) {

						bool check = false;
						node* temp = cur2;
						while (temp != NULL) {
							if (temp->confirmed > findHighest(tempCasesByDay)) {
								check = true;
								break;
							}
							temp = temp->next;
						}
						if (check == false)
							flag = false;
						else
							tempSpreadDays++;
					}

					cur2 = cur2->next;
				}

				if (tempSpreadDays > spreadDays) {

					spreadDays = tempSpreadDays;
					CasesByDay.clear();
					for (int i = 0; i < tempCasesByDay.size(); i++)
						CasesByDay.push_back(tempCasesByDay.at(i));

					startday = tempstartday;
					startmonth = tempstartmonth;
					startyear = tempstartyear;

					endday = tempendday;
					endmonth = tempendmonth;
					endyear = tempendyear;
				}

				cur1 = cur1->next;
			}

			cout << endl << endl << "Longest Spread for " << cname << " is " << spreadDays << " days between " << startday << "-" << startmonth << "-" << startyear << " and " << endday << "-" << endmonth << "-" << endyear << " and the increase is as follows: " << endl;
			for (int i = 0; i < CasesByDay.size(); i++) {
				cout << CasesByDay.at(i) << " ";
			}
			cout <<endl << endl;
		}
		else
			cout << "Error: Country Does not exist..." << endl << endl;
	}

	int findLowest(vector<int>V) {
		int lowest = 99999;
		for (int i = 0; i < V.size(); i++) {
			if (V.at(i) < lowest)
				lowest = V.at(i);
		}
		return lowest;
	}

	void print_all(const vector<int> seq, const vector<int>& len, int max, int pos, vector<int>& sofar, vector<int>& temp) {
		if (max == 0) {
			for (int i = sofar.size() - 1; i >= 0; i--) {
				if (sofar.size() > temp.size())
					temp.push_back(sofar.at(i));
			}
			return;
		}
		int val = pos < seq.size() ? seq[pos] : -1;
		for (int i = pos - 1; i >= 0; i--) {
			if (len[i] == max && seq[i] > val) {
				sofar.push_back(seq[i]);
				print_all(seq, len, max - 1, i, sofar, temp);
				sofar.erase(sofar.end() - 1);
			}
		}
	}

	void findDeathToll(string country) {

		// create a vector of nodes of the particular country and then use 3 loops on that vector adn keep storing results in another vector and updating them

		int ind = -1;
		for (int i = 0; i < sizeCountryList; i++) {
			if (countryList[i].head->countryName == country) {
				ind = i;
				break;
			}
		}

		if (ind != -1) {

			node* cur = countryList[ind].head;
			int counter = 0;
			while (cur != NULL) {
				counter++;
				cur = cur->next;
			}

			int* data = new int[counter];

			cur = countryList[ind].head;
			counter = 0;
			while (cur != NULL) {
				data[counter] = cur->deaths;
				counter++;
				cur = cur->next;
			}

			vector<int> seq(data, data + counter);
			vector<int> len(seq.size());
			vector<int>temp;
			for (int i = 0; i < seq.size(); i++) {
				len[i] = 1;
				for (int j = i - 1; j >= 0; j--)
					if (seq[j] > seq[i] && len[j] + 1 > len[i])
						len[i] = len[j] + 1;
			}
			int max = *max_element(len.begin(), len.end());
			vector<int> sofar;

			print_all(seq, len, max, seq.size(), sofar, temp);

			cout << endl;
			cout << "Longest death toll decrease for " << country << " is " << temp.size() << " days." << endl << "Death toll: {";
			for (int i = 0; i < temp.size(); i++) {
				if (i < temp.size() - 1)
					cout << temp.at(i) << ",";
				else if (i == temp.size() - 1)
					cout << temp.at(i);
			}
			cout << "}" << endl;

		}

		else
			cout << "Error: Country does not exist..." << endl << endl;
	}

	int max(int a, int b) { return (a > b) ? a : b; }

	void Knapsack() {

		vector<int>save;

		int* profit = new int[CountryCost.size()];
		for (int i = 0; i < LastDayConfirmed.size(); i++)
			profit[i] = LastDayConfirmed.at(i).second;

		int* weight = new int[CountryCost.size()];
		for (int i = 0; i < CountryCost.size(); i++)
			weight[i] = CountryCost.at(i).second;

		int W = Budget;

		int i, w;

		int** matrix = new int* [CountryCost.size() + 1];
		for (int i = 0; i < CountryCost.size() + 1; i++)
			matrix[i] = new int[Budget + 1];

		for (i = 0; i <= CountryCost.size(); i++) {
			for (w = 0; w <= W; w++) {
				if (i == 0 || w == 0)
					matrix[i][w] = 0;
				else if (weight[i - 1] <= w) {
					matrix[i][w] = max(profit[i - 1] + matrix[i - 1][w - weight[i - 1]], matrix[i - 1][w]);
				}
				else
					matrix[i][w] = matrix[i - 1][w];
			}
		}

		int res = matrix[CountryCost.size()][W];
		cout << endl << "Final Score: " << res << endl << endl;

		w = W;
		for (i = CountryCost.size(); i > 0 && res > 0; i--) {

			if (res == matrix[i - 1][w])
				continue;

			else {

				save.push_back(i - 1);

				res = res - profit[i - 1];
				w = w - weight[i - 1];
			}
		}

		cout << "Following countries are selected: " << endl << endl;
		cout << "Cost			" << "Country" << endl << endl;
		int combined = 0;
		for (int i = 0; i < CountryCost.size(); i++) {
			for (int j = 0; j < save.size(); j++) {
				if (i == save.at(j)) {
					combined += CountryCost.at(i).second;
					cout << CountryCost.at(i).second << "			" << CountryCost.at(i).first << endl;
				}
			}
		}
		cout << endl << "Combined budget spent : " << combined << endl << endl;
	}

	void FindMaxLength(int A[], int B[], int n, int m, string country1, string country2) {
		int** dp = new int* [n + 1];
		for (int i = 0; i < n + 1; i++)
			dp[i] = new int[m + 1];

		for (int i = 0; i <= n; i++)
			for (int j = 0; j <= m; j++)
				dp[i][j] = 0;

		for (int i = n - 1; i >= 0; i--)
		{
			for (int j = m - 1; j >= 0; j--)
			{
				if (abs(A[i] - B[j]) <= 10) {
					dp[i][j] = dp[i + 1][j + 1] + 1;
				}
			}
		}
		int maxm = 0;

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				maxm = max(maxm, dp[i][j]);
			}
		}

		int r, c;
		for (int i = 0; i <= n; i++) {
			for (int j = 0; j <= m; j++) {
				if (dp[i][j] == maxm) {
					r = i;
					c = j;
				}
			}
		}

		vector<int>C1;
		vector<int>C2;
		int counter = 1;
		for (int i = r, j = c; i <= n && j <= m; i++, j++) {
			if (counter <= maxm) {
				C2.push_back(i);
				C1.push_back(j);
			}
			counter++;
		}

		int ind = 0;
		cout << endl << "Similarity ( \"" << country1 << "\" " << "," << " \"" << country2 << "\" )=" << maxm << " days, active cases were as follows..." << endl << endl;
		cout << country1 << ":" << endl;
		counter = 0;

		for (int i = 0; i < sizeCountryList; i++) {
			if (countryList[i].head->countryName == country1) {
				ind = i;
				break;
			}
		}

		node* cur = countryList[ind].head;
		while (cur != NULL) {
			if (counter >= C1.at(0) && counter <= C1.at(C1.size() - 1)) {
				cout << cur->confirmed << " ";
			}
			cur = cur->next;
			counter++;
		}
		cout << endl << endl;

		cout << country2 << ":" << endl;
		counter = 0;
		for (int i = 0; i < sizeCountryList; i++) {
			if (countryList[i].head->countryName == country2) {
				ind = i;
				break;
			}
		}

		counter = 0;
		cur = countryList[ind].head;
		while (cur != NULL) {
			if (counter >= C2.at(0) && counter <= C2.at(C2.size() - 1)) {
				cout << cur->confirmed << " ";
			}
			cur = cur->next;
			counter++;
		}
		cout << endl << endl;
	}

	void compareCases(string country1, string country2) {
		int size1 = 0;// number of cases in first country ( to be updated )
		int size2 = 0;// number of cases in second country ( to be updated )
		int ind1 = -1;
		int ind2 = -1;
		bool check = true;
		vector<int>Country1;
		vector<int>Country2;

		for (int i = 0; i < sizeCountryList; i++) {
			if (countryList[i].head->countryName == country1) {
				ind1 = i;
				break;
			}
		}

		for (int i = 0; i < sizeCountryList; i++) {
			if (countryList[i].head->countryName == country2) {
				ind2 = i;
				break;
			}
		}

		if (ind1 != -1 && ind2 != -1) {

			for (int i = 0; i < sizeCountryList; i++) {
				if (countryList[i].head->countryName == country1)
					ind1 = i;

				else if (countryList[i].head->countryName == country2)
					ind2 = i;
			}

			node* cur = countryList[ind1].head;
			while (cur != NULL) {
				size1 += 1;
				cur = cur->next;
			}

			cur = countryList[ind2].head;
			while (cur != NULL) {
				size2 += 1;
				cur = cur->next;
			}

			int* c1 = new int[size1];
			int* c2 = new int[size2];

			int in = 0;
			cur = countryList[ind1].head;
			while (cur != NULL) {
				c1[in] = cur->confirmed;
				cur = cur->next;
				in++;
			}

			in = 0;
			cur = countryList[ind2].head;
			while (cur != NULL) {
				c2[in] = cur->confirmed;
				cur = cur->next;
				in++;
			}

			if (size1 >= size2)
				FindMaxLength(c1, c2, size1, size2, country2, country1);
			else
				FindMaxLength(c2, c1, size2, size1, country1, country2);

		}

		else
			cout << "Error: Country does not exist..." << endl << endl;
	}

	~Graph() {

	}
};

int main()
{
	vector<string> countries;
	vector<string>date;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ifstream fs("WHO-COVID-19.csv");
	if (fs.is_open()) {
		string read;
		bool check;
		bool flag = true;
		getline(fs, read);

		while (!fs.eof()) {
			flag = true;
			string d;
			check = true;
			getline(fs, read, ',');
			d = read;
			getline(fs, read, ',');
			getline(fs, read, ',');

			if (read != "undefined") {

				if (read[0] != '\"') {

					if (countries.size() == 0) {
						countries.push_back(read);
						check = false;
					}

					if (date.size() == 0) {
						date.push_back(d);
						flag = false;
					}

					else {
						for (int i = 0; i < countries.size(); i++) {
							if (countries.at(i) == read)
								check = false;
						}

						for (int i = 0; i < date.size(); i++) {
							if (date.at(i) == d)
								flag = false;
						}
					}

					if (check == true)
						countries.push_back(read);

					if (flag == true)
						date.push_back(d);
				}
				else {
					string read1;
					getline(fs, read1, ',');

					string combine;

					for (int i = 0; read[i] != '\0'; i++)
						combine += read[i];

					combine += ",";

					for (int i = 0; read1[i] != '\0'; i++)
						combine += read1[i];

					if (countries.size() == 0) {
						countries.push_back(combine);
						check = false;
					}

					if (date.size() == 0) {
						date.push_back(d);
						flag = false;
					}

					else {
						for (int i = 0; i < countries.size(); i++) {
							if (countries.at(i) == combine)
								check = false;
						}
						for (int i = 0; i < date.size(); i++) {
							if (date.at(i) == d)
								flag = false;
						}
					}

					if (check == true)
						countries.push_back(combine);

					if (flag == true)
						date.push_back(d);
				}

			}

			getline(fs, read, ',');
			getline(fs, read, ',');
			getline(fs, read, ',');
			getline(fs, read, ',');
			getline(fs, read, '\n');

		}

	}
	else {
		cout << "File not opened..." << endl;
	}

	fs.close();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Graph G;
	G.CountryList(countries.size());

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ifstream fs1("WHO-COVID-19.csv");

	int index = 0;
	if (fs1.is_open()) {
		string read;
		getline(fs1, read);

		while (!fs1.eof()) {

			// reading date and chopping it down into day,month and year
			getline(fs1, read, ',');
			string year, month, day;

			int i = 0;
			for (i = 0; read[i] != '-'; i++)
				year += read[i];

			i = i + 1;
			for (i; read[i] != '-'; i++)
				month += read[i];

			i = i + 1;
			for (i; read[i] != '\0'; i++)
				day += read[i];

			stringstream ss(year);
			int yearint = 0;
			ss >> yearint;

			stringstream ss1(month);
			int monthint = 0;
			ss1 >> monthint;

			stringstream ss2(day);
			int dayint = 0;
			ss2 >> dayint;

			getline(fs1, read, ',');
			string ccode = read;

			getline(fs1, read, ',');
			string cname;

			if (read != "undefined") {

				if (read[0] != '\"') {
					cname = read;
				}

				else {
					string read2;
					getline(fs1, read2, ',');

					string combine;

					for (int i = 0; read[i] != '\0'; i++)
						combine += read[i];

					combine += ",";

					for (int i = 0; read2[i] != '\0'; i++)
						combine += read2[i];

					cname = combine;
				}

			}

			else if (read == "undefined") {
				cname = "undefined";
			}

			getline(fs1, read, ',');
			string reg = read;

			int deathint = 0;
			int confirmedcumint = 0;
			int deathcumint = 0;
			int confirmedint = 0;

			getline(fs1, read, ',');
			if (cname != "undefined") {
				stringstream ss3(read);
				ss3 >> deathint;
			}

			getline(fs1, read, ',');
			if (cname != "undefined") {
				stringstream ss4(read);
				ss4 >> deathcumint;
			}

			getline(fs1, read, ',');
			if (cname != "undefined") {
				stringstream ss5(read);
				ss5 >> confirmedint;
			}

			getline(fs1, read, '\n');
			if (cname != "undefined") {
				stringstream ss6(read);
				ss6 >> confirmedcumint;
			}

			if (cname != "undefined") {
				if (cname == countries.at(index)) {
					G.createCountryList(yearint, monthint, dayint, ccode, cname, reg, deathint, deathcumint, confirmedint, confirmedcumint, index);
				}

				else {
					index++;
					G.createCountryList(yearint, monthint, dayint, ccode, cname, reg, deathint, deathcumint, confirmedint, confirmedcumint, index);
				}
			}
		}
	}

	else {
		cout << "File not opened..." << endl;
	}

	//G.DisplayCountryList(); uncomment this

	fs1.close();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	G.DateList(date.size());
	G.createDateList();

	ifstream fs2("CountryWeight.csv");
	string country1;
	string cost;
	vector<int>Cost;

	if (fs2.is_open()) {
		getline(fs2, country1);
		while (!fs2.eof()) {
			getline(fs2, country1, ',');

			if (country1[0] == '\"') {
				getline(fs2, country1, ',');
			}

			getline(fs2, cost, '\n');

			stringstream sss(cost);
			int cost = 0;
			sss >> cost;

			Cost.push_back(cost);
		}
	}
	else
		cout << "Error: File not opened..." << endl;

	Cost.pop_back();
	G.createCountryCost(countries, Cost);

	int choice;
	bool end = true;
	cout << endl;
	cout << "What query do you want to execute?" << endl;
	cout << "1. Top 20 countries with the most confirmed cases on a given date. " << endl;
	cout << "2. Country(s) with the most cases between two given dates. " << endl;
	cout << "3. Longest spread period of a given country. " << endl;
	cout << "4. Longest Death toll decrease of a given country. " << endl;
	cout << "5. Find Countries selected for aid. " << endl;
	cout << "6. Compare active cases of two countries. " << endl;
	cout << "7. Exit. " << endl;
	cin >> choice;

	while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 && choice != 7) {
		cout << endl << "Invalid input! Try again..." << endl;
		cin >> choice;
	}

	while (end) {
		if (choice == 1) {

			int day, month, year;
			cout << endl << "Enter a specific day: " << endl;
			cin >> day;
			cout << "Enter a specific month: " << endl;
			cin >> month;
			cout << "Enter a specific year: " << endl;
			cin >> year;

			G.Top20Confirmed(day, month, year);
			cout << endl;

		}

		else if (choice == 2) {

			int day, month, year, day1, month1, year1;
			cout << endl << "Enter starting day: " << endl;
			cin >> day;
			cout << "Enter starting month: " << endl;
			cin >> month;
			cout << "Enter starting year: " << endl;
			cin >> year;

			cout << endl;

			cout << "Enter ending day: " << endl;
			cin >> day1;
			cout << "Enter ending month: " << endl;
			cin >> month1;
			cout << "Enter ending year: " << endl;
			cin >> year1;

			G.HighestNewCases(day, month, year, day1, month1, year1);
			cout << endl;

		}

		else if (choice == 3) {

			string country;
			cout << endl << "Enter name of country for which you want to find the longest spread period: ";
			cin.ignore();
			getline(cin, country);
			G.findLongestSpread(country);

		}

		else if (choice == 4) {

			string country;
			cout << endl << "Enter name of country for which you want to find the longest death toll decrease period: ";
			cin.ignore();
			getline(cin, country);
			G.findDeathToll(country);
			cout << endl;

		}

		else if (choice == 5) {

			G.Knapsack();

		}

		else if (choice == 6) {

			string c1, c2;
			cout << endl << "Enter name of first country: " << endl;
			cin.ignore();
			getline(cin, c1);
			cout << endl << "Enter name of second country: " << endl;
			getline(cin, c2);

			G.compareCases(c1, c2);

		}

		else if (choice == 7) {

			end = false;

		}

		cout << "Is there any other query you want to execute?" << endl;
		cout << "1. Top 20 countries with the most confirmed cases on a given date. " << endl;
		cout << "2. Country(s) with the most cases between two given dates. " << endl;
		cout << "3. Longest spread period of a given country. " << endl;
		cout << "4. Longest Death toll decrease of a given country. " << endl;
		cout << "5. Find Countries selected for aid. " << endl;
		cout << "6. Compare active cases of two countries. " << endl;
		cout << "7. Exit. " << endl;
		cin >> choice;

		while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 && choice != 7) {
			cout << endl << "Invalid input! Try again..." << endl;
			cin >> choice;
		}

		if (choice == 7) {
			end = false;
			cout << endl << "-------------------------------------------- Goodbye and stay clean! ---------------------------------------------------" << endl << endl;
		}
		else
			system("cls");

	}
}
