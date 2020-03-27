#include <vector>

#include "DijkstraFileParser.hh"

#include "DijkstraAlgorithm.hh"

namespace graph::dijkstra
{
  DijkstraAlgorithm::VertexCumulDistGreater::VertexCumulDistGreater
  (const std::unordered_map<VertexName_t, Distance_t>& bestDistMap) :
    _bestDistMap(bestDistMap)
  { }
  
  bool DijkstraAlgorithm::VertexCumulDistGreater::VertexCumulDistGreater
  ::operator()(const VertexName_t& lhs, const VertexName_t& rhs)
    const noexcept
  {
    return _bestDistMap.at(lhs) > _bestDistMap.at(rhs);
  }

  constexpr Distance_t DijkstraAlgorithm::INFINITE_VALUE;
  
  DijkstraAlgorithm::DijkstraAlgorithm() :
    _graph(nullptr),
    _result { false, { }, { } },
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
    
    std::priority_queue<VertexName_t,
			std::vector<VertexName_t>,
			VertexCumulDistGreater>
      vertexQueue(_result.bestDistMap);

    vertexQueue.push(_graph->startVertexName);
    while (!vertexQueue.empty())
      {
	VertexName_t vertexName = std::move(vertexQueue.top());

	vertexQueue.pop();
	if (vertexName == _graph->endVertexName)
	  {
	    _result.pathFound = true;
	    break;
	  }
	for (const auto& [destVertex, srcToDestDist] :
	       _graph->vertices.at(vertexName))
	  {
	    if (relax(vertexName, destVertex, srcToDestDist))
	      vertexQueue.push(destVertex);
	  }
      }
  }

  bool DijkstraAlgorithm::relax(const VertexName_t& srcVertex,
				const VertexName_t& destVertex,
				Distance_t srcToDestDist) noexcept
  {
    Distance_t vertexCumulDist = _result.bestDistMap.at(srcVertex);
    
    if (vertexCumulDist + srcToDestDist > _result.bestDistMap.at(destVertex))
      return false;
    _result.bestDistMap.at(destVertex) = vertexCumulDist + srcToDestDist;
    if (auto res = _result.predecessorMap.try_emplace(destVertex, srcVertex);
	!res.second)
      res.first->second = srcVertex;
    return true;
  }

  const DijkstraAlgoReport& DijkstraAlgorithm::getReport() const noexcept
  {
    return _report;
  }
}
