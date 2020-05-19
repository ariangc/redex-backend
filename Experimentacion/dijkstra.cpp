#include <bits/stdc++.h>
using namespace std;
#define sz(x) int(x.size())
typedef pair<int, int> ii;

//Important constants for implementation

const int N = 41; //Max number of Airports
const int M = 3600; //Max number of Flights
const int INF = 1e9; //Infinity


// Airport struct used to store airport data 
struct Airport{
	string icao, continent, country, city, code;
	Airport(){}
	Airport(vector<string> &v){
		icao = v[0], continent = v[1], country = v[2];
		city = v[3], code = v[4];
	}
};

//Triplet structure (used to implement dijkstra)
struct T{
	int itm1,itm2, itm3;
	T(){}
	T(int x, int y, int z):itm1(x),itm2(y),itm3(z){}
	bool operator < (const T& x) const{
		if(itm1 != x.itm1)
			return itm1 < x.itm1;
		if(itm2 != x.itm2)
			return itm2 < x.itm2;
		return itm3 < x.itm3;
	}
	bool operator == (const T& x) const{
		return (itm1 == x.itm1) and (itm2 == x.itm2) and (itm3 == x.itm3);
	}
	bool operator > (const T& x) const{
		return !((*this < x) || (*this == x));
	}
};

/*** GLOBAL VARIABLE DECLARATIONS ***/

T flightInfo[M];
int flightId[N][N][1440], fId = 0;
map<ii, int> mapa;
vector<T> adj[N];
vector<Airport> airports;
map<string, int> id;
int n_airp, n_flights;

/*** HELPER ROUTINES ***/

//Gets total time between two given times (expressed in minutes)
int getTotal(int start_time, int end_time){ 
	if(start_time > end_time){
		return 1440 - (start_time - end_time);
	}
	else return end_time - start_time;
}

//Gets time expressed in minutes given string hh:mm
int getTime(string &s){
	int h = atoi(s.substr(0, 2).c_str());
	int m = atoi(s.substr(3, 2).c_str());
	return h*60 + m;
}

//Gets two-digit number expressed in string 
string getstr(int x){
	string ret = "";
	while(x != 0){
		ret += (char)(x % 10 + '0');
		x /= 10;
	}
	while(sz(ret) < 2) ret += '0';
	reverse(ret.begin(), ret.end());
	return ret;
}

//Gets hh:mm format given time expressed in minutes
string getHour(int x){
	string ret = "";
	int h = x / 60;
	int m = x % 60;
	ret += getstr(h);
	ret += ':';
	ret += getstr(m);
	return ret;
}

//Adds Edge between two nodes (cities) by a given flight info
void addEdge(int from, int to, int start_time, int end_time){
	int total_time = getTotal(start_time, end_time);
	adj[from].push_back(T(to, start_time, total_time));
	flightId[from][to][start_time] = fId;
	flightInfo[fId++] = T(from, to, start_time);
}

void read_data(){
	cin >> n_airp >> n_flights;
	string ig; getline(cin, ig);
	for(int idx = 0; idx < n_airp; ++ idx){
		vector<string> v;
		for(int j = 0; j < 5; ++ j) {
			string str; 
			char sep = (j == 4 ? '\n' : ',');
			getline(cin,str, sep);
			v.push_back(str);
		}
		airports.push_back(Airport(v));
		id[v[0]] = idx;
	}
	for(int idx = 0; idx < n_flights; ++ idx){
		vector<string> v;
		for(int j = 0; j < 4; ++ j){
			string str;
			char sep = (j == 3 ? '\n' : ',');
			getline(cin,str, sep);
			v.push_back(str);
		}
		int from = id[v[0]], to = id[v[1]];
		int start_time = getTime(v[2]), end_time = getTime(v[3]);
		addEdge(from, to, start_time, end_time);
		mapa[make_pair(from, to)]++;
	}
}

void print_data(){
	for(int i = 0; i < sz(airports); ++ i){
		for(auto vv: adj[i]){
			int dest = vv.itm1, departure = vv.itm2, duration = vv.itm3;
			cout << "From " << airports[i].city << " to " << airports[dest].city << ": ";
			cout << "Departs at " << getHour(departure) << " and total time is " << duration << "min." << endl; 
		}
	}
	for(auto x: mapa){
		int from = x.first.first, to = x.first.second;
		cout << "From " << airports[from].city << " to " << airports[to].city << ": ";
		cout << x.second << endl;
	}
	cout << "Total direct flights: " << sz(mapa) << endl;
}

const int shutdown_prob = 1; //1 out of 1000
const int cancel_prob = 70; //1.3%
const int delay_prob = 24; //24.38%

int flightIssues[M]; //-1 if cancelled, > 0 if delayed

vector<ii> issues;
int badAirport; //-1 if nothing, >= 0 if there's one off

void set_preconditions(int src, int snk){
	issues.clear();
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());	
	
	int shutdown_roll = rng()%1001;
	if(shutdown_roll <= shutdown_prob){
		badAirport = rng()%n_airp;
		while(badAirport == src || badAirport == snk){
			badAirport = rng()%n_airp;
		}
	}
	else badAirport = -1;

	vector<int> flight_pool(fId);
	for(int i = 0; i < fId; ++ i) flight_pool[i] = i;
	
	//delay_prob% of the flights will be delayed no more than 60 minutes
	//cancel_prob% of the flights will be cancelled

	//Delayed flights
	int cntDelayed = delay_prob * fId / 100;
	for(int i = 0; i < cntDelayed; ++ i){
		int pos = rng()%sz(flight_pool);
		int delayed = flight_pool[pos];
		flight_pool.erase(flight_pool.begin() + pos);
		issues.push_back({delayed, rng()%60 + 1});
	}

	//Cancelled flights
	int cntCancelled = cancel_prob * fId / 100;
	for(int i = 0; i < cntCancelled; ++ i){
		int pos = rng()%sz(flight_pool);
		int cancelled = flight_pool[pos];
		flight_pool.erase(flight_pool.begin() + pos);
		issues.push_back({cancelled, -1});
	}
}

void apply_preconditions(){
	for(ii x: issues){
		flightIssues[x.first] += x.second;
	}
}

void reverse_preconditions(){
	for(ii x: issues){
		flightIssues[x.first] -= x.second;
	}
}

void print_preconditions(){
	cout << "Delayed flights: " << endl;
	for(int i = 0; i < n_flights; ++ i){
		if(flightIssues[i] > 0){
			int from = flightInfo[i].itm1, to = flightInfo[i].itm2, start_time = flightInfo[i].itm3;
			cout << getHour(start_time) << " (" << getHour((start_time + flightIssues[i])%1440) << ") from ";
			cout << airports[from].city << " to " << airports[to].city << endl;
		}
	}
	cout << "Cancelled flights: " << endl;
	for(int i = 0; i < n_flights; ++ i){
		if(flightIssues[i] == -1){
			int from = flightInfo[i].itm1, to = flightInfo[i].itm2, start_time = flightInfo[i].itm3;
			cout << getHour(start_time) << " from ";
			cout << airports[from].city << " to " << airports[to].city << endl;
		}
	}
	if(badAirport != -1) cout << "Closed Airport: " << airports[badAirport].city << endl;
}

int dist[N], on_air[N];
int p[N];

int dijkstra(int arrival, int src, int snk){
	for(int i = 0; i < n_airp; ++ i) dist[i] = INF, on_air[i] = INF, p[i] = -1;
	priority_queue<ii, vector<ii>, greater<ii>> pq;
	dist[src] = on_air[src] = 0; pq.push({dist[src], src});
	while(!pq.empty()){
		ii fr = pq.top(); pq.pop();
		int d = fr.first, u = fr.second;
		if(d > dist[u]) continue;
		int current_time = (arrival + d) % 1440;
		for(auto vv: adj[u]){
			int to = vv.itm1, start_time = vv.itm2, duration = vv.itm3;
			if(to == badAirport) continue; //Shutdown on this airport
			int flight_id = flightId[u][to][start_time];
			
			int wait_time = getTotal(current_time, start_time); // Wait time
			if(flightIssues[flight_id] == -1) continue; //Cancelled Flight
			else wait_time += flightIssues[flight_id];

			if(dist[to] > dist[u] + wait_time + duration){
				dist[to] = dist[u] + wait_time + duration;
				on_air[to] = on_air[u] + duration;
				p[to] = u;
				pq.push({dist[to], to});
			}
		}
	}
	return dist[snk];
}

int main(){
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());	
	read_data();
	//print_data();
	int n_tests = 1000;
	int src = rng()%n_airp, snk = src;
	while(src == snk) snk = rng()%n_airp;

	cerr << "Origin,Destination,N_flights,Flight_time,Wait_time,Total_time" << endl;
	while(n_tests > 0){
		set_preconditions(src, snk);
		apply_preconditions();
		print_preconditions();
		int arrival = rng()%1440;
		int result = dijkstra(arrival, src, snk);
		
		int cur = snk;
		vector<int> route;
		while(cur != src){
			route.push_back(cur);
			cur = p[cur];
		}
		route.push_back(cur);
		reverse(route.begin(), route.end());
			
		//Pretty format to stdout

		cout << getHour(arrival) << " From " << airports[src].city << "(" << airports[src].icao << ") ";
		cout <<"to " << airports[snk].city << "(" << airports[snk].icao << "): ";
		if(result != INF) cout << result << " minutes" << endl;
		else cout << "IMPOSSIBLE" << endl;
		int flight_time = on_air[snk];
		int wait_time = result - on_air[snk];
		cout << "Flight time: " << flight_time << "minutes" << endl;
		cout << "Wait time: " << wait_time << "minutes" << endl;
		
		cout << "Route: ";
		for(int i = 0; i < sz(route); ++ i){
			cout << airports[route[i]].city;
			if(i == sz(route) - 1) cout << endl;
			else cout << " -> ";
		}

		//CSV to stderr
		cerr << airports[src].icao << "," << airports[snk].icao << "," << sz(route)-1 << "," << flight_time << ",";
		cerr << wait_time << "," << result << endl;
		

		reverse_preconditions();
		n_tests--;

	}
	return 0;
}
