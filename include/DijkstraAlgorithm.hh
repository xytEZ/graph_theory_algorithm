#ifndef DIJKSTRA_ALGORITHM_HH_
# define DIJKSTRA_ALGORITHM_HH_

# include "AbstractGraph.hh"
# include "DijkstraUtils.hh"
# include "DijkstraResPrinter.hh"

namespace graph::dijkstra
{
  class DijkstraAlgorithm : public AGraphAlgorithm
  {
    class NodeDistanceWithTraceGreater
    {
    public :
      bool operator()(const NodeDistanceWithTrace&,
		      const NodeDistanceWithTrace&) const noexcept;
    };
    
    friend void DijkstraResPrinter::init(const DijkstraAlgorithm&);
    
    const GraphDesc *_desc;
    GraphResult _result;
    
  public :
    DijkstraAlgorithm();
    DijkstraAlgorithm(const DijkstraAlgorithm&) = delete;
    DijkstraAlgorithm(DijkstraAlgorithm&&) = delete;
    ~DijkstraAlgorithm() override = default;
    DijkstraAlgorithm& operator=(const DijkstraAlgorithm&) = delete;
    DijkstraAlgorithm& operator=(DijkstraAlgorithm&&) = delete;
    void init(const DijkstraFileParser&) override;
    void accept(AGraphResPrinter&) const noexcept override;
    void execute() override;

  private :
    inline void graphDescribe() const noexcept;
  };
}

#endif
