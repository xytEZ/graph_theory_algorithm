#ifndef ABSTRACT_GRAPH_HH_
# define ABSTRACT_GRAPH_HH_

# include <string>

namespace graph
{
  namespace dijkstra
  {
    class DijkstraAlgorithm;
    class DijkstraFileParser;
  }
    
  class AGraphResPrinter
  {
  public :
    virtual ~AGraphResPrinter() = default;
    virtual void printAlgoResult() const noexcept= 0;
    virtual void init(const dijkstra::DijkstraAlgorithm&);
  };
  
  class AGraphAlgorithm
  {
  public :
    virtual ~AGraphAlgorithm() = default;
    virtual void execute() = 0;
    virtual void accept(AGraphResPrinter&) const noexcept = 0;
    virtual void init(const dijkstra::DijkstraFileParser&);
  };
  
  class AGraphFileParser
  {
  protected :
    const std::string& _fileName;
    
  public :
    AGraphFileParser(const std::string&);
    virtual ~AGraphFileParser() = default;
    virtual void parse() = 0;
    virtual void accept(AGraphAlgorithm&) const noexcept = 0;
  };
}

#endif
