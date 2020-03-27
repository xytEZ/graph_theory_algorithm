#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "DijkstraFileParser.hh"

namespace graph::dijkstra
{
  DijkstraFileParser::DijkstraFileParser(const std::string& fileName) :
    AGraphFileParser(fileName),
    _graph { "", "", 0, { } }
  { }

  void DijkstraFileParser::onParse(std::ifstream& ifs)
  {
    parseStartEndVertices(ifs);
    parseEdgeNumber(ifs);
    parseEdges(ifs);
  }

  void DijkstraFileParser::accept(AGraphAlgorithm& graphAlgo)
    const noexcept
  {
    graphAlgo.init(*this);
  }

  void DijkstraFileParser::parseStartEndVertices(std::ifstream& ifs)
  {
    std::string line;
    
    std::getline(ifs, line, '\n');
    if (ifs.fail())
      throw std::runtime_error("Getline error");

    std::vector<std::string> tokenVect;

    boost::split(tokenVect, line, boost::is_any_of(" \t"));
    if (tokenVect.size() < 2)
      {
	std::ostringstream oss;

	oss << "Missing arguments about start and end vertices line";
	throw std::invalid_argument(oss.str());
      }
    else if (tokenVect.size() > 2)
      {
	std::ostringstream oss;

	oss << "Too much arguments about start and end vertices line";
	throw std::invalid_argument(oss.str());
      }
    _graph.startVertexName = tokenVect.at(0);
    _graph.endVertexName = tokenVect.at(1);
    if (_graph.startVertexName == _graph.endVertexName)
      {
	std::ostringstream oss;

	oss << "Start and end vertices must be different";
	throw std::invalid_argument(oss.str());
      }
  }

  void DijkstraFileParser::parseEdgeNumber(std::ifstream& ifs)
  {
    std::string line;

    std::getline(ifs, line, '\n');
    if (ifs.fail())
      throw std::runtime_error("Getline error");

    std::vector<std::string> tokenVect;
    
    boost::split(tokenVect, line, boost::is_any_of(" \t"));
    if (tokenVect.size() < 1)
      {
	std::ostringstream oss;

	oss << "Missing argument about edge number line";
	throw std::invalid_argument(oss.str());
      }
    else if (tokenVect.size() > 1)
      {
	std::ostringstream oss;

	oss << "Too much arguments about edge number line";
	throw std::invalid_argument(oss.str());
      }
    try
      {
	const std::string& edgeNbStr = tokenVect.at(0);

	_graph.edgeNb = boost::lexical_cast<EdgeNumber_t>(edgeNbStr);
	if (edgeNbStr.at(0) == '-')
	  {
	    std::ostringstream oss;

	    oss << "Edge number must be a positive integer";
	    throw std::invalid_argument(oss.str());
	  }
	if (_graph.edgeNb == 0)
	  {
	    std::ostringstream oss;
	    
	    oss << "Edge number cannot be equal to 0";
	    throw std::invalid_argument(oss.str());
	  }
      }
    catch (const boost::bad_lexical_cast&)
      {
	std::ostringstream oss;

	oss << "Bad type. Edge number must be a integer";
	throw std::invalid_argument(oss.str());
      }
  }

  void DijkstraFileParser::parseEdges(std::ifstream& ifs)
  {
    std::string line;
    std::uint32_t edgeCount = 0;

    while (std::getline(ifs, line, '\n'))
      {
	if (ifs.fail())
	  throw std::runtime_error("Getline error");

	std::vector<std::string> tokenVect;

	boost::split(tokenVect, line, boost::is_any_of(" \t"));
	if (tokenVect.size() < 3)
	  {
	    std::ostringstream oss;

	    oss << "Missing arguments about "
		<< "vertex, neighbor vertex and distance line";
	    throw std::invalid_argument(oss.str());
	  }
	else if (tokenVect.size() > 3)
	  {
	    std::ostringstream oss;

	    oss << "Too much arguments about "
		<< "vertex, neighbor vertex and distance line";
	    throw std::invalid_argument(oss.str());
	  }
	if (tokenVect.at(0) == tokenVect.at(1))
	  {
	    std::ostringstream oss;
	    
	    oss << "Vertex and neighbor vertex must have different name";
	    throw std::invalid_argument(oss.str());
	  }
	try
	  {
	    const std::string& distanceStr = tokenVect.at(2);
	    Distance_t distance = boost::lexical_cast<Distance_t>(distanceStr);

	    if (distanceStr.at(0) == '-')
	      {
		std::ostringstream oss;

		oss << "Distance must be a positive integer";
		throw std::invalid_argument(oss.str());
	      }
	    else if (distance == 0)
	      {
		std::ostringstream oss;

		oss << "Distance cannot be equal to 0";
		throw std::invalid_argument(oss.str());
	      }   
	    if (!_graph
		.vertices
		.try_emplace(tokenVect.at(0), NeighboringVertices_t { })
		.first->second.try_emplace(tokenVect.at(1), distance)
		.second)
	      {
		std::ostringstream oss;

		oss << "Duplicate edge from "
		    << tokenVect.at(0)
		    << " to "
		    << tokenVect.at(1);
		throw std::invalid_argument(oss.str());
	      }
	    if (auto it = _graph.vertices.find(tokenVect.at(1));
		it != _graph.vertices.cend())
	      {
		if (auto it2 = it->second.find(tokenVect.at(0));
		    it2 != it->second.cend())
		  {
		    if (it2->second != distance)
		      {
			std::ostringstream oss;
			
			oss << "Duplicate edge from "
			    << tokenVect.at(0)
			    << " to "
			    << tokenVect.at(1)
			    << " with different distance";
			throw std::invalid_argument(oss.str());
		      }
		  }
		else
		  ++edgeCount;
	      }
	    else
	      ++edgeCount;
	  }
	catch (const boost::bad_lexical_cast&)
	  {
	    std::ostringstream oss;

	    oss << "Bad type. Distance must be a integer";
	    throw std::invalid_argument(oss.str());
	  }
      }
    if (edgeCount < _graph.edgeNb)
      {
	std::ostringstream oss;

	oss << "Missing edges. Number of edges must be equal to "
	    << _graph.edgeNb;
	throw std::invalid_argument(oss.str());
      }
    else if (edgeCount > _graph.edgeNb)
      {
	std::ostringstream oss;

	oss << "Too much edges. Number of edges must be equal to "
	    << _graph.edgeNb;
	throw std::invalid_argument(oss.str());
      }
    for (const auto& [srcVertex, neighboringVertices] : _graph.vertices)
      {
	for (const auto& [destVertex, srcToDestDist] : neighboringVertices)
	  {
	    if (_graph.vertices.find(destVertex) == _graph.vertices.cend()
		&& destVertex != _graph.endVertexName)
	      {
		std::ostringstream oss;
		
		oss << "Missing info edge with vertex " << destVertex;
		throw std::invalid_argument(oss.str());
	      }
	  }
      }
  }
}
