#include "Simplifier.h"
#include "SimpleObject.h"

#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <iostream>

using std::string;
using std::vector;
using std::list;
using std::set;


Simplifier::Simplifier()
{
}


Simplifier::~Simplifier()
{
}


void Simplifier::load(string filename)
{
    SimpleOBJ::CSimpleObject object;
    object.LoadFromObj(filename.c_str());
    for (int i = 0; i < object.m_nVertices; i++) {
        Vertex v(i);
        v.position = object.m_pVertexList[i];
        vertexes.insert(v);
    }
    for (int i = 0; i < object.m_nTriangles; i++) {
        Facet f;
        vertex_iter v1 = vertexes.find(Vertex(object.m_pTriangleList[i][0]));
        vertex_iter v2 = vertexes.find(Vertex(object.m_pTriangleList[i][1]));
        vertex_iter v3 = vertexes.find(Vertex(object.m_pTriangleList[i][2]));
        f.vertexes[0] = v1;
        f.vertexes[1] = v2;
        f.vertexes[2] = v3;
        facet_iter fi = facets.insert(f).first;
        const_cast<Vertex&>(*v1).facets.push_back(fi);
        const_cast<Vertex&>(*v2).facets.push_back(fi);
        const_cast<Vertex&>(*v3).facets.push_back(fi);
    }
    for (auto& facet : facets) {
        const_cast<Facet&>(facet).recalculate_n();
        const_cast<Facet&>(facet).recalculate_Kp();
    }
    for (auto& vertex : vertexes) {
        const_cast<Vertex&>(vertex).recalculate_Q();
    }
    for (auto& facet : facets) {
        static int const ac[3] = { 0, 1, 2 };
        static int const bc[3] = { 1, 2, 0 };
        for (int i = 0; i < 3; i++) {
            vertex_iter v1 = facet.vertexes[ac[i]];
            vertex_iter v2 = facet.vertexes[bc[i]];
            if (*v2 < *v1) {
                vertex_iter tmp = v1;
                v1 = v2;
                v2 = tmp;
            }
            Edge e;
            e.vertexes[0] = v1;
            e.vertexes[1] = v2;
            e.recalculate_v();
            e.recalculate_deltav();
            edge_iter ei = edges.find(e);
            if (ei == edges.end()) {
                ei = edges.insert(e).first;
            }
            if (std::find(v1->edges.begin(), v1->edges.end(), ei) == v1->edges.end()) {
                const_cast<Vertex&>(*v1).edges.push_back(ei);
            }
            if (std::find(v2->edges.begin(), v2->edges.end(), ei) == v2->edges.end()) {
                const_cast<Vertex&>(*v2).edges.push_back(ei);
            }
        }
    }
}


void Simplifier::save(string filename)
{
    SimpleOBJ::CSimpleObject object;
    object.m_nVertices = vertexes.size();
    object.m_nTriangles = facets.size();
    object.m_pVertexList = new Vec3f[object.m_nVertices];
    object.m_pTriangleList = new SimpleOBJ::Array<int, 3>[object.m_nTriangles];

    int k = 0;
    for (auto& vertex : vertexes) {
        const_cast<Vertex&>(vertex).rank = k++;
    }

    auto v = vertexes.begin();
    for (int i = 0; i < object.m_nVertices; i++) {
        object.m_pVertexList[i] = v->position;
        ++v;
    }

    auto f = facets.begin();
    for (int i = 0; i < object.m_nTriangles; i++) {
        object.m_pTriangleList[i][0] = f->vertexes[0]->rank;
        object.m_pTriangleList[i][1] = f->vertexes[1]->rank;
        object.m_pTriangleList[i][2] = f->vertexes[2]->rank;
        ++f;
    }

    object.SaveToObj(filename.c_str());
}


void Simplifier::simplify(double ratio)
{
    int iter_n = static_cast<int>((1 - ratio) * vertexes.size());
    std::cout << iter_n << std::endl;
    for (int i = 0; i < iter_n; i++) {
        if (i % 1000 == 0) {
            std::cout << i << "/" << iter_n << std::endl;
        }
        edge_iter e = edges.begin();
        vertex_iter v1 = e->vertexes[0];
        vertex_iter v2 = e->vertexes[1];
        Vec3f new_pos = (v1->position + v2->position) / 2;
        const_cast<Vertex&>(*v1).position = new_pos;
        merge(v1, v2);
        update(v1);
    }
}


void Simplifier::merge(vertex_iter v1, vertex_iter v2)
{
    list<facet_iter> new_facets(v1->facets.begin(), v1->facets.end());
    new_facets.insert(new_facets.end(), v2->facets.begin(), v2->facets.end());

    const_cast<Vertex&>(*v1).facets.clear();

    auto i1 = new_facets.begin();
    auto i2 = new_facets.begin();
    while (i2 != new_facets.end()) {
        bool has = false;
        for (auto i3 = new_facets.begin(); i3 != i1; ++i3) {
            if (*i3 == *i2) {
                has = true;
                break;
            }
        }
        if (!has) {
            std::swap(*i2, *i1);
            ++i1;
        }
        ++i2;
    }
    new_facets.erase(i1, new_facets.end());

    for (auto& facet : new_facets) {
        for (int j = 0; j < 3; j++) {
            if (facet->vertexes[j] == v2) {
                Facet f = *facet;
                f.vertexes[j] = v1;
                auto res = facets.insert(f);
                auto it = res.first;
                if (res.second) {
                    it->reset(facet, it);
                    facets.erase(facet);
                    facet = it;
                }
            }
        }
        if (facet->is_not_facet()) {
            facet->clear(facet);
            facets.erase(facet);
            continue;
        }

        const_cast<Vertex&>(*v1).facets.push_back(facet);
    }

    list<edge_iter> new_edges(v1->edges.begin(), v1->edges.end());
    new_edges.insert(new_edges.end(), v2->edges.begin(), v2->edges.end());

    const_cast<Vertex&>(*v1).edges.clear();

    auto ii1 = new_edges.begin();
    auto ii2 = new_edges.begin();
    while (ii2 != new_edges.end()) {
        bool has = false;
        for (auto ii3 = new_edges.begin(); ii3 != ii1; ++ii3) {
            if (*ii3 == *ii2) {
                has = true;
                break;
            }
        }
        if (!has) {
            std::swap(*ii2, *ii1);
            ++ii1;
        }
        ++ii2;
    }
    new_edges.erase(ii1, new_edges.end());

    for (auto i = new_edges.begin(); i != new_edges.end();) {
        edge_iter& edge = *i;
        for (int j = 0; j < 2; j++) {
            if (edge->vertexes[j] == v2) {
                Edge e = *edge;
                e.vertexes[j] = v1;
                if (*e.vertexes[1] < *e.vertexes[0]) {
                    vertex_iter tmp = e.vertexes[0];
                    e.vertexes[0] = e.vertexes[1];
                    e.vertexes[1] = tmp;
                }
                auto res = edges.insert(e);
                auto it = res.first;
                if (res.second) {
                    it->reset(edge, it);
                    edges.erase(edge);
                    edge = it;
                }
            }
        }
        if (edge->is_not_edge()) {
            edge->clear(edge);
            edges.erase(edge);
            new_edges.erase(i++);
            continue;
        }
        bool has = false;
        for (auto j = new_edges.begin(); j != i; ++j) {
            if (*j == edge) {
                has = true;
                break;
            }
        }
        if (!has) {
            const_cast<Vertex&>(*v1).edges.push_back(edge);
        }
        ++i;
    }

    vertexes.erase(v2);
}


void Simplifier::update(vertex_iter v)
{
    vector<vertex_iter> vertex_to_update;
    vector<edge_iter> edge_to_update;
    for (auto& facet : v->facets) {
        const_cast<Facet&>(*facet).recalculate_n();
        const_cast<Facet&>(*facet).recalculate_Kp();
        for (int j = 0; j < 3; j++) {
            if (std::find(vertex_to_update.begin(), vertex_to_update.end(), facet->vertexes[j])
                == vertex_to_update.end()) {
                vertex_to_update.push_back(facet->vertexes[j]);
            }
        }
    }
    for (auto& vertex : vertex_to_update) {
        const_cast<Vertex&>(*vertex).recalculate_Q();
        for (auto& edge : vertex->edges) {
            if (std::find(edge_to_update.begin(), edge_to_update.end(), edge) == edge_to_update.end()) {
                edge_to_update.push_back(edge);
            }
        }
    }
    for (auto& edge : edge_to_update) {
        Edge e = *edge;
        e.recalculate_v();
        e.recalculate_deltav();
        auto res = edges.insert(e);
        auto it = res.first;
        if (res.second) {
            it->reset(edge, it);
            edges.erase(edge);
            edge = it;
        }
    }
}
