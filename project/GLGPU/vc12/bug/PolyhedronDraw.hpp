
#pragma once
#include "Vector4.hpp"
#include "glew.h"
#include "Polyhedron.h"

struct PolyhedronDraw
{
    inline void operator()(Polyhedron_collection const& collection)
    {
        if (collection.size() < 1) return;
        vec3 min_corner, max_corner;
        collection[0]->bbox(min_corner, max_corner);
        for (int i = 1; i < collection.size(); i++)
        {
            vec3 a, b;
            collection[i]->bbox(a, b);
            if (min_corner.x() > a.x()) min_corner.x() = a.x();
            if (min_corner.y() > a.y()) min_corner.y() = a.y();
            if (min_corner.z() > a.z()) min_corner.z() = a.z();
            if (max_corner.x() < b.x()) max_corner.x() = b.x();
            if (max_corner.y() > b.y()) max_corner.y() = b.y();
            if (max_corner.z() > b.z()) max_corner.z() = b.z();
        }
        vec3 center = (min_corner + max_corner) / 2.0;
        glPushMatrix();
        glLoadIdentity();
        //static float lightPosF[] = { 0.0,0.0,1.0,0.0 };
        static float lightPosF[] = { min_corner.x(), min_corner.y(), min_corner.z(), 0.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosF);
        //static float lightPosB[] = { 0.0,0.0,-1.0,0.0 };
        static float lightPosB[] = { max_corner.x(), max_corner.y(), max_corner.z(), 0.0 };
        glLightfv(GL_LIGHT1, GL_POSITION, lightPosB);
        glPopMatrix();
        glPushMatrix();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        static float baseLightAmbientColor[] = { 32.0 / 255, 32.0 / 255, 32.0 / 255, 1.0 };
        //static float baseLightDiffuseColor1[] = { 255.0 / 255,204.0 / 255,204.0 / 255,1.0 };
        static float baseLightDiffuseColor1[] = { 204.0 / 255, 204.0 / 255, 204.0 / 255, 1.0 };
        static float baseLightDiffuseColor2[] = { 204.0 / 255, 204.0 / 255, 255.0 / 255, 1.0 };
        static float baseLightSpecularColor[] = { 255.0 / 255, 255.0 / 255, 255.0 / 255, 1.0 };
        glLightfv(GL_LIGHT0, GL_AMBIENT, baseLightAmbientColor);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, baseLightDiffuseColor1);
        glLightfv(GL_LIGHT0, GL_SPECULAR, baseLightSpecularColor);
        glLightfv(GL_LIGHT1, GL_AMBIENT, baseLightAmbientColor);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, baseLightDiffuseColor2);
        glLightfv(GL_LIGHT1, GL_SPECULAR, baseLightSpecularColor);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(1);
        float amb[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
        float col[4] = { 0.5f, 0.5f, 0.8f, 1.0f };
        float spe[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glPolygonMode(GL_FRONT, GL_FILL);
        for (int k = 0; k < collection.size(); k++)
        {
            Polyhedron_ptr polyhedron = collection[k];
            for (int i = 0; i < polyhedron->facet_ds().size(); i++)
            {
                P_triangle_ptr tri = polyhedron->facet(i);
                P_vertex_ptr v0 = polyhedron->vertex(tri->i0_);
                P_vertex_ptr v1 = polyhedron->vertex(tri->i1_);
                P_vertex_ptr v2 = polyhedron->vertex(tri->i2_);
                vec3 direction = triangle_normal(v0->position_, v1->position_, v2->position_);
                glBegin(GL_TRIANGLES);
                glNormal3f(direction.x(), direction.y(), direction.z());
                glVertex3d(v0->position_.x(), v0->position_.y(), v0->position_.z());
                glVertex3d(v1->position_.x(), v1->position_.y(), v1->position_.z());
                glVertex3d(v2->position_.x(), v2->position_.y(), v2->position_.z());
                glEnd();
            }
        }
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1, -1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glColor3f(0.4, 0.4, 0.8);
        for (int k = 0; k < collection.size(); k++)
        {
            Polyhedron_ptr polyhedron = collection[k];
            for (int i = 0; i < polyhedron->facet_ds().size(); i++)
            {
                P_triangle_ptr tri = polyhedron->facet(i);
                glBegin(GL_TRIANGLES);
                P_vertex_ptr v0 = polyhedron->vertex(tri->i0_);
                P_vertex_ptr v1 = polyhedron->vertex(tri->i1_);
                P_vertex_ptr v2 = polyhedron->vertex(tri->i2_);
                glVertex3d(v0->position_.x(), v0->position_.y(), v0->position_.z());
                glVertex3d(v1->position_.x(), v1->position_.y(), v1->position_.z());
                glVertex3d(v2->position_.x(), v2->position_.y(), v2->position_.z());
                glEnd();
            }
        }
        glPopAttrib();
        glPopMatrix();
    }

    inline void operator()(Polyhedron_ptr polyhedron)
    {
        vec3 min_corner, max_corner;
        polyhedron->bbox(min_corner, max_corner);
        vec3 center = (min_corner + max_corner) / 2.0;
        glPushMatrix();
        glLoadIdentity();
        //static float lightPosF[] = { 0.0,0.0,1.0,0.0 };
        static float lightPosF[] = { min_corner.x(), min_corner.y(), min_corner.z(), 0.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosF);
        //static float lightPosB[] = { 0.0,0.0,-1.0,0.0 };
        static float lightPosB[] = { max_corner.x(), max_corner.y(), max_corner.z(), 0.0 };
        glLightfv(GL_LIGHT1, GL_POSITION, lightPosB);
        glPopMatrix();
        glPushMatrix();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        static float baseLightAmbientColor[] = { 32.0 / 255, 32.0 / 255, 32.0 / 255, 1.0 };
        //static float baseLightDiffuseColor1[] = { 255.0 / 255,204.0 / 255,204.0 / 255,1.0 };
        static float baseLightDiffuseColor1[] = { 204.0 / 255, 204.0 / 255, 204.0 / 255, 1.0 };
        static float baseLightDiffuseColor2[] = { 204.0 / 255, 204.0 / 255, 255.0 / 255, 1.0 };
        static float baseLightSpecularColor[] = { 255.0 / 255, 255.0 / 255, 255.0 / 255, 1.0 };
        glLightfv(GL_LIGHT0, GL_AMBIENT, baseLightAmbientColor);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, baseLightDiffuseColor1);
        glLightfv(GL_LIGHT0, GL_SPECULAR, baseLightSpecularColor);
        glLightfv(GL_LIGHT1, GL_AMBIENT, baseLightAmbientColor);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, baseLightDiffuseColor2);
        glLightfv(GL_LIGHT1, GL_SPECULAR, baseLightSpecularColor);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(1);
        float amb[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
        float col[4] = { 0.5f, 0.5f, 0.8f, 1.0f };
        float spe[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glPolygonMode(GL_FRONT, GL_FILL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0, 0, 0, 0.5);
        for (int i = 0; i < polyhedron->facet_ds().size(); i++)
        {
            P_triangle_ptr tri = polyhedron->facet(i);
            P_vertex_ptr v0 = polyhedron->vertex(tri->i0_);
            P_vertex_ptr v1 = polyhedron->vertex(tri->i1_);
            P_vertex_ptr v2 = polyhedron->vertex(tri->i2_);
            vec3 direction = triangle_normal(v0->position_, v1->position_, v2->position_);
            glBegin(GL_TRIANGLES);
            glNormal3f(direction.x(), direction.y(), direction.z());
            glVertex3d(v0->position_.x(), v0->position_.y(), v0->position_.z());
            glVertex3d(v1->position_.x(), v1->position_.y(), v1->position_.z());
            glVertex3d(v2->position_.x(), v2->position_.y(), v2->position_.z());
            glEnd();
        }
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1, -1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        // glColor3f(0.4, 0.4, 0.8);
        //for (int i = 0; i < polyhedron->facet_ds().size(); i++) {
        //    P_triangle_ptr tri = polyhedron->facet(i);
        //    glBegin(GL_TRIANGLES);
        //    P_vertex_ptr v0 = polyhedron->vertex(tri->i0_);
        //    P_vertex_ptr v1 = polyhedron->vertex(tri->i1_);
        //    P_vertex_ptr v2 = polyhedron->vertex(tri->i2_);
        //    glVertex3d(v0->position_.x(), v0->position_.y(), v0->position_.z());
        //    glVertex3d(v1->position_.x(), v1->position_.y(), v1->position_.z());
        //    glVertex3d(v2->position_.x(), v2->position_.y(), v2->position_.z());
        //    glEnd();
        //}
        glDisable(GL_BLEND);
        glPopAttrib();
        glPopMatrix();
    }

    inline void operator()(Polyhedron_collection const& collection, Polyhedron_texture_collection& texture_collection)
    {
        if (collection.size() < 1) return;
        if (collection.size() != texture_collection.size())
        {
            operator()(collection);
            return;
        }
        vec3 min_corner, max_corner;
        collection[0]->bbox(min_corner, max_corner);
        for (int i = 1; i < collection.size(); i++)
        {
            vec3 a, b;
            collection[i]->bbox(a, b);
            if (min_corner.x() > a.x()) min_corner.x() = a.x();
            if (min_corner.y() > a.y()) min_corner.y() = a.y();
            if (min_corner.z() > a.z()) min_corner.z() = a.z();
            if (max_corner.x() < b.x()) max_corner.x() = b.x();
            if (max_corner.y() > b.y()) max_corner.y() = b.y();
            if (max_corner.z() > b.z()) max_corner.z() = b.z();
        }
        vec3 center = (min_corner + max_corner) / 2.0;
        glPushMatrix();
        glLoadIdentity();
        //static float lightPosF[] = { 0.0,0.0,1.0,0.0 };
        static float lightPosF[] = { min_corner.x(), min_corner.y(), min_corner.z(), 0.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosF);
        //static float lightPosB[] = { 0.0,0.0,-1.0,0.0 };
        static float lightPosB[] = { max_corner.x(), max_corner.y(), max_corner.z(), 0.0 };
        glLightfv(GL_LIGHT1, GL_POSITION, lightPosB);
        glPopMatrix();
        glPushMatrix();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        static float baseLightAmbientColor[] = { 32.0 / 255, 32.0 / 255, 32.0 / 255, 1.0 };
        //static float baseLightDiffuseColor1[] = { 255.0 / 255,204.0 / 255,204.0 / 255,1.0 };
        static float baseLightDiffuseColor1[] = { 204.0 / 255, 204.0 / 255, 204.0 / 255, 1.0 };
        static float baseLightDiffuseColor2[] = { 204.0 / 255, 204.0 / 255, 255.0 / 255, 1.0 };
        static float baseLightSpecularColor[] = { 255.0 / 255, 255.0 / 255, 255.0 / 255, 1.0 };
        glLightfv(GL_LIGHT0, GL_AMBIENT, baseLightAmbientColor);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, baseLightDiffuseColor1);
        glLightfv(GL_LIGHT0, GL_SPECULAR, baseLightSpecularColor);
        glLightfv(GL_LIGHT1, GL_AMBIENT, baseLightAmbientColor);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, baseLightDiffuseColor2);
        glLightfv(GL_LIGHT1, GL_SPECULAR, baseLightSpecularColor);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(1);
        float amb[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
        float col[4] = { 0.5f, 0.5f, 0.8f, 1.0f };
        float spe[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spe);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (int k = 0; k < collection.size(); k++)
        {
            Polyhedron_ptr polyhedron = collection[k];
            Polyhedron_texture_ptr texture = texture_collection[k];
            if (texture != 0)
            {
                glEnable(GL_TEXTURE_2D);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->width(), texture->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, &view(*texture.get())[0][0]);
            }
            for (int i = 0; i < polyhedron->facet_ds().size(); i++)
            {
                P_triangle_ptr tri = polyhedron->facet(i);
                P_vertex_ptr v0 = polyhedron->vertex(tri->i0_);
                P_vertex_ptr v1 = polyhedron->vertex(tri->i1_);
                P_vertex_ptr v2 = polyhedron->vertex(tri->i2_);
                //error at v2
                //
                vec3 direction = triangle_normal(v0->position_, v1->position_, v2->position_);
                glBegin(GL_TRIANGLES);
                glNormal3f(direction.x(), direction.y(), direction.z());
                if (texture != 0)
                    glTexCoord2d(v0->texture_.x(), v0->texture_.y());
                glVertex3d(v0->position_.x(), v0->position_.y(), v0->position_.z());
                if (texture != 0)
                    glTexCoord2d(v1->texture_.x(), v1->texture_.y());
                glVertex3d(v1->position_.x(), v1->position_.y(), v1->position_.z());
                if (texture != 0)
                    glTexCoord2d(v2->texture_.x(), v2->texture_.y());
                glVertex3d(v2->position_.x(), v2->position_.y(), v2->position_.z());
                glEnd();
            }
            if (texture != 0)
                glDisable(GL_TEXTURE_2D);
        }
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_COLOR_MATERIAL);
        glPopAttrib();
        glPopMatrix();
    }
};

