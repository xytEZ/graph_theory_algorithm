#ifndef ASTAR_ALGO_REPORT_HH_
# define ASTAR_ALGO_REPORT_HH_

# include "AbstractGraph.hh"
# include "AStarUtils.hh"

namespace graph::astar
{
  class AStarAlgoReport : public IGraphAlgoReport
  {
    const Graph *&_graph;
    const Result& _result;
    
  public :
    AStarAlgoReport(const Graph *&, const Result&);
    ~AStarAlgoReport() override = default;
    AStarAlgoReport(const AStarAlgoReport&) = delete;
    AStarAlgoReport(AStarAlgoReport&&) = delete;
    AStarAlgoReport& operator=(const AStarAlgoReport&) = delete;
    AStarAlgoReport& operator=(AStarAlgoReport&&) = delete;
    std::ostream& description(std::ostream&) const noexcept override;
    std::ostream& result(std::ostream&) const noexcept override;

  private :
    bool isAdmissible(const VertexName_t&) const noexcept;
  };
}

#endif
