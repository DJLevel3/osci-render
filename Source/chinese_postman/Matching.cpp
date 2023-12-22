#include "Matching.h"
#include <deque>
#include <stack>

Matching::Matching(Graph & G):
	G(G),
	outer(2*G.GetNumVertices()),
	deep(2*G.GetNumVertices()),
	shallow(2*G.GetNumVertices()),
	tip(2*G.GetNumVertices()),
	active(2*G.GetNumVertices()),
	type(2*G.GetNumVertices()),
	forest(2*G.GetNumVertices()),
	root(2*G.GetNumVertices()),
	blocked(2*G.GetNumVertices()),
	dual(2*G.GetNumVertices()),
	slack(G.GetNumEdges()),
	mate(2*G.GetNumVertices()),
	m(G.GetNumEdges()),
	n(G.GetNumVertices()),
	visited(2*G.GetNumVertices())
{
}

void Matching::Grow()
{
	Reset();

	//All unmatched vertices will be roots in a forest that will be grown
	//The forest is grown by extending a unmatched vertex w through a matched edge u-v in a BFS fashion
	while(!forestList.empty())
	{
		int w = outer[forestList.front()];
		forestList.pop_front();

		//w might be a blossom
		//we have to explore all the connections from vertices inside the blossom to other vertices
		for(int u : deep[w]) {

			int cont = false;
			for(int v : G.AdjList(u)) {

				if(IsEdgeBlocked(u, v)) continue;

				//u is even and v is odd
				if(type[outer[v]] == ODD) continue;	

				//if v is unlabeled
				if(type[outer[v]] != EVEN)
				{
					//We grow the alternating forest
					int vm = mate[outer[v]];

					forest[outer[v]] = u;
					type[outer[v]] = ODD;
					root[outer[v]] = root[outer[u]];
					forest[outer[vm]] = v;
					type[outer[vm]] = EVEN;
					root[outer[vm]] = root[outer[u]];

					if(!visited[outer[vm]])
					{
						forestList.push_back(vm);
						visited[outer[vm]] = true;
					}
				}
				//If v is even and u and v are on different trees
				//we found an augmenting path
				else if(root[outer[v]] != root[outer[u]])
				{
					Augment(u,v);
					Reset();

					cont = true;
					break;
				}
				//If u and v are even and on the same tree
				//we found a blossom
				else if(outer[u] != outer[v])
				{
					int b = Blossom(u,v);

					forestList.push_front(b);
					visited[b] = true;

					cont = true;
					break;
				} 
			}
			if(cont) break;
		}
	}

	//Check whether the matching is perfect
	perfect = true;
	for(int i = 0; i < n; i++)
		if(mate[outer[i]] == -1)
			perfect = false;
}

bool Matching::IsAdjacent(int u, int v)
{
	return (G.AdjMat()[u * G.GetNumVertices() + v] and not IsEdgeBlocked(u, v));
}

bool Matching::IsEdgeBlocked(int u, int v)
{
	return GREATER(slack[ G.GetEdgeIndex(u, v) ], 0);
}

bool Matching::IsEdgeBlocked(int e)
{
	return GREATER(slack[e], 0);
}

//Vertices will be selected in non-decreasing order of their degree
//Each time an unmatched vertex is selected, it is matched to its adjacent unmatched vertex of minimum degree
void Matching::Heuristic()
{
	vector<int> degree(n, 0);
	BinaryHeap B;

	for(int i = 0; i < m; i++)
	{
		if(IsEdgeBlocked(i)) continue;

		pair<int, int> p = G.GetEdge(i);
		int u = p.first;
		int v = p.second;

		degree[u]++;
		degree[v]++;
	}

	for(int i = 0; i < n; i++)
		B.Insert(degree[i], i);

	while(B.Size() > 0)
	{
		int u = B.DeleteMin();
		if(mate[outer[u]] == -1)
		{
			int min = -1;
			for (int v : G.AdjList(u)) {
				if(IsEdgeBlocked(u, v) or
					(outer[u] == outer[v]) or
					(mate[outer[v]] != -1) )
					continue;

				if(min == -1 or degree[v] < degree[min])
					min = v;	
			}
			if(min != -1)
			{
				mate[outer[u]] = min;
				mate[outer[min]] = u;
			}
		}
	}
}

//Destroys a blossom recursively
void Matching::DestroyBlossom(int t)
{
	if((t < n) or
		(blocked[t] and GREATER(dual[t], 0))) return;

	for(int s : shallow[t]) {
		outer[s] = s;
		for(int d : deep[s])
			outer[d] = s;	

		DestroyBlossom(s);
	}

	active[t] = false;
	blocked[t] = false;
	AddFreeBlossomIndex(t);
	mate[t] = -1;
}

void Matching::Expand(int start, bool expandBlocked = false)
{
	std::stack<int> Q;
	Q.push(start);

	while (!Q.empty()) {
		int u = Q.top();
		Q.pop();
		int v = outer[mate[u]];

		int index = m;
		int p = -1, q = -1;
		//Find the regular edge {p,q} of minimum index connecting u and its mate
		//We use the minimum index to grant that the two possible blossoms u and v will use the same edge for a mate
		for (int di : deep[u]) {
			for (int dj : deep[v]) {
				if (IsAdjacent(di, dj) and G.GetEdgeIndex(di, dj) < index)
				{
					index = G.GetEdgeIndex(di, dj);
					p = di;
					q = dj;
				}
			}
		}

		mate[u] = q;
		mate[v] = p;
		//If u is a regular vertex, we are done
		if (u < n or (blocked[u] and not expandBlocked)) continue;

		bool found = false;
		//Find the position t of the new tip of the blossom
		for (list<int>::iterator it = shallow[u].begin(); it != shallow[u].end() and not found; )
		{
			int si = *it;
			for (vector<int>::iterator jt = deep[si].begin(); jt != deep[si].end() and not found; ++jt)
			{
				if (*jt == p)
					found = true;
			}
			++it;
			if (not found)
			{
				shallow[u].push_back(si);
				shallow[u].pop_front();
			}
		}

		list<int>::iterator it = shallow[u].begin();
		//Adjust the mate of the tip
		mate[*it] = mate[u];
		++it;
		//
		//Now we go through the odd circuit adjusting the new mates
		while (it != shallow[u].end())
		{
			list<int>::iterator itnext = it;
			++itnext;
			mate[*it] = *itnext;
			mate[*itnext] = *it;
			++itnext;
			it = itnext;
		}

		//We update the sets blossom, shallow, and outer since this blossom is being deactivated
		for (int s : shallow[u]) {
			outer[s] = s;
			for (int d : deep[s])
				outer[d] = s;
		}
		active[u] = false;
		AddFreeBlossomIndex(u);

		//Expand the vertices in the blossom
		for (int s : shallow[u]) {
			Q.push(s);
		}
	}

}

//Augment the path root[u], ..., u, v, ..., root[v]
void Matching::Augment(int u, int v)
{
	//We go from u and v to its respective roots, alternating the matching
	int p = outer[u];
	int q = outer[v];
    int outv = q;
	int fp = forest[p];
	mate[p] = q;
	mate[q] = p;
	Expand(p);
	Expand(q);
	while(fp != -1)
	{
		q = outer[forest[p]];
		p = outer[forest[q]];
		fp = forest[p];

		mate[p] = q;
		mate[q] = p;
		Expand(p);
		Expand(q);
	}

	p = outv;
	fp = forest[p];
	while(fp != -1)
	{
		q = outer[forest[p]];
		p = outer[forest[q]];
		fp = forest[p];

		mate[p] = q;
		mate[q] = p;
		Expand(p);
		Expand(q);
	}
}

void Matching::Reset()
{
	for(int i = 0; i < 2*n; i++)
	{
		forest[i] = -1;
		root[i] = i;

		if(i >= n and active[i] and outer[i] == i)
			DestroyBlossom(i);
	}

	visited.assign(2*n, 0);
	forestList.clear();
	for(int i = 0; i < n; i++)
	{
		if(mate[outer[i]] == -1)
		{
			type[outer[i]] = 2;
			if(!visited[outer[i]])
				forestList.push_back(i);
			visited[outer[i]] = true;
		}
		else type[outer[i]] = 0;
	}
}

int Matching::GetFreeBlossomIndex()
{
	int i = free.back();
	free.pop_back();
	return i;
}

void Matching::AddFreeBlossomIndex(int i)
{
	free.push_back(i);
}

void Matching::ClearBlossomIndices()
{
	free.clear();
	for(int i = n; i < 2*n; i++)
		AddFreeBlossomIndex(i);
}

//Contracts the blossom w, ..., u, v, ..., w, where w is the first vertex that appears in the paths from u and v to their respective roots
int Matching::Blossom(int u, int v)
{
	int t = GetFreeBlossomIndex();

	vector<bool> isInPath(2*n, false);

	//Find the tip of the blossom
	int u_ = u; 
	while(u_ != -1)
	{
		isInPath[outer[u_]] = true;

		u_ = forest[outer[u_]];
	}

	int v_ = outer[v];
	while(not isInPath[v_])
		v_ = outer[forest[v_]];
	tip[t] = v_;

	//Find the odd circuit, update shallow, outer, blossom and deep
	//First we construct the set shallow (the odd circuit)
	list<int> circuit;
	u_ = outer[u];
	circuit.push_front(u_);
	while(u_ != tip[t])
	{
		u_ = outer[forest[u_]];
		circuit.push_front(u_);
	}

	shallow[t].clear();
	deep[t].clear();
	for(list<int>::iterator it = circuit.begin(); it != circuit.end(); ++it)
	{
		shallow[t].push_back(*it);
	}

	v_ = outer[v];
	while(v_ != tip[t])
	{
		shallow[t].push_back(v_);
		v_ = outer[forest[v_]];
	}

	//Now we construct deep and update outer
	for(int u_ : shallow[t]) {
		outer[u_] = t;
		for(int d : deep[u_]) {
			deep[t].push_back(d);
			outer[d] = t;
		}
	}

	forest[t] = forest[tip[t]];
	type[t] = EVEN;
	root[t] = root[tip[t]];
	active[t] = true;
	outer[t] = t;
	mate[t] = mate[tip[t]];

	return t;
}

void Matching::UpdateDualCosts()
{
	double e1 = 0, e2 = 0, e3 = 0;
	int inite1 = false, inite2 = false, inite3 = false;
	for(int i = 0; i < m; i++)
	{
		auto edge = G.GetEdge(i);
		int u = edge.first,
			v = edge.second;

		int outer_u = outer[u];
		int outer_v = outer[v];
		int type_u = type[outer_u];
		int type_v = type[outer_v];

		if( (type_u == EVEN and type_v == UNLABELED) or (type_v == EVEN and type_u == UNLABELED) )
		{
			if(!inite1 or GREATER(e1, slack[i]))
			{
				e1 = slack[i];
				inite1 = true;
			}
		}
		else if( (outer_u != outer_v) and type_u == EVEN and type_v == EVEN )
		{
			if(!inite2 or GREATER(e2, slack[i]))
			{
				e2 = slack[i];
				inite2 = true;
			}
		}
	}
	for(int i = n; i < 2*n; i++)
	{
		if(active[i] and i == outer[i] and type[outer[i]] == ODD and (!inite3 or GREATER(e3, dual[i])))
		{
			e3 = dual[i]; 
			inite3 = true;
		}	
	}
	double e = 0;
	if(inite1) e = e1;
	else if(inite2) e = e2;
	else if(inite3) e = e3;

	if(GREATER(e, e2/2.0) and inite2)
		e = e2/2.0;
	if(GREATER(e, e3) and inite3)
		e = e3;
	 
	for(int i = 0; i < 2*n; i++)
	{
		if(i != outer[i]) continue;

		if(active[i] and type[outer[i]] == EVEN)	
		{
			dual[i] += e; 
		}
		else if(active[i] and type[outer[i]] == ODD)
		{
			dual[i] -= e; 
		}
	}

	for(int i = 0; i < m; i++)
	{
		auto edge = G.GetEdge(i);
		int u = edge.first,
			v = edge.second;

		int outer_u = outer[u];
		int outer_v = outer[v];
		int type_u = type[outer_u];
		int type_v = type[outer_v];

		if(outer_u != outer_v)
		{	
			if(type_u == EVEN and type_v == EVEN)
				slack[i] -= 2.0*e;
			else if(type_u == ODD and type_v == ODD)
				slack[i] += 2.0*e;
			else if( (type_v == UNLABELED and type_u == EVEN) or (type_u == UNLABELED and type_v == EVEN) )
				slack[i] -= e;
			else if( (type_v == UNLABELED and type_u == ODD) or (type_u == UNLABELED and type_v == ODD) )
				slack[i] += e;
		}
	}
	for(int i = n; i < 2*n; i++)
	{
		if(GREATER(dual[i], 0))
		{
			blocked[i] = true;
		}
		else if(active[i] and blocked[i])
		{
			//The blossom is becoming unblocked
			if(mate[i] == -1)
			{
				DestroyBlossom(i);
			}
			else
			{
				blocked[i] = false;
				Expand(i);
			}
		}
	}	
}

pair< list<int>, double> Matching::SolveMinimumCostPerfectMatching(vector<double> & cost)
{
	SolveMaximumMatching();
	if(!perfect)
		throw "Error: The graph does not have a perfect matching";

	Clear();

	//Initialize slacks (reduced costs for the edges)
	slack = cost;

	PositiveCosts();

	//If the matching on the compressed graph is perfect, we are done
	perfect = false;
	while(not perfect)
	{
		//Run an heuristic maximum matching algorithm
		Heuristic();
		//Grow a hungarian forest
		Grow();
		UpdateDualCosts();
		//Set up the algorithm for a new grow step
		Reset();
	}

	list<int> matching = RetrieveMatching();

	double obj = 0;
	for(list<int>::iterator it = matching.begin(); it != matching.end(); ++it)
		obj += cost[*it];
	
	return pair< list<int>, double >(matching, obj);
}

void Matching::PositiveCosts()
{
	double minEdge = 0;
	for(int i = 0; i < m ;i++)
		if(GREATER(minEdge - slack[i], 0)) 
			minEdge = slack[i];

	for(int i = 0; i < m; i++)
		slack[i] -= minEdge;
}

list<int> Matching::SolveMaximumMatching()
{
	Clear();
	Grow();
	return RetrieveMatching();
}

//Sets up the algorithm for a new run
void Matching::Clear()
{
	ClearBlossomIndices();

	for(int i = 0; i < 2*n; i++)
	{
		outer[i] = i;
		deep[i].clear();
		if(i<n)
			deep[i].push_back(i);
		shallow[i].clear();
		if(i < n) active[i] = true;
		else active[i] = false;
	
		type[i] = 0;
		forest[i] = -1;
		root[i] = i;

		blocked[i] = false;
		dual[i] = 0;
		mate[i] = -1;
		tip[i] = i;
	}
	slack.assign(m, 0);
}

list<int> Matching::RetrieveMatching()
{
	list<int> matching;

	for(int i = 0; i < 2*n; i++)
		if(active[i] and mate[i] != -1 and outer[i] == i)
			Expand(i, true);

	for(int i = 0; i < m; i++)
	{
		int u = G.GetEdge(i).first;
		int v = G.GetEdge(i).second;

		if(mate[u] == v)
			matching.push_back(i);
	}
	return matching;
}
