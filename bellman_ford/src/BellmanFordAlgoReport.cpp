#include <iostream>
#include <stack>
#include <utility>

#include "BellmanFordAlgoReport.hh"

namespace graph::bellman_ford
{
  BellmanFordAlgoReport::BellmanFordAlgoReport(const Graph *&graph,
					       const Result& result) :
    _graph(graph),
    _result(result)
  { }

  std::ostream& BellmanFordAlgoReport::description(std::ostream& os)
    const noexcept
  {
    os << "Start vertex : " << _graph->startVertexName << std::endl;
    os << "End vertex : " << _graph->endVertexName << std::endl;
    os << "Vertex number : " << _graph->vertexNb << std::endl;
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
    os << std::endl << std::endl;
    return os;
  }

  std::ostream& BellmanFordAlgoReport::result(std::ostream& os) const noexcept
  {
    if (!_result.pathFound)
      {
	os << "Path not found";
	if (_result.negativeCycle)
	  os << ". Negative cycle !";
	os << std::endl;
      }
    else
      {
	os << "Vertex   Best distance from start vertex"
	   << " until destination vertex"
	   << std::endl;
	for (const auto& [destVertex, destVertexDist] : _result.bestDistMap)
	  os << destVertex << "\t\t\t\t" << destVertexDist << std::endl;
	os << std::endl
	   << "Best distance from start to end vertex : "
	   << _result.bestDistMap.at(_graph->endVertexName)
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
	
	os << "Best path : ";
	while (!bestPathStack.empty())
	  {
	    VertexName_t vertexName = std::move(bestPathStack.top());

	    bestPathStack.pop();
	    os << vertexName;
	    if (prevVertexName.empty())
	      os << " (0)";
	    else
	      os << " ("
		 << _graph->vertices.at(prevVertexName).at(vertexName)
		 << ")";
	    prevVertexName = std::move(vertexName);
	    if (!bestPathStack.empty())
	      os << " -> ";
	  }
	os << std::endl;
      }
    return os;
  }
}
