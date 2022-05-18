#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jrb.h"
#include "dllist.h"
#include "jval.h"

int sodinh;
//Do thi JRB
typedef struct
{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);

void dropGraph(Graph graph);
//phA2
int lienthonga(Graph graph);
void printPath(Graph graph, int p[], int diemcuoi);
int ktcoduonghaykhong(Graph graph, int diemdau, int diemcuoi);
void dijkstra(Graph graph, int diemdau, int diemcuoi);
//phB2 tarjan

int DFS(Graph graph);
void explore(Graph g1, int *visited, int v, int *clock);
int sothanhphanlienthongmanh(Graph g1, Graph gr);

int min(int a, int b)
{
    if (a > b)
        return b;
    return a;
}
//Xu li file
#define MAX 10000
typedef struct tu
{
    char kitu[20];
    int maso;
    int output[1000];
} wordtext;
wordtext word[MAX];
//Ham xu li file txt
int docFile(char *filename);
int timsotua(int n);
int checkstra(char *str1, char *str2);

int timsotub(int n);
int checkstrb(char *str1, char *str2);

int findmaso(char *s, int dinh)
{
    for (int i = 0; i < dinh; i++)
    {
        if (strcmp(s, word[i].kitu) == 0)
            return word[i].maso;
    }
    return -1;
}
void idword(int *a, int *b, int dinh)
{
    char s1[10], s2[10];
    int x, y;
    do
    {
        printf("\nu: ");
        scanf("%s", s1);
        x = findmaso(s1, dinh);
        if (x == -1)
            printf("Khong ton tai word nay?\n");
    } while (x == -1);

    do
    {
        printf("v: ");
        scanf("%s", s2);
        y = findmaso(s2, dinh);
        if (y == -1)
            printf("Khong ton tai word nay?\n");
    } while (y == -1);
    *a = x;
    *b = y;
}
int main()
{ //Phan A
    printf("\nPhan A:\n");
    int ndinh = docFile("sgb-words.txt");
    printf("So dinh %d\n", ndinh);
    int ncung = timsotua(ndinh);
    printf("So cung %d\n", ncung);
    int i, n, output[1000];
    Graph g = createGraph();
    sodinh = ndinh;
    for (int i = 0; i < ndinh; i++)
    {
        addVertex(g, i, word[i].kitu);
    }
    for (int i = 0; i < ndinh; i++)
    {
        for (int j = 0; j < ndinh; j++)
        {
            if (word[i].output[j] == -1)
                break;
            addEdge(g, word[i].maso, word[i].output[j]);
            addEdge(g, word[i].output[j], word[i].maso);
        }
    }

    printf("Lien thong: %d \n", lienthonga(g));
    printf("Tim duong: ");
    int tu1, tu2;
    idword(&tu1, &tu2, ndinh);
    printf("Duong di ngan nhat: \n");
    dijkstra(g, tu1, tu2);
    dropGraph(g);
    //Phan B
    printf("\n================================\n");
    Graph g1 = createGraph();
    Graph gr = createGraph();
    printf("Phan B:\n");
    printf("So dinh %d\n", ndinh);
    ncung = timsotub(ndinh);
    printf("So cung %d\n", ncung);
    for (int i = 0; i < ndinh; i++)
    {
        addVertex(g1, i, word[i].kitu);
    }
    for (int i = 0; i < ndinh; i++)
    {
        for (int j = 0; j < ndinh; j++)
        {
            if (word[i].output[j] == -1)
                break;
            addEdge(g1, word[i].maso, word[i].output[j]);
        }
    }
    //do thi nguoc
    for (int i = 0; i < ndinh; i++)
    {
        addVertex(gr, i, word[i].kitu);
    }
    for (int i = 0; i < ndinh; i++)
    {
        for (int j = 0; j < ndinh; j++)
        {
            if (word[i].output[j] == -1)
                break;
            addEdge(gr, word[i].output[j], word[i].maso);
        }
    }
    //printf("Lt: %d \n",DFS(g1,0));
    printf("Lien thong: %d \n", sothanhphanlienthongmanh(g1, gr));
    printf("Tim duong: \n");
    idword(&tu1, &tu2, ndinh);
    printf("Duong di ngan nhat: \n");
    dijkstra(g1, tu1, tu2);
    dropGraph(g1);
    return 0;
}
//file txt
//file txt
int docFile(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error file?");
        exit(1);
    }
    int i = 0;
    while (!feof(fp))
    {
        fscanf(fp, "%s\n", word[i].kitu);
        word[i].maso = i;
        i++;
    }
    fclose(fp);
    return i;
}
int checkstra(char *str1, char *str2)
{
    int n = strlen(str1);
    int check = 0;
    for (int i = 0; i < n; i++)
    {
        if (str1[i] == str2[i])
        {
            check++;
        }
    }
    if (check == 4)
        return 1;
    return 0;
}

int timsotua(int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            word[i].output[j] = -1;
        }
    }

    int check = 0;
    for (int i = 0; i < size; i++)
    {
        int visit = 0;
        for (int j = i + 1; j < size; j++)
        {
            if (checkstra(word[i].kitu, word[j].kitu))
            {
                word[i].output[visit] = j;
                visit++;
                check++;
            }
        }
    }
    return check;
}

//Do thi
Graph createGraph()
{
    Graph g;
    g.edges = make_jrb();
    g.vertices = make_jrb();
    return g;
}

void addVertex(Graph g, int id, char *name)
{
    JRB node = jrb_find_int(g.vertices, id);
    if (node == NULL) // only add new vertex
        jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));
}

char *getVertex(Graph g, int id)
{
    JRB node = jrb_find_int(g.vertices, id);
    if (node == NULL)
        return NULL;
    else
        return jval_s(node->val);
}

void addEdge(Graph graph, int v1, int v2)
{
    JRB node, tree;
    if (!hasEdge(graph, v1, v2))
    {
        node = jrb_find_int(graph.edges, v1);
        if (node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_int(tree, v2, new_jval_i(1));
    }
}

int hasEdge(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
        return 0;
    tree = (JRB)jval_v(node->val);
    if (jrb_find_int(tree, v2) == NULL)
        return 0;
    else
        return 1;
}

int indegree(Graph graph, int v, int *output)
{
    JRB tree, node;
    int total = 0;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        if (jrb_find_int(tree, v))
        {
            output[total] = jval_i(node->key);
            total++;
        }
    }
    return total;
}

int outdegree(Graph graph, int v, int *output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges, v);
    if (node == NULL)
        return 0;
    tree = (JRB)jval_v(node->val);
    total = 0;
    jrb_traverse(node, tree)
    {
        output[total] = jval_i(node->key);
        total++;
    }
    return total;
}

void dropGraph(Graph graph)
{
    JRB node, tree;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}
//Phan A
//Tim lien thong bang BFS
int lienthonga(Graph graph)
{
    int visited[10000] = {};
    int n, output[1000], i, u, v;
    Dllist node, queue;
    JRB tmp;
    queue = new_dllist();
    int nlienthong = 0;
    jrb_traverse(tmp, graph.vertices)
    {
        u = tmp->key.i;
        if (!visited[u])
        {
            nlienthong++;
            dll_append(queue, new_jval_i(u));
            while (!dll_empty(queue))
            {
                node = dll_last(queue);
                v = jval_i(node->val);
                dll_delete_node(node);
                if (!visited[v])
                {
                    visited[v] = 1;
                    n = outdegree(graph, v, output);
                    for (i = 0; i < n; i++)
                    {
                        v = output[i];
                        if (!visited[v])
                            dll_append(queue, new_jval_i(v));
                    }
                }
            }
        }
    }
    return nlienthong;
}
void printPath(Graph graph, int p[], int diemcuoi)
{
    if (diemcuoi == -1)
        return;
    printPath(graph, p, p[diemcuoi]);
    printf("-> %s ", getVertex(graph, diemcuoi));
}
int ktcoduonghaykhong(Graph graph, int diemdau, int diemcuoi)
{
    int d[1000], p[1000], visited[10000];
    int i, u, output[1000], k, v, dlen;
    for (i = 0; i < 10000; i++)
        visited[i] = 0;
    //Khoi tao diem dau
    //Duyet
    for (i = 0; i < 10000; i++)
    {
        visited[i] = 0;
    }
    Dllist node, queue;
    //queue trong dllist
    queue = new_dllist();
    //l=enQueue(diemdau,l);
    dll_append(queue, new_jval_i(diemdau));
    while (!dll_empty(queue))
    {
        node = dll_first(queue); //lay ra phan tu dau trong queue
        u = jval_i(node->val);
        dll_delete_node(node);
        if (u == diemcuoi)
            return 1; //CO duong
        if (visited[u] == 0)
        {
            visited[u] = 1;
            k = outdegree(graph, u, output);
            for (i = 0; i < k; i++)
            {
                dll_append(queue, new_jval_i(v));
            }
        }
    }
    return 0; //Khong co duong
}
void dijkstra(Graph graph, int diemdau, int diemcuoi)
{
    int d[10000], p[10000], visited[10000];
    int i, u, output[10000], k, v, dlen;
    //Khoi tao ban dau
    for (i = 0; i < 10000; i++)
    {
        d[i] = 1000;
        p[i] = -1;
    }
    //Khoi tao diem dau
    d[diemdau] = 0;
    //Duyet
    for (i = 0; i < 10000; i++)
    {
        visited[i] = 0;
    }
    Dllist node, queue;
    //queue trong dllist
    queue = new_dllist();
    dll_append(queue, new_jval_i(diemdau));
    while (!dll_empty(queue))
    {
        node = dll_first(queue); //lay ra phan tu dau trong queue
        u = jval_i(node->val);
        dll_delete_node(node); //xoa node
        if (visited[u] == 0)
        {
            visited[u] = 1;
            k = outdegree(graph, u, output);
            for (i = 0; i < k; i++)
            {
                v = output[i];
                dll_append(queue, new_jval_i(v));
                //dlen=jval_i(temp->val);
                dlen = 1;
                //Tu cac lan sau tro di
                if (dlen + d[u] < d[v])
                {
                    d[v] = d[u] + 1;
                    p[v] = u;
                }
            }
        }
    }
    printPath(graph, p, diemcuoi);
}
//Phan B
int checkstrb(char *str1, char *str2)
{
    int n = strlen(str1);
    int check = 0;
    int visit[10];
    for (int i = 0; i < n; i++)
    {
        visit[i] = i;
    }
    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (visit[j] == -1)
                continue;
            if (str1[i] == str2[visit[j]])
            {
                check++;
                visit[j] = -1;
                break;
            }
        }
    }
    if (check == 4)
        return 1;
    return 0;
}

int timsotub(int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            word[i].output[j] = -1;
        }
    }

    int check = 0;
    for (int i = 0; i < size; i++)
    {
        int visit = 0;
        for (int j = 0; j < size; j++)
        {
            if (checkstrb(word[i].kitu, word[j].kitu) == 1 && i != j)
            {
                word[i].output[visit] = j;
                visit++;
                check++;
            }
        }
    }
    return check;
}
//PHan B
int pre[5800];
int post[5800];

void explore(Graph g, int *visited, int v, int *clock)
{
    visited[v] = 1;
    pre[v] = (*clock)++;
    JRB tmp = jrb_find_int(g.edges, v);
    JRB tmp1, tmp2;
    if (tmp != NULL)
    {
        tmp1 = (JRB)tmp->val.v;
        jrb_traverse(tmp2, tmp1)
        {
            if (!visited[tmp2->key.i])
                explore(g, visited, tmp2->key.i, clock);
        }
    }
    post[v] = (*clock)++;
}

int DFS(Graph g)
{
    JRB tmp;
    int n = 0;
    int clock = 1;
    int visited[10000] = {};
    int u;
    JRB tmp1;
    jrb_traverse(tmp, g.vertices)
    {
        u = tmp->key.i;
        if (!visited[u])
        {
            explore(g, visited, u, &clock);
        }
    }
}

int sothanhphanlienthongmanh(Graph g, Graph gr)
{
    DFS(gr);
    int output[2000], tmp[10000];
    int visited[10000] = {};
    int u, n, v;
    int count = 0;
    for (int i = 0; i < sodinh; i++)
    {
        tmp[i] = i;
    }
    for (int i = 0; i < sodinh - 1; i++)
    {
        for (int j = i + 1; j < sodinh; j++)
        {
            if (post[tmp[i]] < post[tmp[j]])
            {
                int a = tmp[i];
                tmp[i] = tmp[j];
                tmp[j] = a;
            }
        }
    }
    Dllist queue, node;
    queue = new_dllist();
    for (int i = 0; i < sodinh; i++)
    {
        if (!visited[tmp[i]])
        {
            count++;
            dll_append(queue, new_jval_i(tmp[i]));
            while (!dll_empty(queue))
            {
                node = dll_first(queue);
                u = jval_i(node->val);
                dll_delete_node(node);
                if (!visited[u])
                {

                    n = outdegree(g, u, output);
                    for (int j = 0; j < n; j++)
                    {
                        v = output[j];
                        if (!visited[v])
                        {
                            dll_append(queue, new_jval_i(v));
                        }
                        visited[u] = 1;
                    }
                }
            }
        }
    }

    return count;
}
