#include "strActions.hpp"
#include <cctype>

const char *dirko::find(const char *start, const char *end, char target)
{
  for (const char *ch = start; ch < end; ++ch) {
    if (*ch == target) {
      return ch;
    }
  }
  return end;
}
size_t dirko::doDifLat(const char *str)
{
  size_t count = 0;
  char seen[alphaSize] = {};
  for (const char *ch = str; *ch != '\0'; ++ch) {
    if (std::isalpha(*ch)) {
      if (dirko::find(seen, seen + count, std::tolower(*ch)) == seen + count) {
        seen[count++] = std::tolower(*ch);
      }
    }
  }
  return count;
}
char *dirko::doUppLow(const char *source, char *distention)
{
  for (size_t i = 0; source[i] != '\0'; ++i) {
    distention[i] = (std::isalpha(source[i])) ? std::tolower(source[i]) : source[i];
  }
  return distention;
}
