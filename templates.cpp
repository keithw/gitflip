#include <tr1/unordered_map>

#include "objects.hpp"

template class std::tr1::unordered_map<string, GitObject *>;
template class std::tr1::unordered_map<off_t, GitObject *>;
