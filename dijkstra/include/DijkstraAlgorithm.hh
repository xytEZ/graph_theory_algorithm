#ifndef DIJKSTRA_ALGORITHM_HH_
# define DIJKSTRA_ALGORITHM_HH_

# include "AbstractGraph.hh"
# include "DijkstraUtils.hh"
# include "DijkstraAlgoReport.hh"

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
    DijkstraAlgoReport _report;
    
  public :
    DijkstraAlgorithm();
    DijkstraAlgorithm(const DijkstraAlgorithm&) = delete;
    DijkstraAlgorithm(DijkstraAlgorithm&&) = delete;
    ~DijkstraAlgorithm() override = default;
    DijkstraAlgorithm& operator=(const DijkstraAlgorithm&) = delete;
    DijkstraAlgorithm& operator=(DijkstraAlgorithm&&) = delete;
    void init(const DijkstraFileParser&) override;
    void execute() override;
    const DijkstraAlgoReport& getReport() const noexcept override;
  };
}

#endif
