#ifndef SIMPLIFIER_H
#define SIMPLIFIER_H

#include <set>
#include <vector>
#include <list>

#include "GeoItems.h"


class Simplifier
{
public:
    Simplifier();
    ~Simplifier();

private:
    std::list<Vertex> vertexes;
    std::set<Edge> edges;
    std::list<Facet> facets;
};


#endif // SIMPLIFIER_H
