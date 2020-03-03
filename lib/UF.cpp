/*
 * Josips simple Union Find code
 *
 * Supports merges and connection queries in logN.
 * Do not need an edge class just add the verticies.
 * The Stucture is 0 based.
 */
struct UF {
  int n; vector<int> A;
  UF (int n) : n(n), A(n) { iota(all(A), 0); }
  int find (int a) { return a == A[a] ? a : A[a] = find(A[a]); }
  bool connected (int a, int b) { return find(a) == find(b); }
  void merge (int a, int b) { A[find(b)] = find(a); }
};
