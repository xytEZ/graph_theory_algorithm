#ifndef DIJKSTRA_ALGORITHM_HH_
# define DIJKSTRA_ALGORITHM_HH_

# include <limits>

# include "AbstractGraph.hh"
# include "DijkstraUtils.hh"
# include "DijkstraAlgoReport.hh"

namespace graph::dijkstra
{
  class DijkstraAlgorithm : public AGraphAlgorithm
  {
    class VertexCumulDistGreater
    {
    public :
      bool operator()(const VertexCumulDist&, const VertexCumulDist&)
	const noexcept;
    };

    static constexpr Distance_t INFINITE_VALUE =
      std::numeric_limits<Distance_t>::max();
    
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

  private :
    bool relax(const VertexCumulDist&, const std::string&, Distance_t)
      noexcept;
  };
}

#endif
