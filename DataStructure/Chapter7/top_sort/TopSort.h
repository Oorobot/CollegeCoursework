#ifndef __TOPSORT_H__
#define __TOPSORT_H__

#include <stack>
#include "AdjListDirGraph.h"			// 邻接矩阵有向图

template <class ElemType>
void StatIndegree(const AdjListDirGraph<ElemType>& g, int* indegree)
// 操作结果：统计图g各顶点的入度
{
	for (int v = 0; v < g.GetVexNum(); v++)	// 初始化入度为0
		indegree[v] = 0;

	for (int v = 0; v < g.GetVexNum(); v++)// 遍历图的顶点
		for (int u = g.FirstAdjVex(v); u != -1; u = g.NextAdjVex(v, u))
			indegree[u]++;
}

template <class ElemType>
Status TopSort(const AdjListDirGraph<ElemType> & g)
// 初始条件：存在有向图g
// 操作结果：如g无回路,则输出g的顶点的一个拓扑序列,并返回SUCCESS,否则返回FAIL
{
	int* indegree = new int[g.GetVexNum()];	// 顶点入度数组		
	int v, u, count = 0, top = -1;
	ElemType e;
	StatIndegree(g, indegree);				// 统计顶点的入度


	for (v = 0; v < g.GetVexNum(); v++)
		if (indegree[v] == 0) {            // 入度为0的顶点入栈 
			indegree[v] = top;
			top = v;
		}

	while (top != -1) {	// 栈非空
		v = top;
		top = indegree[v];
		g.GetElem(v, e);
		cout << e << "  ";
		count++;			// 对输出顶点进行记数
		for (u = g.FirstAdjVex(v); u != -1; u = g.NextAdjVex(v, u))
			// 对v的每个邻接点u入度减1
			if (--indegree[u] == 0) {// u入度为0,将u入栈
				indegree[u] = top;
				top = u;
			}
	}
	delete[]indegree;						// 释放indegree所占用的存储空间

	if (count < g.GetVexNum()) return FAIL;	// 图g有回路
	else return SUCCESS;					// 拓扑排序成功
}

template <class ElemType>
Status DfsTopSort(const AdjListDirGraph<ElemType> & g)
{
	int* indegress = new int[g.GetVexNum()];
	int v, count = 0;
	StatIndegree(g, indegress);
	stack<int> s;
	for (v = 0; v < g.GetVexNum(); v++)
		if (indegress[v] == 0) {
			s.push(v);
		}

	while (!s.empty()) {
		v = s.top();
		s.pop();
		DfsTopSort(g, indegress, v, count, 1);
	}
	delete[] indegress;
	if (count < g.GetVexNum()) return FAIL;
	else return SUCCESS;
}

template <class ElemType>
void DfsTopSort(const AdjListDirGraph<ElemType> & g, int* indegress, int v, int& count)
{
	ElemType e;
	g.GetElem(v, e);
	cout << e << " ";
	count++;
	for (int u = g.FirstAdjVex(v); u != -1; u = g.NextAdjVex(v, u))
	{
		if (--indegress[u] == 0)  DfsTopSort(g, indegress, u, count);
	}
}
template <class ElemType>
void DfsTopSort(const AdjListDirGraph<ElemType> & g, int* indegress, int v, int& count, int flag)
{
	ElemType e;
	g.GetElem(v, e);
	if (flag == 1) {
		cout << e << " ";
		count++;
		for (int u = g.FirstAdjVex(v); u != -1; u = g.NextAdjVex(v, u))
		{
			if (--indegress[u] == 0)  DfsTopSort(g, indegress, u, count, 1);
			else DfsTopSort(g, indegress, u, count, 0);
		}
	}
	if (flag == 0) {
		for (int u = g.FirstAdjVex(v); u != -1; u = g.NextAdjVex(v, u))
			DfsTopSort(g, indegress, u, count, 0);
	}
}



template <class ElemType>
Status Checkhuan(const AdjListDirGraph<ElemType>& g)
{
	int* indegree = new int[g.GetVexNum()];	// 顶点入度数组		
	int v, u, count = 0, top = -1;
	ElemType e;
	StatIndegree(g, indegree);				// 统计顶点的入度


	for (v = 0; v < g.GetVexNum(); v++)
		if (indegree[v] == 0) {            // 入度为0的顶点入栈 
			indegree[v] = top;
			top = v;
		}
	while (top != -1) {	// 栈非空
		v = top;
		top = indegree[v];
		count++;			// 对输出顶点进行记数
		for (u = g.FirstAdjVex(v); u != -1; u = g.NextAdjVex(v, u))
			// 对v的每个邻接点u入度减1
			if (--indegree[u] == 0) {// u入度为0,将u入栈
				indegree[u] = top;
				top = u;
			}
	}
	delete[]indegree;						// 释放indegree所占用的存储空间

	if (count < g.GetVexNum()) return FAIL;	// 图g有回路
	else return SUCCESS;					// 拓扑排序成功

}

template <class ElemType>
Status Dfs(const AdjListDirGraph<ElemType> & g)
{
	int* indegress = new int[g.GetVexNum()];
	StatIndegree(g, indegress);
	int v;
	stack<int> s, t;

	for (v = 0; v < g.GetVexNum(); v++)
		g.SetTag(v, UNVISITED);
	for (v = 0; v < g.GetVexNum(); v++)
		if (indegress[v] == 0) {
			s.push(v);
		}
	while (!s.empty()) {
		v = s.top();
		s.pop();
		Dfs(g, v, t);
	}
	while (!t.empty()) {
		ElemType e;
		v = t.top();
		g.GetElem(v, e);
		cout << e << " ";
		t.pop();

	}

	if (!Checkhuan(g))  return SUCCESS;
	else return FAIL;

	delete[] indegress;
}

template <class ElemType>
void Dfs(const AdjListDirGraph<ElemType> & g, int v, stack<int> & t)
{
	g.SetTag(v, VISITED);
	for (int u = g.FirstAdjVex(v); u != -1; u = g.NextAdjVex(v, u)) {
		if (g.GetTag(u) == UNVISITED)
			Dfs(g, u, t);
	}
	t.push(v);
}

#endif

