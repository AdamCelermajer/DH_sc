// Array2d<T>: a grid that grows in any direction. Rows are held in a deque of
// deques: rows_[y - minY_][x - minX_]. New cells are value-initialised. The original
// instantiates this for rnd::Tile*; the template is declared generically here.
#pragma once

#include <cstddef>
#include <deque>

template <typename T>
class Array2d {
public:
    Array2d();  // 0x487dd8: starts empty (partial, see Clear)

    // 0x48b7fc: grows the grid to include (x, y) and returns that cell.
    T& operator()(int x, int y);

    // 0x487bb0: resets every cell to T() and keeps the grid's shape.
    // Partial: the original refills the deque map in place; the same result is
    // reached here by assigning each cell.
    void Clear();

    // 0x48b578: grows the grid so that (x, y) is inside it. Partial: the empty-grid
    // start point (origin at the first cell asked for) is inferred from the growth
    // branches.
    void EnsurePosition(int x, int y);

private:
    int minX_ = 0;               // +0x0, x of the first column
    int minY_ = 0;               // +0x4, y of the first row
    int width_ = 0;              // +0x8, number of columns
    std::deque<std::deque<T>> rows_;  // +0xc, one deque per row (height = rows_.size())
};

template <typename T>
Array2d<T>::Array2d() = default;

template <typename T>
T& Array2d<T>::operator()(int x, int y) {
    EnsurePosition(x, y);
    return rows_[static_cast<std::size_t>(y - minY_)][static_cast<std::size_t>(x - minX_)];
}

template <typename T>
void Array2d<T>::Clear() {
    for (auto& row : rows_) {
        for (auto& cell : row) {
            cell = T();
        }
    }
}

template <typename T>
void Array2d<T>::EnsurePosition(int x, int y) {
    if (rows_.empty()) {
        minX_ = x;
        minY_ = y;
        width_ = 1;
        rows_.emplace_back(1, T());
        return;
    }
    while (x < minX_) {
        for (auto& row : rows_) {
            row.push_front(T());
        }
        --minX_;
        ++width_;
    }
    while (x >= minX_ + width_) {
        for (auto& row : rows_) {
            row.push_back(T());
        }
        ++width_;
    }
    while (y < minY_) {
        rows_.emplace_front(static_cast<std::size_t>(width_), T());
        --minY_;
    }
    while (y >= minY_ + static_cast<int>(rows_.size())) {
        rows_.emplace_back(static_cast<std::size_t>(width_), T());
    }
}
