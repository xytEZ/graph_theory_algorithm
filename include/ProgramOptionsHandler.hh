#ifndef PROGRAM_OPTIONS_HANDLER_HH_
# define PROGRAM_OPTIONS_HANDLER_HH_

# include <string>
# include <iostream>

# include "GraphAlgo.hh"

namespace line_arguments
{
  class ProgramOptionsHandler
  {
    std::ostream& _os;
    int _ac;
    char **_av;
    bool _hasHelp;
    graph::GraphAlgo _algo;
    std::string _fileName;

  public :
    ProgramOptionsHandler() = delete;
    ProgramOptionsHandler(std::ostream&, int, char **);
    ProgramOptionsHandler(const ProgramOptionsHandler&) = delete;
    ProgramOptionsHandler(ProgramOptionsHandler&&) = delete;
    ProgramOptionsHandler& operator=(const ProgramOptionsHandler&) = delete;
    ProgramOptionsHandler& operator=(ProgramOptionsHandler&&) = delete;
    void parseArgs();
    bool hasHelp() const noexcept;
    graph::GraphAlgo getAlgo() const noexcept;
    const std::string& getFileName() const noexcept;
  };
}

#endif
