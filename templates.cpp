#include <tr1/unordered_map>

#include "objects.hpp"

using namespace std;
using namespace std::tr1;

template class unordered_map<string, GitObject *>;
template class unordered_map<off_t, GitObject *>;
template class unordered_multimap<GitObject *, GitObject *>;
template class unordered_map<GitObject *, char>;
template class unordered_multimap<string, string>;
