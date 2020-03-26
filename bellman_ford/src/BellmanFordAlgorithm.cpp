#include <iostream>

#include "BellmanFordFileParser.hh"

#include "BellmanFordAlgorithm.hh"

namespace graph::bellman_ford
{
  constexpr Distance_t BellmanFordAlgorithm::INFINITE_VALUE;
  
  BellmanFordAlgorithm::BellmanFordAlgorithm() :
    _graph(nullptr),
    _result { false, false, { }, { } },
    _report(_graph, _result)
  { }

  void BellmanFordAlgorithm::init(const BellmanFordFileParser& parser)
  {
    _graph = &parser._graph;
  }

  void BellmanFordAlgorithm::execute()
  {
    _result.bestDistMap.reserve(_graph->vertexNb);
    for (const auto& [srcVertex, neighboringVertices] : _graph->vertices)
      _result.bestDistMap.try_emplace(srcVertex, INFINITE_VALUE);
    _result.bestDistMap.try_emplace(_graph->endVertexName, INFINITE_VALUE);
    _result.bestDistMap.at(_graph->startVertexName) = 0;

    bool modification = true;
    
    for (std::uint32_t i = 1; i <= _graph->vertexNb - 1 && modification; ++i)
      {
	modification = false;
	for (const auto& [srcVertex, neighboringVertices] : _graph->vertices)
	  {
	    for (const auto& [destVertex, srcToDestDist] : neighboringVertices)
	      relax(modification, srcVertex, destVertex, srcToDestDist);
	  }
      }
    for (const auto& [srcVertex, neighboringVertices] : _graph->vertices)
      {
	for (const auto& [destVertex, srcToDestDist] : neighboringVertices)
	  {
	    Distance_t srcVertexDist = _result.bestDistMap.at(srcVertex);
	    Distance_t destVertexDist = _result.bestDistMap.at(destVertex);

	    if (srcVertexDist + srcToDestDist < destVertexDist)
	      {
		_result.negativeCycle = true;
		return;
	      }
	  }
      }
    _result.pathFound = true;
  }
  
  void BellmanFordAlgorithm::relax(bool& modification,
				   const std::string& srcVertex,
				   const std::string& destVertex,
				   Distance_t srcToDestDist) noexcept
  {
    Distance_t srcVertexDist = _result.bestDistMap.at(srcVertex);
    Distance_t destVertexDist = _result.bestDistMap.at(destVertex);

    if (srcVertexDist != INFINITE_VALUE
	&& srcVertexDist + srcToDestDist < destVertexDist)
      {
	_result.bestDistMap.at(destVertex) = srcVertexDist + srcToDestDist;
	
	auto res = _result.predecessorMap.try_emplace(destVertex, srcVertex);

	if (!res.second)
	  res.first->second = srcVertex;
	modification = true;
      }
  }

  const BellmanFordAlgoReport& BellmanFordAlgorithm::getReport() const noexcept
  {
    return _report;
  }
}
