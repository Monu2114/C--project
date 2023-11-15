#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// The coordinates file should follow a proper syntax like

// first line should contain " W UG or w ug "  for weighted and " UW UG or uw ug "  for unweighted
// second line conatins the dimensions for the 2d Array
// from 3rd line starts the coordinates along with weights if necessory

class graph
{
public:
    string line;
    int rows, x, y, val;
    int **arr;
    int **path;

    void createGraph()
    {
        fstream inputFile("Coordinates.txt");

        getline(inputFile, line); // skipping the first line to initialize the array with second line's dimensions.
        if (getline(inputFile, line))
        {
            istringstream ss(line);
            ss >> rows;
            arr = new int *[rows];
            for (int i = 0; i < rows; i++)
            {
                arr[i] = new int[rows];
            }

            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < rows; j++)
                {
                    arr[i][j] = 0;
                }
            }
        }
        inputFile.clear();        // clearing the file to reuse if from beginning
        inputFile.seekg(0);       // pointing the pointer to the start of the file
        getline(inputFile, line); // getting the first line which consist of the nature of the graph

        // if Graph is Weighted and Directed

        if (line == "w dg" || line == "W DG")
        {

            getline(inputFile, line); // skipping the second line containing the dimensions of array
            while (getline(inputFile, line))
            {
                istringstream ss(line);
                ss >> x >> y >> val;
                arr[x][y] = val;
            }
        }
        // if Graph is Weighed and Undirected

        else if (line == "w ug" || line == "W UG")
        {
            getline(inputFile, line); // skipping the second line containing the dimensions of array
            while (getline(inputFile, line))
            {
                istringstream ss(line);
                ss >> x >> y >> val;
                arr[x][y] = val;
                arr[y][x] = val;
            }
        }

        // if Graph is Unweighted and Directed

        else if (line == "uw dg" || line == "UW DG")
        {

            getline(inputFile, line); // skipping the second line containing the dimensions of array
            while (getline(inputFile, line))
            {
                istringstream ss(line);
                ss >> x >> y;
                arr[x][y] = 1;
            }
        }

        // if Graph is Unweighted and Undirected

        else if (line == "uw ug" || line == "UW UG")
        {
            getline(inputFile, line); // skipping the second line containing the dimensions of array
            while (getline(inputFile, line))
            {
                istringstream ss(line);
                ss >> x >> y;
                arr[x][y] = 1;
                arr[y][x] = 1;
            }
        }
    }

    int SmallestUnknownVertex(int dist[], bool UnknownKnownVertices[])
    {
        int min_vertex;
        int min = INT_MAX;
        for (int i = 0; i < rows; i++)
        {
            if (dist[i] <= min && UnknownKnownVertices[i] == false)
            {
                min_vertex = i;
                min = dist[i];
            }
        }
        return min_vertex;
    }

    void dijkstra(int src, int des)
{
    int *dist;
    bool *UnknownKnownVertices;
    int count = 0;

    // creating 1d Array for distance and UnUnknownKnownVertices
    dist = new int[rows];
    UnknownKnownVertices = new bool[rows];

    // initializing
    for (int i = 0; i < rows; i++)
    {
        dist[i] = INT_MAX;
        UnknownKnownVertices[i] = false;
    }

    // creating 2d Array "path" for Path directions
    path = new int *[rows];

    // Declaring
    for (int i = 0; i < rows; i++)
    {
        path[i] = new int[rows];
    }

    // Initializing 2d Array "path" cells with "-1"
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            path[i][j] = -1;
        }
    }

    // Initialzing the src in the distance array to "0"
    dist[src] = 0;

    for (int i = 0; i < rows; i++)
    {
        int v = SmallestUnknownVertex(dist, UnknownKnownVertices); // finding the smallest unknown Vertex
        if (v == -1)                                               // if vertex is invalid
        {
            break;
        }
        else
        {
            UnknownKnownVertices[v] = true; // making the found vertex true so it won't be chosen in the future
            for (int j = 0; j < rows; j++)
            {
                if (arr[v][j] > 0 && UnknownKnownVertices[j] != true && dist[v] + arr[v][j] < dist[j])
                {
                    dist[j] = dist[v] + arr[v][j]; // updating the dist array
                    path[j][j] = v;                // updating the path 2d Array to track down the vertices
                    count++;
                }
            }
        }
    }

    printGraph(dist, src, des);
}


    void printGraph(int dist[], int src, int des)
    {
        vector<int> pathInfo;
        // cout << "Vertex \t Distance from Source" << endl;
        // for (int i = 0; i < rows; i++)
        // {
        //     cout << i << " \t\t\t\t" << dist[i] << endl;
        // }

        // cout << endl
        //      << endl
        //      << "Directions :" << endl
        //      << endl;

        // for (int i = 0; i < rows; i++)
        // {
        location(src);
        for (int j = 0; j < rows; j++)
        {
            if (path[des][j] > -1)
            {
                cout << "-->";
                int u = path[des][des];
                // if(path[u][u] > - 1) cout << path[u][u] << "-->";
                // if(path[u][u] > - 1)
                // {
                //     cout << path[u][u] << "-->";
                // }
                // else
                // {
                while (u != -1)
                {
                    if (path[u][u] == -1)
                        break;
                    // cout << path[u][u] << "-->";
                    pathInfo.push_back(path[u][u]);
                    u = path[u][u];
                }
                // }
                for (int x = pathInfo.size() - 1; x >= 0; x--)
                {
                    // cout << pathInfo[x] << "-->";
                    location(pathInfo[x]);
                    cout << "-->";
                }
                // cout << path[des][j] << "-->" << des;
                location(path[des][j]);
                cout << "-->";
                location(des);
            }
        }
        cout << endl
             << endl;
        cout << "Arrival time is " << dist[des] << " minutes" << endl;
        // }
    }

    void location(int num)
    {
        switch (num)
        {
        case 0:
            cout << " Bennett Main Gate ";
            break;
        case 1:
            cout << "Wellness Centre";
            break;
        case 2:
            cout << "M Block";
            break;
        case 3:
            cout << " D Block ";
            break;
        case 4:
            cout << "Laundry";
            break;
        case 5:
            cout << " Canteen ";
            break;
        case 6:
            cout << " B Block ";
            break;
        case 7:
            cout << " Admin Block ";
            break;
        case 8:
            cout << "Quench";
            break;
        case 9:
            cout << " Gymnasium ";
            break;
        case 10:
            cout << " Tuck Shop ";
            break;
        case 11:
            cout << "N Block";
            break;
        case 12:
            cout << " Sports Complex ";
            break;
        case 13:
            cout << "Rangeela Launge ";
            break;
        case 14:
            cout << "Swimming";
            break;
        case 15:
            cout << "H Block" ;
            break;
        default:
            break;
        }
    }
};

void printLocation()
{
    cout << "1. Bennett Main Gate" << endl;
    cout << "2.  Wellness Centre" << endl;
    cout << "3. M Block" << endl;
    cout << "4. D Block " << endl;
    cout << "5. Laundry " << endl;
    cout << "6. Canteen" << endl;
    cout << "7. B Block" << endl;
    cout << "8. Admin Block" << endl;
    cout << "9. Quench" << endl;
    cout << "10. Gymnasium" << endl;
    cout << "11. Tuck Shop" << endl;
    cout << "12.  N Block" << endl;
    cout << "13. Sports Complex" << endl;
    cout << "14. Rangeela Launge  " << endl;
    cout << "15. Swimming " << endl;
    cout << "16. H Block" << endl;
}

int main()
{
    graph g;
    g.createGraph();
    int src, des;
    cout << endl;
    printLocation();
    cout << endl;
    cout << "Select Starting Position : ";
    cin >> src;

    cout << endl;
    cout << endl;
    printLocation();
    cout << endl;
    cout << "Select Destination : ";
    cin >> des;
    cout << endl;
    g.dijkstra(src - 1, des - 1);
}