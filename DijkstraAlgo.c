#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define V 5
struct MinHeapNode {
    int v;
    int dist;
};
struct MinHeap {
    int size;
    int capacity;
    int *pos;
    struct MinHeapNode **array;
};
struct MinHeapNode* newMinHeapNode(int v, int dist) {
    struct MinHeapNode* node = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    node->v = v;
    node->dist = dist;
    return node;
}
struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;
    if (smallest != idx) {
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}
struct MinHeapNode* extract_min(struct MinHeap* minHeap) {
    if (minHeap->size == 0)
        return NULL;
    struct MinHeapNode* root = minHeap->array[0];
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
}
void decrease_key(struct MinHeap* minHeap, int v, int dist) {
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}
int isInMinHeap(struct MinHeap *minHeap, int v) {
    return minHeap->pos[v] < minHeap->size;
}
void build_heap(struct MinHeap* minHeap, int dist[]) {
    for (int v = 0; v < V; ++v) {
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }
    minHeap->size = V;
    for (int i = (minHeap->size - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}
void dijkstra(int graph[V][V], int src) {
    int dist[V];
    struct MinHeap* minHeap = createMinHeap(V);
    for (int v = 0; v < V; ++v)
        dist[v] = INT_MAX;
    dist[src] = 0;
    build_heap(minHeap, dist);
    while (minHeap->size != 0) {
        struct MinHeapNode* minHeapNode = extract_min(minHeap);
        int u = minHeapNode->v;
        for (int v = 0; v < V; ++v) {
            if (graph[u][v] && isInMinHeap(minHeap, v) && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                decrease_key(minHeap, v, dist[v]);
            }
        }
    }
    printf("Vertex\tDistance from Source %d\n", src);
    for (int i = 0; i < V; ++i)
        printf("%d\t%d\n", i, dist[i]);
}
int main() {
    int graph[V][V] = {
        {0, 9, 6, 0, 0},
        {9, 0, 11, 20, 0},
        {6, 11, 0, 5, 1},
        {0, 20, 5, 0, 2},
        {0, 0, 1, 2, 0}
    };
    dijkstra(graph, 0);
    return 0;
}