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
                
        static GridRef create();
        
    protected:
        
        Grid();
        
    private:
        
        
        
    };
    
}
}
