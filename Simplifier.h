#ifndef SIMPLIFIER_H
#define SIMPLIFIER_H

#include <set>
#include <vector>
#include <list>
#include <string>

#include "GeoItems.h"


class Simplifier
{
public:
    Simplifier();
    ~Simplifier();

    void load(std::string);
    void save(std::string);
    void simplify(double ratio);

private:
    void merge(vertex_iter v1, vertex_iter v2);
    void update();

    std::set<Vertex> vertexes;
    std::set<Edge> edges;
    std::set<Facet> facets;
};


#endif // SIMPLIFIER_H
