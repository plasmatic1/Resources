#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFind.h"
using namespace std;

// Computes the minimum spanning tree (or forest) using Kruskal's algorithm
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges
// Constructor Arguments:
//   V: number of vertices in the graph
//   edges: a vector of tuples in the form (v, w, weight) representing
//     an undirected edge in the graph between vertices v and w with
//     weight of weight
// Fields:
//   mstWeight: the weight of the mst
//   mstEdges: a vector of tuples of the edges in the mst
// In practice, has a small constant, faster than Prim and Boruvka
// Time Complexity:
//   constructor: O(V + E log E)
// Memory Complexity: O(V + E)
// Tested:
//   Stress Tested
//   https://open.kattis.com/problems/minspantree
template <class T> struct KruskalMST {
  using Edge = tuple<int, int, T>;
  T mstWeight; vector<Edge> mstEdges; UnionFind uf;
  KruskalMST(int V, vector<Edge> edges) : mstWeight(), uf(V) {
    sort(edges.begin(), edges.end(), [&] (const Edge &a, const Edge &b) {
      return get<2>(a) < get<2>(b);
    });
    for (auto &&e : edges) {
      if (int(mstEdges.size()) >= V - 1) break;
      if (uf.join(get<0>(e), get<1>(e))) {
        mstEdges.push_back(e); mstWeight += get<2>(e);
      }
    }
  }
};
