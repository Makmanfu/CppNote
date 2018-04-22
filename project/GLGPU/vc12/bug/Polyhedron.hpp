
#pragma once

#include "Vector4.hpp"
#include "blockallocator.h"
#include <list>
#include <vector>
#include <memory>
using namespace std;
#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>
using namespace boost::gil;

#ifndef LIBREP_DECL
    #define LIBREP_DECL __declspec(dllexport)
#endif

struct P_vertex
{
    vec3 position_;
    vec2 texture_;
    vec3 direction_;

    unsigned char rep_;
    unsigned char ref_;
    short texture_id_;

    P_vertex(): rep_(0), ref_(0), texture_id_(0)
    {
    }

    P_vertex(vec3 const& position, vec2 const& texture, vec3 const& direction): position_(position)
        , texture_(texture)
        , direction_(direction)
        , rep_(0)
        , ref_(0)
        , texture_id_(0)
    {
    }

    inline P_vertex operator = (P_vertex const& rhs)
    {
        position_ = rhs.position_;
        texture_  = rhs.texture_;
        direction_ = rhs.direction_;
        rep_ = rhs.rep_;
        texture_id_ = rhs.texture_id_;
        return (*this);
    }
};

typedef std::list<P_vertex, block_allocator<P_vertex, 1024> > P_vertex_collection;
typedef P_vertex_collection::iterator P_vertex_ptr;
typedef std::vector<P_vertex_ptr> P_vertex_index_collection;

struct P_edge
{
    int i0_;
    int i1_;

    P_edge(): i0_(UINT_MAX), i1_(UINT_MAX)
    {
    };

    P_edge(const P_edge& rhs): i0_(rhs.i0_), i1_(rhs.i1_)
    {
    };

    P_edge operator = (const P_edge& rhs)
    {
        i0_ = rhs.i0_;
        i1_ = rhs.i1_;
        return (*this);
    };

    bool is_valid() const
    {
        return i0_ != i1_;
    };

    void remap_vertex(int from_vertex_id, int to_vertex_id)
    {
        if (i0_ == from_vertex_id)
            i0_ = to_vertex_id;
        else if (i1_ == from_vertex_id)
            i1_ = to_vertex_id;
    };
};

typedef std::list<P_edge, block_allocator<P_edge, 1024> > P_edge_collection;
typedef P_edge_collection::iterator P_edge_ptr;
typedef std::vector<P_edge_ptr> P_edge_index_collection;

struct P_triangle
{
    int i0_, i1_, i2_;
    int rep_;

    P_triangle(): rep_(0)
    {
    };

    P_triangle(int i0, int i1, int i2): i0_(i0), i1_(i1), i2_(i2), rep_(0)
    {
    };

    inline P_triangle operator = (P_triangle const& rhs)
    {
        i0_ = rhs.i0_;
        i1_ = rhs.i1_;
        i2_ = rhs.i2_;
        rep_ = rhs.rep_;
        return (*this);
    };

    void remap_vertex(int from_vertex_id, int to_vertex_id)
    {
        if (i0_ == from_vertex_id)
            i0_ = to_vertex_id;
        else if (i1_ == from_vertex_id)
            i1_ = to_vertex_id;
        else if (i2_ == from_vertex_id)
            i2_ = to_vertex_id;
    };

    bool is_valid() const
    {
        return i0_ != i1_ && i0_ != i2_ && i1_ != i2_;
    };
};

typedef std::list<P_triangle, block_allocator<P_triangle, 1024> > P_triangle_collection;
typedef P_triangle_collection::iterator P_triangle_ptr;
typedef std::vector<P_triangle_ptr> P_triangle_index_collection;

typedef std::vector<int> P_id_collection;
typedef std::vector<P_id_collection> P_v_f_linked_collection;
typedef std::vector<P_id_collection> P_v_e_linked_collection;
typedef std::vector<P_id_collection> P_f_e_linked_collection;

struct Polyhedron
{
    int texture_id;

    P_vertex_collection vertex_ds_;
    P_vertex_index_collection vertex_index_ds_;

    P_edge_collection edge_ds_;
    P_edge_index_collection edge_index_ds_;

    P_triangle_collection facet_ds_;
    P_triangle_index_collection facet_index_ds_;

    P_v_f_linked_collection v_f_linked_index_ds_;
    P_v_e_linked_collection v_e_linked_index_ds_;

    LIBREP_DECL Polyhedron();
    LIBREP_DECL ~Polyhedron();

    LIBREP_DECL P_vertex_collection& vertex_ds();
    LIBREP_DECL P_vertex_index_collection& vertex_index_ds();
    LIBREP_DECL P_triangle_collection& facet_ds();
    LIBREP_DECL P_triangle_index_collection& facet_index_ds();

    LIBREP_DECL void delegate();

    LIBREP_DECL void bbox(vec3& a, vec3& b);

    LIBREP_DECL int vertex_count() const;
    LIBREP_DECL P_vertex_ptr vertex(int vertex_id);
    LIBREP_DECL void vertex_linked_star(int vertex_id, P_id_collection& star);
    LIBREP_DECL P_id_collection* vertex_linked_facet(int vertex_id);
    LIBREP_DECL P_id_collection* vertex_linked_edge(int vertex_id);

    LIBREP_DECL int facet_count() const;
    LIBREP_DECL P_triangle_ptr facet(int facet_id);

    LIBREP_DECL int edge_count() const;
    LIBREP_DECL P_edge_ptr edge(int edge_id);
    LIBREP_DECL void edge_linked_facet(int i0, int i1, P_id_collection& linked);
};

typedef std::shared_ptr<Polyhedron> Polyhedron_ptr;
typedef std::vector<Polyhedron_ptr> Polyhedron_collection;

typedef rgb8_image_t Polyhedron_texture;
typedef std::shared_ptr<Polyhedron_texture> Polyhedron_texture_ptr;
typedef std::vector<Polyhedron_texture_ptr> Polyhedron_texture_collection;

struct Opposite
{
    inline int operator()(P_edge_ptr e, P_triangle_ptr f)
    {
        return operator()(*e, f);
    };
    inline int operator()(P_edge const& e, P_triangle_ptr f)
    {
        if (e.i0_ == f->i0_)
        {
            if (e.i1_ == f->i1_) return f->i2_;
            else return f->i1_;
        }
        else if (e.i0_ == f->i1_)
        {
            if (e.i1_ == f->i0_) return f->i2_;
            else  return f->i0_;
        }
        else
        {
            if (e.i1_ == f->i0_) return f->i1_;
            else return f->i0_;
        }
    };
};

struct clamp
{
    inline double operator()(double x, double minval, double maxval)
    {
        return min(max(x, minval), maxval);
    };
};

struct Polyhedron2off
{
    LIBREP_DECL inline void operator()(Polyhedron_ptr polyhedron, string const& filename);
    LIBREP_DECL inline void operator()(Polyhedron_collection const& collection, string const& filename);
};

struct PolyhedronSpliter
{
    const double tolerance_;
    LIBREP_DECL PolyhedronSpliter(double tolerance = 30.0f);
    LIBREP_DECL inline void operator()(Polyhedron_ptr polyhedron, Polyhedron_collection& exp);
protected:
    void next(Polyhedron_ptr polyhedron, Polyhedron_ptr s, vec3 const& n, int vertex_id, int triangle_id);
};

struct PolyhedronRemoveDeplicatedVertex
{
    LIBREP_DECL inline void operator()(Polyhedron_ptr polyhedron, double tolerance);
};

struct PolyhedronRemoveUnreferencedVertex
{
    LIBREP_DECL inline void operator()(Polyhedron_ptr polyhedron);
};

struct PolyhedronRemoveUnreferenceFacet
{
    LIBREP_DECL inline void operator()(Polyhedron_ptr polyhedron);
};

struct PolyhedronRemoveDeplicatedFacet
{
    LIBREP_DECL inline void operator()(Polyhedron_ptr polyhedron);
};

struct PolyhedronReorientation
{
    LIBREP_DECL inline void operator()(Polyhedron_ptr polyhedron);
};

struct PolyhedronVertexNormal
{
    LIBREP_DECL inline void operator()(Polyhedron_ptr polyhedron);
};



