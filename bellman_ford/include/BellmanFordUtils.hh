#ifndef BELLMAN_FORD_UTILS_HH_
# define BELLMAN_FORD_UTILS_HH_

# include <string>
# include <unordered_map>

namespace graph::bellman_ford
{
  using VertexName_t = std::string;
  using Distance_t = std::int32_t;
  using VertexNumber_t = std::uint32_t;
  using EdgeNumber_t = std::uint32_t;
  using NeighboringVertices_t = std::unordered_map<VertexName_t, Distance_t>;
  using VertexAdjacencyList_t =
    std::unordered_map<VertexName_t, NeighboringVertices_t>;
  
  struct Graph
  {
    VertexName_t startVertexName;
    VertexName_t endVertexName;
    VertexNumber_t vertexNb;
    EdgeNumber_t edgeNb;
    VertexAdjacencyList_t vertices;
  };

  struct Result
  {
    bool pathFound;
    bool negativeCycle;
    std::unordered_map<VertexName_t, Distance_t> bestDistMap;
    std::unordered_map<VertexName_t, VertexName_t> predecessorMap;
  };
}

#endif
