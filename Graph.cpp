#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <exception>
#include <queue>
#include <utility>

using namespace std;

class GraphException: public exception {};

class Graph
{
public:

    vector<vector<int>> adj;
    int V, E;

    Graph(const string &inputFilePath, bool directedBool = false)
    {
        ifstream f(inputFilePath);
        int nodeA, nodeB;

        f >> V >> E;
        adj.resize(V);

        for (int _ = 0; _ < E; _++)
        {
            f >> nodeA >> nodeB;
            adj[nodeA].push_back(nodeB);
            if (!directedBool) adj[nodeB].push_back(nodeA);
        }

        f.close();
    }


    string str() const
    {
        string resStr;

        for (int i = 0; i < adj.size(); i++)
        {
            resStr += to_string(i) + ": ";

            for (int j: adj[i])
            {
                resStr += to_string(j) + " ";
            }

            resStr += "\n";
        }

        return resStr;
    }


    void __dfs(int nowNode, vector<bool> &markList, int &nodeCount)
    {
        for (auto nextNode: adj[nowNode])
        {
            if (!markList[nextNode])
            {
                markList[nextNode] = true;
                nodeCount++;
                __dfs(nextNode, markList, nodeCount);
            }
        }
    }


    int DFS(int startNode = 0)
    {
        vector<bool> markList(V, false);
        int nodeCount = 0;
        __dfs(startNode, markList, nodeCount);

        return nodeCount;
    }


    void __findPathDFS(vector<int> &pathStack, vector<bool> &markList, int tarNode)
    {
        int nowNode = pathStack.back();

        for (auto nextNode: adj[nowNode])
        {
            if (nextNode == tarNode)
            {
                pathStack.push_back(nextNode);
                throw GraphException();
            }
            else if (!markList[nextNode])
            {
                markList[nextNode] = true;
                pathStack.push_back(nextNode);
                __findPathDFS(pathStack, markList, tarNode);
                markList[nextNode] = false;
                pathStack.erase(pathStack.end() - 1);
            }
        }
    }


    vector<int> findPathDFS(int startNode, int endNode)
    {
        vector<int> pathStack;
        pathStack.push_back(startNode);
        vector<bool> markList(V, false);

        try { __findPathDFS(pathStack, markList, endNode); } catch (GraphException) {}

        return pathStack;
    }


    vector<int> findPathBFS(int startNode, int endNode)
    {
        queue<pair<int, int>> bfsQueue;
        bfsQueue.push({0, startNode});
        vector<bool> markList(V, false);
        pair<int, int> nowNode;
        vector<int> pathList;
        int findIdx = 0;

        while (bfsQueue.size())
        {
            nowNode = bfsQueue.front();
            bfsQueue.pop();

            if (pathList.size() <= nowNode.first)
            {
                pathList.push_back(-1);
            }

            pathList[nowNode.first] = nowNode.second;

            if (nowNode.second == endNode)
            {
                findIdx = nowNode.first;
                break;
            }

            for (auto i: adj[nowNode.second])
            {
                if (!markList[i])
                {
                    markList[i] = true;
                    bfsQueue.push({nowNode.first + 1, i});
                }
            }
        }

        pathList.resize(findIdx + 1);

        return pathList;
    }


    int calcConnectCount()
    {
        vector<bool> markList(V, false);
        int connectCount = 0, _;

        for (int idx = 0; idx < V; idx++)
        {
            if (!markList[idx])
            {
                connectCount++;
                __dfs(idx, markList, _);
            }
        }

        return connectCount;
    }
};


int main()
{
    Graph g("tinyG.txt");
    cout << g.calcConnectCount();
}