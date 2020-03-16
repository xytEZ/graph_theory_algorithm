#ifndef FACTORY_GRAPH_HH_
# define FACTORY_GRAPH_HH_

# include <string>
# include <memory>

# include "GraphAlgo.hh"

namespace graph
{
  class AGraphFileParser;
  class AGraphAlgorithm;
  
  namespace factory
  {
    class GraphFileParser
    {
    public :
      static std::unique_ptr<AGraphFileParser>
      getParser(GraphAlgo, const std::string&);
    };

    class GraphAlgorithm
    {
    public :
      static std::unique_ptr<AGraphAlgorithm>
      getAlgorithm(GraphAlgo);
    };
  }
}

#endif
