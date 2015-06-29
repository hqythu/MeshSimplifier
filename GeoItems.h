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

typedef std::set<Vertex>::iterator vertex_iter;
typedef std::set<Edge>::iterator edge_iter;
typedef std::set<Facet>::iterator facet_iter;


struct Vertex
{
    int label;
    int rank;
    Vec3f position;
    std::vector<facet_iter> facets;
    std::vector<edge_iter> edges;
    Vertex(int label) : label(label) {}
    bool operator < (const Vertex& op) const
    {
        return label < op.label;
    }
    bool operator == (const Vertex& op) const
    {
        return label == op.label;
    }
};


struct Edge
{
    double delta_v;
    vertex_iter vetexes[2];
    bool operator < (const Edge& op) const
    {
        if (delta_v < op.delta_v) {
            return true;
        }
        else if (op.delta_v < delta_v) {
            return false;
        }
        else if (*vetexes[0] < *op.vetexes[0]) {
            return true;
        }
        else if (*op.vetexes[0] < *vetexes[0]) {
            return false;
        }
        else if (*vetexes[1] < *op.vetexes[1]) {
            return true;
        }
        else {
            return false;
        }
    }
    bool operator == (const Edge& op) const
    {
        if (!(*vetexes[0] == *op.vetexes[0])) {
            return false;
        }
        else if (!(*vetexes[1] == *op.vetexes[1])) {
            return false;
        }
        else {
            return true;
        }
    }
};


struct Facet
{
    vertex_iter vetexes[3];
    bool operator < (const Facet& op) const
    {
        if (*vetexes[0] < *op.vetexes[0]) {
            return true;
        }
        else if (*op.vetexes[0] < *vetexes[0]) {
            return false;
        }
        else if (*vetexes[1] < *op.vetexes[1]){
            return true;
        }
        else if (*op.vetexes[1] < *vetexes[1]) {
            return false;
        }
        else if (*vetexes[2] < *op.vetexes[2]) {
            return true;
        }
        else {
            return false;
        }
    }
};


#endif // GEOITEM_H
