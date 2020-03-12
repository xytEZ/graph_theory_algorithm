#ifndef DIJKSTRA_RES_PRINTER_HH_
# define DIJKSTRA_RES_PRINTER_HH_

# include "AbstractGraph.hh"
# include "DijkstraUtils.hh"

namespace graph::dijkstra
{
  class DijkstraResPrinter : public AGraphResPrinter
  {
    const GraphDesc *_desc;
    GraphResult *_result;
    
  public :
    DijkstraResPrinter();
    DijkstraResPrinter(const DijkstraResPrinter&) = delete;
    DijkstraResPrinter(DijkstraResPrinter&&) = delete;
    ~DijkstraResPrinter() override = default;
    DijkstraResPrinter& operator=(const DijkstraResPrinter&) = delete;
    DijkstraResPrinter& operator=(DijkstraResPrinter&&) = delete;
    void init(const DijkstraAlgorithm&) override;
    void printAlgoResult() const noexcept override;
  };
}

#endif
