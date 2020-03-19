#ifndef DIJKSTRA_UTILS_HH_
# define DIJKSTRA_UTILS_HH_

# include <string>
# include <unordered_set>
# include <unordered_map>
# include <queue>

namespace graph::dijkstra
{
  using VertexName_t = std::string;
  using Distance_t = std::uint32_t;
  using EdgeNumber_t = std::uint32_t;
  using NeighboringVertices_t = std::unordered_map<VertexName_t, Distance_t>;
  using VertexAdjacencyList_t =
    std::unordered_map<VertexName_t, NeighboringVertices_t>;

  struct Graph
  {
    VertexName_t startVertexName;
    VertexName_t endVertexName;
    EdgeNumber_t edgeNb;
    VertexAdjacencyList_t vertices;
  };
  
  struct Result
  {
    bool pathFound;
    Distance_t totalDistance;
    std::queue<VertexName_t> visitedVertexQueue;
  };
  
  struct VertexDistanceWithTrace
  {
    VertexName_t vertexName;
    Distance_t cumulativeDist;
    std::queue<VertexName_t> visitedVertexQueue;
  };
  
}

#endif
