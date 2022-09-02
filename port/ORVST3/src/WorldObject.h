#pragma once
#include "utility.h"

using namespace CXXGRAPH;

class WorldObject
{
  private:
    vector<Vector3> objVertices;
    vector<float> triangles;
    vector<vector<Vector3>> vertexPath;
    Vector3 position;
    Vector3 rotation;
    WorldObject(vector<Vector3> objVerticesIn, vector<vector<Vector3>> vertexPathIn, Vector3 positionIn, Vector3 rotationIn)
    {
        objVertices = objVerticesIn;
        position = positionIn;
        rotation = rotationIn;
        vertexPath = vertexPathIn;
    }
    WorldObject(vector<Vector3> objVertices, Vector3 position, Vector3 rotation) { WorldObject(objVertices, vector<vector<Vector3>>(), position, rotation); }
    Vector3 addEdgeToPath(Vector3 realSource, Graph<Vector3> graph, Edge<Vector3> edge, std::set<Edge<Vector3>> visited, std::set<Edge<Vector3>> notVisited, std::vector<Vector3> path); // seemingly unused
    
  public:
    WorldObject();
    WorldObject(vector<Vector3> vertices, vector<int> edgeIndices, vector<vector<int>> faceIndices)
    {
        vertexPath.clear();
        objVertices = vertices;
        vector<Line3D> edges = loadFromArrays(vertices, edgeIndices, faceIndices);
        getDrawPath(edges);
    }
    WorldObject(std::string input, Vector3 position, Vector3 rotation)
    {
        WorldObject(vector<Vector3>(), position, rotation);
        getDrawPath(loadFromInput(input));
    }
    WorldObject(std::string input) { WorldObject(input, Vector3(), Vector3()); }
    vector<vector<Vector3>> getVertexPath() { return vertexPath; }
    vector<float> getTriangles() { return triangles; }
    vector<Line3D> loadFromArrays(vector<Vector3> vertices, vector<int> edgeIndices, vector<vector<int>> faceIndices);
    void getDrawPath(vector<Line3D> edges)
    {
        Graph<Vector3> graph;

        // Add all lines in frame to graph as vertices and edges.
        Stream.concat(edges.stream().map(Line3D::getStart), edges.stream().map(Line3D::getEnd)).distinct().forEach(graph::addVertex);

        int i = 0;
        for (Line3D edge : edges)
        {
            graph.addEdge(Edge<Vector3>(i,Node<Vector3>();
        }

        ConnectivityInspector<Vector3, DefaultEdge> inspector = new ConnectivityInspector<>(graph);

        // Chinese Postman can only be performed on connected graphs, so iterate over all connected
        // sub-graphs.
        vertexPath = inspector.connectedSets()
                         .parallelStream()
                         .map(vertices->{
                             ChinesePostman<Vector3, DefaultEdge> cp = new ChinesePostman<>();
                             AsSubgraph<Vector3, DefaultEdge> subgraph = new AsSubgraph<>(graph, vertices);
                             return cp.getCPPSolution(subgraph).getVertexList();
                         })
                         .toList();
    }
};