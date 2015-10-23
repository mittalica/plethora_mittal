// Contains all the class and method declarations for computing a quote on the workpiece

// Point datatype
struct point2D {
    double x, y;
};

// Line segment datatype
struct lineSegment {
    point2D begin, end;
};

// Circular arc datatype (clockwise from begin to end)
struct circularArc {
    point2D center, begin, end;
};

// Rectangle datatype
struct rectangle {
    point2D bottomLeft, topRight;
};

class workpiece {
private:
    int numSegments, numArcs; // Number of line segments and circular arcs
    lineSegment* segments; // Array of line segments
    circularArc* arcs; // Array of circular arcs
    double padding; //Padding in inches
    double materialCost; //Material cost in dollars/inch^2
    double maxCutterSpeed; //Maximum cutter speed in inches/second
    double machiningCost; // Cost of running the laser cutter in dollars/second
public:
    workpiece(); //Default constructor
    workpiece(const char*); // Construct object from file
    double quote(); //Compute the quote
    ~workpiece(); //Destructor
};

