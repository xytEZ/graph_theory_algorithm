#ifndef ABSTRACT_GRAPH_HH_
# define ABSTRACT_GRAPH_HH_

# include <string>
# include <ostream>
# include <fstream>

namespace graph
{
  namespace dijkstra
  {
    class DijkstraFileParser;
  }

  namespace bellman_ford
  {
    class BellmanFordFileParser;
  }
  
  class AGraphAlgorithm
  {
  public :
    virtual ~AGraphAlgorithm() = default;
    virtual void init(const dijkstra::DijkstraFileParser&);
    virtual void init(const bellman_ford::BellmanFordFileParser&);
    virtual void execute() = 0;
    virtual std::ostream& description(std::ostream&) const noexcept = 0;
    virtual std::ostream& result(std::ostream&) const noexcept = 0;
  };
  
  class AGraphFileParser
  {
  protected :
    const std::string& _fileName;
    
  public :
    AGraphFileParser(const std::string&);
    virtual ~AGraphFileParser() = default;
    virtual void accept(AGraphAlgorithm&) const noexcept = 0;
    void parse();

  private :
    virtual void onParse(std::ifstream&) = 0; 
  };
}

#endif
