#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "QTree.h"

// Destroy all points inside the explosion range in quadtree.
// The explosion range is 4*radius starting from the center of all collision dots
// [Return] number of points have been deleted from the quadtree due to the explosions
int explosion(quadNode Q, pointsContainer ap, int radius)
{
    //Check the distance of 4*radius of collision nodes

    int n = 0;
    for (int i = 0; i < ap->size; i++)
    {
        point collisionPoint = ap->list[i];
        pointsContainer cp = getNeighbors(Q, collisionPoint, radius);
        for (int i = 0; i < cp->size; i++)
        {
            if (deletePoint(Q, cp->list[i]))
                n++;
        }
        freePointsContainer(&cp);
    }
    return n;
}

// Check how many collision events occur in the current map
// Leverage explosion function to destroy all points in the explosion range
// [Return] the number of collision events
int collision(quadNode Q, int radius)
{

    pointsContainer colliedPoints = createPointsContainer();
    // allPoints - all of the points in the quad tree
    pointsContainer allPoints = getAllPoints(Q);
    int n = 0;
    point a, b;
    for (int i = 0; i < allPoints->size; i++)
    {
        a = allPoints->list[i];
        pointsContainer neighbors = getNeighbors(Q, a, radius);
        for (int j = 1; j < neighbors->size; j++)
        {
            b = neighbors->list[j];
            int x = a->x - b->x;
            int y = a->y - b->y;
            if (sqrt(pow(x, 2) - pow(y, 2)) < (2 * radius))
            {
                colliedPoints->list[colliedPoints->size] = copyPoint(a);
                colliedPoints->size++;
                colliedPoints->list[colliedPoints->size] = copyPoint(b);
                colliedPoints->size++;
                n++; // we are counting duplicate
            }
        }
    }
    explosion(Q, colliedPoints, radius);

    return n / 2;
}

int main()
{
    char line[100];
    char *command;
    int done = 0;

    quadNode Q = NULL;
    int size, x, y, radius, seed, numPoints;

    // process input
    while (!done)
    {
        // read an entire line as a string
        fgets(line, 100, stdin);

        // pulls command code, assume comma or tab separated
        command = strtok(line, " \t");

        if (strncmp(command, "ex", 2) == 0)
        {
            if (Q)
                deleteQuadTree(&Q);
            printf("Exited Program\n");
            done = 1;
        }
        else if (strncmp(command, "ct", 2) == 0)
        {
            size = atoi(strtok(NULL, " "));
            radius = atoi(strtok(NULL, " "));

            if (Q != NULL)
                deleteQuadTree(&Q);

            Q = createQuadTree(createPoint(0, 0), createPoint(size - 1, size - 1));
        }
        else if (strncmp(command, "ap", 2) == 0)
        {
            if (Q == NULL)
            {
                printf("Please create QuadTree first...\n");
                continue;
            }
            x = atoi(strtok(NULL, " "));
            y = atoi(strtok(NULL, " "));
            insertPoint(Q, createPoint(x, y));
        }
        else if (strncmp(command, "dp", 2) == 0)
        {
            if (Q == NULL)
            {
                printf("Please create QuadTree first...\n");
                continue;
            }
            x = atoi(strtok(NULL, " "));
            y = atoi(strtok(NULL, " "));
            point p = createPoint(x, y);
            deletePoint(Q, p);
            free(p);
        }
        else if (strncmp(command, "se", 2) == 0)
        {
            seed = atoi(strtok(NULL, " "));
            srand(seed);
        }
        else if (strncmp(command, "ip", 2) == 0)
        {
            if (Q == NULL)
            {
                printf("Please create QuadTree first...\n");
                continue;
            }
            numPoints = atoi(strtok(NULL, " "));
            for (int i = 0; i < numPoints; i++)
            {
                x = rand() % size;
                y = rand() % size;
                insertPoint(Q, createPoint(x, y));
            }
        }
        else if (strncmp(command, "gn", 2) == 0)
        {
            // Get neighbors of (x, y)
            if (Q == NULL)
            {
                printf("Please create QuadTree first...\n");
                continue;
            }
            x = atoi(strtok(NULL, " "));
            y = atoi(strtok(NULL, " "));
            point p = createPoint(x, y);
            pointsContainer pc = getNeighbors(Q, p, radius);

            // VALIDATION
            // point* list = pc->list;
            // for (int i = 0; i < pc->size; i++) {
            //     printPoint(list[i]);
            //     printf(", ");
            // }
            // printf("\n");

            free(p);
            freePointsContainer(&pc);
        }
        else if (strncmp(command, "pt", 2) == 0)
        {
            if (Q == NULL)
            {
                printf("Please create QuadTree first...\n");
                continue;
            }
            displayQuadTree(Q);
            printf("\n");
        }
        else if (strncmp(command, "co", 2) == 0)
        {
            if (Q == NULL)
            {
                printf("Please create QuadTree first...\n");
                continue;
            }
            int numCollision = collision(Q, radius);
            printf("%d collisions happened in the map!\n", numCollision);
        }
        else
            printf("Unrecognized command: %2s\n", command);
    }

    return 0;
}

// if(Q->node !=NULL){
//     return counter;
// }else if(Q->child != NULL){
//     for (int i = 0; i < 4; i++)
//     {
//         counter += collision(Q->child[i], radius);
//     }
// }
// return counter;