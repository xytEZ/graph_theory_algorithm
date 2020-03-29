#include <iostream>
#include <utility>
#include <stack>
#include <unordered_set>

#include "AStarAlgoReport.hh"

namespace graph::astar
{
  AStarAlgoReport::AStarAlgoReport(const Graph *&graph, const Result& result) :
    _graph(graph),
    _result(result)
  { }

  std::ostream& AStarAlgoReport::description(std::ostream& os)
    const noexcept
  {
    os << "Start vertex : " << _graph->startVertexName << std::endl;
    os << "End vertex : " << _graph->endVertexName << std::endl;
    os << "Edge number : " << _graph->edgeNb << std::endl;
    for (const auto& [srcVertex, neighboringVertices] : _graph->vertices)
      {
        os << srcVertex << " neighbors : ";

        auto start = neighboringVertices.cbegin();
        auto end = neighboringVertices.cend();

        while (start != end)
          {
            os << start->first << " (" << start->second << ")";
            if (std::next(start) != end)
              os << ", ";
            ++start;
          }
        os << std::endl;
      }

    std::unordered_set<VertexName_t> overestimatedHristicSet;

    overestimatedHristicSet.reserve(_graph->vertices.size());
    for (const auto& [vertex, heuristic] : _graph->heuristics)
      {
	os << vertex << " heuristic : " << heuristic << std::endl;
	if (vertex == _graph->endVertexName)
	  continue;
	for (const auto& [destVertex, srcToDestDist] :
	       _graph->vertices.at(vertex))
	  {
	    if (heuristic > srcToDestDist)
	      {
		overestimatedHristicSet.emplace(vertex);
		break;
	      }
	  }
      }
    if (!overestimatedHristicSet.empty())
      {
	os << std::endl
	   << "Following vertices heuristics are overestimated : ";
	for (const auto& vertex : overestimatedHristicSet)
	  os << vertex << " ";
	os << std::endl;
      }
    os << std::endl << std::endl;
    return os;
  }

  std::ostream& AStarAlgoReport::result(std::ostream& os) const noexcept
  {
    if (!_result.pathFound)
      os << "Path not found" << std::endl;
    else
      {
        os << "Possible optimal path with \"vertex (f, g, h, w)\" format where : "
	   << std::endl
	   << "f : total estimated cost from source until destination through a vertex n"
	   << std::endl
	   << "g : real cost from source until current vertex"
	   << std::endl
	   << "h : heuristic estimated cost from current vertex until destination (crow flies distance)"
	   << std::endl
	   << "w : edge weight from predecessor until current vertex"
	   << std::endl
	   << std::endl;
	  
	std::stack<VertexName_t> bestPathStack;
        VertexName_t predVertexName = _graph->endVertexName;
        std::unordered_map<VertexName_t, VertexName_t>::const_iterator it;

        while ((it = _result.predecessorMap.find(predVertexName))
               != _result.predecessorMap.cend())
          {
            bestPathStack.push(predVertexName);
            predVertexName = it->second;
          }
        bestPathStack.push(predVertexName);

	std::string prevVertexName;
	
	while (!bestPathStack.empty())
          {
            VertexName_t vertexName = std::move(bestPathStack.top());
	    
            bestPathStack.pop();
	    
	    const FGScore& fGScore = _result.fGScoreMap.at(vertexName);
	    
	    os << vertexName
	       << " ("
	       << fGScore.f()
	       << ", "
	       << fGScore.g()
	       << ", "
	       << _graph->heuristics.at(vertexName)
	       << ", ";
	    if (prevVertexName.empty())
	      os << "0";
	    else
	      os << _graph->vertices.at(prevVertexName).at(vertexName);
	    os << ")";
	    prevVertexName = std::move(vertexName);
            if (!bestPathStack.empty())
              os << " -> ";
          }
        os << std::endl;
      }
    return os;
  }
}
