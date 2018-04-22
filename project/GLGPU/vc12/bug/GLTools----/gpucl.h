#ifndef _GPUCCL_H
#define _GPUCCL_H

#include <vector>

class GPUCCL
{
public:
    struct Component
    {
        int ID;
        int minX, minY, maxX, maxY;
    };

    ~GPUCCL();

    bool init(int width, int height, int numBlocks);
    void begin(unsigned char* image);
    void createRuns();
    void processColumns();
    void postProcessColumns();
    void computeBBoxes();
    void stencilRoute();
    void end();

    int labeledTextureID();
    float* labeledFrame();
    std::vector<Component> results();

private:
    int _width, _height;
    void* _internal;
};

#endif