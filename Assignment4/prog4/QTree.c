//--------------------------------
// Your quadtree ADT implementation
//--------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "QTree.h"

// Definitions of helper funcitons---------------------------------------------
// Some useful helper functions that can be used to finish this program (not mandatory to use)
// Feel free to add more if needed
point getCenter(point tl, point br);
int isSamePoint(point p1, point p2);
int getIndexOfChild(point p, point tl, point br);

// Constructors-Destructors ---------------------------------------------------
point createPoint(int x, int y)
{

    point p = (point)malloc(sizeof(pointObj));
    p->x = x;
    p->y = y;

    return p;
}

// Return a new point which contains the same coordinate as point q
point copyPoint(point q)
{
    point copy = (point)malloc(sizeof(pointObj));
    copy->x = q->x;
    copy->y = q->y;

    return copy;
}

pointsContainer createPointsContainer()
{
    pointsContainer pc = (pointsContainer)malloc(sizeof(pointsContainerObj));
    //memset(pc, 0, sizeof(pointsContainerObj));
    pc->size = 0;
    pc->list = (point *)malloc(sizeof(point));
    pc->topLeft = NULL;
    pc->botRight = NULL;
    pc->target = NULL;
    return pc;
}

void freePointsContainer(pointsContainer *pc_ptr)
{
    if (pc_ptr && *pc_ptr)
    {
        pointsContainer pc = *pc_ptr;
        for (int i = 0; i < pc->size; i++)
            free(pc->list[i]);
        free(pc->topLeft);
        free(pc->botRight);
        free(pc->target);
        free(pc->list);
        free(pc);
        pc = NULL;
        *pc_ptr = NULL;
    }
}

quadNode createQuadTree(point topLeft, point botRight)
{

    quadNode pt = (quadNode)malloc(sizeof(quadNodeObj));
    pt->topLeft = topLeft;
    pt->botRight = botRight;
    pt->child = (quadNode *)malloc(sizeof(quadNode) * 4);
    for (int i = 0; i < 4; i++)
    {
        pt->child[i] = NULL;
    }
    pt->node = NULL;
    pt->count = 0;

    return pt;
}

void deleteQuadTree(quadNode *pRoot)
{
    if (pRoot && *pRoot)
    {
        quadNode pc = *pRoot;
        free(pc->topLeft);
        free(pc->botRight);
        free(pc->node);
        free(pc);
        pc->count = 0;
        pc = NULL;
        *pRoot = NULL;
    }
}

//Checking if Quadtree is empty or full. Returns 1 if it is empty
int isQuadtreeEmpty(quadNode root)
{
    if (root->node == NULL)
    {
        for (int i = 0; i < 4; i++)
        {
            if (root->child[i] != NULL)
                return 0;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

//Creating Quadtree by checking the index
void createQuadtreeAtIndex(quadNode root, int index)
{
    point newCenter = getCenter(root->topLeft, root->botRight);
    point newCenterPlusOne = createPoint(newCenter->x + 1, newCenter->y + 1);
    switch (index)
    {
    case 0:
        root->child[0] = createQuadTree(root->topLeft, newCenter); // Add NW subdevision
        break;
    case 1:
        root->child[1] = createQuadTree(createPoint(root->topLeft->x, newCenterPlusOne->y),
                                        createPoint(newCenter->x, root->botRight->y)); // Add NE
        break;
    case 2:
        root->child[2] = createQuadTree(createPoint(newCenterPlusOne->x, root->topLeft->y),
                                        createPoint(root->botRight->x, newCenter->y)); // ADD SW
        break;
    case 3:
        root->child[3] = createQuadTree(newCenterPlusOne, root->botRight); // Add SE subdevission
        break;
    default:
        break;
    }
}

// Insert point p into the quadtree. If p is already in the quadtree, do nothing
void insertPoint(quadNode root, point p)
{
    if (root == NULL || p == NULL)
    {
        return;
    }

    if (!(p->x >= root->topLeft->x && p->x <= root->botRight->x && p->y >= root->topLeft->y && p->y <= root->botRight->y))
    {
        return;
    }
    // printf("after bound check in insert func\n");
    // We are at a quad of unit area
    // We cannot subdivide this quad further
    if (abs(root->botRight->x - root->topLeft->x) <= 1 && abs(root->topLeft->y - root->botRight->y) <= 1)
    {

        if (root->node == NULL)
        {
            root->node = copyPoint(p);
        }
        else
        {
            return;
        }
    }
    if (isQuadtreeEmpty(root))
    {
        root->node = copyPoint(p);
    }
    else if (root->node != NULL)
    {
        // we need a subdevide
        int index = getIndexOfChild(p, root->topLeft, root->botRight);
        createQuadtreeAtIndex(root, index);
        insertPoint(root->child[index], p);
        int nodeIndex = getIndexOfChild(root->node, root->topLeft, root->botRight);
        createQuadtreeAtIndex(root, nodeIndex);
        insertPoint(root->child[nodeIndex], root->node);
        root->node = NULL;
    }
    else
    {
        //What subdevition the point goes into
        int index = getIndexOfChild(p, root->topLeft, root->botRight);
        if (root->child[index] == NULL)
        {
            createQuadtreeAtIndex(root, index);
        }
        insertPoint(root->child[index], p);
    }
}

// Delete the point from the quadtree
// Return 1 if point p is in the quadtree, otherwise return 0
int deletePoint(quadNode root, point p)
{

    if (root->node != NULL)
    {
        if (isSamePoint(root->node, p))
        {
            free(root->node);
            root->node = NULL;
            return 1;
        }
    }
    else if (root->child != NULL)
    {
        for (int i = 0; i < 4; i++)
        {
            if (deletePoint(root->child[i], p))
            {
                if (isQuadtreeEmpty(root->child[i]))
                {
                    root->child[i] = NULL;
                    return 1;
                }
            }
        }
    }
    return 0;
}

// Get all points in the quadtree
// Return a container that contains all points in the quadtree
pointsContainer getAllPoints(quadNode root)
{
    pointsContainer container = createPointsContainer();
    //container->list = (point *)malloc(sizeof(point) * root->count);
    container->list = (point *)realloc(container->list, sizeof(point) * root->count);
    container->botRight = copyPoint(root->botRight);
    container->topLeft = copyPoint(root->topLeft);
    container->target = getCenter(root->topLeft, root->botRight);
    if (root->node != NULL)
    {
        container->list[container->size] = copyPoint(root->node);
        container->size++;
    }
    else if (root->child != NULL)
    {
        for (int i = 0; i < 4; i++)
        {
            if (root->child[i] == NULL)
            {
                continue;
            }
            pointsContainer pointsC = getAllPoints(root->child[i]);
            for (int j = 0; j < pointsC->size; j++)
            {
                container->list[container->size] = copyPoint(pointsC->list[j]);
                container->size++;
            }
            freePointsContainer(&pointsC);
        }
    }

    return container;
}

//Find child and return 1 if that child is founded
int foundedChild(quadNode child, point tl, point br)
{
    if (br->x < child->topLeft->x || br->y < child->topLeft->y)
    {
        return 0;
    }
    if (tl->x > child->botRight->x || tl->y > child->botRight->y)
    {
        return 0;
    }
    return 1;
}

// Get the neighbouring points of point p in 4*radius region
// Return a container that contains all neighbors of point p
pointsContainer getNeighbors(quadNode root, point p, int radius)
{
    pointsContainer container = createPointsContainer();
    //container->list = (point *)malloc(sizeof(point) * root->count);
    container->list = (point *)realloc(container->list, sizeof(point) * root->count);
    point tl = createPoint(p->x - 2 * radius, p->y - 2 * radius);
    point br = createPoint(p->x + 2 * radius, p->y + 2 * radius);
    container->botRight = copyPoint(root->botRight);
    container->topLeft = copyPoint(root->topLeft);
    container->target = copyPoint(p);

    if (root->node != NULL)
    {
        int x = root->node->x - p->x;
        int y = root->node->y - p->y;
        if (sqrt(pow(x, 2) - pow(y, 2)) < (4 * radius))
        {
            container->list[container->size] = copyPoint(root->node);
            container->size++;
        }
    }
    else if (root->child != NULL)
    {
        for (int i = 0; i < 4; i++)
        {
            if (root->child[i] == NULL)
            {
                continue;
            }
            if (!foundedChild(root->child[i], tl, br))
            {
                continue;
            }
            pointsContainer pointsC = getNeighbors(root->child[i], p, radius);
            for (int j = 0; j < pointsC->size; j++)
            {
                container->list[container->size] = copyPoint(pointsC->list[j]);
                container->size++;
            }
            freePointsContainer(&pointsC);
        }
    }

    return container;
}

void printPoint(point p)
{
    if (p)
        printf("(%d,%d)", p->x, p->y);
}

void displayQuadTree(quadNode root)
{
    assert(root);

    printf("[");
    printPoint(root->topLeft);
    printf(", ");
    printPoint(root->botRight);
    printf(", ");
    if (root->node)
    {
        printPoint(root->node);
    }
    else
    {
        printf("\n");
        for (int i = 0; i < 4; i++)
        {
            if (root->child[i])
            {
                displayQuadTree(root->child[i]);
                printf(", ");
            }
            else
                printf("[], ");
        }
    }
    printf("]");
}

// Helper functions
// Get the center point within a region
point getCenter(point tl, point br)
{
    int mid_x = (int)(br->x + tl->x) / 2;
    int mid_y = (int)(br->y + tl->y) / 2;
    return createPoint(mid_x, mid_y);
}

// Check if two points are the same
int isSamePoint(point p1, point p2)
{
    return (p1->x == p2->x) && (p1->y == p2->y);
}

// Get the index of the child node
int getIndexOfChild(point p, point tl, point br)
{
    point center = getCenter(tl, br);
    int res = -1;
    if (p->y <= center->y)
    {
        if (p->x <= center->x)
        {
            // NW
            res = 0;
        }
        else
        {
            // SW
            res = 2;
        }
    }
    else
    {
        if (p->x <= center->x)
        {
            // NE
            res = 1;
        }
        else
        {
            // SE
            res = 3;
        }
    }
    free(center);
    return res;
}
