#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

#include "BellmanFordFileParser.hh"

namespace graph::bellman_ford
{
  BellmanFordFileParser::BellmanFordFileParser(const std::string& fileName) :
    AGraphFileParser(fileName),
    _graph { "", "", 0, 0, { } }
  { }

  void BellmanFordFileParser::onParse(std::ifstream& ifs)
  {
    parseStartEndVertices(ifs);
    parseVertexEdgeNumber(ifs);
    parseEdges(ifs);
  }

  void BellmanFordFileParser::accept(AGraphAlgorithm& graphAlgo) const noexcept
  {
    graphAlgo.init(*this);
  }

  void BellmanFordFileParser::parseStartEndVertices(std::ifstream& ifs)
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

  void BellmanFordFileParser::parseVertexEdgeNumber(std::ifstream& ifs)
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

	oss << "Missing arguments about vertex and edge number";
	throw std::invalid_argument(oss.str());
      }
    else if (tokenVect.size() > 2)
      {
	std::ostringstream oss;

	oss << "Too much arguments about edge number line";
	throw std::invalid_argument(oss.str());
      }
    try
      {
	const std::string& vertexNbStr = tokenVect.at(0);

	_graph.vertexNb = boost::lexical_cast<VertexNumber_t>(vertexNbStr);
	if (vertexNbStr.at(0) == '-')
	  {
	    std::ostringstream oss;

	    oss << "Vertex number must be a positive integer";
	    throw std::invalid_argument(oss.str());
	  }
	else if (_graph.vertexNb == 0)
	  {
	    std::ostringstream oss;

	    oss << "Vertex number cannot be equal to 0";
	    throw std::invalid_argument(oss.str());
	  }
      }
    catch (const boost::bad_lexical_cast&)
      {
	std::ostringstream oss;

	oss << "Bad type. Vertex number must be a integer";
	throw std::invalid_argument(oss.str());
      }
    try
      {
	const std::string& edgeNbStr = tokenVect.at(1);

	_graph.edgeNb = boost::lexical_cast<EdgeNumber_t>(edgeNbStr);
	if (edgeNbStr.at(0) == '-')
	  {
	    std::ostringstream oss;

	    oss << "Edge number must be a positive integer";
	    throw std::invalid_argument(oss.str());
	  }
	else if (_graph.edgeNb == 0)
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

  void BellmanFordFileParser::parseEdges(std::ifstream& ifs)
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
	else if (tokenVect.at(0) == _graph.endVertexName)
	  {
	    std::ostringstream oss;

	    oss << "Cannot have links for "
		<< tokenVect.at(0)
		<< " vertex because it's the destination";
	    throw std::invalid_argument(oss.str());
	  }
	else if (tokenVect.at(0) == tokenVect.at(1))
	  {
	    std::ostringstream oss;
	    
	    oss << "Vertex and neighbor vertex must have different name";
	    throw std::invalid_argument(oss.str());
	  }
	try
	  {
	    const std::string& distanceStr = tokenVect.at(2);
	    Distance_t distance = boost::lexical_cast<Distance_t>(distanceStr);
   
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
	    
	    auto it = _graph.vertices.find(tokenVect.at(1));
	    
	    if (it != _graph.vertices.cend()
		&& it->second.find(tokenVect.at(0)) != it->second.cend())
	      {
		std::ostringstream oss;
		
		oss << "Duplicate edge from "
		    << tokenVect.at(0)
		    << " to "
		    << tokenVect.at(1)
		    << ". Graph must be oriented";
		throw std::invalid_argument(oss.str());
	      }
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

	oss << "Missing edge. Number of edges must be equal to "
	    << _graph.edgeNb;
	throw std::invalid_argument(oss.str());
      }
    else if (edgeCount > _graph.edgeNb)
      {
	std::ostringstream oss;

	oss << "Too much edge. Number of edges must be equal to "
	    << _graph.edgeNb;
	throw std::invalid_argument(oss.str());
      }
    else if (_graph.vertices.size() < _graph.vertexNb - 1)
      {
	std::ostringstream oss;

	oss << "Missing vertex. Number of vertex must be equal to "
	    << _graph.vertexNb;
	throw std::invalid_argument(oss.str());
      }
    else if (_graph.vertices.size() > _graph.vertexNb - 1)
      {
	std::ostringstream oss;

	oss << "Too much vertex. Number of vertex must be equal to "
	    << _graph.vertexNb;
	throw std::invalid_argument(oss.str());
      }
    for (const auto& pair : _graph.vertices)
      {
	for (const auto& pair2 : pair.second)
	  {
	    const VertexName_t& destVertex = pair2.first;
	    
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
