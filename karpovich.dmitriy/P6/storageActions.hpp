#ifndef STORAGE_ACTIONS_HPP
#define STORAGE_ACTIONS_HPP
#include "storageShape.hpp"

namespace karpovich
{
  void output(StorageShape& shapes, size_t size);
  void scalefrompt(StorageShape& shapes, size_t size, double k, point_t pt);
}
#endif
