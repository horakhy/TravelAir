#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct edge
{
    struct node *from;
    struct node *to;
    float distance;
} Edge;

typedef struct node
{
    int airportId;
    char airportName[64];
    Edge **connections;
    int connectionsLength;
    char IATA[4];
    char name[64];
    float lat;
    float lng;
} Node;

typedef struct graph
{
    Node **nodes;
    int length;
} Graph;

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
    edge->distance = 111 * sqrt(pow(to->lat - from->lat, 2) + pow(to->lng - from->lng, 2));

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

    char from[4];
    printf("Entre com o IATA: ");
    scanf("%s", from);
    Node *airport = getNodeFromIATA(&graph, from);

    if (airport != NULL)
    {
        printf("Amount of connections: %d\n", airport->connectionsLength);
        for (int i = 0; i < airport->connectionsLength; i++)
            printf("%s: %.2f\n", airport->connections[i]->to->IATA, airport->connections[i]->distance);
    }
    else
    {
        printf("IATA Inválido!!!\n");
    }

    return 0;
}