#ifndef BELLMAN_FORD_FILE_PARSER_HH_
# define BELLMAN_FORD_FILE_PARSER_HH_

# include <fstream>
# include <string>

# include "AbstractGraph.hh"
# include "BellmanFordUtils.hh"
# include "BellmanFordAlgorithm.hh"

namespace graph::bellman_ford
{
  class BellmanFordFileParser : public AGraphFileParser
  {
    friend void BellmanFordAlgorithm::init(const BellmanFordFileParser&);

    Graph _graph;

  public :
    BellmanFordFileParser(const std::string&);
    ~BellmanFordFileParser() override = default;
    void accept(AGraphAlgorithm&) const noexcept override;

  private :
    void onParse(std::ifstream&) override;
    void parseStartEndVertices(std::ifstream&);
    void parseVertexEdgeNumber(std::ifstream&);
    void parseEdges(std::ifstream&);
  };
}

#endif
