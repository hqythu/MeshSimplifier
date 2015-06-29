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
    void recalculate_Q()
    {

    }
};


struct Edge
{
    double delta_v;
    vertex_iter vertexes[2];
    bool operator < (const Edge& op) const
    {
        if (delta_v < op.delta_v) {
            return true;
        }
        else if (op.delta_v < delta_v) {
            return false;
        }
        else if (*vertexes[0] < *op.vertexes[0]) {
            return true;
        }
        else if (*op.vertexes[0] < *vertexes[0]) {
            return false;
        }
        else if (*vertexes[1] < *op.vertexes[1]) {
            return true;
        }
        else {
            return false;
        }
    }
    bool operator == (const Edge& op) const
    {
        if (!(*vertexes[0] == *op.vertexes[0])) {
            return false;
        }
        else if (!(*vertexes[1] == *op.vertexes[1])) {
            return false;
        }
        else {
            return true;
        }
    }
    bool is_not_edge() const
    {
        return vertexes[0] == vertexes[1];
    }
    void recalculate_v()
    {
        Vec3f diff = vertexes[0]->position - vertexes[1]->position;
        delta_v = diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2];
    }
};


struct Facet
{
    vertex_iter vertexes[3];
    bool operator < (const Facet& op) const
    {
        if (*vertexes[0] < *op.vertexes[0]) {
            return true;
        }
        else if (*op.vertexes[0] < *vertexes[0]) {
            return false;
        }
        else if (*vertexes[1] < *op.vertexes[1]){
            return true;
        }
        else if (*op.vertexes[1] < *vertexes[1]) {
            return false;
        }
        else if (*vertexes[2] < *op.vertexes[2]) {
            return true;
        }
        else {
            return false;
        }
    }
    bool is_not_facet() const
    {
        return vertexes[0] == vertexes[1]
            || vertexes[0] == vertexes[2]
            || vertexes[1] == vertexes[2];
    }
    void recalculate_n()
    {

    }
    void recalculate_Kp()
    {

    }
};


#endif // GEOITEM_H
