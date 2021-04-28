#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define V_MEDIA 840

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
} Graph;

typedef struct queue
{
    int first;
    int last;
    int size;
    int *array;
} Queue;

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

int checkConnections(Graph *g, Node *from, Node *to, int *distanceBtwAirp, int *numOfConnections, char connections[]){
    
    Node *queue = from;
    //Queue q = (Queue *)malloc(sizeof(* Queue));

    
    return 0;
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

// codigo divino
// favor nao mexer
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

    // ex. de utilidade -> calcular distancias, Verificar voos conectivos, tempo médio de voo.
    // supondo que a terra eh plana...
    //edge->distance = 111 * sqrt(pow(to->lat - from->lat, 2) + pow(to->lng - from->lng, 2));

    // Corrigindo a distância com a curvatura da Terra
    edge->distance = calcGeodesicLength(from->lat, from->lng, to->lat, to->lng);
    averageFlightTime(edge);

    from->connections[from->connectionsLength] = edge;
    from->connectionsLength++;
}

Node *getNodeFromIATA(Graph *g, char *iata)
{
    for (int i = 0; i < g->length; i++)
    {
        if (strcmp(g->nodes[i]->IATA, iata) == 0)
            return g->nodes[i];
    }

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
            if (index == 0)
                n->airportId = atoi(ptr);
            else if (index == 1)
                strcpy(n->IATA, ptr);
            else if (index == 2)
                strcpy(n->name, ptr);
            else if (index == 3)
                strcpy(n->airportName, ptr);
            else if (index == 4)
                n->lat = atof(ptr);
            else if (index == 5)
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
            if (index == 0)
            { // airport id index
                airportId = atoi(ptr);
                from = getNodeAirport(&graph, airportId);
            }
            else if (index >= 6)
            { // connections start index
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

    printf("ESCOLHA SUA OPÇÃO: \n");
    printf("1.Verificar as conexões de um aeroporto \n");
    printf("2.Verificar a distância entre dois aeroportos: \n");
    scanf("%d", &option);
    
    char from[4];
    char to[4];
    int distanceBtwAirp;
    int numOfConnections;
    char connections[50];

    switch (option){
        case 1:
            printf("Entre com o IATA: ");
            scanf("%s", from);
            Node *airport = getNodeFromIATA(&graph, from);

            if (airport != NULL){
                printf("Amount of connections: %d\n", airport->connectionsLength);
                for (int i = 0; i < airport->connectionsLength; i++)
                printf("\n%s: %.2fkm\n     %.2f hours\n", airport->connections[i]->to->IATA, airport->connections[i]->distance,
                 airport->connections[i]->flightTime);
            }
            else{
                printf("IATA Inválido!!!\n");
            }
            break;
        case 2:

            printf("Entre com o IATA inicial: ");
            scanf("%s", from);

            printf("Entre com o IATA destino: ");
            scanf("%s", to);

            Node *airpFrom = getNodeFromIATA(&graph, from);
            Node *airpTo = getNodeFromIATA(&graph, to);

            checkConnections(&graph, airpFrom, airpTo, &distanceBtwAirp, &numOfConnections, connections);

            printf("O numero de conexões é: ");
            printf("%d", numOfConnections);
            
            printf("A distancia é de: ");
            printf("%s", connections);

            printf("A distancia é de: ");
            printf("%d", distanceBtwAirp);

            break;
        default:
            printf("Escolha uma opção válida!!!\n");
            break;
    }

    system("pause");
    return 0;
}