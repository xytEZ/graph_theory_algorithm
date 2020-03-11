#include <stdexcept>

#include "AbstractGraph.hh"

namespace graph
{
  void AGraphResPrinter::init(const dijkstra::DijkstraAlgorithm&)
  {
    throw std::logic_error("Must be redefined by subclass");
  }

  void AGraphAlgorithm::init(const dijkstra::DijkstraFileParser&)
  {
    throw std::logic_error("Must be redefined by subclass");
  }

  AGraphFileParser::AGraphFileParser(const std::string& fileName) :
    _fileName(fileName)
  { }
}
