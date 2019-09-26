#ifndef __SHORTEST_PATH_FLOYD_H__
#define __SHORTEST_PATH_FLOYD_H__

#include "AdjListDirNetwork.h"			// 邻接表有向网
#include "LinkStack.h"					// 栈

template <class ElemType, class WeightType>
void ShortestPathFloyd(const AdjListDirNetwork<ElemType, WeightType>& g, int** path,
	WeightType** dist)
	// 操作结果: 用Floyd算法求有向网g中各对顶点u和v之间的最短路径path[u][v]和路径长度
	//	dist[u][v],path[u][v]存储从u到v的最短路径上至此顶点的前一顶点的顶点号,dist[u][v]
	//	存储从u到v的最短路径的长度
{
	for (int u = 0; u < g.GetVexNum(); u++)
		for (int v = 0; v < g.GetVexNum(); v++)
		{	// 初始化path和dist
			dist[u][v] = (u != v) ? g.GetWeight(u, v) : 0;
			if (u != v && dist[u][v] < g.GetInfinity())
				path[u][v] = u;	// 存在边<u,v>
			else
				path[u][v] = -1;											// 不存在边<u,v>
		}

	for (int k = 0; k < g.GetVexNum(); k++)
		for (int i = 0; i < g.GetVexNum(); i++)
			for (int j = 0; j < g.GetVexNum(); j++)
				if (dist[i][k] + dist[k][j] < dist[i][j]) {
					// 从i到k再到j的路径长度更短
					dist[i][j] = dist[i][k] + dist[k][j];
					path[i][j] = path[k][j];
				}
}

template <class ElemType, class WeightType>
void DisplayPathAndDist(const AdjListDirNetwork<ElemType, WeightType> & g, int** path, WeightType * *dist)
{
	ElemType e1, e2;
	for (int v1 = 0; v1 < g.GetVexNum(); v1++)
		for (int v2 = 0; v2 < g.GetVexNum(); v2++)
			if (v1 != v2) {
				cout << endl;
				g.GetElem(v1, e1);
				g.GetElem(v2, e2);
				if (dist[v1][v2] < g.GetInfinity()) {	// 存在从v1到v2最短路径
					cout << "从 " << e1 << " 到 " << e2 << ":";
					cout << " 路径长度 = " << dist[v1][v2];
					cout << " 路径：";
					LinkStack<int> s;	// 存储路上的顶点
					int v = v2;
					while (v != v1) {
						s.Push(v);	// v为路径上的顶点
						v = path[v1][v];// 取上路径上的前一个顶点
					}
					s.Push(v1);		// v是路径上起始点
					while (!s.IsEmpty()) {	// 输出路上的顶点
						s.Pop(v);
						g.GetElem(v, e1);
						cout << e1 << " ";
					}
				}
				else {	// 从v1到v2无路径
					cout << "from " << e1 << " to " << e2 << ":";
					cout << " dist = ∞";
				}
			}
	cout << endl;
}

template <class ElemType, class WeightType>
void longestPathFloyd(const AdjListDirNetwork<ElemType, WeightType> & g, int** path,
	WeightType * *dist)
{
	
	for (int u = 0; u < g.GetVexNum(); u++)
		for (int v = 0; v < g.GetVexNum(); v++)
		{	
			dist[u][v] = (u != v) ? g.GetWeight(u, v) : 0;
			if (u != v && dist[u][v] < g.GetInfinity())
				path[u][v] = u;	
			else
				path[u][v] = -1;
		}
	for (int k = 0; k < g.GetVexNum(); k++)
		for (int i = 0; i < g.GetVexNum(); i++)
			for (int j = 0; j < g.GetVexNum(); j++)
				if (dist[i][j] == DEFAULT_INFINITY && dist[i][k] != DEFAULT_INFINITY && dist[k][j] != DEFAULT_INFINITY) {
					dist[i][j] = dist[i][k] + dist[k][j];
					path[i][j] = path[k][j];
				}
				else if (dist[i][k] + dist[k][j] > dist[i][j] && dist[i][k] != DEFAULT_INFINITY && dist[k][j] != DEFAULT_INFINITY) {
					// 从i到k再到j的路径长度更长
					dist[i][j] = dist[i][k] + dist[k][j];
					path[i][j] = path[k][j];
				}
}

template <class ElemType, class WeightType>
void TheLongestPath(const AdjListDirNetwork<ElemType, WeightType> & g, int** path,
	WeightType * *dist)
{
	longestPathFloyd(g, path, dist);
	int max = 0, i1 = 0, j1 = 0;
	for (int i = 0; i < g.GetVexNum(); i++)
		for (int j = 0; j < g.GetVexNum(); j++) {
			if (dist[i][j] != DEFAULT_INFINITY && dist[i][j] > max && i != j)
			{
				max = dist[i][j]; i1 = i; j1 = j;
			}
		}
	ElemType e1, e2;
	cout << endl;
	g.GetElem(i1, e1);
	g.GetElem(j1, e2);
	if (dist[i1][j1] < g.GetInfinity()) {	// 存在从v1到v2最短路径
		cout << "最长路径是从 " << e1 << " 到 " << e2 << ":";
		cout << " 路径长度 = " << dist[i1][j1];
		cout << " 路径：";
		LinkStack<int> s;	// 存储路上的顶点
		int v = j1;
		while (v != i1) {
			s.Push(v);	// v为路径上的顶点
			v = path[i1][v];// 取上路径上的前一个顶点
		}
		s.Push(i1);		// v是路径上起始点
		while (!s.IsEmpty()) {	// 输出路上的顶点
			s.Pop(v);
			g.GetElem(v, e1);
			cout << e1 << " ";
		}
	}
	else {	// 从v1到v2无路径
		cout << "from " << e1 << " to " << e2 << ":";
		cout << " dist = ∞";
	}
}
#endif

