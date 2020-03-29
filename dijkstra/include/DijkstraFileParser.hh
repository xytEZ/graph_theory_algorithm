#ifndef DIJKSTRA_FILE_PARSER_HH_
# define DIJKSTRA_FILE_PARSER_HH_

# include <string>

# include "AbstractGraph.hh"
# include "DijkstraUtils.hh"
# include "DijkstraAlgorithm.hh"

namespace graph::dijkstra
{
  class DijkstraFileParser : public AGraphFileParser
  {
    friend void DijkstraAlgorithm::init(const DijkstraFileParser&);

    Graph _graph;
    
  public :
    DijkstraFileParser(const std::string&);
    ~DijkstraFileParser() override = default;
    void accept(AGraphAlgorithm&) const noexcept override;

  private :
    void onParse(std::ifstream&) override;
    void parseStartEndVertices(std::ifstream&);
    void parseEdgeNumber(std::ifstream&);
    void parseEdges(std::ifstream&);
  };
}

#endif
