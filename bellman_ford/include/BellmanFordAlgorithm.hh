#ifndef BELLMAN_FORD_ALGORITHM_
# define BELLMAN_FORD_ALGORITHM_

# include <unordered_map>
# include <limits>

# include "AbstractGraph.hh"
# include "BellmanFordUtils.hh"
# include "BellmanFordAlgoReport.hh"

namespace graph::bellman_ford
{
  class BellmanFordAlgorithm : public AGraphAlgorithm
  {
    static constexpr Distance_t INFINITE_VALUE =
      std::numeric_limits<Distance_t>::max();
    
    const Graph *_graph;
    Result _result;
    BellmanFordAlgoReport _report;

  public :
    BellmanFordAlgorithm();
    BellmanFordAlgorithm(const BellmanFordAlgorithm&) = delete;
    BellmanFordAlgorithm(BellmanFordAlgorithm&&) = delete;
    ~BellmanFordAlgorithm() override = default;
    BellmanFordAlgorithm& operator=(const BellmanFordAlgorithm&) = delete;
    BellmanFordAlgorithm& operator=(BellmanFordAlgorithm&&) = delete;
    void init(const BellmanFordFileParser&) override;
    void execute() override;
    const BellmanFordAlgoReport& getReport() const noexcept override;

  private :
    void relax(std::unordered_map<VertexName_t, Distance_t>&,
	       std::unordered_map<VertexName_t, VertexName_t>&,
	       bool&,
	       const std::string&,
	       const std::string&,
	       Distance_t) const noexcept;
  };
}

#endif
