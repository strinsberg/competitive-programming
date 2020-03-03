// Needs a full re-write
#include<bits/stdc++.h>
using namespace std;

struct Team {
  vector<string> pref;
  vector<string> players;
  int idx=0;
};

int main(){
  int n,k;
  int Index=0;
  cin>>n>>k;
  vector<Team> teams;
  vector<string> pool;
  set<string> drafted;
  
  for(int i=0; i<n; i++) {
    teams.push_back(Team());
    int teamJ;
    cin>>teamJ;
    for(int j=0; j<teamJ; j++) {
      string temp;
      cin>>temp;
      teams[i].pref.push_back(temp);
    }
  }

  int p;
  cin>>p;
  for(int i=0; i<p; i++) {
    string temp;
    cin>>temp;
    pool.push_back(temp);
  }

  for(int a=0; a<k; a++) { 
  for(int i=0; i<teams.size(); i++) {
    bool picked = false;
    for(teams[i].idx; teams[i].idx<teams[i].pref.size(); teams[i].idx++) {
      if(drafted.find(teams[i].pref[teams[i].idx])==drafted.end()){
    teams[i].players.push_back(teams[i].pref[teams[i].idx]);
    drafted.insert(teams[i].pref[teams[i].idx]);
    picked = true;
    break;
      }
    }
    if(!picked){
      for(int j=Index; j<pool.size(); j++) {
    if(drafted.find(pool[j])==drafted.end()){
      Index = j;
      teams[i].players.push_back(pool[j]);
      drafted.insert(pool[j]);
      break;
    }
      }
    }
    picked = false;
  }
  }

  for(auto i:teams) {
    for(int j=0; j<i.players.size(); j++) {
      cout<<i.players[j]<<" ";
    }
    cout<<endl;
  }
}
