 #include <bits/stdc++.h>
 using namespace std;

//abitrary (can be changed later sort of depends)
#define ROW 100
#define COLUMN 100
 
//creating a "coordinate" and "vector" system
typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;
 
// nodes essentially hold a parent value and pathing info
struct node {
    // 0 <= i <= row-1 & 0 <= j <= col-1
    int parent_i, parent_j;
    // (f = g + h)
    double f, g, h;
};
 
bool isValid(int row, int col){
    // Returns true if row number and column number are in range to defined ROW and COLUMN
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COLUMN);
}
 
// checks wether our node is blocked by an obstacle
bool isUnblocked(int grid[][COLUMN], int row, int col){
    if (grid[row][col] == 1)
        return (true);
    else
        return (false);
}
 
// checks wether we made it
bool isDestination(int row, int col, Pair dest){
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}
 
//calculates the 'h' heuristics
double hGet(int row, int col, Pair dest){
    return ((double)sqrt(
        (row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second)));
}
 
// traces the path from the source to the dest
void tracePath(node nodeDetails[][COLUMN], Pair dest){
    printf("\nThe Path is ");
    int row = dest.first;
    int col = dest.second;
 
    stack<Pair> Path;
 
    while (!(nodeDetails[row][col].parent_i == row && nodeDetails[row][col].parent_j == col)) {
        Path.push(make_pair(row, col));
        int temp_row = nodeDetails[row][col].parent_i;
        int temp_col = nodeDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
 
    Path.push(make_pair(row, col));
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        printf("-> (%d,%d) ", p.first, p.second);
    }
 
    return;
}
 
// A Function to find the shortest path between
void aStar(int grid[][COLUMN], Pair src, Pair dest)
{
    // If the source is out of range
    if (isValid(src.first, src.second) == false) {
        printf("Source is invalid\n");
        return;
    }
 
    // If the destination is out of range
    if (isValid(dest.first, dest.second) == false) {
        printf("Destination is invalid\n");
        return;
    }
 
    // Either the source or the destination is blocked
    if (isUnblocked(grid, src.first, src.second) == false
        || isUnblocked(grid, dest.first, dest.second)
               == false) {
        printf("Source or the destination is blocked\n");
        return;
    }
 
    // If the destination node is the same as source node
    if (isDestination(src.first, src.second, dest)
        == true) {
        printf("We are already at the destination\n");
        return;
    }
 
    // Create a closed list and initialise it to false which
    // means that no node has been included yet This closed
    // list is implemented as a boolean 2D array
    bool closedList[ROW][COLUMN];
    memset(closedList, false, sizeof(closedList));
 
    // Declare a 2D array of structure to hold the details
    // of that node
    node nodeDetails[ROW][COLUMN];
 
    int i, j;
 
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            nodeDetails[i][j].f = FLT_MAX;
            nodeDetails[i][j].g = FLT_MAX;
            nodeDetails[i][j].h = FLT_MAX;
            nodeDetails[i][j].parent_i = -1;
            nodeDetails[i][j].parent_j = -1;
        }
    }
 
    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    nodeDetails[i][j].f = 0.0;
    nodeDetails[i][j].g = 0.0;
    nodeDetails[i][j].h = 0.0;
    nodeDetails[i][j].parent_i = i;
    nodeDetails[i][j].parent_j = j;
 
    /*
     Create an open list having information as-
     <f, <i, j>>
     where f = g + h,
     and i, j are the row and column index of that node
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
     This open list is implemented as a set of pair of
     pair.*/
    set<pPair> openList;
 
    // Put the starting node on the open list and set its
    // 'f' as 0
    openList.insert(make_pair(0.0, make_pair(i, j)));
 
    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;
 
    while (!openList.empty()) {
        pPair p = *openList.begin();
 
        // Remove this vertex from the open list
        openList.erase(openList.begin());
 
        // Add this vertex to the closed list
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;
 
        /*
         Generating all the 8 successor of this node
 
             N.W   N   N.E
               \   |   /
                \  |  /
             W----node----E
                  / | \
                /   |  \
             S.W    S   S.E
 
         node-->Popped node (i, j)
         N -->  North       (i-1, j)
         S -->  South       (i+1, j)
         E -->  East        (i, j+1)
         W -->  West           (i, j-1)
         N.E--> North-East  (i-1, j+1)
         N.W--> North-West  (i-1, j-1)
         S.E--> South-East  (i+1, j+1)
         S.W--> South-West  (i+1, j-1)*/
 
        // To store the 'g', 'h' and 'f' of the 8 successors
        double gNew, hNew, fNew;
 
        //----------- 1st Successor (North) ------------
 
        // Only process this node if this is a valid one
        if (isValid(i - 1, j) == true) {
            // If the destination node is the same as the
            // current successor
            if (isDestination(i - 1, j, dest) == true) {
                // Set the Parent of the destination node
                nodeDetails[i - 1][j].parent_i = i;
                nodeDetails[i - 1][j].parent_j = j;
                printf("The destination node is found\n");
                tracePath(nodeDetails, dest);
                foundDest = true;
                return;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i - 1][j] == false
                     && isUnblocked(grid, i - 1, j)
                            == true) {
                gNew = nodeDetails[i][j].g + 1.0;
                hNew = hGet(i - 1, j, dest);
                fNew = gNew + hNew;
 
                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square node
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (nodeDetails[i - 1][j].f == FLT_MAX
                    || nodeDetails[i - 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j)));
 
                    // Update the details of this node
                    nodeDetails[i - 1][j].f = fNew;
                    nodeDetails[i - 1][j].g = gNew;
                    nodeDetails[i - 1][j].h = hNew;
                    nodeDetails[i - 1][j].parent_i = i;
                    nodeDetails[i - 1][j].parent_j = j;
                }
            }
        }
 
        //----------- 2nd Successor (South) ------------
 
        // Only process this node if this is a valid one
        if (isValid(i + 1, j) == true) {
            // If the destination node is the same as the
            // current successor
            if (isDestination(i + 1, j, dest) == true) {
                // Set the Parent of the destination node
                nodeDetails[i + 1][j].parent_i = i;
                nodeDetails[i + 1][j].parent_j = j;
                printf("The destination node is found\n");
                tracePath(nodeDetails, dest);
                foundDest = true;
                return;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i + 1][j] == false
                     && isUnblocked(grid, i + 1, j)
                            == true) {
                gNew = nodeDetails[i][j].g + 1.0;
                hNew = hGet(i + 1, j, dest);
                fNew = gNew + hNew;
 
                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square node
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (nodeDetails[i + 1][j].f == FLT_MAX
                    || nodeDetails[i + 1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j)));
                    // Update the details of this node
                    nodeDetails[i + 1][j].f = fNew;
                    nodeDetails[i + 1][j].g = gNew;
                    nodeDetails[i + 1][j].h = hNew;
                    nodeDetails[i + 1][j].parent_i = i;
                    nodeDetails[i + 1][j].parent_j = j;
                }
            }
        }
 
        //----------- 3rd Successor (East) ------------
 
        // Only process this node if this is a valid one
        if (isValid(i, j + 1) == true) {
            // If the destination node is the same as the
            // current successor
            if (isDestination(i, j + 1, dest) == true) {
                // Set the Parent of the destination node
                nodeDetails[i][j + 1].parent_i = i;
                nodeDetails[i][j + 1].parent_j = j;
                printf("The destination node is found\n");
                tracePath(nodeDetails, dest);
                foundDest = true;
                return;
            }
 
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i][j + 1] == false
                     && isUnblocked(grid, i, j + 1)
                            == true) {
                gNew = nodeDetails[i][j].g + 1.0;
                hNew = hGet(i, j + 1, dest);
                fNew = gNew + hNew;
 
                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square node
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (nodeDetails[i][j + 1].f == FLT_MAX
                    || nodeDetails[i][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j + 1)));
 
                    // Update the details of this node
                    nodeDetails[i][j + 1].f = fNew;
                    nodeDetails[i][j + 1].g = gNew;
                    nodeDetails[i][j + 1].h = hNew;
                    nodeDetails[i][j + 1].parent_i = i;
                    nodeDetails[i][j + 1].parent_j = j;
                }
            }
        }
 
        //----------- 4th Successor (West) ------------
 
        // Only process this node if this is a valid one
        if (isValid(i, j - 1) == true) {
            // If the destination node is the same as the
            // current successor
            if (isDestination(i, j - 1, dest) == true) {
                // Set the Parent of the destination node
                nodeDetails[i][j - 1].parent_i = i;
                nodeDetails[i][j - 1].parent_j = j;
                printf("The destination node is found\n");
                tracePath(nodeDetails, dest);
                foundDest = true;
                return;
            }
 
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i][j - 1] == false
                     && isUnblocked(grid, i, j - 1)
                            == true) {
                gNew = nodeDetails[i][j].g + 1.0;
                hNew = hGet(i, j - 1, dest);
                fNew = gNew + hNew;
 
                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square node
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (nodeDetails[i][j - 1].f == FLT_MAX
                    || nodeDetails[i][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j - 1)));
 
                    // Update the details of this node
                    nodeDetails[i][j - 1].f = fNew;
                    nodeDetails[i][j - 1].g = gNew;
                    nodeDetails[i][j - 1].h = hNew;
                    nodeDetails[i][j - 1].parent_i = i;
                    nodeDetails[i][j - 1].parent_j = j;
                }
            }
        }
 
        //----------- 5th Successor (North-East)
        //------------
 
        // Only process this node if this is a valid one
        if (isValid(i - 1, j + 1) == true) {
            // If the destination node is the same as the
            // current successor
            if (isDestination(i - 1, j + 1, dest) == true) {
                // Set the Parent of the destination node
                nodeDetails[i - 1][j + 1].parent_i = i;
                nodeDetails[i - 1][j + 1].parent_j = j;
                printf("The destination node is found\n");
                tracePath(nodeDetails, dest);
                foundDest = true;
                return;
            }
 
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i - 1][j + 1] == false
                     && isUnblocked(grid, i - 1, j + 1)
                            == true) {
                gNew = nodeDetails[i][j].g + 1.414;
                hNew = hGet(i - 1, j + 1, dest);
                fNew = gNew + hNew;
 
                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square node
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (nodeDetails[i - 1][j + 1].f == FLT_MAX
                    || nodeDetails[i - 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j + 1)));
 
                    // Update the details of this node
                    nodeDetails[i - 1][j + 1].f = fNew;
                    nodeDetails[i - 1][j + 1].g = gNew;
                    nodeDetails[i - 1][j + 1].h = hNew;
                    nodeDetails[i - 1][j + 1].parent_i = i;
                    nodeDetails[i - 1][j + 1].parent_j = j;
                }
            }
        }
 
        //----------- 6th Successor (North-West)
        //------------
 
        // Only process this node if this is a valid one
        if (isValid(i - 1, j - 1) == true) {
            // If the destination node is the same as the
            // current successor
            if (isDestination(i - 1, j - 1, dest) == true) {
                // Set the Parent of the destination node
                nodeDetails[i - 1][j - 1].parent_i = i;
                nodeDetails[i - 1][j - 1].parent_j = j;
                printf("The destination node is found\n");
                tracePath(nodeDetails, dest);
                foundDest = true;
                return;
            }
 
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i - 1][j - 1] == false
                     && isUnblocked(grid, i - 1, j - 1)
                            == true) {
                gNew = nodeDetails[i][j].g + 1.414;
                hNew = hGet(i - 1, j - 1, dest);
                fNew = gNew + hNew;
 
                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square node
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (nodeDetails[i - 1][j - 1].f == FLT_MAX
                    || nodeDetails[i - 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i - 1, j - 1)));
                    // Update the details of this node
                    nodeDetails[i - 1][j - 1].f = fNew;
                    nodeDetails[i - 1][j - 1].g = gNew;
                    nodeDetails[i - 1][j - 1].h = hNew;
                    nodeDetails[i - 1][j - 1].parent_i = i;
                    nodeDetails[i - 1][j - 1].parent_j = j;
                }
            }
        }
 
        //----------- 7th Successor (South-East)
        //------------
 
        // Only process this node if this is a valid one
        if (isValid(i + 1, j + 1) == true) {
            // If the destination node is the same as the
            // current successor
            if (isDestination(i + 1, j + 1, dest) == true) {
                // Set the Parent of the destination node
                nodeDetails[i + 1][j + 1].parent_i = i;
                nodeDetails[i + 1][j + 1].parent_j = j;
                printf("The destination node is found\n");
                tracePath(nodeDetails, dest);
                foundDest = true;
                return;
            }
 
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i + 1][j + 1] == false
                     && isUnblocked(grid, i + 1, j + 1)
                            == true) {
                gNew = nodeDetails[i][j].g + 1.414;
                hNew = hGet(i + 1, j + 1, dest);
                fNew = gNew + hNew;
 
                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square node
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (nodeDetails[i + 1][j + 1].f == FLT_MAX
                    || nodeDetails[i + 1][j + 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j + 1)));
 
                    // Update the details of this node
                    nodeDetails[i + 1][j + 1].f = fNew;
                    nodeDetails[i + 1][j + 1].g = gNew;
                    nodeDetails[i + 1][j + 1].h = hNew;
                    nodeDetails[i + 1][j + 1].parent_i = i;
                    nodeDetails[i + 1][j + 1].parent_j = j;
                }
            }
        }
 
        //----------- 8th Successor (South-West)
        //------------
 
        // Only process this node if this is a valid one
        if (isValid(i + 1, j - 1) == true) {
            // If the destination node is the same as the
            // current successor
            if (isDestination(i + 1, j - 1, dest) == true) {
                // Set the Parent of the destination node
                nodeDetails[i + 1][j - 1].parent_i = i;
                nodeDetails[i + 1][j - 1].parent_j = j;
                printf("The destination node is found\n");
                tracePath(nodeDetails, dest);
                foundDest = true;
                return;
            }
 
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i + 1][j - 1] == false
                     && isUnblocked(grid, i + 1, j - 1)
                            == true) {
                gNew = nodeDetails[i][j].g + 1.414;
                hNew = hGet(i + 1, j - 1, dest);
                fNew = gNew + hNew;
 
                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square node
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (nodeDetails[i + 1][j - 1].f == FLT_MAX
                    || nodeDetails[i + 1][j - 1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i + 1, j - 1)));
 
                    // Update the details of this node
                    nodeDetails[i + 1][j - 1].f = fNew;
                    nodeDetails[i + 1][j - 1].g = gNew;
                    nodeDetails[i + 1][j - 1].h = hNew;
                    nodeDetails[i + 1][j - 1].parent_i = i;
                    nodeDetails[i + 1][j - 1].parent_j = j;
                }
            }
        }
    }
 
    // When the destination node is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destination node. This may happen when the
    // there is no way to destination node (due to
    // blockages)
    if (foundDest == false)
        printf("Failed to find the Destination node\n");
 
    return;
}
 
// Driver program to test above function
int main()
{
    /* Description of the Grid-
     1--> The node is not blocked
     0--> The node is blocked    */
    int grid[ROW][COLUMN]
        = { { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
            { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
            { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
            { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
            { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
            { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
            { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
            { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
            { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 } };
 
    // Source is the left-most bottom-most corner
    Pair src = make_pair(8, 0);
 
    // Destination is the left-most top-most corner
    Pair dest = make_pair(0, 0);
 
    aStar(grid, src, dest);
 
    return (0);
}