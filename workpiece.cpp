// Contains all the definitions of the functions defined in workpiece.h

#include "workpiece.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Private function declarations
double lengthOfSegment(lineSegment); // Computes the length of the line segment
double radiusOfArc(circularArc); // Computes the radius of the arc
double angleOfArc(circularArc); // Computes the angle of the arc
double lengthOfArc(circularArc); // Computes the length of the arc
double changePeriodOfAngle(double, double); // Recomputes the angle using a different maximum value
rectangle boundingRectangle(int, lineSegment*); // Computes the bounding rectangle for a set of line segments
rectangle boundingRectangle(circularArc); // Computes the bounding rectangle for a circular arc
rectangle boundingRectangle(int, rectangle*); // Computes the bounding rectangle for set of rectangles
double areaOfRectangle(rectangle, double); // Computes the area of a rectangle
bool indexInIndices(long*, int, long);
int findIndexInIndices(long*, int, long);
void skipLines(FILE*, int);

double lengthOfSegment(lineSegment seg) {
    return sqrt(pow(seg.end.x - seg.begin.x, 2) + pow(seg.end.y - seg.begin.y, 2)); // Pythagoras rule
}

double radiusOfArc(circularArc arc) {
    return sqrt(pow(arc.end.x - arc.center.x,2) + pow(arc.end.y - arc.center.y, 2));
}

double angleOfArc(circularArc arc) {
    double angle = atan2(arc.begin.y - arc.center.y, arc.begin.x - arc.center.x) - atan2(arc.end.y - arc.center.y, arc.end.x - arc.center.x);
    if (angle <= 0)
        angle += 2 * M_PI;
    return angle;
}

double lengthOfArc(circularArc arc) {
    return radiusOfArc(arc) * angleOfArc(arc); // Radius times angle
}

double changePeriodOfAngle(double angle, double maxAngle) {
    double newAngle = fmod(angle + 2 * M_PI, 2 * M_PI);
    if (newAngle > maxAngle)
        newAngle -= 2 * M_PI;
    return newAngle;
}

rectangle boundingRectangle(int numSegments, lineSegment* segments) {
    rectangle rect;
    rect.bottomLeft.x = INFINITY;
    rect.bottomLeft.y = INFINITY;
    rect.topRight.x = -1.0 * INFINITY;
    rect.topRight.y = -1.0 * INFINITY;
    
    // Set the rectangle bound coordinates to the minimum and maximum coordinate values of the segments
    for (int i = 0; i < numSegments; ++i) {
        rect.bottomLeft.x = fmin(rect.bottomLeft.x, fmin(segments[i].begin.x, segments[i].end.x));
        rect.bottomLeft.y = fmin(rect.bottomLeft.y, fmin(segments[i].begin.y, segments[i].end.y));
        rect.topRight.x = fmax(rect.topRight.x, fmax(segments[i].begin.x, segments[i].end.x));
        rect.topRight.y = fmax(rect.topRight.y, fmax(segments[i].begin.y, segments[i].end.y));
    }
    
    return rect;
}

rectangle boundingRectangle(circularArc arc) {
    rectangle rect;
    // Initialze the rectangle bound coordinates to the minimum and maximum coordinate values of the center and vertices
    rect.bottomLeft.x = fmin(arc.center.x, fmin(arc.begin.x, arc.end.x));
    rect.bottomLeft.y = fmin(arc.center.y, fmin(arc.begin.y, arc.end.y));
    rect.topRight.x = fmax(arc.center.x, fmax(arc.begin.x, arc.end.x));
    rect.topRight.y = fmax(arc.center.y, fmax(arc.begin.y, arc.end.y));
    
    double radius = radiusOfArc(arc); // Arc radius
    double angleBegin = atan2(arc.begin.y - arc.center.y, arc.begin.x - arc.center.x); // Angle of the first vertex in XY plane
    double angleEnd = atan2(arc.end.y - arc.center.y, arc.end.x - arc.center.x); // Angle of the last vertex in XY plane
        
    // Check if the angle subtended by the arc contains pi and modify the rectangle's horizontal bound
    if (changePeriodOfAngle(angleBegin, M_PI) <= changePeriodOfAngle(angleEnd, M_PI))
        rect.bottomLeft.x = arc.center.x - radius;
    // Check if the angle subtended by the arc contains 2pi and modify the rectangle's horizontal bound
    if (changePeriodOfAngle(angleBegin, 2 * M_PI) <= changePeriodOfAngle(angleEnd, 2 * M_PI))
        rect.topRight.x = arc.center.x + radius;
    // Check if the angle subtended by the arc contains 0.5pi and modify the rectangle's horizontal bound
    if (changePeriodOfAngle(angleBegin, 0.5 * M_PI) <= changePeriodOfAngle(angleEnd, 0.5 * M_PI))
        rect.topRight.y = arc.center.y + radius;
    // Check if the angle subtended by the arc contains 1.5pi and modify the rectangle's horizontal bound
    if (changePeriodOfAngle(angleBegin, 1.5 * M_PI) <= changePeriodOfAngle(angleEnd, 1.5 * M_PI))
        rect.bottomLeft.y = arc.center.y - radius;
    
    return rect;
}

rectangle boundingRectangle(int numRectangles, rectangle* rects) {
    rectangle bRect;
    bRect.bottomLeft.x = INFINITY;
    bRect.bottomLeft.y = INFINITY;
    bRect.topRight.x = -1.0 * INFINITY;
    bRect.topRight.y = -1.0 * INFINITY;
    
    // Set the rectangle bound coordinates to the minimum and maximum coordinate values of the segments
    for (int i = 0; i < numRectangles; ++i) {
        bRect.bottomLeft.x = fmin(bRect.bottomLeft.x, rects[i].bottomLeft.x);
        bRect.bottomLeft.y = fmin(bRect.bottomLeft.y, rects[i].bottomLeft.y);
        bRect.topRight.x = fmax(bRect.topRight.x, rects[i].topRight.x);
        bRect.topRight.y = fmax(bRect.topRight.y, rects[i].topRight.y);
    }
    
    return bRect;
}


double areaOfRectangle(rectangle rect, double padding) {
    return (padding + rect.topRight.x - rect.bottomLeft.x) * (padding + rect.topRight.y - rect.bottomLeft.y); // Include padding in both directions while computing the area
}

bool indexInIndices(long* indices, int numIndices, long index)
{
    for (int i = 0; i < numIndices; ++i) {
        if (indices[i]==index)
            return true;
    }
    return false;
}

int findIndexInIndices(long* indices, int numIndices, long index)
{
    for (int i = 0; i < numIndices; ++i) {
        if (indices[i]==index)
            return i;
    }
    return -1;
}

void skipLines(FILE* f, int numLines) {
    const int maxLineSize = 256;
    static char line[maxLineSize];
    for (int i = 0; i < numLines; ++i)
        fgets(line,maxLineSize,f);
}

workpiece::workpiece() {
    // Set default values
    // Set default values
    numSegments = 0;
    numArcs = 0;
    segments = 0;
    arcs = 0;
    padding = 0.1;
    maxCutterSpeed = 0.5;
    machiningCost = 0.07;
    materialCost = 0.75;
}

workpiece::workpiece(const char* filename) { 
    // Set default values
    numSegments = 0;
    numArcs = 0;
    segments = 0;
    arcs = 0;
    padding = 0.1;
    maxCutterSpeed = 0.5;
    machiningCost = 0.07;
    materialCost = 0.75;
    FILE* f; // Open file
    const int maxLineSize = 256; // Max buffer size
    char line[maxLineSize]; // Buffer for reading lines
    if (!(f=fopen(filename,"r")))
        printf("ERROR - File %s could not be opened",filename);
    long id; // Variable for reading the id
    const int maxVerticesSize = 100; // Maximum number of vertices
    long vertexIndices[maxVerticesSize]; // Array for storing vertex indices
    point2D vertices[maxVerticesSize]; // Array for storing vertices
    // Initialize all vertex indices to 0
    for (int i = 0; i < maxVerticesSize; ++i)
        vertexIndices[i] = 0;
    int numVertices = 0; // Vertex counter
    int index; // Temporary integer to read index values
    bool loop; // Logical loop switch
    // First read through file to count the number of segments and arcs, and to read the vertices
    while (!feof(f)) {
        fgets(line, maxLineSize, f);
        if (strstr(line, "Edges")) {
            loop = true;
            while (loop) {
                id = 0;
                fgets(line, maxLineSize, f);
                sscanf(line, "\t\t\"%li", &id);
                if (!id) {
                    loop = false;
                    break;
                }
                fgets(line, maxLineSize, f);
                if (strstr(line, "LineSegment")) {
                    skipLines(f, 1);
                    fgets(line, maxLineSize, f);
                    sscanf(line, "\t\t\t\t%li", &id);
                    if (!indexInIndices(vertexIndices, numVertices, id)) {
                        vertexIndices[numVertices] = id;
                        numVertices += 1;
                    }
                    fgets(line, maxLineSize, f);
                    sscanf(line, "\t\t\t\t%li", &id);
                    if (!indexInIndices(vertexIndices, numVertices, id)) {
                        vertexIndices[numVertices] = id;
                        numVertices += 1;
                    }
                    skipLines(f, 2);
                    numSegments++;
                }
                else if (strstr(line, "CircularArc")) {
                    skipLines(f, 1);
                    fgets(line, maxLineSize, f);
                    sscanf(line, "\t\t\t\t%li", &id);
                    if (!indexInIndices(vertexIndices, numVertices, id)) {
                        vertexIndices[numVertices] = id;
                        numVertices += 1;
                    }
                    fgets(line, maxLineSize, f);
                    sscanf(line, "\t\t\t\t%li", &id);
                    if (!indexInIndices(vertexIndices, numVertices, id)) {
                        vertexIndices[numVertices] = id;
                        numVertices += 1;
                    }
                    skipLines(f, 7);
                    numArcs++;
                }
            }
        }
        else if (strstr(line,"Vertices")) {
            loop = true;
            while (loop) {
                id = 0;
                fgets(line, maxLineSize, f);
                sscanf(line, "\t\t\"%li", &id);
                if (!id) {
                    loop = false;
                    break;
                }
                index = findIndexInIndices(vertexIndices, numVertices, id);
                skipLines(f,1);
                fgets(line, maxLineSize, f);
                sscanf(line, "\t\t\t\t\"X\": %lf", &(vertices[index].x));
                fgets(line, maxLineSize, f);
                sscanf(line, "\t\t\t\t\"Y\": %lf", &(vertices[index].y));
                skipLines(f, 2);
            }
        }
    }
    
    segments = (lineSegment*)malloc(sizeof(lineSegment) * numSegments);
    arcs = (circularArc*)malloc(sizeof(circularArc) * numArcs);
    
    // Second read through file to set the coordinates of the line segments and circular arcs
    fseek(f,0,SEEK_SET);
    while (!feof(f)) {
        fgets(line, maxLineSize,f);
        if (strstr(line, "Edges")) {
            numSegments = 0;
            numArcs = 0;
            loop = true;
            while (loop) {
                id = 0;
                fgets(line, maxLineSize, f);
                sscanf(line, "\t\t\"%li", &id);
                if (!id) {
                    loop = false;
                    break;
                }
                fgets(line, maxLineSize, f);
                if (strstr(line,"LineSegment")) {
                    skipLines(f, 1);
                    fgets(line, maxLineSize, f);
                    sscanf(line, "\t\t\t\t%li", &id);
                    index = findIndexInIndices(vertexIndices, numVertices, id);
                    segments[numSegments].begin = vertices[index];
                    fgets(line, maxLineSize, f);
                    sscanf(line, "\t\t\t\t%li", &id);
                    index = findIndexInIndices(vertexIndices, numVertices, id);
                    segments[numSegments].end = vertices[index];
                    skipLines(f, 2);
                    numSegments++;
                }
                else if (strstr(line, "CircularArc")) {
                    skipLines(f, 1);
                    fgets(line, maxLineSize, f);
                    sscanf(line, "\t\t\t\t%li", &id);
                    index = findIndexInIndices(vertexIndices, numVertices, id);
                    arcs[numArcs].begin = vertices[index];
                    fgets(line, maxLineSize, f);
                    sscanf(line, "\t\t\t\t%li", &id);
                    index = findIndexInIndices(vertexIndices, numVertices, id);
                    arcs[numArcs].end = vertices[index];
                    skipLines(f, 2);
                    fgets(line, maxLineSize, f);
                    sscanf(line, "\t\t\t\t\"X\": %lf", &(arcs[numArcs].center.x));
                    fgets(line, maxLineSize, f);
                    sscanf(line, "\t\t\t\t\"Y\": %lf", &(arcs[numArcs].center.y));
                    skipLines(f, 1);
                    fgets(line, maxLineSize, f);
                    sscanf(line, "\t\t\t\"ClockwiseFrom\": %li", &id);
                    if (id == vertexIndices[index])
                    {
                        point2D temp = arcs[numArcs].begin;
                        arcs[numArcs].begin = arcs[numArcs].end;
                        arcs[numArcs].end = temp;
                    }
                    skipLines(f, 1);
                    numArcs++;
                }
            }
        }
    }
    fclose(f);
}

double workpiece::quote() {
    double cost = 0; // Net cost counter
    rectangle* rects = (rectangle*)malloc(sizeof(rectangle) * (numArcs + 1)); // Array of bounding rectangles

    for (int i = 0; i < numSegments ; ++i)
        cost += lengthOfSegment(segments[i]) * machiningCost / maxCutterSpeed; // Add the cost of cutting line segments
    
    for (int i = 0; i < numArcs ; ++i) {
        cost += lengthOfArc(arcs[i]) * machiningCost * exp(1/radiusOfArc(arcs[i])) / maxCutterSpeed; // Add the cost of cutting circular arcs
        rects[i] = boundingRectangle(arcs[i]); // Compute bounding rectangle of arcs
    }
    
    rects[numArcs] = boundingRectangle(numSegments, segments); // Compute bounding rectangle of line segments
    
    // Add material cost of total boundign rectangle + padding
    cost += areaOfRectangle(boundingRectangle(numArcs + 1, rects), padding) * materialCost;
    
    return round(cost * 100) / 100;
}

workpiece::~workpiece() {
    if (segments)
        free(segments);
    if (arcs)
        free(arcs);
}
