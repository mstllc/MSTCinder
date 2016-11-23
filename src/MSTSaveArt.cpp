//
//  MSTSaveArt.cpp
//  SaveArt
//
//  Created by Max Thorson on 8/12/16.
//
//

#include "MSTSaveArt.h"

using namespace mst::saveart;

SaverJobRef SaverJob::create() {
    return shared_ptr<SaverJob>(new SaverJob());
}

SaverJob::SaverJob() { }

SaverRef Saver::create(int artWidth, int artHeight, int dpi, int canvasWidth, int canvasHeight) {
    return shared_ptr<Saver>(new Saver(ivec2(artWidth, artHeight), dpi, ivec2(canvasWidth, canvasHeight)));
}

SaverRef Saver::create(ivec2 artSize, int dpi, ivec2 canvasSize) {
    return shared_ptr<Saver>(new Saver(artSize, dpi, canvasSize));
}

Saver::Saver(ivec2 artSize, int dpi, ivec2 canvasSize) : mArtSize(artSize), mDpi(dpi), mCanvasSize(canvasSize), mInputBuffer(1), mOutputBuffer(1) {
    mThread = thread(bind(&Saver::serviceJobs, this));
    
    gl::Fbo::Format fmt;
    fmt.samples(8);
    mArtFbo = gl::Fbo::create(mArtSize.x * mDpi, mArtSize.y * mDpi, fmt);
}

Saver::~Saver() {
    mCanceled = true;
    mInputBuffer.cancel();
    mOutputBuffer.cancel();
    mThread.join();
}

void Saver::update() {
    
}

void Saver::draw() {
    
}

void Saver::save(fs::path filepath) {
    drawToArtFbo();
    
    SaverJobRef job = SaverJob::create();
    job->filepath = filepath;
    job->artSource = mArtFbo->getColorTexture()->createSource();
    
    mInputBuffer.pushFront(job);
}

void Saver::drawToArtFbo() {
    gl::ScopedFramebuffer framebufferScope(mArtFbo);
    gl::ScopedMatrices matricesScope;
    gl::setMatricesWindow(mCanvasSize);
    gl::ScopedViewport viewportScope(mArtFbo->getSize());
    gl::ScopedModelMatrix modelMatrixScope;
    
    mDrawArtFunction();
}

void Saver::setArtSize(int artWidth, int artHeight) {
    setArtSize(ivec2(artWidth, artHeight));
}

void Saver::setArtSize(ivec2 artSize) {
    mArtSize = artSize;
}

void Saver::setDPI(int dpi) {
    mDpi = dpi;
}

void Saver::setCanvasSize(int canvasWidth, int canvasHeight) {
    setCanvasSize(ivec2(canvasWidth, canvasHeight));
}

void Saver::setCanvasSize(ivec2 canvasSize) {
    mCanvasSize = canvasSize;
}

void Saver::resizeArtFbo() {
    gl::Fbo::Format fmt;
    fmt.samples(8);
    mArtFbo = gl::Fbo::create(mArtSize.x * mDpi, mArtSize.y * mDpi, fmt);
}

gl::FboRef Saver::getArtFbo() {
    return mArtFbo;
}

gl::Texture2dRef Saver::getArtColorTexture() {
    return mArtFbo->getColorTexture();
}

ivec2 Saver::getArtSize() {
    return mArtSize;
}

ivec2 Saver::getCanvasSize() {
    return mCanvasSize;
}

int Saver::getArtDPI() {
    return mDpi;
}

void Saver::connectDrawArtFunction(const function<void ()> &drawArtFunction) {
    mDrawArtFunction = drawArtFunction;
}

bool Saver::resultAvailable() {
    return mOutputBuffer.isNotEmpty();
}

SaverJobResult Saver::getResult() {
    SaverJobResult result;
    mOutputBuffer.popBack(&result);
    return result;
}

void Saver::serviceJobs() {
    ThreadSetup threadSetup;
    
    while (!mCanceled) {
        mInputBuffer.popBack(&mCurrentJob);
        
        if (mCurrentJob == nullptr || mCurrentJob->artSource == nullptr) {
            continue;
        }
        
        Surface8uRef surface = Surface8u::create(mCurrentJob->artSource);
        ip::unpremultiply(surface.get());
        
        ImageTarget::Options opts;
        opts.quality(1.0f);
        try {
            writeImage(mCurrentJob->filepath, *surface, opts, "png");
        } catch (Exception &exc) {
            console() << "Error saving art to filesystem. Exception: " << exc.what() << endl;
            mOutputBuffer.pushFront(SaverJobResult::Error);
            mIsSaving = false;
            continue;
        }
        
        surface = nullptr;
        
        string dpiCommand = "/usr/local/bin/convert -density " + toString(mDpi) + " -units PixelsPerInch " + mCurrentJob->filepath.string() + " " + mCurrentJob->filepath.string();
        int dpiResult = system(dpiCommand.c_str());
        if (dpiResult != 0) {
            console() << "Error setting DPI header in image file. ImageMagick exited with code: " << dpiResult << endl;
            mOutputBuffer.pushFront(SaverJobResult::Error);
            mIsSaving = false;
        } else {
            mOutputBuffer.pushFront(SaverJobResult::Success);
            mIsSaving = false;
        }
    }
}