#ifndef FACTORY_GRAPH_HH_
# define FACTORY_GRAPH_HH_

# include <string>
# include <memory>

# include "GraphAlgo.hh"

namespace graph
{
  class AGraphFileParser;
  class AGraphAlgorithm;
  class AGraphResPrinter;
  
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

    class GraphResPrinter
    {
    public :
      static std::unique_ptr<AGraphResPrinter>
      getPrinter(GraphAlgo);
    };
  }
}

#endif
