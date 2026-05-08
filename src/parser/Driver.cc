#include "Driver.hh"
#include "parser.hh"

Driver::Driver() : trace_parsing(false), trace_scanning(false)
{

}

int Driver::parse(const std::string &f)
{
  file = f;
  location.initialize(&file);
  scan_begin();
  yy::parser parse(*this);
  parse.set_debug_level(trace_parsing);
  int res = parse();
  scan_end();
  return res;
}

void Driver::error(const yy::location &l, const std::string &m)
{
  std::string err = "ERROR -> " + std::to_string(l.begin.line) +
                    ":" + std::to_string(l.begin.column) + ": " + m;
  errors.push_back(err);
  std::cerr << err << std::endl;
}