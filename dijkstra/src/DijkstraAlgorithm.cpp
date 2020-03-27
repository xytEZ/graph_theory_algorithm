#include <vector>

#include "DijkstraFileParser.hh"

#include "DijkstraAlgorithm.hh"

namespace graph::dijkstra
{
  bool DijkstraAlgorithm::VertexCumulDistGreater::VertexCumulDistGreater
  ::operator()(const VertexCumulDist& lhs, const VertexCumulDist& rhs)
    const noexcept
  {
    return lhs.cumulativeDist > rhs.cumulativeDist;
  }

  constexpr Distance_t DijkstraAlgorithm::INFINITE_VALUE;
  
  DijkstraAlgorithm::DijkstraAlgorithm() :
    _graph(nullptr),
    _result { false, 0, { }, { }},
    _report(_graph, _result)
  { }
  
  void DijkstraAlgorithm::init(const DijkstraFileParser& parser)
  {
    _graph = &parser._graph;
  }

  void DijkstraAlgorithm::execute()
  {
    _result.bestDistMap.reserve(_graph->vertices.size());
    for (const auto& [srcVertex, neighboringVertices] : _graph->vertices)
      _result.bestDistMap.try_emplace(srcVertex, INFINITE_VALUE);
    _result.bestDistMap.try_emplace(_graph->endVertexName, INFINITE_VALUE);
    _result.bestDistMap.at(_graph->startVertexName) = 0;
    
    std::priority_queue<VertexCumulDist,
			std::vector<VertexCumulDist>,
			VertexCumulDistGreater> vertexCumulDistQueue;

    vertexCumulDistQueue.push({
			       _graph->startVertexName,
			       _result.bestDistMap
			       .at(_graph->startVertexName)
                             });
    while (!vertexCumulDistQueue.empty())
      {
	VertexCumulDist vertexCumulDist =
	  std::move(vertexCumulDistQueue.top());

	vertexCumulDistQueue.pop();
	if (vertexCumulDist.vertexName == _graph->endVertexName)
	  {
	    _result.pathFound = true;
	    break;
	  }
	for (const auto& [destVertex, srcToDestDist] :
	       _graph->vertices.at(vertexCumulDist.vertexName))
	  {
	    if (relax(vertexCumulDist, destVertex, srcToDestDist))
	      vertexCumulDistQueue.push({
					 destVertex,
					 _result.bestDistMap.at(destVertex)
		                       });
	  }
      }
  }

  bool DijkstraAlgorithm::relax(const VertexCumulDist& vertexCumulDist,
				const std::string& destVertex,
				Distance_t srcToDestDist) noexcept
  {
    if (vertexCumulDist.cumulativeDist + srcToDestDist
	> _result.bestDistMap.at(destVertex))
      return false;
    
    _result.bestDistMap.at(destVertex) = vertexCumulDist.cumulativeDist
      + srcToDestDist;
	
    auto res = _result
      .predecessorMap
      .try_emplace(destVertex, vertexCumulDist.vertexName);
	
    if (!res.second)
      res.first->second = vertexCumulDist.vertexName;
    return true;
  }

  const DijkstraAlgoReport& DijkstraAlgorithm::getReport() const noexcept
  {
    return _report;
  }
}
