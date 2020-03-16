#include <stdexcept>

#include "AbstractGraph.hh"

namespace graph
{
  void AGraphAlgorithm::init(const dijkstra::DijkstraFileParser&)
  {
    throw std::logic_error("Must be redefined by subclass");
  }

  AGraphFileParser::AGraphFileParser(const std::string& fileName) :
    _fileName(fileName)
  { }
}
