//-------------------------------
// Structs and function headers for
// quadtree ADT
//-------------------------------

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef struct pointObj {
    int x;
    int y;
} pointObj;
typedef pointObj* point;

// A container to store points
typedef struct pointsContainerObj {
    int size;           // # of points we store in this container
    point* list;        // An array of points with unknow size

    // The following three fields are used when you look for points in a region
    point target;       // The start point which is used to determine a region
    point topLeft;      // The top left point of the region
    point botRight;     // The bottom right point of the region
}pointsContainerObj;
typedef pointsContainerObj* pointsContainer;

// node structure of quadtree
typedef struct quadNodeObj {
    int                 count;    // How many objects (dots) in this node (and its children)
    point               topLeft;  // The top left point of this node
    point               botRight; // The bottom right point of the region

    // The following two fields should have only one valid at a time
    point               node;     // A point in this node
    struct quadNodeObj  **child;  // List to children (subregion).  0=NW, 1=NE, 2=SW, 3=SE
} quadNodeObj;
typedef quadNodeObj* quadNode;


// Constructors-Destructors ---------------------------------------------------
point createPoint(int x, int y);
// Return a new point which contains the same coordinate as point q
point copyPoint(point q);

pointsContainer createPointsContainer();
void freePointsContainer(pointsContainer* pc_ptr);

quadNode createQuadTree(point topLeft, point botRight);
void deleteQuadTree(quadNode* pRoot);

// Access functions -----------------------------------------------------------
// Insert a new point into the quadtree
void insertPoint(quadNode root, point p);

// Delete the point from the quadtree
// Retrun 1 if point p is in the quadtree, otherwise retrun 0
int deletePoint(quadNode root, point p);

// Get all points in the quadtree
// Return a container that contains all points in the quadtree
pointsContainer getAllPoints(quadNode root);

// Get the neighbouring points of point p in region with length 4*radius
// Return a container that contains all neighbors of point p
pointsContainer getNeighbors(quadNode root, point p, int radius);

void printPoint(point p);
void displayQuadTree(quadNode root);