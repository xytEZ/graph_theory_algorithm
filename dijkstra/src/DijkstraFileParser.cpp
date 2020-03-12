#include <sstream>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "DijkstraFileParser.hh"

namespace graph::dijkstra
{
  DijkstraFileParser::DijkstraFileParser(const std::string& fileName) :
    AGraphFileParser(fileName),
    _desc({ "", "", 0, { } })
  { }

  void DijkstraFileParser::parse()
  {    
    std::ifstream ifs(AGraphFileParser::_fileName);

    if (ifs.fail())
      {
	std::ostringstream oss;

	oss << "Error by opening \""
	    << AGraphFileParser::_fileName
	    << "\" file";
	throw std::runtime_error(oss.str());
      }
    parseStartEndNodes(ifs);
    parseArcNumber(ifs);
    parseNodes(ifs);
  }

  void DijkstraFileParser::accept(AGraphAlgorithm& graphAlgo)
    const noexcept
  {
    graphAlgo.init(*this);
  }

  void DijkstraFileParser::parseStartEndNodes(std::ifstream& ifs)
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

	oss << "Missing arguments about start and end nodes line";
	throw std::invalid_argument(oss.str());
      }
    else if (tokenVect.size() > 2)
      {
	std::ostringstream oss;

	oss << "Too much arguments about start and end nodes line";
	throw std::invalid_argument(oss.str());
      }
    _desc.startNodeName = tokenVect.at(0);
    _desc.endNodeName = tokenVect.at(1);
    if (_desc.startNodeName == _desc.endNodeName)
      {
	std::ostringstream oss;

	oss << "Start and end nodes must be different";
	throw std::invalid_argument(oss.str());
      }
  }

  void DijkstraFileParser::parseArcNumber(std::ifstream& ifs)
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

	oss << "Missing argument about arc number line";
	throw std::invalid_argument(oss.str());
      }
    else if (tokenVect.size() > 1)
      {
	std::ostringstream oss;

	oss << "Too much arguments about arc number line";
	throw std::invalid_argument(oss.str());
      }
    try
      {
	const std::string& arcNbStr = tokenVect.at(0);

	_desc.arcNb = boost::lexical_cast<ArcNumber_t>(arcNbStr);
	if (arcNbStr.at(0) == '-')
	  {
	    std::ostringstream oss;

	    oss << "Arc number must be a positive integer";
	    throw std::invalid_argument(oss.str());
	  }
      }
    catch (const boost::bad_lexical_cast&)
      {
	std::ostringstream oss;

	oss << "Bad type. Arc number must be a integer";
	throw std::invalid_argument(oss.str());
      }
    if (_desc.arcNb == 0)
      {
	std::ostringstream oss;

	oss << "Arc number cannot be equal to 0";
	throw std::invalid_argument(oss.str());
      }
  }

  void DijkstraFileParser::parseNodes(std::ifstream& ifs)
  {
    std::string line;
    std::uint32_t arcCount = 0;

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
		<< "node, neighbor node and distance line";
	    throw std::invalid_argument(oss.str());
	  }
	else if (tokenVect.size() > 3)
	  {
	    std::ostringstream oss;

	    oss << "Too much arguments about "
		<< "node, neighbor node and distance line";
	    throw std::invalid_argument(oss.str());
	  }
	if (tokenVect.at(0) == tokenVect.at(1))
	  {
	    std::ostringstream oss;
	    
	    oss << "Node and neighbor node must have different name";
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
	    if (!_desc
		.nodes
		.try_emplace(tokenVect.at(0), NeighboringNodes_t { })
		.first->second.try_emplace(tokenVect.at(1), distance)
		.second)
	      {
		std::ostringstream oss;

		oss << "Duplicate arc from "
		    << tokenVect.at(0)
		    << " to "
		    << tokenVect.at(1);
		throw std::invalid_argument(oss.str());
	      }

	    auto it = _desc.nodes.find(tokenVect.at(1));
	    
	    if (it != _desc.nodes.cend())
	      {
		auto it2 = it->second.find(tokenVect.at(0));
		
		if (it2 != it->second.cend())
		  {
		    if (it2->second != distance)
		      {
			std::ostringstream oss;
			
			oss << "Duplicate arc from "
			    << tokenVect.at(0)
			    << " to "
			    << tokenVect.at(1)
			    << " with different distance";
			throw std::invalid_argument(oss.str());
		      }
		  }
		else
		  ++arcCount;
	      }
	    else
	      ++arcCount;
	  }
	catch (const boost::bad_lexical_cast&)
	  {
	    std::ostringstream oss;

	    oss << "Bad type. Distance must be a integer";
	    throw std::invalid_argument(oss.str());
	  }
      }
    if (arcCount < _desc.arcNb)
      {
	std::ostringstream oss;

	oss << "Missing arcs. Number of arcs must be equal to "
	    << _desc.arcNb;
	throw std::invalid_argument(oss.str());
      }
    else if (arcCount > _desc.arcNb)
      {
	std::ostringstream oss;

	oss << "Too much arcs. Number of arcs must be equal to "
	    << _desc.arcNb;
	throw std::invalid_argument(oss.str());
      }
    for (const auto& pair : _desc.nodes)
      {
	for (const auto& pair2 : pair.second)
	  {
	    auto it = _desc.nodes.find(pair2.first);

	    if (it == _desc.nodes.cend())
	      {
		std::ostringstream oss;

		oss << "Missing info arc with node " << pair2.first;
		throw std::invalid_argument(oss.str());
	      }

	    auto it2 = it->second.find(pair.first);

	    if (it2 == it->second.cend())
	      {
		std::ostringstream oss;

		oss << "Missing info arc with node "
		    << pair2.first
		    << " and neighbor node "
		    << pair.first;
		throw std::invalid_argument(oss.str());
	      }
	  }
      }
  }
}
