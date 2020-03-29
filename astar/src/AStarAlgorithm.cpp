#include <vector>
#include <queue>
#include <unordered_set>

#include "AStarFileParser.hh"

#include "AStarAlgorithm.hh"

namespace graph::astar
{
  AStarAlgorithm::VertexFScoreGreater::VertexFScoreGreater
  (const FGScoreMap_t& fGScoreMap) : _fGScoreMap(fGScoreMap)
  { }

  bool AStarAlgorithm::VertexFScoreGreater::operator()
    (const VertexName_t& lhs, const VertexName_t& rhs) const noexcept
  {
    return _fGScoreMap.at(lhs).f() > _fGScoreMap.at(rhs).f();
  }
  
  constexpr Distance_t AStarAlgorithm::INFINITE_VALUE;

  AStarAlgorithm::AStarAlgorithm() :
    _graph(nullptr),
    _result { false, { }, { } },
    _report(_graph, _result)
  { }

  void AStarAlgorithm::init(const AStarFileParser& parser)
  {
    _graph = &parser._graph;
  }

  void AStarAlgorithm::execute()
  {
    using VertexPrQueue_t = std::priority_queue<VertexName_t,
						std::vector<VertexName_t>,
						VertexFScoreGreater>;
    
    const std::size_t allocatedSize = _graph->vertices.size(); 
    
    _result.fGScoreMap.reserve(allocatedSize);
    for (const auto& [srcVertex, neighboringVertices] : _graph->vertices)
      _result.fGScoreMap.try_emplace(srcVertex,
				     INFINITE_VALUE,
				     INFINITE_VALUE);
    _result.fGScoreMap.try_emplace(_graph->endVertexName,
				   INFINITE_VALUE,
				   INFINITE_VALUE);
    
    FGScore& startFGScore = _result.fGScoreMap.at(_graph->startVertexName);

    startFGScore.g() = 0;
    startFGScore.f() = startFGScore.g() + heuristic(_graph->startVertexName);

    VertexPrQueue_t vertexQueue(_result.fGScoreMap);
    std::unordered_set<VertexName_t> visitedVertexSet;
    
    visitedVertexSet.reserve(allocatedSize);
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
	if (!visitedVertexSet.emplace(vertexName).second)
	  continue;
	for (const auto& [destVertex, srcToDestDist] :
	       _graph->vertices.at(vertexName))
          {
            if (visitedVertexSet.find(destVertex) == visitedVertexSet.cend()
		&& relax(vertexName, destVertex, srcToDestDist))
              vertexQueue.push(destVertex);
          }
      }
  }

  bool AStarAlgorithm::relax(const VertexName_t& srcVertex,
			     const VertexName_t& destVertex,
			     Distance_t srcToDestDist) noexcept
  {
    GScore_t gScore = _result.fGScoreMap.at(srcVertex).g();

    if (gScore + srcToDestDist > _result.fGScoreMap.at(destVertex).g())
      return false;
    _result.fGScoreMap.at(destVertex).g() = gScore + srcToDestDist;
    _result.fGScoreMap.at(destVertex).f() =
      (gScore + srcToDestDist) + heuristic(destVertex);
    if (auto res = _result.predecessorMap.try_emplace(destVertex, srcVertex);
        !res.second)
      res.first->second = srcVertex;
    return true;
  }

  Distance_t AStarAlgorithm::heuristic(const VertexName_t& vertexName)
    const noexcept
  {
    return _graph->heuristics.at(vertexName);
  }

  const AStarAlgoReport& AStarAlgorithm::getReport() const noexcept
  {
    return _report;
  }
}
