//
//  MSTGrid.h
//  Grid
//
//  Created by Max Thorson on 11/22/16.
//
//

#pragma once

using namespace ci;
using namespace ci::app;
using namespace std;

namespace mst {
namespace grid {
    
    typedef shared_ptr<class Grid> GridRef;
    class Grid {
    public:
                
        static GridRef createFromBoundsAndCellSize(Rectf bounds, vec2 cellSize);
        static GridRef createFromBoundsAndCellCount(Rectf bounds, ivec2 cellCount);
        
        Rectf getBounds();
        void setPosition(vec2 position);
        
        bool pointInBounds(vec2 point);
        ivec2 cellForPoint(vec2 point);
        Rectf boundsForCell(ivec2 cell);
        
        void drawBounds(bool fill = false);
        void drawWires();
        
    protected:
        
        Grid(Rectf bounds, vec2 cellSize);
        
    private:
        
        vec2 mCellSize;
        ivec2 mCellCount;
        Rectf mBounds;
        
    };
    
}
}
