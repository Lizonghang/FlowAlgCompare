#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include "ShortestAugmentingPath.h"
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

void fileInput(CGraph & graph, string fileName);

int main(int argc, const char * argv[]) {
    CGraph graph;
    string graphFile = argv[1];
    int maxFlow;
    fileInput(graph, graphFile);
    // clock_t startTime = clock();    // 起始计时器
    graph.ShortestAugmentingPath();
    // clock_t endTime = clock();  // 结束计时器
    maxFlow = graph.getMaxFlowValue();
    cout << "最大输入流的大小为: " << maxFlow << endl;
    cout << "图中各边通过流量值:" << endl;
    graph.showFlowInPath();
    // cout << "最短增广路算法耗时: " << endTime - startTime << " clock" << endl;
    return 0;
}

void fileInput(CGraph & graph, string fileName)
{
    int i = 0;
    int head, tail, capacity;
    CVertex *v;
    CEdge *e;
    ifstream fileIn;
    fileIn.open(fileName);
    fileIn >> graph.edgeNum;
    fileIn >> graph.vertexNum;
    for (i = 0; i < graph.vertexNum; i++) {
        v = new CVertex();
        v->id = i;
        graph.listVertex.push_back(v);
    }
    for (i = 0; i < graph.edgeNum; i++) {
        fileIn >> head;
        fileIn >> tail;
        fileIn >> capacity;
        graph.listVertex[head]->relateList.push_back(tail);
        e = new CEdge(head, tail, capacity, 1);
        graph.IncidentList.push_back(e);
        graph.sIncidentList.push_back(e);
    }
}
