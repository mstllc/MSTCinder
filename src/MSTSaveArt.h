//
//  MSTSaveArt.h
//  SaveArt
//
//  Created by Max Thorson on 8/12/16.
//
//

#pragma once

#define ART_SAVER_DEFAULT_DPI 300

#include "cinder/ip/Premultiply.h"
#include "cinder/ConcurrentCircularBuffer.h"
#include "cinder/Thread.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace mst {
namespace saveart {
    
    enum SaverJobResult {
        Success,
        Error
    };
    
    typedef shared_ptr<class SaverJob> SaverJobRef;
    class SaverJob {
    public:
        
        static SaverJobRef create();
        
        fs::path filepath;
        ImageSourceRef artSource;
        
    protected:
        
        SaverJob();
    };
 
    typedef shared_ptr<class Saver> SaverRef;
    class Saver {
    public:
        
        ~Saver();
        
        static SaverRef create(int artWidth, int artHeight, int dpi = ART_SAVER_DEFAULT_DPI, int canvasWidth = getWindowWidth(), int canvasHeight = getWindowHeight());
        static SaverRef create(ivec2 artSize, int dpi = ART_SAVER_DEFAULT_DPI, ivec2 canvasSize = getWindowSize());
        
        virtual void update();
        virtual void draw();
        
        virtual void save(fs::path filepath);
        
        virtual void setArtSize(int artWidth, int artHeight);
        virtual void setArtSize(ivec2 artSize);
        virtual void setDPI(int dpi);
        virtual void setCanvasSize(int canvasWidth, int canvasHeight);
        virtual void setCanvasSize(ivec2 canvasSize);
        virtual void resizeArtFbo();
        
        virtual gl::FboRef getArtFbo();
        virtual gl::Texture2dRef getArtColorTexture();
        virtual ivec2 getArtSize();
        virtual ivec2 getCanvasSize();
        virtual int getArtDPI();
        
        bool resultAvailable();
        SaverJobResult getResult();
        
        void connectDrawArtFunction(const function<void ()> &drawArtFunction);
        
    protected:
        
        Saver(ivec2 artSize, int dpi, ivec2 canvasSize);
        
    private:
        
        ivec2 mCanvasSize;
        ivec2 mArtSize;
        int mDpi;
        
        bool mCanceled = false;
        bool mIsSaving = false;
        
        gl::FboRef mArtFbo;
        
        void drawToArtFbo();
        function<void ()> mDrawArtFunction;
        
        SaverJobRef mCurrentJob;
        
        thread mThread;
        void serviceJobs();
        ConcurrentCircularBuffer<SaverJobRef> mInputBuffer;
        ConcurrentCircularBuffer<SaverJobResult> mOutputBuffer;
        
    };
    
}
}