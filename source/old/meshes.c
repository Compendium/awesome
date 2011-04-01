#include <stdlib.h>
#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include "meshes.h"
#include "vec-util.h"

void init_mesh(
    struct flag_mesh *out_mesh,
    struct flag_vertex const *vertex_data, GLsizei vertex_count,
    GLushort const *element_data, GLsizei element_count,
    GLenum hint
) {
    glGenBuffers(1, &out_mesh->vertex_buffer);
    glGenBuffers(1, &out_mesh->element_buffer);
    out_mesh->element_count = element_count;

    glBindBuffer(GL_ARRAY_BUFFER, out_mesh->vertex_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertex_count * sizeof(struct flag_vertex),
        vertex_data,
        hint
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out_mesh->element_buffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        element_count * sizeof(GLushort),
        element_data,
        GL_STATIC_DRAW
    );
}

#define FLAGPOLE_TRUCK_TOP            0.5f
#define FLAGPOLE_TRUCK_CROWN          0.41f
#define FLAGPOLE_TRUCK_BOTTOM         0.38f
#define FLAGPOLE_SHAFT_TOP            0.3775f
#define FLAGPOLE_SHAFT_BOTTOM        -1.0f
#define FLAGPOLE_TRUCK_TOP_RADIUS     0.005f
#define FLAGPOLE_TRUCK_CROWN_RADIUS   0.020f
#define FLAGPOLE_TRUCK_BOTTOM_RADIUS  0.015f
#define FLAGPOLE_SHAFT_RADIUS         0.010f
#define FLAGPOLE_SHININESS            4.0f

void init_background_mesh(struct flag_mesh *out_mesh)
{
    static const GLsizei FLAGPOLE_RES = 16, FLAGPOLE_SLICE = 6;
    GLfloat FLAGPOLE_AXIS_XZ[2] = { -FLAGPOLE_SHAFT_RADIUS, 0.0f };
    static const GLubyte FLAGPOLE_SPECULAR[4] = { 255, 255, 192, 0 };

    GLfloat
        GROUND_LO[3] = { -0.875f, FLAGPOLE_SHAFT_BOTTOM, -2.45f },
        GROUND_HI[3] = {  1.875f, FLAGPOLE_SHAFT_BOTTOM,  0.20f },
        WALL_LO[3] = { GROUND_LO[0], FLAGPOLE_SHAFT_BOTTOM, GROUND_HI[2] },
        WALL_HI[3] = { GROUND_HI[0], FLAGPOLE_SHAFT_BOTTOM + 3.0f, GROUND_HI[2] };

    static GLfloat
        TEX_FLAGPOLE_LO[2] = { 0.0f,    0.0f },
        TEX_FLAGPOLE_HI[2] = { 0.03125f,  1.0f },
        TEX_GROUND_LO[2]   = { 0.03125f,  0.0078125f },
        TEX_GROUND_HI[2]   = { 0.515625f, 0.9921875f },
        TEX_WALL_LO[2]     = { 0.515625f, 0.0078125f },
        TEX_WALL_HI[2]     = { 1.0f,      0.9921875f };

#define _FLAGPOLE_T(y) \
    (TEX_FLAGPOLE_LO[1] \
        + (TEX_FLAGPOLE_HI[1] - TEX_FLAGPOLE_LO[1]) \
        * ((y) - FLAGPOLE_TRUCK_TOP)/(FLAGPOLE_SHAFT_BOTTOM - FLAGPOLE_TRUCK_TOP) \
    )

    GLfloat
        theta_step = 2.0f * (GLfloat)M_PI / (GLfloat)FLAGPOLE_RES,
        s_step = (TEX_FLAGPOLE_HI[0] - TEX_FLAGPOLE_LO[0]) / (GLfloat)FLAGPOLE_RES,
        t_truck_top    = TEX_FLAGPOLE_LO[1],
        t_truck_crown  = _FLAGPOLE_T(FLAGPOLE_TRUCK_CROWN),
        t_truck_bottom = _FLAGPOLE_T(FLAGPOLE_TRUCK_BOTTOM),
        t_shaft_top    = _FLAGPOLE_T(FLAGPOLE_SHAFT_TOP),
        t_shaft_bottom = _FLAGPOLE_T(FLAGPOLE_SHAFT_BOTTOM);

#undef _FLAGPOLE_T

    GLsizei
        flagpole_vertex_count = 2 + FLAGPOLE_RES * FLAGPOLE_SLICE,
        wall_vertex_count = 4,
        ground_vertex_count = 4,
        vertex_count = flagpole_vertex_count
            + wall_vertex_count
            + ground_vertex_count;

    GLsizei vertex_i = 0, element_i, i;

    GLsizei
        flagpole_element_count = 3 * ((FLAGPOLE_SLICE - 1) * 2 * FLAGPOLE_RES),
        wall_element_count = 6,
        ground_element_count = 6,
        element_count = flagpole_element_count
            + wall_element_count
            + ground_element_count;

    struct flag_vertex *vertex_data
        = (struct flag_vertex*) malloc(vertex_count * sizeof(struct flag_vertex));

    GLushort *element_data
        = (GLushort*) malloc(element_count * sizeof(GLushort));

    vertex_data[0].position[0] = GROUND_LO[0];
    vertex_data[0].position[1] = GROUND_LO[1];
    vertex_data[0].position[2] = GROUND_LO[2];
    vertex_data[0].position[3] = 1.0f;
    vertex_data[0].normal[0]   = 0.0f;
    vertex_data[0].normal[1]   = 1.0f;
    vertex_data[0].normal[2]   = 0.0f;
    vertex_data[0].normal[3]   = 0.0f;
    vertex_data[0].texcoord[0] = TEX_GROUND_LO[0];
    vertex_data[0].texcoord[1] = TEX_GROUND_LO[1];
    vertex_data[0].shininess   = 0.0f;
    vertex_data[0].specular[0] = 0;
    vertex_data[0].specular[1] = 0;
    vertex_data[0].specular[2] = 0;
    vertex_data[0].specular[3] = 0;

    vertex_data[1].position[0] = GROUND_HI[0];
    vertex_data[1].position[1] = GROUND_LO[1];
    vertex_data[1].position[2] = GROUND_LO[2];
    vertex_data[1].position[3] = 1.0f;
    vertex_data[1].normal[0]   = 0.0f;
    vertex_data[1].normal[1]   = 1.0f;
    vertex_data[1].normal[2]   = 0.0f;
    vertex_data[1].normal[3]   = 0.0f;
    vertex_data[1].texcoord[0] = TEX_GROUND_HI[0];
    vertex_data[1].texcoord[1] = TEX_GROUND_LO[1];
    vertex_data[1].shininess   = 0.0f;
    vertex_data[1].specular[0] = 0;
    vertex_data[1].specular[1] = 0;
    vertex_data[1].specular[2] = 0;
    vertex_data[1].specular[3] = 0;

    vertex_data[2].position[0] = GROUND_HI[0];
    vertex_data[2].position[1] = GROUND_LO[1];
    vertex_data[2].position[2] = GROUND_HI[2];
    vertex_data[2].position[3] = 1.0f;
    vertex_data[2].normal[0]   = 0.0f;
    vertex_data[2].normal[1]   = 1.0f;
    vertex_data[2].normal[2]   = 0.0f;
    vertex_data[2].normal[3]   = 0.0f;
    vertex_data[2].texcoord[0] = TEX_GROUND_HI[0];
    vertex_data[2].texcoord[1] = TEX_GROUND_HI[1];
    vertex_data[2].shininess   = 0.0f;
    vertex_data[2].specular[0] = 0;
    vertex_data[2].specular[1] = 0;
    vertex_data[2].specular[2] = 0;
    vertex_data[2].specular[3] = 0;

    vertex_data[3].position[0] = GROUND_LO[0];
    vertex_data[3].position[1] = GROUND_LO[1];
    vertex_data[3].position[2] = GROUND_HI[2];
    vertex_data[3].position[3] = 1.0f;
    vertex_data[3].normal[0]   = 0.0f;
    vertex_data[3].normal[1]   = 1.0f;
    vertex_data[3].normal[2]   = 0.0f;
    vertex_data[3].normal[3]   = 0.0f;
    vertex_data[3].texcoord[0] = TEX_GROUND_LO[0];
    vertex_data[3].texcoord[1] = TEX_GROUND_HI[1];
    vertex_data[3].shininess   = 0.0f;
    vertex_data[3].specular[0] = 0;
    vertex_data[3].specular[1] = 0;
    vertex_data[3].specular[2] = 0;
    vertex_data[3].specular[3] = 0;

    vertex_data[4].position[0] = WALL_LO[0];
    vertex_data[4].position[1] = WALL_LO[1];
    vertex_data[4].position[2] = WALL_LO[2];
    vertex_data[4].position[3] = 1.0f;
    vertex_data[4].normal[0]   = 0.0f;
    vertex_data[4].normal[1]   = 0.0f;
    vertex_data[4].normal[2]   = -1.0f;
    vertex_data[4].normal[3]   = 0.0f;
    vertex_data[4].texcoord[0] = TEX_WALL_LO[0];
    vertex_data[4].texcoord[1] = TEX_WALL_LO[1];
    vertex_data[4].shininess   = 0.0f;
    vertex_data[4].specular[0] = 0;
    vertex_data[4].specular[1] = 0;
    vertex_data[4].specular[2] = 0;
    vertex_data[4].specular[3] = 0;

    vertex_data[5].position[0] = WALL_HI[0];
    vertex_data[5].position[1] = WALL_LO[1];
    vertex_data[5].position[2] = WALL_LO[2];
    vertex_data[5].position[3] = 1.0f;
    vertex_data[5].normal[0]   = 0.0f;
    vertex_data[5].normal[1]   = 0.0f;
    vertex_data[5].normal[2]   = -1.0f;
    vertex_data[5].normal[3]   = 0.0f;
    vertex_data[5].texcoord[0] = TEX_WALL_HI[0];
    vertex_data[5].texcoord[1] = TEX_WALL_LO[1];
    vertex_data[5].shininess   = 0.0f;
    vertex_data[5].specular[0] = 0;
    vertex_data[5].specular[1] = 0;
    vertex_data[5].specular[2] = 0;
    vertex_data[5].specular[3] = 0;

    vertex_data[6].position[0] = WALL_HI[0];
    vertex_data[6].position[1] = WALL_HI[1];
    vertex_data[6].position[2] = WALL_LO[2];
    vertex_data[6].position[3] = 1.0f;
    vertex_data[6].normal[0]   = 0.0f;
    vertex_data[6].normal[1]   = 0.0f;
    vertex_data[6].normal[2]   = -1.0f;
    vertex_data[6].normal[3]   = 0.0f;
    vertex_data[6].texcoord[0] = TEX_WALL_HI[0];
    vertex_data[6].texcoord[1] = TEX_WALL_HI[1];
    vertex_data[6].shininess   = 0.0f;
    vertex_data[6].specular[0] = 0;
    vertex_data[6].specular[1] = 0;
    vertex_data[6].specular[2] = 0;
    vertex_data[6].specular[3] = 0;

    vertex_data[7].position[0] = WALL_LO[0];
    vertex_data[7].position[1] = WALL_HI[1];
    vertex_data[7].position[2] = WALL_LO[2];
    vertex_data[7].position[3] = 1.0f;
    vertex_data[7].normal[0]   = 0.0f;
    vertex_data[7].normal[1]   = 0.0f;
    vertex_data[7].normal[2]   = -1.0f;
    vertex_data[7].normal[3]   = 0.0f;
    vertex_data[7].texcoord[0] = TEX_WALL_LO[0];
    vertex_data[7].texcoord[1] = TEX_WALL_HI[1];
    vertex_data[7].shininess   = 0.0f;
    vertex_data[7].specular[0] = 0;
    vertex_data[7].specular[1] = 0;
    vertex_data[7].specular[2] = 0;
    vertex_data[7].specular[3] = 0;

    vertex_data[8].position[0] = FLAGPOLE_AXIS_XZ[0];
    vertex_data[8].position[1] = FLAGPOLE_TRUCK_TOP;
    vertex_data[8].position[2] = FLAGPOLE_AXIS_XZ[1];
    vertex_data[8].position[3] = 1.0f;
    vertex_data[8].normal[0]   = 0.0f;
    vertex_data[8].normal[1]   = 1.0f;
    vertex_data[8].normal[2]   = 0.0f;
    vertex_data[8].normal[3]   = 0.0f;
    vertex_data[8].texcoord[0] = TEX_FLAGPOLE_LO[0];
    vertex_data[8].texcoord[1] = t_truck_top;
    vertex_data[8].shininess   = FLAGPOLE_SHININESS;
    vertex_data[8].specular[0] = 0;
    vertex_data[8].specular[1] = 0;
    vertex_data[8].specular[2] = 0;
    vertex_data[8].specular[3] = 0;

    element_i = 0;

    element_data[element_i++] = 0;
    element_data[element_i++] = 1;
    element_data[element_i++] = 2;

    element_data[element_i++] = 0;
    element_data[element_i++] = 2;
    element_data[element_i++] = 3;

    element_data[element_i++] = 4;
    element_data[element_i++] = 5;
    element_data[element_i++] = 6;

    element_data[element_i++] = 4;
    element_data[element_i++] = 6;
    element_data[element_i++] = 7;

    init_mesh(
        out_mesh,
        vertex_data, vertex_count,
        element_data, element_count,
        GL_STATIC_DRAW
    );

    free(element_data);
    free(vertex_data);
}
