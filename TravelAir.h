
typedef struct edge
{
    struct node *from;
    struct node *to;
    float distance;
    float flightTime;
} Edge;

typedef struct node
{
    int airportId;
    char airportName[64];
    Edge **connections;
    int connectionsLength;
    char IATA[4];
    char name[64];
    double lat;
    double lng;
} Node;

typedef struct graph
{
    Node **nodes;
    int length;
    int *visited;
} Graph;

typedef struct queue {
	Node **items;
	int front;
	int size;
} Queue;

typedef struct _nodeQueue {
	Node* node; // node a ser verificado
	Queue queue; // caminho
} NodeQueue;

// functions for Queues
Queue* createQueue();
void enqueue(Queue *q, Node* n);
Node* dequeue(Queue *q);
int inQueue(Queue *q, Node* n);
Queue copyQueue(Queue *q);

// support functions for the calculations
float deg2rad(double deg);
void averageFlightTime(Edge *e);
double calcGeodesicLength(float lat1, float lng1, float lat2, float lng2);

// main functions to manipulate the graph
void addNode(Graph *g, Node *node);
Node *getNodeAirport(Graph *g, int airportId);
void checkConnections(Graph *g, Node *from, Node *dest);
void addAirportConnection(Node *from, Node *to);
Node *getNodeFromIATA(Graph *g, char *iata);