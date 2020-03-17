#ifndef DIJKSTRA_ALGORITHM_HH_
# define DIJKSTRA_ALGORITHM_HH_

# include "AbstractGraph.hh"
# include "DijkstraUtils.hh"

namespace graph::dijkstra
{
  class DijkstraAlgorithm : public AGraphAlgorithm
  {
    class VertexDistanceWithTraceGreater
    {
    public :
      bool operator()(const VertexDistanceWithTrace&,
		      const VertexDistanceWithTrace&) const noexcept;
    };
    
    const Graph *_graph;
    Result _result;
    
  public :
    DijkstraAlgorithm();
    DijkstraAlgorithm(const DijkstraAlgorithm&) = delete;
    DijkstraAlgorithm(DijkstraAlgorithm&&) = delete;
    ~DijkstraAlgorithm() override = default;
    DijkstraAlgorithm& operator=(const DijkstraAlgorithm&) = delete;
    DijkstraAlgorithm& operator=(DijkstraAlgorithm&&) = delete;
    void init(const DijkstraFileParser&) override;
    void execute() override;
    std::ostream& description(std::ostream&) const noexcept override;
    std::ostream& result(std::ostream&) const noexcept override;
  };
}

#endif
