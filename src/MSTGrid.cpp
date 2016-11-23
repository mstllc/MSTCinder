//
//  MSTGrid.cpp
//  Grid
//
//  Created by Max Thorson on 11/22/16.
//
//

#include "MSTGrid.h"

using namespace mst::grid;

GridRef Grid::createFromBoundsAndCellSize(Rectf bounds, vec2 cellSize) {
    return shared_ptr<Grid>(new Grid(bounds, cellSize));
}

GridRef Grid::createFromBoundsAndCellCount(Rectf bounds, ivec2 cellCount) {
    vec2 cellSize = bounds.getSize() / (vec2)cellCount;
    return shared_ptr<Grid>(new Grid(bounds, cellSize));
}

Grid::Grid(Rectf bounds, vec2 cellSize) : mBounds(bounds), mCellSize(cellSize) {
    mCellCount = mBounds.getSize() / mCellSize;
    mBounds.x2 = mBounds.x1 + (mCellCount.x * mCellSize.x);
    mBounds.y2 = mBounds.y1 + (mCellCount.y * mCellSize.y);
}

Rectf Grid::getBounds() {
    return mBounds;
}

void Grid::setPosition(vec2 position) {
    mBounds.offset(position - mBounds.getUpperLeft());
}

bool Grid::pointInBounds(vec2 point) {
    return mBounds.contains(point);
}

ivec2 Grid::cellForPoint(vec2 point) {
    return (ivec2)glm::floor((point - mBounds.getUpperLeft()) / mCellSize);
}

Rectf Grid::boundsForCell(ivec2 cell) {
    Rectf bounds(vec2(0.0f, 0.0f), mCellSize);
    bounds.offset(((vec2)cell * mCellSize) + mBounds.getUpperLeft());
    return bounds;
}

void Grid::drawBounds(bool fill) {
    if (fill) {
        gl::drawSolidRect(mBounds);
    } else {
        gl::drawStrokedRect(mBounds);
    }
}

void Grid::drawWires() {
    for (int c = 1; c < mCellCount.x; ++c) {
        gl::drawLine(vec2(c * mCellSize.x, mBounds.y1), vec2(c * mCellSize.x, mBounds.y2));
    }
    for (int r = 1; r < mCellCount.y; ++r) {
        gl::drawLine(vec2(mBounds.x1, r * mCellSize.y), vec2(mBounds.x2, r * mCellSize.y));
    }
}
