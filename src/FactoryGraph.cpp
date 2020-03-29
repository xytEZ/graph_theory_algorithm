#include "dijkstra/include/DijkstraFileParser.hh"
#include "dijkstra/include/DijkstraAlgorithm.hh"

#include "bellman_ford/include/BellmanFordFileParser.hh"
#include "bellman_ford/include/BellmanFordAlgorithm.hh"

#include "astar/include/AStarFileParser.hh"
#include "astar/include/AStarAlgorithm.hh"

#include "FactoryGraph.hh"

namespace graph::factory
{
  std::unique_ptr<AGraphFileParser>
  GraphFileParser::getParser(GraphAlgo algo, const std::string& fileName)
  {
    if (algo == GraphAlgo::DIJKSTRA)
      return std::make_unique<dijkstra::DijkstraFileParser>(fileName);
    else if (algo == GraphAlgo::BELLMAN_FORD)
      return std::make_unique<bellman_ford::BellmanFordFileParser>(fileName);
    else if (algo == GraphAlgo::ASTAR)
      return std::make_unique<astar::AStarFileParser>(fileName);
    else
      return nullptr;
  }

  std::unique_ptr<AGraphAlgorithm>
  GraphAlgorithm::getAlgorithm(GraphAlgo algo)
  {
    if (algo == GraphAlgo::DIJKSTRA)
      return std::make_unique<dijkstra::DijkstraAlgorithm>();
    else if (algo == GraphAlgo::BELLMAN_FORD)
      return std::make_unique<bellman_ford::BellmanFordAlgorithm>();
    else if (algo == GraphAlgo::ASTAR)
      return std::make_unique<astar::AStarAlgorithm>();
    else
      return nullptr;
  }
}
