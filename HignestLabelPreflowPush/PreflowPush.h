#ifndef PreflowPush_hpp
#define PreflowPush_hpp
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

    CEdge(int h, int t, int c){ head = h; tail = t; rest = capacity = c; };
};

class CVertex
{
public:
    int flag;   // 标识是否已被更新
    int id;     // 节点ID
    int d;      // 节点高度
    int e;      // 节点溢出流
    list<int> relateList;   // 关联列表

    CVertex(){ flag = 1; e = d = 0; };
};

class CGraph
{
public:
    int vertexNum;  // 节点数
    int edgeNum;    // 边数
    list<CEdge*> sIncidentList; // 原始边列表,用于输出结果
    list<CEdge*> IncidentList;  // 边的列表
    list<int> activeList;   // 活跃点列表
    vector<CVertex*> listVertex;   // 所有节点

    CGraph(){};
    ~CGraph(){};
    int getRest(int head, int tail);    // 获取边上剩余流量值
    int check(int head, int tail);      // 检查边是否存在
    void Update(int head, int tail, int mind);  // 更新边
    void preprocess();
    int isActive(int ID);   // 判断是否active
    int getHignestLabelVertexID();   // 获取activeList中label最大的节点ID
    void HignestLabelPreflowPush();     // 预流推送算法
    void showFlowInPath();  // 显示流在图中的流动情况
};

#endif
