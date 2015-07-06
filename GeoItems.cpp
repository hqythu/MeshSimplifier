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
    cv::Matx44f Q = vertexes[0]->Q + vertexes[1]->Q;
    Q(3, 0) = 0; Q(3, 1) = 0; Q(3, 2) = 0; Q(3, 3) = 1;
    if (cv::determinant(Q) == 0) {
        v = (vertexes[0]->position + vertexes[1]->position) / 2;
    }
    else {
        cv::Vec4f res = Q.inv() * cv::Vec4f(0, 0, 0, 1);
        v = Vec3f(res[0], res[1], res[2]);
    }
}


void Edge::recalculate_deltav()
{
    cv::Matx44f Q = vertexes[0]->Q + vertexes[1]->Q;
    cv::Vec4f x(v.x, v.y, v.z, 1);
    delta_v = (x.t() * Q * x)[0];
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
