#include "GeoItems.h"

#include <iostream>

void Vertex::recalculate_Q()
{
    Q = 0;
    for (auto& facet : facets) {
        Q += facet->Kp;
    }
}


void Edge::recalculate_v()
{
    v = (vertexes[0]->position + vertexes[1]->position) / 2;
}


void Edge::recalculate_deltav()
{
    cv::Matx44f Q = vertexes[0]->Q + vertexes[1]->Q;
    cv::Vec4f x(v.x, v.y, v.z, 1);
    delta_v = (x.t() * Q * x)[0];
    /*Vec3f diff = vertexes[0]->position - vertexes[1]->position;
    delta_v = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;*/
}


void Facet::recalculate_n()
{
    normal = Vec3f::multi_out(vertexes[1]->position - vertexes[0]->position,
        vertexes[2]->position - vertexes[1]->position);
    normal.to_unit();
    d = -normal.x * vertexes[0]->position.x - normal.y * vertexes[0]->position.y
        - normal.z * vertexes[0]->position.z;
}
void Facet::recalculate_Kp()
{
    cv::Vec4f x(normal.x, normal.y, normal.z, d);
    Kp = x * x.t();
}
