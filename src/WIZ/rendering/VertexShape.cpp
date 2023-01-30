//
// Created by william on 29/01/23.
//

#include "WIZ/rendering/VertexShape.h"

wiz::VertexShape::VertexShape(float vertices[], unsigned int indices[], unsigned verticesSize, unsigned indicesSize) {
    this->vertices = vertices;
    this->indices = indices;

    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
}
