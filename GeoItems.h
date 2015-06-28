#ifndef GEOITEM_H
#define GEOITEM_H

#include <list>
#include <vector>
#include <set>

#include "Vec3f.h"

using SimpleOBJ::Vec3f;

struct Vertex;
struct Edge;
struct Facet;

typedef std::list<Vertex>::iterator vertex_iter;
typedef std::set<Edge>::iterator edge_iter;
typedef std::list<Facet>::iterator facet_iter;


struct Vertex
{
    Vec3f position;
    std::vector<facet_iter> facets;
};


struct Edge
{
    vertex_iter vetexes[2];
};


struct Facet
{
    vertex_iter vetexes[3];
};


#endif // GEOITEM_H
