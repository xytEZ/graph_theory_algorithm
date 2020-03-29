#ifndef ASTAR_FILE_PARSER_HH_
# define ASTAR_FILE_PARSER_HH_

# include <string>

# include "AbstractGraph.hh"
# include "AStarUtils.hh"
# include "AStarAlgorithm.hh"

namespace graph::astar
{
  class AStarFileParser : public AGraphFileParser
  {
    friend void AStarAlgorithm::init(const AStarFileParser&);

    Graph _graph;

  public :
    AStarFileParser(const std::string&);
    ~AStarFileParser() override = default;
    void accept(AGraphAlgorithm&) const noexcept override;

  private :
    void onParse(std::ifstream&) override;
    void parseStartEndVertices(std::ifstream&);
    void parseEdgeNumber(std::ifstream&);
    void parseEdges(std::ifstream&);
    void parseHeuristics(std::ifstream&);
    void checkAllHeuristicsAreDefined() const;
  };
}

#endif
