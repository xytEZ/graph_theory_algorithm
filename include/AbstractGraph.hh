#ifndef ABSTRACT_GRAPH_HH_
# define ABSTRACT_GRAPH_HH_

# include <string>
# include <ostream>

namespace graph
{
  namespace dijkstra
  {
    class DijkstraFileParser;
  }
  
  class AGraphAlgorithm
  {
  public :
    virtual ~AGraphAlgorithm() = default;
    virtual void init(const dijkstra::DijkstraFileParser&); 
    virtual void execute() = 0;
    virtual std::ostream& graphDesc(std::ostream&) const noexcept = 0;
    virtual std::ostream& graphRes(std::ostream&) const noexcept = 0;
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
