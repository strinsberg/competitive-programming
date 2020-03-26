///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 8
// UVA 10511
//
// Steven Deutekom
// March 25 2020
//
// Used network flow approach described in the video.
// Constructing the graph was the harder part. Elected to get all input and
// store the names for people, clubs, and parties in maps with the value
// being the index that they were first encountered. These indicies were
// then used when making the node numbers. Also, kept some reverse maps
// to get the names back from a node number when finding edges.
//
// Spent many hourse trying to figure out a mistake because the output can
// be correct but, not match another persons solution. Turned out to be the
// period after Impossible missing. Sigh!!!
//
// Also, sorry I did it all in one function. Sometimes I code it that way
// and then split it up for ease of marking, but I didn't want to break it
// after all the trouble that period gave me.
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

// Network flow library code //////////////////////////////////////////

struct Edge;
typedef list<Edge>::iterator EdgeIter;

struct Edge {
  int to;
  int cap;
  int flow;
  bool is_real;
  EdgeIter partner;
  
  Edge(int t, int c, bool real = true)
    : to(t), cap(c), flow(0), is_real(real)
  {};

  int residual() const
  {
    return cap - flow;
  }
};

struct Graph {
  list<Edge> *nbr;
  int num_nodes;
  Graph(int n)
    : num_nodes(n)
  {
    nbr = new list<Edge>[num_nodes];
  }

  ~Graph()
  {
    delete[] nbr;
  }

  // note: this routine adds an edge to the graph with the specified capacity,
  // as well as a residual edge.  There is no check on duplicate edge, so it
  // is possible to add multiple edges (and residual edges) between two
  // vertices
  void add_edge(int u, int v, int cap)
  {
    nbr[u].push_front(Edge(v, cap));
    nbr[v].push_front(Edge(u, 0, false));
    nbr[v].begin()->partner = nbr[u].begin();
    nbr[u].begin()->partner = nbr[v].begin();
  }
};

void push_path(Graph &G, int s, int t, const vector<EdgeIter> &path, int flow)
{
  for (int i = 0; s != t; i++) {
    if (path[i]->is_real) {
      path[i]->flow += flow;
      path[i]->partner->cap += flow;
    } else {
      path[i]->cap -= flow;
      path[i]->partner->flow -= flow;
    }
    s = path[i]->to;
  }
}

// the path is stored in a peculiar way for efficiency: path[i] is the
// i-th edge taken in the path.
int augmenting_path(const Graph &G, int s, int t, vector<EdgeIter> &path,
		    vector<bool> &visited, int step = 0)
{
  if (s == t) {
    return -1;
  }
  for (EdgeIter it = G.nbr[s].begin(); it != G.nbr[s].end(); ++it) {
    int v = it->to;
    if (it->residual() > 0 && !visited[v]) {
      path[step] = it;
      visited[v] = true;
      int flow = augmenting_path(G, v, t, path, visited, step+1);
      if (flow == -1) {
	return it->residual();
      } else if (flow > 0) {
	return min(flow, it->residual());
      }
    }
  }
  return 0;
}

// note that the graph is modified
int network_flow(Graph &G, int s, int t)
{
  vector<bool> visited(G.num_nodes);
  vector<EdgeIter> path(G.num_nodes);
  int flow = 0, f;

  do {
    fill(visited.begin(), visited.end(), false);
    if ((f = augmenting_path(G, s, t, path, visited)) > 0) {
      push_path(G, s, t, path, f);
      flow += f;
    }
  } while (f > 0);
  
  return flow;
}


// My code ////////////////////////////////////////////////////////////

// Person struct to keep track of all info on each person
struct Person {
  string name;
  string party;
  vector<string> clubs;
};


int main() {
  ios_base::sync_with_stdio(0); cin.tie(NULL);

  // Get input
  int n; cin >> n;
  cin.ignore();
  cin.ignore();
  
  while (n--) {
    string line;
    getline(cin, line);
    
    vector<Person> people;     // All the people with their info
    map<string, int> member;   // All names mapped to indicies
    map<string, int> party;    // All party names mapped to indicies
    map<string, int> club;     // All club names mapped to indicies
    map<int, string> club_idx; // Indicies mapped to club names

    while (line != "" and !cin.eof()) {
      // Create a person from the input
      Person p;
      stringstream ss(line);
      ss >> p.name;
      ss >> p.party;
      
      string c;
      while (ss >> c) {
        p.clubs.push_back(c);
        // Add clubs to the club maps
        if (club.find(c) == club.end()) {
          club[c] = club.size();
          club_idx[club[c]] = c;
        }
      }

      // Add people and meber names to maps
      if (member.find(p.name) == member.end())
        member[p.name] = member.size();

      if (party.find(p.party) == party.end())
        party[p.party] = party.size();

      // Add the person and get next line
      people.push_back(p);
      getline(cin, line);
    }


    // Build graph
    int m,p,c;
    m = member.size();
    p = party.size();
    c = club.size();

    Graph G(m+p+c+2); 


    // Add all the edges. Node structure as described in video listed below.
    // s = 0, t = 1
    // clubs = 2 -> c + 1, members = c + 2 -> c + m + 1
    // parties = c + m + 2 -> c + m + p + 1

    // Add source edges
    for (int i = 2; i < c + 2; i++)
      G.add_edge(0, i, 1);

    // Add sink edges
    for (int i = 0; i < p; i++)
      G.add_edge(c + m + 2 + i, 1, (c - 1) / 2);

    // Add edges from clubs to people to parties
    for (auto & peep : people) {
      G.add_edge(c + 2 + member[peep.name], c + m + 2 + party[peep.party], 1);
      for (auto & cl : peep.clubs)
        G.add_edge(2 + club[cl], c + 2 + member[peep.name], 1);
    }


    // Find flow and reconstruct the club representatives if possible
    int flow = network_flow(G, 0, 1);

    if (flow == c) {
      for (int i = 2; i < c + 2; i++) {
        // Check each edge to see wich ones are being used
        for (auto & e : G.nbr[i]) {
          if (e.is_real and e.flow > 0) 
            cout << people[e.to - c - 2].name << " " << club_idx[i - 2] << endl;
        }
      }
    } else {
      cout << "Impossible." << endl;
    }

    cout << endl;
  }
  return 0;
}
