#include "PreflowPush.h"
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
    listVertex[head]->e -= mind;  // 更新出节点溢出流值
    listVertex[tail]->e += mind;  // 更新入节点溢出流值
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
                {
                    if ((*j)->head == tail && (*j)->tail == head){
                        (*j)->rest = (*j)->rest + mind;
                    }
                    
                }
            }
            else
            {
                CEdge *s = new CEdge(tail, head, mind);
                for (j = IncidentList.begin(); j != IncidentList.end(); j++) {
                    if ((*j)->head == head && (*j)->tail == tail) {
                        s->capacity = (*j)->capacity;
                    }
                }
                IncidentList.push_back(s);
            }
            // 查重,将反向边加入相应relateList
            list<int>::iterator i;
            int exist = 0;
            for (i = listVertex[tail]->relateList.begin(); i != listVertex[tail]->relateList.end(); i++) {
                if (*i == head) {
                    exist = 1;
                    break;
                }
            }
            if (!exist) {
                listVertex[tail]->relateList.push_back(head);
            }
        }
    }
}

// 初始化操作,计算距离标记d,头节点相邻点溢出流,更新活跃列表activeList
void CGraph::preprocess()
{
    list<int>::iterator i;
    list<CEdge*>::iterator lc;
    list<int> tempDFSList;      // 广度优先算法临时列表
    int tempID;     // 广度优先算法临时记录节点ID
    int tempVertexNum = vertexNum;  // 节点数副本,用于更新d值
    tempDFSList.push_back(0);   // 初始值为入口点ID
    listVertex[0]->flag = 0;    // 初始flag=0,未初始flag=1
    listVertex[0]->d = tempVertexNum;   // 入口点初始d为节点个数
    // 按照广度优先原则更新节点高度d
    while (!tempDFSList.empty())
    {
        tempID = *(tempDFSList.begin());
        listVertex[tempID]->flag = 0;
        tempDFSList.pop_front();
        tempVertexNum--;
        for (i = listVertex[tempID]->relateList.begin(); i != listVertex[tempID]->relateList.end(); i++)
        {
            if (listVertex[*i]->flag != 0) {
                tempDFSList.push_back(listVertex[*i]->id);
                listVertex[*i]->flag = 0;
                listVertex[*i]->d = tempVertexNum;
            }
        }
    }
    // 初始化s点e值
    for (lc = IncidentList.begin(); lc != IncidentList.end(); lc++) {
        if ((*lc)->head == 0) {
            listVertex[0]->e += (*lc)->capacity;
        }
    }
    // 对头节点关联的节点进行推入流，并加入到活跃点列表中
    for (i = listVertex[0]->relateList.begin(); i != listVertex[0]->relateList.end(); i++)
    {
        Update(0, *i, getRest(0, *i));
        activeList.push_back(*i);
    }
}

// 判断顶点是否处于active状态
int CGraph::isActive(int ID)
{
    list<int>::iterator l;
    for (l = activeList.begin(); l != activeList.end(); l++)
    {
        if (*l == ID)   return 1;
    }
    return 0;
}

// 预流推送算法
void CGraph::FIFOPreflowPush()
{
    int tempActiveVertexID;   // 临时活跃点
    int mind;   // 应推出的流大小
    int minh;
    int existAdmissible;    // 标识admissible边是否存在
    list<int>::iterator l;
    
    preprocess();   // 初始化操作
    
    while (!activeList.empty())
    {
        // FIFO Preflow-Push 算法
        minh = INT_MAX;     // 不存在admissible边时的min{d(j)+1}记录
        tempActiveVertexID = *(activeList.begin());
        activeList.pop_front();
        existAdmissible = 0;

        // 不处理s点与t点,回溯到s点的流与到达t点的流不再流动
        if (tempActiveVertexID == vertexNum - 1 || tempActiveVertexID == 0)     continue;
        
        for (l = listVertex[tempActiveVertexID]->relateList.begin(); l != listVertex[tempActiveVertexID]->relateList.end(); l++)
        {
            // 如果存在admissible边
            if (listVertex[*l]->d + 1 == listVertex[tempActiveVertexID]->d && getRest(tempActiveVertexID, *l) > 0)
            {
                // 找到合适的推出流大小
                if (getRest(tempActiveVertexID, listVertex[*l]->id) >= listVertex[tempActiveVertexID]->e)
                    mind = listVertex[tempActiveVertexID]->e;
                else
                    mind = getRest(tempActiveVertexID, listVertex[*l]->id);
                Update(tempActiveVertexID, *l, mind);   // 更新边
                // 如果入节点更新且不是尾节点则插入到活跃列表
                if (isActive(*l) == 0 && (*l) != vertexNum - 1)
                    activeList.push_back(*l);
                existAdmissible = 1;
                if (listVertex[tempActiveVertexID]->e == 0) break;  // 该激活点已没有可提供的流
            }
        }
        // 如果不存在admissible边,需要进行relabel确保得到新的admissible边
        if (!existAdmissible)
        {
            for (l = listVertex[tempActiveVertexID]->relateList.begin(); l != listVertex[tempActiveVertexID]->relateList.end(); l++)
                if (listVertex[*l]->d + 1 < minh && getRest(tempActiveVertexID, *l) > 0)
                    minh = listVertex[*l]->d + 1;
            if (minh != INT_MAX)
            {
                listVertex[tempActiveVertexID]->d = minh;
                activeList.push_back(tempActiveVertexID);
            }
        }
        else
            if (listVertex[tempActiveVertexID]->e != 0)
                activeList.push_back(tempActiveVertexID);
    }
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