#ifndef DIJKSTRA_UTILS_HH_
# define DIJKSTRA_UTILS_HH_

# include <string>
# include <unordered_set>
# include <unordered_map>
# include <queue>

namespace graph::dijkstra
{
  using NodeName_t = std::string;
  using Distance_t = std::uint32_t;
  using ArcNumber_t = std::uint32_t;
  using NeighboringNodes_t = std::unordered_map<NodeName_t, Distance_t>;
  using NodesWithNeighboringNodes_t =
    std::unordered_map<NodeName_t, NeighboringNodes_t>;

  struct NodeDistanceWithTrace
  {
    NodeName_t nodeName;
    Distance_t cumulativeDist;
    std::queue<NodeName_t> visitedNodeQueue;
  };
  
  struct GraphDesc
  {
    NodeName_t startNodeName;
    NodeName_t endNodeName;
    ArcNumber_t arcNb;
    NodesWithNeighboringNodes_t nodes;
  };
  
  struct GraphResult
  {
    bool pathFound;
    Distance_t totalDistance;
    std::queue<NodeName_t> visitedNodeQueue;
  };
}

#endif
