#ifndef GRAPH_ALGO_HH_
# define GRAPH_ALGO_HH_

namespace graph
{
  enum class GraphAlgo : std::uint8_t
    {
     NULL_VALUE,
     DIJKSTRA,
     BELLMAN_FORD,
     ASTAR
    };
}

#endif
