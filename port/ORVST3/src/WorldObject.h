#pragma once
#include "utility.h"

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

  public:
    WorldObject();
    WorldObject(vector<Vector3> vertices, vector<int> edgeIndices, vector<vector<int>> faceIndices)
    {
        vertexPath.clear();
        objVertices = vertices;
        vector<Line3D> edges = loadFromArrays(vertices, edgeIndices, faceIndices);
        getDrawPath(edges);
    }
    WorldObject(InputStream input, Vector3 position, Vector3 rotation) throws IOException
    {
        this(new ArrayList<>(), position, rotation);
        getDrawPath(loadFromInput(input));
    }
    vector<Line3D> loadFromArrays(vector<Vector3> vertices, vector<int> edgeIndices, vector<vector<int>> faceIndices);
    void getDrawPath(vector<Line3D> edges);
};