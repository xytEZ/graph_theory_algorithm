#ifndef ASTAR_ALGORITHM_HH_
# define ASTAR_ALGORITHM_HH_

# include <limits>

# include "AbstractGraph.hh"
# include "AStarUtils.hh"
# include "AStarAlgoReport.hh"

namespace graph::astar
{
  class AStarAlgorithm : public AGraphAlgorithm
  {
    class VertexFScoreGreater
    {
      const FGScoreMap_t& _fGScoreMap;

    public :
      VertexFScoreGreater(const FGScoreMap_t&);
      bool operator()(const VertexName_t&, const VertexName_t&) const noexcept;
    };
    
    static constexpr Distance_t INFINITE_VALUE =
      std::numeric_limits<Distance_t>::max();

    const Graph *_graph;
    Result _result;
    AStarAlgoReport _report;

  public :
    AStarAlgorithm();
    AStarAlgorithm(const AStarAlgorithm&) = delete;
    AStarAlgorithm(AStarAlgorithm&&) = delete;
    ~AStarAlgorithm() override = default;
    AStarAlgorithm& operator=(const AStarAlgorithm&) = delete;
    AStarAlgorithm& operator=(AStarAlgorithm&&) = delete;
    void init(const AStarFileParser&) override;
    void execute() override;
    const AStarAlgoReport& getReport() const noexcept override;
    
  private :
    bool relax(const VertexName_t&, const VertexName_t&, Distance_t) noexcept;
    Distance_t heuristic(const VertexName_t&) const noexcept;
  };
}

#endif
