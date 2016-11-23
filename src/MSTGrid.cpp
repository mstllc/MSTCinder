//
//  MSTGrid.cpp
//  Grid
//
//  Created by Max Thorson on 11/22/16.
//
//

#include "MSTGrid.h"

GridRef Grid::create() {
    return shared_ptr<Grid>(new Grid());
}

Grid::Grid() { }
