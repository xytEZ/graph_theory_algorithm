#ifndef BELLMAN_FORD_ALGO_REPORT_HH_
# define BELLMAN_FORD_ALGO_REPORT_HH_

# include "AbstractGraph.hh"
# include "BellmanFordUtils.hh"

namespace graph::bellman_ford
{
  class BellmanFordAlgoReport : public IGraphAlgoReport
  {
    const Graph *&_graph;
    const Result& _result;

  public :
    BellmanFordAlgoReport(const Graph *&, const Result&);
    ~BellmanFordAlgoReport() override = default;
    BellmanFordAlgoReport(const BellmanFordAlgoReport&) = delete;
    BellmanFordAlgoReport(BellmanFordAlgoReport&&) = delete;
    BellmanFordAlgoReport& operator=(const BellmanFordAlgoReport&) = delete;
    BellmanFordAlgoReport& operator=(BellmanFordAlgoReport&&) = delete;
    std::ostream& description(std::ostream&) const noexcept override;
    std::ostream& result(std::ostream&) const noexcept override;
  };
}

#endif
