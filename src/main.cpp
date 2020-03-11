#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>

#include "ProgramOptionsHandler.hh"
#include "GraphAlgo.hh"
#include "FactoryGraph.hh"
#include "AbstractGraph.hh"

namespace factoGrph = graph::factory;

int main(int ac, char **av)
{
  try
    {
      line_arguments::ProgramOptionsHandler poHandler(ac, av);

      poHandler.parseArgs();
      if (poHandler.hasHelp())
	return EXIT_SUCCESS;

      graph::GraphAlgo algo = poHandler.getAlgo();
      const std::string& fileName = poHandler.getFileName();
      std::unique_ptr<graph::AGraphFileParser> parser =
	factoGrph::GraphFileParser::getParser(algo, fileName);

      parser->parse();
      
      std::unique_ptr<graph::AGraphAlgorithm> algorithm =
	factoGrph::GraphAlgorithm::getAlgorithm(algo);

      parser->accept(*algorithm);
      algorithm->execute();
      
      std::unique_ptr<graph::AGraphResPrinter> printer =
	factoGrph::GraphResPrinter::getPrinter(algo);

      algorithm->accept(*printer);
      printer->printAlgoResult();
    }
  catch (const std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      return EXIT_FAILURE;
    }
  catch (...)
    {
      std::cerr << "Unknown exception caught" << std::endl;
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
