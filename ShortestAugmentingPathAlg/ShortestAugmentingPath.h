#ifndef ShortestAugmentingPath_hpp
#define ShortestAugmentingPath_hpp
#include <stdio.h>
#include <list>
#include <map>
#include <vector>

using std::list;
using std::map;
using std::vector;

class CEdge
{
public:
    int head;   // 头节点
    int tail;   // 尾节点
    int capacity;   // 容量限制
    int rest;   // 剩余网络流量

    CEdge(int h, int t, int c, int p){ head = h; tail = t; rest = capacity = c;};
};

class CVertex
{
public:
    int flag;   // 标识点的d
    int id;     // 节点ID
    int parent; // 父节点
    list<int> relateList;   // 关联列表

    CVertex(){ flag = 1; parent = INT_MAX; };
};

class CGraph
{
public:
    int vertexNum;  // 节点数
    int edgeNum;    // 边数
    list<CEdge*> sIncidentList; // 原始边列表,用于输出结果
    list<CEdge*> IncidentList;  // 边的列表
    vector<CVertex*> listVertex;   // 所有节点

    CGraph(){};
    ~CGraph(){};
    int getRest(int head, int tail);    // 获取边上剩余流量值
    int check(int head, int tail);      // 检查边是否存在
    void Update(int head, int tail, int mind);  // 更新边
    void ShortestAugmentingPath();  // 最短增广路径算法
    void showFlowInPath();  // 显示流在图中的流动情况
    int getMaxFlowValue();    // 获取图中最大流
};

#endif
