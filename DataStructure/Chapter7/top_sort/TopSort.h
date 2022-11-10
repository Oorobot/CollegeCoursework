#ifndef __TOPSORT_H__
#define __TOPSORT_H__

#include <stack>
#include "AdjListDirGraph.h"			// �ڽӾ�������ͼ

template <class ElemType>
void StatIndegree(const AdjListDirGraph<ElemType>& g, int* indegree)
// ���������ͳ��ͼg����������
{
	for (int v = 0; v < g.GetVexNum(); v++)	// ��ʼ�����Ϊ0
		indegree[v] = 0;

	for (int v = 0; v < g.GetVexNum(); v++)// ����ͼ�Ķ���
		for (int u = g.FirstAdjVex(v); u != -1; u = g.NextAdjVex(v, u))
			indegree[u]++;
}

template <class ElemType>
Status TopSort(const AdjListDirGraph<ElemType> & g)
// ��ʼ��������������ͼg
// �����������g�޻�·,�����g�Ķ����һ����������,������SUCCESS,���򷵻�FAIL
{
	int* indegree = new int[g.GetVexNum()];	// �����������		
	int v, u, count = 0, top = -1;
	ElemType e;
	StatIndegree(g, indegree);				// ͳ�ƶ�������


	for (v = 0; v < g.GetVexNum(); v++)
		if (indegree[v] == 0) {            // ���Ϊ0�Ķ�����ջ 
			indegree[v] = top;
			top = v;
		}

	while (top != -1) {	// ջ�ǿ�
		v = top;
		top = indegree[v];
		g.GetElem(v, e);
		cout << e << "  ";
		count++;			// �����������м���
		for (u = g.FirstAdjVex(v); u != -1; u = g.NextAdjVex(v, u))
			// ��v��ÿ���ڽӵ�u��ȼ�1
			if (--indegree[u] == 0) {// u���Ϊ0,��u��ջ
				indegree[u] = top;
				top = u;
			}
	}
	delete[]indegree;						// �ͷ�indegree��ռ�õĴ洢�ռ�

	if (count < g.GetVexNum()) return FAIL;	// ͼg�л�·
	else return SUCCESS;					// ��������ɹ�
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
	int* indegree = new int[g.GetVexNum()];	// �����������		
	int v, u, count = 0, top = -1;
	ElemType e;
	StatIndegree(g, indegree);				// ͳ�ƶ�������


	for (v = 0; v < g.GetVexNum(); v++)
		if (indegree[v] == 0) {            // ���Ϊ0�Ķ�����ջ 
			indegree[v] = top;
			top = v;
		}
	while (top != -1) {	// ջ�ǿ�
		v = top;
		top = indegree[v];
		count++;			// �����������м���
		for (u = g.FirstAdjVex(v); u != -1; u = g.NextAdjVex(v, u))
			// ��v��ÿ���ڽӵ�u��ȼ�1
			if (--indegree[u] == 0) {// u���Ϊ0,��u��ջ
				indegree[u] = top;
				top = u;
			}
	}
	delete[]indegree;						// �ͷ�indegree��ռ�õĴ洢�ռ�

	if (count < g.GetVexNum()) return FAIL;	// ͼg�л�·
	else return SUCCESS;					// ��������ɹ�

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

