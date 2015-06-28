#include "Simplifier.h"
#include "SimpleObject.h"

#include <vector>
#include <set>

using std::string;
using std::vector;
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
        f.vetexes[0] = v1;
        f.vetexes[1] = v2;
        f.vetexes[2] = v3;
        facet_iter fi = facets.insert(f).first;
        const_cast<Vertex&>(*v1).facets.push_back(fi);
        const_cast<Vertex&>(*v2).facets.push_back(fi);
        const_cast<Vertex&>(*v3).facets.push_back(fi);
    }
    for (auto& facet : facets) {
        static int const ac[3] = { 0, 1, 2 };
        static int const bc[3] = { 1, 2, 0 };
        for (int i = 0; i < 3; i++) {
            vertex_iter v1 = facet.vetexes[ac[i]];
            vertex_iter v2 = facet.vetexes[bc[i]];
            if (*v2 < *v1) {
                vertex_iter tmp = v1;
                v1 = v2;
                v2 = tmp;
            }
            Edge e;
            Vec3f diff = v1->position - v2->position;
            e.delta_v = diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2];
            e.vetexes[0] = v1;
            e.vetexes[1] = v2;
            if (edges.find(e) != edges.end()) {
                edges.insert(e);
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

    auto v = vertexes.begin();
    for (int i = 0; i < object.m_nVertices; i++) {
        object.m_pVertexList[i] = v->position;
        ++v;
    }

    auto f = facets.begin();
    for (int i = 0; i < object.m_nTriangles; i++) {
        object.m_pTriangleList[i][0] = f->vetexes[0]->label;
        object.m_pTriangleList[i][1] = f->vetexes[1]->label;
        object.m_pTriangleList[i][2] = f->vetexes[2]->label;
        ++f;
    }

    object.SaveToObj(filename.c_str());
}


void Simplifier::simplify()
{

}
