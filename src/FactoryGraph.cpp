#include "dijkstra/include/DijkstraFileParser.hh"
#include "dijkstra/include/DijkstraAlgorithm.hh"
#include "dijkstra/include/DijkstraResPrinter.hh"

#include "FactoryGraph.hh"

namespace graph::factory
{
  std::unique_ptr<AGraphFileParser>
  GraphFileParser::getParser(GraphAlgo algo, const std::string& fileName)
  {
    if (algo == GraphAlgo::DIJKSTRA)
      return std::make_unique<dijkstra::DijkstraFileParser>(fileName);
    else
      return nullptr;
  }

  std::unique_ptr<AGraphAlgorithm>
  GraphAlgorithm::getAlgorithm(GraphAlgo algo)
  {
    if (algo == GraphAlgo::DIJKSTRA)
      return std::make_unique<dijkstra::DijkstraAlgorithm>();
    else
      return nullptr;
  }

  std::unique_ptr<AGraphResPrinter>
  GraphResPrinter::getPrinter(GraphAlgo algo)
  {
    if (algo == GraphAlgo::DIJKSTRA)
      return std::make_unique<dijkstra::DijkstraResPrinter>();
    else
      return nullptr;
  }
}
