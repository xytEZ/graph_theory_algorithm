#ifndef ASTAR_UTILS_HH_
# define ASTAR_UTILS_HH_

# include <string>
# include <unordered_map>

namespace graph::astar
{
  using VertexName_t = std::string;
  using Distance_t = std::int32_t;
  using EdgeNumber_t = std::uint32_t;
  using Heuristics_t = std::unordered_map<VertexName_t, Distance_t>;
  using FScore_t = Distance_t;
  using GScore_t = Distance_t;
  using NeighboringVertices_t = std::unordered_map<VertexName_t, Distance_t>;
  using VertexAdjacencyList_t = std::unordered_map<VertexName_t,
						   NeighboringVertices_t>;
  using PredecessorMap_t = std::unordered_map<VertexName_t, VertexName_t>;

  struct Graph
  {
    VertexName_t startVertexName;
    VertexName_t endVertexName;
    EdgeNumber_t edgeNb;
    Heuristics_t heuristics;
    VertexAdjacencyList_t vertices;
  };
  
  struct FGScore : std::pair<FScore_t, GScore_t>
  {
    using Base_t = std::pair<FScore_t, GScore_t>;
    using Base_t::Base_t;
    
    FScore_t f() const noexcept { return Base_t::first; }
    FScore_t& f() noexcept { return Base_t::first; }
    GScore_t g() const noexcept { return Base_t::second; }
    GScore_t& g() noexcept { return Base_t::second; }
  };
  
  using FGScoreMap_t = std::unordered_map<VertexName_t, FGScore>;
  
  struct Result
  {
    bool pathFound;
    FGScoreMap_t fGScoreMap;
    PredecessorMap_t predecessorMap;
  };
}

#endif
