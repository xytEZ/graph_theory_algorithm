#include <boost/program_options.hpp>
#include <stdexcept>
#include <iostream>

#include "GraphAlgo.hh"
#include "ProgramOptionsHandler.hh"

namespace po = boost::program_options;

namespace line_arguments
{
  ProgramOptionsHandler::ProgramOptionsHandler(int ac, char **av) :
    _ac(ac),
    _av(av),
    _hasHelp(false),
    _algo(graph::GraphAlgo::NULL_VALUE),
    _fileName()
  { }

  void ProgramOptionsHandler::parseArgs()
  {
    po::options_description desc("Allowed options");

    desc.add_options()
      ("help,h",
       "this program apply a graph theory algorithm with map file")
      ("algorithm,a",
       po::value<std::string>(),
       "graph algorithm to apply. Available with \"dijkstra\" arg")
      ("file,f",
       po::value<std::string>(),
       "file containing the graph description");

    po::variables_map vm;

    po::store(po::parse_command_line(_ac, _av, desc), vm);
    po::notify(vm);
    if (vm.count("help"))
      {
	std::cout << desc << std::endl;	
	std::cout << "Examples of usage:" << std::endl;
	std::cout << _av[0]
		  << " -a dijkstra"
		  << " -f map.txt"
		  << std::endl;
	std::cout << _av[0]
		  << " --algorithm dijkstra"
		  << " --file map.txt"
		  << std::endl;
	_hasHelp = true;
	return;
      }
    if (vm.count("algorithm"))
      {
	std::string algoName = vm["algorithm"].as<std::string>();
	
	if (algoName == "dijkstra")
	  _algo = graph::GraphAlgo::DIJKSTRA;
	else
	  throw std::invalid_argument("Unknown algorithm");
      }
    else
      throw std::invalid_argument("algorithm was not set");
    if (vm.count("file"))
      _fileName = vm["file"].as<std::string>();
    else
      throw std::invalid_argument("file was not set");
  }

  bool ProgramOptionsHandler::hasHelp() const noexcept
  {
    return _hasHelp;
  }
  
  graph::GraphAlgo ProgramOptionsHandler::getAlgo() const noexcept
  {
    return _algo;
  }

  const std::string& ProgramOptionsHandler::getFileName() const noexcept
  {
    return _fileName;
  }
}
