#include <stdexcept>
#include <sstream>

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

  void AGraphFileParser::parse()
  {
    std::ifstream ifs(_fileName);
    
    if (ifs.fail())
      {
	std::ostringstream oss;

	oss << "Error by opening \""
	    << AGraphFileParser::_fileName
	    << "\" file";
	throw std::runtime_error(oss.str());
      }
    onParse(ifs);
  }
}
