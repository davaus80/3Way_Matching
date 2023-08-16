#ifndef _PATIENT_KDTREE_H_
#define _PATIENT_KDTREE_H_

#include <algorithm>
#include <set>
#include <array>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include "Patient.h"

// This code is based on the code from Rosetta Code at https://rosettacode.org/wiki/K-d_tree#C++

/**
 * Class for representing a point. 
 */
class point {
public:
    point(float p_score1, float p_score2, Patient *the_patient);

    /**
     * Returns the coordinate in the given dimension.
     *
     * @param index dimension index (zero based)
     * @return coordinate in the given dimension
     */
    float get(size_t index) const;

    /**
     * Returns the distance squared from this point to another
     * point.
     *
     * @param pt another point
     * @return distance squared from this point to the other point
     */
    double distance(const point& pt) const;

    Patient *get_patient() const;

private:
    Patient *patient_;
    std::array<float, 2> coords_;
};

std::ostream& operator<<(std::ostream& out, const point& pt);

inline bool operator<(const point& lhs, const point& rhs) {
    return lhs.get_patient()->get_id() < rhs.get_patient()->get_id();
}

/**
 * C++ k-d tree implementation, based on the C version at rosettacode.org.
 */
class kdtree {
private:
    struct node {
        node(const point& pt) : point_(pt), left_(nullptr), right_(nullptr) {}
        float get(size_t index) const {
            return point_.get(index);
        }
        double distance(const point& pt) const {
            return point_.distance(pt);
        }
        point point_;
        node* left_;
        node* right_;
    };
    node* root_ = nullptr;
    node* best_ = nullptr;
    double best_dist_ = 0;
    size_t visited_ = 0;
    std::vector<node> nodes_;

    struct node_cmp {
        node_cmp(size_t index) : index_(index) {}
        bool operator()(const node& n1, const node& n2) const {
            return n1.point_.get(index_) < n2.point_.get(index_);
        }
        size_t index_;
    };

    node* make_tree(size_t begin, size_t end, size_t index);

    void nearest(node* root, const point& point, size_t index);
public:
    kdtree(const kdtree&) = delete;
    kdtree& operator=(const kdtree&) = delete;
    /**
     * Constructor taking a pair of iterators. Adds each
     * point in the range [begin, end) to the tree.
     *
     * @param begin start of range
     * @param end end of range
     */
    template<typename iterator>
    kdtree(iterator begin, iterator end);
    
    /**
     * Constructor taking a function object that generates
     * points. The function object will be called n times
     * to populate the tree.
     *
     * @param f function that returns a point
     * @param n number of points to add
     */
    template<typename func>
    kdtree(func&& f, size_t n);

    /**
     * Constructor taking a vector of points to
     * populate the tree.
     *
     * @param pts vector of points
     */
    kdtree(std::set<point> *pts);

    /**
     * Returns true if the tree is empty, false otherwise.
     */
    bool empty() const;

    /**
     * Returns the number of nodes visited by the last call
     * to nearest().
     */
    size_t visited() const;

    /**
     * Returns the distance between the input point and return value
     * from the last call to nearest().
     */
    double distance() const;

    /**
     * Finds the nearest point in the tree to the given point.
     * It is not valid to call this function if the tree is empty.
     *
     * @param pt a point
     * @return the nearest point in the tree to the given point
     */
    const point& nearest(const point& pt);
};

#endif