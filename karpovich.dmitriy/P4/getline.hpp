#ifndef GET_LINE_HPP
#define GET_LINE_HPP
#include <iosfwd>
namespace karpovich
{
  char* getline(std::istream& in, size_t& s);
}
#endif
