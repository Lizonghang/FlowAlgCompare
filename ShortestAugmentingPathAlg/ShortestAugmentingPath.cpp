#include "ShortestAugmentingPath.h"
#include <stdlib.h>
#include <climits>
#include <iostream>

// 获取边上允许通过的剩余流量
int CGraph::getRest(int head, int tail)
{
    list<CEdge*>::iterator l;
    for (l = IncidentList.begin(); l != IncidentList.end(); l++)
    {
        if ((*l)->head == head && (*l)->tail == tail)
            return (*l)->rest;
    }
    return 0;
}

// 检查边是否存在
int CGraph::check(int head, int tail)
{
    list<CEdge*>::iterator l;
    for (l = IncidentList.begin(); l != IncidentList.end(); l++)
    {
        if ((*l)->head == head && (*l)->tail == tail)
            return 1;
    }
    return 0;
}

// 更新head-->tail边与tail-->head边剩余流量
void CGraph::Update(int head, int tail, int mind)
{
    list<CEdge*>::iterator i,j;
    for (i = IncidentList.begin(); i != IncidentList.end(); i++)
    {
        if ((*i)->head == head && (*i)->tail == tail)
        {
            // 获取指定边,更新该边剩余流量
            (*i)->rest = (*i)->rest - mind;
            // 检查该边的反向边是否存在,若存在更新反向边剩余流量,若不存在新建一条反向边并更新边列表
            if (check(tail, head))
            {
                for (j = IncidentList.begin(); j != IncidentList.end(); j++)
                    if ((*j)->head == tail && (*j)->tail == head)
                        (*j)->rest = (*j)->rest + mind;
            }
            else
            {
                CEdge *s = new CEdge(tail, head, mind, -1);
                for (j = IncidentList.begin(); j != IncidentList.end(); j++)
                    if ((*j)->head == head && (*j)->tail == tail)
                        s->capacity = (*j)->capacity;
                IncidentList.push_back(s);
            }
            // 查重,将反向边加入相应relateList
            list<int>::iterator i;
            int exist = 0;
            for (i = listVertex[tail]->relateList.begin(); i != listVertex[tail]->relateList.end(); i++)
                if (*i == head){ exist = 1; break; }
            if (!exist)
                listVertex[tail]->relateList.push_back(head);
        }
    }
}

// 最短增广路径算法
void CGraph::ShortestAugmentingPath()
{
    list<int> tempDFSList;
    list<int>::iterator l, target;
    int i, j, tempDFSVertexID, mind;
    // 开始广度优先遍历
    listVertex[vertexNum - 1]->flag = 1;
    while (listVertex[vertexNum - 1]->flag)
    {
        mind = INT_MAX;
        for (i = 0; i < vertexNum; i++)
        {
            listVertex[i]->flag = 0;
            listVertex[i]->parent = INT_MAX;
        }
        listVertex[0]->flag = 1;
        tempDFSList.push_back(0);
        while (!tempDFSList.empty() && listVertex[vertexNum - 1]->flag == 0)
        {
            tempDFSVertexID = *(tempDFSList.begin());
            tempDFSList.pop_front();
            for (l = listVertex[tempDFSVertexID]->relateList.begin(); l != listVertex[tempDFSVertexID]->relateList.end(); l++)
                if (getRest(tempDFSVertexID, listVertex[*l]->id) > 0 && listVertex[*l]->flag == 0)
                {
                    listVertex[*l]->parent = tempDFSVertexID;  // 更新节点的父节点
                    listVertex[*l]->flag = 1;
                    tempDFSList.push_back(*l); // 更新广度优先遍历路径表
                }
        }
        tempDFSList.clear();    // 找到增广路径后清空广度优先路径表,防止进入下一次while循环
        if (listVertex[vertexNum - 1]->flag)
        {
            // 确定s到t的增广路径p,确定p的增广容量d*
            j = vertexNum - 1;
            while (listVertex[j]->parent != INT_MAX)
            {
                if (getRest(listVertex[j]->parent, j) < mind)
                    mind = getRest(listVertex[j]->parent, j);
                j = listVertex[j]->parent;
            }
            // 沿着路径p输送d*单位的流;更新图中各边上的rij
            j = vertexNum - 1;
            while (listVertex[j]->parent != INT_MAX)
            {
                Update(listVertex[j]->parent, j, mind);
                j = listVertex[j]->parent;
            }
        }
    }
}

// 获取图中最大流值
int CGraph::getMaxFlowValue()
{
    list<CEdge*>::iterator l;
    int flow, sumFlow = 0;
    for (l = IncidentList.begin(); l != IncidentList.end(); l++)
    {
        if ((*l)->tail == vertexNum - 1) {
            flow = (*l)->capacity - (*l)->rest;
            sumFlow += flow;
        }
    }
    return sumFlow;
}

// 显示流在图中的流动情况
void CGraph::showFlowInPath()
{
    list<CEdge*>::iterator l;
    int flow;
    for (l = sIncidentList.begin(); l != sIncidentList.end(); l++) {
        flow = (*l)->capacity - (*l)->rest;
        if (flow >= 0)
            std::cout << "经过边" << (*l)->head << "-->" << (*l)->tail << "的流量值为 " << flow << std::endl;
    }
}