#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define V_MEDIA 840                         // Velocidade media
#define BFS_NODE_LIST_START     10000       // Tamanho inicial da lista de nós a manter em memoria para procura em BFS
#define BFS_NODE_LIST_STEP      1000        // Tamanho a ser incrementado na lista caso esteja perto de estar cheio

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

Queue* createQueue();
void enqueue(Queue *q, Node* n);
Node* dequeue(Queue *q);
int inQueue(Queue *q, Node* n);

void addNode(Graph *g, Node *node)
{
    if (g->length == 0)
    {
        g->nodes = (Node **)malloc(sizeof(Node *));
    }
    else
    {
        g->nodes = (Node **)realloc(g->nodes, sizeof(Node *) * (g->length + 1));
    }
    g->nodes[g->length] = node;
    g->length++;
}

Node *getNodeAirport(Graph *g, int airportId)
{
    for (int i = 0; i < g->length; i++)
        if (g->nodes[i]->airportId == airportId)
            return g->nodes[i];
    return NULL;
}

Queue* createQueue() {
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->front = -1;
	q->items = NULL;
	q->size = 0;
	return q;
}
void enqueue(Queue *q, Node *n) {
	q->size++;
	if(q->size == 1) q->items = (Node**)malloc(sizeof(Node*)*q->size);
	else q->items = (Node**)realloc(q->items, sizeof(Node*)*q->size);
	if (q->size == 1) q->front = 0;
	q->items[q->size-1] = n;
}
Node* dequeue(Queue *q) {
	if(q->size == 0) return NULL;
	if(q->front == q->size) return NULL;
	Node* n = q->items[q->front];
    q->front++;
	return n;
}
int inQueue(Queue* q, Node* n) {
	for(int i = 0; i < q->size; i++)
		if(q->items[i] == n) return 1;
	return 0;
}

typedef struct _nodeQueue {
	Node* node; // node a ser verificado
	Queue queue; // caminho
} NodeQueue;

// void printQueue(Queue *q){
// 	printf("QUEUE [");
// 	for(int i = 0; i < q->size; i++)
// 		printf("%s, ", q->items[i]->IATA);
// 	printf("].\n");
// }

Queue copyQueue(Queue *q){
    Queue* newQ = createQueue();
    newQ->front = q->front;
    newQ->size = q->size;
    newQ->items = (Node**)malloc(sizeof(Node*)*q->size);
    memcpy(newQ->items, q->items, sizeof(Node*)*q->size);
    return *newQ;
}

void checkConnections(Graph *g, Node *from, Node *dest){
	Queue *visited = createQueue();

    int currentSize = 10000; // current size of the nodeQueue
	NodeQueue* nodeQueue = (NodeQueue*)malloc(sizeof(NodeQueue)*currentSize);

	int index = 0;
	int done = 0;
	int size = 0;
    float fullDistance = 0;
	nodeQueue[index].node = from;
    nodeQueue[index].queue = *(createQueue());

    enqueue(visited, from);

	while (done == 0) {
		Node* currentNode = nodeQueue[index].node;

		if(currentNode == NULL)
			break;

        // for(int i = 0; i < 500; i++){
        //     if(nodeQueue[i].queue.size == 0) continue;
        //     printf("nodeQueue[%d]:\n", i);
        //     printf("\tNode: %s", nodeQueue[i].node->IATA);
        //     printf("\t");
        //     printQueue(&(nodeQueue[i].queue));
        // }

		enqueue(&(nodeQueue[index].queue), currentNode);

		// printf("Antes Node add: %s\n", currentNode->IATA);
		// printQueue(&(nodeQueue[index].queue));
		// printf("\n");

		for(int i = 0; i < currentNode->connectionsLength; i++){
			if(inQueue(visited, currentNode->connections[i]->to) == 1){continue;}

			size++;
			nodeQueue[size].node = currentNode->connections[i]->to;
			nodeQueue[size].queue = copyQueue(&(nodeQueue[index].queue));

			// printf("Current Node: %s\n", currentNode->IATA);
			// printf("Node add: %s\n", nodeQueue[size].node->IATA);
			// printQueue(&(nodeQueue[size].queue));
			// printf("\n");

			enqueue(visited, currentNode->connections[i]->to);

			if(currentNode->connections[i]->to == dest){
				done = 1;
				break;
			}

            if(size+1 == currentSize){
                currentSize += 1000;
                nodeQueue = (NodeQueue*)realloc(nodeQueue, sizeof(NodeQueue)*currentSize);
            }
		}

		index++;
	}

	if(done == 1)
	{
        int nodeQSize = nodeQueue[size].queue.size;
		for(int i = 0; i < nodeQSize; i++){
			printf("Conexao: %d/%d -> %s\n", i + 1, nodeQSize + 1, nodeQueue[size].queue.items[i]->IATA);
		}
		printf("Conexao: %d/%d -> %s\n", nodeQSize + 1, nodeQSize + 1, dest->IATA);
	}
	else printf("Nenhum caminho encontrado!\n");
}

float deg2rad(double deg)
{
    return deg * (M_PI / 180);
}

void averageFlightTime(Edge *e)
{
    e->flightTime = e->distance / V_MEDIA;
}

// Calculate the distance between airports on the surface of the Earth
double calcGeodesicLength(float lat1, float lng1, float lat2, float lng2)
{
    float R = 6371.0087;                // Radius of the earth in km
    double dLat = deg2rad(lat2 - lat1); // deg2rad below
    double dLon = deg2rad(lng2 - lng1);
    double a =
        sin(dLat / 2) * sin(dLat / 2) +
        cos(deg2rad(lat1)) * cos(deg2rad(lat2)) *
            sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double d = R * c; // Distance in km
    return d;
}

void addAirportConnection(Node *from, Node *to)
{
    if (to == NULL)
        return;

    if (from->connectionsLength == 0)
    {
        from->connections = (Edge **)malloc(sizeof(Edge *));
    }
    else
    {
        from->connections = (Edge **)realloc(from->connections, sizeof(Edge *) * (from->connectionsLength + 1));
    }
    Edge *edge = (Edge *)malloc(sizeof(Edge));
    edge->to = to;
    edge->from = from;

    // Corrigindo a distância com a curvatura da Terra
    edge->distance = calcGeodesicLength(from->lat, from->lng, to->lat, to->lng);
    averageFlightTime(edge);

    from->connections[from->connectionsLength] = edge;
    from->connectionsLength++;
}

Node *getNodeFromIATA(Graph *g, char *iata)
{
    for (int i = 0; i < g->length; i++)
        if (strcmp(g->nodes[i]->IATA, iata) == 0)
            return g->nodes[i];

    return NULL;
}

int main(void)
{
    Graph graph;
    graph.length = 0;

    FILE *fp;
    fp = fopen("airports.txt", "r");
    char buff[1024 * 4];

    while (fgets(buff, 1024 * 4, (FILE *)fp))
    {
        Node *n = (Node *)malloc(sizeof(Node));
        addNode(&graph, n);
        char delim[] = ",";
        char *ptr = strtok(buff, delim);

        int index = 0;
        while (ptr != NULL)
        {
            if (index == 0) // Airport ID
                n->airportId = atoi(ptr);
            else if (index == 1) // Airport IATA
                strcpy(n->IATA, ptr);
            else if (index == 2) // City
                strcpy(n->name, ptr);
            else if (index == 3) // Airport name
                strcpy(n->airportName, ptr);
            else if (index == 4) // Latitude
                n->lat = atof(ptr);
            else if (index == 5) // Longitude
                n->lng = atof(ptr);
            n->connectionsLength = 0;
            n->connections = NULL;

            ptr = strtok(NULL, delim);
            index++;
        }
    }
    rewind((FILE *)fp);

    while (fgets(buff, 1024 * 4, (FILE *)fp))
    {
        char delim[] = ",";
        char *ptr = strtok(buff, delim);
        int index = 0;
        int airportId = -1;
        Node *from;
        while (ptr != NULL)
        {
            if (index == 0) // airport id index
            {
                airportId = atoi(ptr);
                from = getNodeAirport(&graph, airportId);
            }
            else if (index >= 6) // connections start index
            {
                int connectionId = atoi(ptr);
                Node *to = getNodeAirport(&graph, connectionId);
                addAirportConnection(from, to);
            }
            ptr = strtok(NULL, delim);
            index++;
        }
    }

    fclose(fp);

    int option;
    printf("\nESCOLHA SUA OPÇÃO: \n");
    printf("1.Verificar as conexões de um aeroporto \n");
    printf("2.Verificar as conexões entre dois aeroportos: \n");
    scanf("%d", &option);

    char from[4];
    char to[4];
    int distanceBtwAirp;
    int numOfConnections;
    char connections[50];

    switch (option){
        case 1:
            printf("Entre com o IATA: \n");
            scanf("%s", from);
            Node *airport = getNodeFromIATA(&graph, from);

            if (airport != NULL){   // NULL pointer verification
                printf("Quantidade de conexoes: %d\n", airport->connectionsLength);
                for (int i = 0; i < airport->connectionsLength; i++)
                printf("\n%s: \n     Distancia:\t   %.2f km\n     Tempo de voo:  %.2f horas\n", airport->connections[i]->to->IATA, airport->connections[i]->distance,
                 airport->connections[i]->flightTime);
            }
            else{
                printf("IATA Inválido!!!\n");
            }
            break;
        case 2:

            printf("Entre com o IATA inicial: \n");
            scanf("%s", from);

            printf("Entre com o IATA destino: \n");
            scanf("%s", to);

            Node *airpFrom = getNodeFromIATA(&graph, from);
            Node *airpTo = getNodeFromIATA(&graph, to);

            if (airpFrom == NULL){ // NULL pointer verification
                printf("\n%s não é um IATA válido!!!\n", from);
            }
            if(airpTo == NULL){
                printf("%s não é um IATA válido!!!\n", to);
            }
            if(airpFrom == airpTo && (airpFrom && airpTo != NULL)){
                printf("Os aeroportos inicial e destino devem ser diferentes!!!\n");
            }
            else if(airpFrom && airpTo != NULL){
                checkConnections(&graph, airpFrom, airpTo);
            }
            break;
        default:
            printf("Escolha uma opção válida!!!\n\n");
            break;
    }

    //system("pause");
    return 0;
}