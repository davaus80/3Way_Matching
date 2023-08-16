#ifndef PATIENT_KDTREE_H_
#define PATIENT_KDTREE_H_

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include "Patient.h"
#include "Patient_KDTree.h"

// This code is based on the code from Rosetta Code at https://rosettacode.org/wiki/K-d_tree#C++

/**
 * Class for representing a point.
 */
point::point(float p_score1, float p_score2, Patient *the_patient) : patient_{the_patient}
{
    coords_[0] = p_score1;
    coords_[1] = p_score2;
}
/**
 * Returns the coordinate in the given dimension.
 *
 * @param index dimension index (zero based)
 * @return coordinate in the given dimension
 */
float point::get(size_t index) const
{
    return coords_[index];
}
/**
 * Returns the distance squared from this point to another
 * point.
 *
 * @param pt another point
 * @return distance squared from this point to the other point
 */
double point::distance(const point &pt) const
{
    double dist = 0;
    for (size_t i = 0; i < 2; ++i)
    {
        double d = get(i) - pt.get(i);
        dist += d * d;
    }
    return dist;
}

Patient *point::get_patient() const { return patient_; }

std::ostream& operator<<(std::ostream &out, const point &pt)
{
    out << '(';
    for (size_t i = 0; i < 2; ++i)
    {
        if (i > 0)
            out << ", ";
        out << pt.get(i);
    }
    out << ')';
    return out;
}

kdtree::node *kdtree::make_tree(size_t begin, size_t end, size_t index)
{
    if (end <= begin)
        return nullptr;
    size_t n = begin + (end - begin) / 2;
    auto i = nodes_.begin();
    std::nth_element(i + begin, i + n, i + end, node_cmp(index));
    index = (index + 1) % 2;
    nodes_[n].left_ = make_tree(begin, n, index);
    nodes_[n].right_ = make_tree(n + 1, end, index);
    return &nodes_[n];
}

// TODO: CHANGE DISTANCE FUNCTION
void kdtree::nearest(node *root, const point &point, size_t index)
{
    if (root == nullptr)
        return;
    ++visited_;
    double d = root->distance(point);
    if (best_ == nullptr || d < best_dist_)
    {
        best_dist_ = d;
        best_ = root;
    }
    if (best_dist_ == 0)
        return;
    double dx = root->get(index) - point.get(index);
    index = (index + 1) % 2;
    nearest(dx > 0 ? root->left_ : root->right_, point, index);
    if (dx * dx >= best_dist_)
        return;
    nearest(dx > 0 ? root->right_ : root->left_, point, index);
}

/**
 * Constructor taking a pair of iterators. Adds each
 * point in the range [begin, end) to the tree.
 *
 * @param begin start of range
 * @param end end of range
 */
template <typename iterator>
kdtree::kdtree(iterator begin, iterator end) : nodes_(begin, end)
{
    root_ = make_tree(0, nodes_.size(), 0);
}

/**
 * Constructor taking a function object that generates
 * points. The function object will be called n times
 * to populate the tree.
 *
 * @param f function that returns a point
 * @param n number of points to add
 */
template <typename func>
kdtree::kdtree(func &&f, size_t n)
{
    nodes_.reserve(n);
    for (size_t i = 0; i < n; ++i)
        nodes_.push_back(f());
    root_ = make_tree(0, nodes_.size(), 0);
}

/**
 * Constructor taking a vector of points to
 * populate the tree.
 *
 * @param pts vector of points
 */
kdtree::kdtree(std::set<point> *pts)
{
    size_t n = pts->size();
    nodes_.reserve(n);
    for (auto f : *pts)
        nodes_.push_back(f);
    root_ = make_tree(0, nodes_.size(), 0);
}

/**
 * Returns true if the tree is empty, false otherwise.
 */
bool kdtree::empty() const { return nodes_.empty(); }

/**
 * Returns the number of nodes visited by the last call
 * to nearest().
 */
size_t kdtree::visited() const { return visited_; }

/**
 * Returns the distance between the input point and return value
 * from the last call to nearest().
 */
double kdtree::distance() const { return std::sqrt(best_dist_); }

/**
 * Finds the nearest point in the tree to the given point.
 * It is not valid to call this function if the tree is empty.
 *
 * @param pt a point
 * @return the nearest point in the tree to the given point
 */
const point &kdtree::nearest(const point &pt)
{
    if (root_ == nullptr)
        throw std::logic_error("tree is empty");
    best_ = nullptr;
    visited_ = 0;
    best_dist_ = 0;
    nearest(root_, pt, 0);
    return best_->point_;
}

void test_wikipedia()
{

    Patient *p1 = new Patient(1, 1, 2, 3);
    Patient *p2 = new Patient(2, 1, 5, 4);
    Patient *p3 = new Patient(3, 2, 9, 6);
    Patient *p4 = new Patient(4, 2, 4, 7);
    Patient *p5 = new Patient(5, 3, 8, 1);
    Patient *p6 = new Patient(6, 3, 7, 2);

    point points[] = {point(p1->get_p_score1(), p1->get_p_score2(), p1),
                      {p2->get_p_score1(), p2->get_p_score2(), p2},
                      {p3->get_p_score1(), p3->get_p_score2(), p3},
                      {p4->get_p_score1(), p4->get_p_score2(), p4},
                      {p5->get_p_score1(), p5->get_p_score2(), p5},
                      {p6->get_p_score1(), p6->get_p_score2(), p6}};

    kdtree tree(std::begin(points), std::end(points));
    Patient *p7 = new Patient(7, 3, 9, 2);
    point n = tree.nearest({p7->get_p_score1(), p7->get_p_score2(), p7});

    std::cout << "Wikipedia example data:\n";
    std::cout << "nearest point: " << n << '\n';
    std::cout << "Point ID: " << n.get_patient()->get_id() << '\n';
    std::cout << "distance: " << tree.distance() << '\n';
    std::cout << "nodes visited: " << tree.visited() << '\n';
}

/*

struct random_point_generator {
    random_point_generator(double min, double max)
        : engine_(std::random_device()()), distribution_(min, max) {}

    point operator()() {
        double x = distribution_(engine_);
        double y = distribution_(engine_);
        Patient *p = new Patient(counter, 1, x, y);
        counter++;

        return point({x, y, p});
    }

    int counter = 0;
    std::mt19937 engine_;
    std::uniform_real_distribution<double> distribution_;
};



void test_random(size_t count) {
    random_point_generator rpg(0, 1);
    kdtree tree(rpg, count);
    point pt(rpg());
    point n = tree.nearest(pt);

    std::cout << "Random data (" << count << " points):\n";
    std::cout << "point: " << pt << '\n';
    std::cout << "nearest point: " << n << '\n';
    std::cout << "distance: " << tree.distance() << '\n';
    std::cout << "nodes visited: " << tree.visited() << '\n';
}

*/

/*
int main() {
    try {
        test_wikipedia();
        std::cout << '\n';
        //test_random(1000);
        //std::cout << '\n';
        //test_random(1000000);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
//*/

#endif