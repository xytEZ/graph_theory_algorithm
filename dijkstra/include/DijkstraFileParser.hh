#ifndef DIJKSTRA_FILE_PARSER_HH_
# define DIJKSTRA_FILE_PARSER_HH_

# include <fstream>
# include <string>

# include "AbstractGraph.hh"
# include "DijkstraUtils.hh"
# include "DijkstraAlgorithm.hh"

namespace graph::dijkstra
{
  class DijkstraFileParser : public AGraphFileParser
  {
    friend void DijkstraAlgorithm::init(const DijkstraFileParser&);

    GraphDesc _desc;
    
  public :
    DijkstraFileParser(const std::string&);
    ~DijkstraFileParser() override = default;
    void parse() override;
    void accept(AGraphAlgorithm&) const noexcept override;

  private :
    void parseStartEndNodes(std::ifstream&);
    void parseArcNumber(std::ifstream&);
    void parseNodes(std::ifstream&);
  };
}

#endif
