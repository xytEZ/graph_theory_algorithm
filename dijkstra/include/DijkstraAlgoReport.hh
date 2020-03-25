#ifndef DIJKSTRA_ALGO_REPORT_HH_
# define DIJKSTRA_ALGO_REPORT_HH_

# include "AbstractGraph.hh"
# include "DijkstraUtils.hh"

namespace graph::dijkstra
{
  class DijkstraAlgoReport : public IGraphAlgoReport
  {
    const Graph *&_graph;
    const Result& _result;

  public :
    DijkstraAlgoReport(const Graph *&, const Result&);
    ~DijkstraAlgoReport() override = default;
    DijkstraAlgoReport(const DijkstraAlgoReport&) = delete;
    DijkstraAlgoReport(DijkstraAlgoReport&&) = delete;
    DijkstraAlgoReport& operator=(const DijkstraAlgoReport&) = delete;
    DijkstraAlgoReport& operator=(DijkstraAlgoReport&&) = delete;
    std::ostream& description(std::ostream&) const noexcept override;
    std::ostream& result(std::ostream&) const noexcept override;
  };
}

#endif
