#include <utility>
#include <unordered_set>
#include <vector>

#include "DijkstraFileParser.hh"

#include "DijkstraAlgorithm.hh"

namespace graph::dijkstra
{
  bool DijkstraAlgorithm::VertexDistanceWithTraceGreater
  ::VertexDistanceWithTraceGreater::operator()
    (const VertexDistanceWithTrace& lhs, const VertexDistanceWithTrace& rhs)
    const noexcept
  {
    return lhs.cumulativeDist > rhs.cumulativeDist;
  }
  
  DijkstraAlgorithm::DijkstraAlgorithm() :
    _graph(nullptr),
    _result { false, 0, std::queue<VertexName_t> { } },
    _report(_graph, _result)
  { }
  
  void DijkstraAlgorithm::init(const DijkstraFileParser& parser)
  {
    _graph = &parser._graph;
  }

  void DijkstraAlgorithm::execute()
  {
    using PrQueue_t = std::priority_queue<VertexDistanceWithTrace,
					  std::vector<VertexDistanceWithTrace>,
					  VertexDistanceWithTraceGreater>;

    std::unordered_set<VertexName_t> visitedVertexSet;
    PrQueue_t vdwtQueue;

    visitedVertexSet.reserve(_graph->vertices.size());
    vdwtQueue
      .push({ _graph->startVertexName, 0, std::queue<VertexName_t> { } }); 
    while (!vdwtQueue.empty())
      {
	VertexDistanceWithTrace vdwt = std::move(vdwtQueue.top());

	if (vdwt.vertexName == _graph->endVertexName)
	  {
	    _result.pathFound = true;
	    _result.totalDistance = vdwt.cumulativeDist;
	    _result.visitedVertexQueue = std::move(vdwt.visitedVertexQueue);
	    _result.visitedVertexQueue.push(vdwt.vertexName);
	    break;
	  }
	vdwtQueue.pop();
	if (!visitedVertexSet.emplace(vdwt.vertexName).second)
	  continue;
	for (const auto& [destVertex, srcToDestDist] :
	       _graph->vertices.at(vdwt.vertexName))
	  {
	    if (visitedVertexSet.find(destVertex) == visitedVertexSet.cend())
	      {
		VertexDistanceWithTrace newVdwt;

		newVdwt.vertexName = destVertex;
		newVdwt.cumulativeDist = vdwt.cumulativeDist + srcToDestDist;
		newVdwt.visitedVertexQueue = vdwt.visitedVertexQueue;
		newVdwt.visitedVertexQueue.push(vdwt.vertexName);
		vdwtQueue.push(std::move(newVdwt));
	      }
	  }    
      }
  }

  const DijkstraAlgoReport& DijkstraAlgorithm::getReport() const noexcept
  {
    return _report;
  }
}
