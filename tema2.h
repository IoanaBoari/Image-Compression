/*BOARI Ioana-Ruxandra - 312CD*/
#ifndef _TEMA_2_H_
#define _TEMA_2_H_
#include <stdlib.h>
typedef struct pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Pixel;

typedef struct zone {
    Pixel **mat_pixel;
    int x, y;
    int size;
    int red_ma;
    int green_ma;
    int blue_ma;
    int mean;
} Zone;  // aceasta structura reprezinta o zona ce va fi reprezentata intr-un nod din quadtree

typedef struct node {
    Zone *zone;
    unsigned char type;  // este 1 pt frunze si 0 pt noduri interne
    struct node *child_1;
    struct node *child_2;
    struct node *child_3;
    struct node *child_4;
} TreeNode, *Tree;

typedef struct QueueNode {
    unsigned char val;
    struct QueueNode *next;
} QueueNode;  // strctura pentru elementele cozii

typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
    long size;
} Queue;  // structura pentru coada

void calculate_mean(Zone *zone) {
    unsigned long long red = 0;
    unsigned long long green = 0;
    unsigned long long blue = 0;
    unsigned long long mean = 0;
    for (int  i = zone->x; i < (zone->x + zone->size); i++) {
        for (int j = zone->y; j < (zone->y + zone->size); j++) {
            red = (red + zone->mat_pixel[i][j].red);
            green = (green + zone->mat_pixel[i][j].green);
            blue = (blue + zone->mat_pixel[i][j].blue);
        }
    }
    red = (red/(zone->size * zone->size));
    zone->red_ma =  (int)red;
    green = (green/(zone->size * zone->size));
    zone->green_ma = (int)green;
    blue = (blue/(zone->size * zone->size));
    zone->blue_ma =  (int)blue;
    for (int  i = zone->x; i < (zone->x + zone->size); i++) {
        for (int j = zone->y; j < (zone->y + zone->size); j++) {
            mean = (mean + (red- zone->mat_pixel[i][j].red)*(red - zone->mat_pixel[i][j].red) + (green- zone->mat_pixel[i][j].green)*(green - zone->mat_pixel[i][j].green) + (blue - zone->mat_pixel[i][j].blue)*(blue - zone->mat_pixel[i][j].blue));

        }
    }
    mean = (mean/(3*zone->size*zone->size));
    zone->mean = (int)mean;
}  // functia calculeaza culorile medii si mean pentru o zona primita ca parametru

int divizare(Zone zone, int factor) {
    if (zone.mean <= factor) {
        return 0;
    } else {
        return 1;
    }
}  // functia stabileste daca o zona ar mai trebui divizata sau nu

TreeNode *createNode_1(Zone zone) {
    TreeNode *node = malloc(sizeof(struct node));
    node->zone = malloc(sizeof(Zone));
    node->zone->size = zone.size;
    node->zone->x = zone.x;
    node->zone->y = zone.y;
    node->zone->mat_pixel = zone.mat_pixel;
    node->zone->red_ma = zone.red_ma;
    node->zone->green_ma = zone.green_ma;
    node->zone->blue_ma = zone.blue_ma;
    node->zone->mean = zone.mean;
    node->type = 1;
    node->child_1 = node->child_2 = node->child_3 = node->child_4 = NULL;
    return node;
}  // creeaza prin alocare dinamica un nod de tip frunza pornind de la o zona

TreeNode *createNode_0(Zone zone, int factor) {
    // nodurile interne: impart zona in 4 si initializeaza nodurile copil
    TreeNode *node = malloc(sizeof(TreeNode));
    node->zone = NULL;
    node->type = 0;
    // zona se imparte in 4 subzone: zone1, zone2, zone3, zone4
    // in functie de mean ul zonei si factorul impus se stabileste daca zona mai trebuie divizata
    Zone zone1;
    zone1.size = (zone.size/2);
    zone1.x = zone.x;
    zone1.y = zone.y;
    zone1.mat_pixel = zone.mat_pixel;
    calculate_mean(&zone1);
    if (divizare(zone1, factor) == 0) {
        node->child_1 = createNode_1(zone1);
    } else {
        node->child_1 = createNode_0(zone1, factor);
    }
    Zone zone2;
    zone2.size = (zone.size/2);
    zone2.x = zone.x;
    zone2.y = (zone.y + zone2.size);;
    zone2.mat_pixel = zone.mat_pixel;
    calculate_mean(&zone2);
    if (divizare(zone2, factor) == 0) {
        node->child_2 = createNode_1(zone2);
    } else {
        node->child_2 = createNode_0(zone2, factor);
    }
    Zone zone3;
    zone3.size = (zone.size/2);
    zone3.x = (zone.x + zone3.size);
    zone3.y = (zone.y + zone3.size);
    zone3.mat_pixel = zone.mat_pixel;
    calculate_mean(&zone3);
    if (divizare(zone3, factor) == 0) {
        node->child_3 = createNode_1(zone3);
    } else {
        node->child_3 = createNode_0(zone3, factor);
    }
    Zone zone4;
    zone4.size = (zone.size/2);
    zone4.x = (zone.x + zone4.size);
    zone4.y = zone.y;
    zone4.mat_pixel = zone.mat_pixel;
    calculate_mean(&zone4);
    if (divizare(zone4, factor) == 0) {
        node->child_4 = createNode_1(zone4);
    } else {
        node->child_4 = createNode_0(zone4, factor);
    }
    return node;
}  // functia creeaza recursiv nodurile arborelui

Tree createTree(Pixel **mat, int size, int factor) {
    Tree root;
    Zone zone;
    zone.size = size;
    zone.x = 0;
    zone.y = 0;
    zone.mat_pixel = mat;
    calculate_mean(&zone);
    if (divizare(zone, factor) == 0) {
        root = createNode_1(zone);
    } else {
       root = createNode_0(zone, factor);
    }
    return root;
}  // functia creeaza arborele cuaternar 

QueueNode *createQueueNode(unsigned char val) {
    QueueNode *node = malloc(sizeof(struct QueueNode));
    node->val = val;
    node->next = NULL;
    return node;
}  // functie pentru creerea unui element al cozii //

Queue *createQueue(void) {
    Queue *queue = malloc(sizeof(struct Queue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
} // functie care creeaza o coada prin alocare dinamica //

int isQueueEmpty(Queue *queue) {
    if (queue->front == NULL && queue->rear == NULL) {
        return 1;
    } else {
        return 0;
    }
}  // functie care verifica daca o coada este sau nu goala //

void enqueue(Queue *queue, unsigned char val) {
    QueueNode *top;
    if (isQueueEmpty(queue)) {
        queue->front = queue->rear = createQueueNode(val);
        queue->size++;
        return;
    } else {
        top = createQueueNode(val);
        queue->rear->next = top;
        queue->rear = top;
        queue->size++;
        return;
    }
}  // functie care introduce un nou element in coada, respectand regula FIFO //

unsigned char front(Queue *queue) {
    if (isQueueEmpty(queue)) {
        return -1;
    } else {
        return queue->front->val;
    }
}  // functie care returneaza primul element din coada //

void dequeue(Queue *queue) {
    QueueNode *temp;
    if (isQueueEmpty(queue)) {
        return;
    }
	if (queue->front == queue->rear) {
        temp = queue->front;
        queue->front = queue->rear = NULL;
        queue->size = 0;
        free(temp);
        return;
    }
    temp = queue->front;
    queue->front = queue->front->next;
    queue->size--;
    free(temp);
    return;
}  // functie care extrage un element din coada, respectand regula FIFO //

void destroyQueue(Queue *q) {
	while (!isQueueEmpty(q)) {
		dequeue(q);
	}
	q->size = 0;
	free(q);

	return;
}  // functie care elibereaza memoria alocata pentru coada //

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

int maxDepth(Tree root) {
	if(root == NULL || (root->child_1 == NULL && root->child_2 == NULL && root->child_3 == NULL && root->child_4 == NULL)) {
		return 0;
	} else {
		int Depth_1 = maxDepth(root->child_1);
		int Depth_2 = maxDepth(root->child_2);
        int Depth_3 = maxDepth(root->child_3);
        int Depth_4 = maxDepth(root->child_4);
        return 1 + max(max(Depth_1, Depth_2), max(Depth_3, Depth_4));
	}
}  // calculeaza adancimea maxima(root se afla pe niv 0)

void printLevel(Tree root, int level, FILE *file) {
    if (root == NULL) {
        return;
    }
    if (level == 0) {
        fwrite(&root->type, sizeof(unsigned char), 1, file);
        if (root->type == 1) {
            unsigned char red = (unsigned char)root->zone->red_ma;
            fwrite(&red, sizeof(unsigned char), 1, file);
            unsigned char green = (unsigned char)root->zone->green_ma;
            fwrite(&green, sizeof(unsigned char), 1, file);
            unsigned char blue = (unsigned char)root->zone->blue_ma;
            fwrite(&blue, sizeof(unsigned char), 1, file);
        }
        return;
    }
    printLevel(root->child_1, level - 1, file);
    printLevel(root->child_2, level - 1, file);
    printLevel(root->child_3, level - 1, file);
    printLevel(root->child_4, level - 1, file);
}  // functie necesara in parcurgerea pe nivel a arborelui

void printLevelOrder(Tree root, FILE *file) {
    int i, h = maxDepth(root);
    for (i = 0; i <= h; i++) {
        printLevel(root, i, file);
    }
}  // parcurge arborele pe nivel

int nrFrunze(Tree root) {
    if (root == NULL) {
        return 0;
    } else if (root->child_1 == NULL && root->child_2 == NULL && root->child_3 == NULL && root->child_4 == NULL) {
        return 1;
    } else {
        return nrFrunze(root->child_1) + nrFrunze(root->child_2) + nrFrunze(root->child_3) + nrFrunze(root->child_4);
    }
}  // calculeaza nr de frunze din arborele dat ca parametru

int firstLeafLevel(Tree root, int level) {
    if (root == NULL) {
        return -1;
    }
    if (root->child_1 == NULL && root->child_2 == NULL && root->child_3 == NULL && root->child_4 == NULL) {
        return level;
    }
    int level_1 = firstLeafLevel(root->child_1, level + 1);
    int level_2 = firstLeafLevel(root->child_2, level + 1);
    int level_3 = firstLeafLevel(root->child_3, level + 1);
    int level_4 = firstLeafLevel(root->child_4, level + 1);
    return min(min(level_1, level_2), min(level_3, level_4));
}  // returneaza primul nivel ce contine un nod frunza

TreeNode *recreateNode_1(int size, int x, int y, unsigned char red, unsigned char green, unsigned char blue) {
    TreeNode *node = malloc(sizeof(TreeNode));
    node->type = 1;
    node->zone = malloc(sizeof(Zone));
    node->zone->size = size;
    node->zone->x = x;
    node->zone->y = y;
    node->zone->red_ma = (int)red;
    node->zone->green_ma = (int)green;
    node->zone->blue_ma = (int)blue;
    node->child_1 = node->child_2 = node->child_3 = node->child_4 = NULL;
    return node;
}  // functia creeaza un nod de tip frunza pentru arborele de decompresie

TreeNode *recreateNode_0(Queue *queue_type, Queue *queue_colors, int size, int x, int y) {
    TreeNode *node = malloc(sizeof(TreeNode));
    node->type = 0;
    node->zone = NULL;
    if(!isQueueEmpty(queue_type) && !isQueueEmpty(queue_colors)) {
        // se extrag tipurile de noduri pentru cei 4 copii din coada queue_type
        unsigned char tip_child_1 = front(queue_type);
        dequeue(queue_type);
        unsigned char tip_child_2 = front(queue_type);
        dequeue(queue_type);
        unsigned char tip_child_3 = front(queue_type);
        dequeue(queue_type);
        unsigned char tip_child_4 = front(queue_type);
        dequeue(queue_type);
        if (tip_child_1 == 0) {
            // functia se apeleaza recursiv
            node->child_1 = recreateNode_0(queue_type, queue_colors, (size/2), x, y);
        } else if (tip_child_1 == 1) {
            // se extrag culorile ce vor fi puse in nodul frunza din coada queue_colors
            unsigned char red = front(queue_colors);
            dequeue(queue_colors);
            unsigned char green = front(queue_colors);
            dequeue(queue_colors);
            unsigned char blue = front(queue_colors);
            dequeue(queue_colors);
            node->child_1 = recreateNode_1((size/2), x, y, red, green, blue);
        }
        if (tip_child_2 == 0) {
            // functia se apeleaza recursiv
            node->child_2 = recreateNode_0(queue_type, queue_colors, (size/2), x, (y + size/2));
        } else if (tip_child_2 == 1) {
            // se extrag culorile ce vor fi puse in nodul frunza din coada queue_colors
            unsigned char red = front(queue_colors);
            dequeue(queue_colors);
            unsigned char green = front(queue_colors);
            dequeue(queue_colors);
            unsigned char blue = front(queue_colors);
            dequeue(queue_colors);
            node->child_2 = recreateNode_1((size/2), x, (y + size/2), red, green, blue);
        }
        if (tip_child_3 == 0) {
            // functia se apeleaza recursiv
            node->child_3 = recreateNode_0(queue_type, queue_colors, (size/2), (x + size/2), (y + size/2));
        } else if (tip_child_3 == 1) {
            // se extrag culorile ce vor fi puse in nodul frunza din coada queue_colors
            unsigned char red = front(queue_colors);
            dequeue(queue_colors);
            unsigned char green = front(queue_colors);
            dequeue(queue_colors);
            unsigned char blue = front(queue_colors);
            dequeue(queue_colors);
            node->child_3 = recreateNode_1((size/2), (x + size/2), (y + size/2), red, green, blue);
        }
        if (tip_child_4 == 0) {
            // functia se apeleaza recursiv
            node->child_4 = recreateNode_0(queue_type, queue_colors, (size/2), (x + size/2), y);
        } else if (tip_child_4 == 1) {
            // se extrag culorile ce vor fi puse in nodul frunza din coada queue_colors
            unsigned char red = front(queue_colors);
            dequeue(queue_colors);
            unsigned char green = front(queue_colors);
            dequeue(queue_colors);
            unsigned char blue = front(queue_colors);
            dequeue(queue_colors);
            node->child_4 = recreateNode_1((size/2), (x + size/2), y, red, green, blue);
        }
    }
    return node;
}  // functia creeaza recursiv nodurile arborelui de decompresie

Tree recreateTree(Queue *queue_type, Queue *queue_colors, int size, int x, int y) {
    Tree root;
    while (!isQueueEmpty(queue_type) && !isQueueEmpty(queue_colors)) {
        unsigned char tip_nod = front(queue_type);
        dequeue(queue_type);
        if (tip_nod == 0) {
            root = recreateNode_0(queue_type, queue_colors, size, x, y);
        } else if (tip_nod == 1) {
            unsigned char red = front(queue_colors);
            dequeue(queue_colors);
            unsigned char green = front(queue_colors);
            dequeue(queue_colors);
            unsigned char blue = front(queue_colors);
            dequeue(queue_colors);
            root = recreateNode_1(size, x, y, red, green, blue);
        }
    }
    return root;
}  // functia creeaza arborele cuaternar pentru decompresie

void fill_matrix(Tree root, Pixel **mat) {
    if (root == NULL) {
        return;
    }
    if (root->type == 1) {
        int x = root->zone->x;
        int y = root->zone->y;
        int size = root->zone->size;
        for (int i = x; i < (x + size); i++) {
            for (int j = y; j < (y + size); j++) {
                mat[i][j].red = (unsigned char)(root->zone->red_ma);
                mat[i][j].green = (unsigned char)(root->zone->green_ma);
                mat[i][j].blue = (unsigned char)(root->zone->blue_ma);
            }
        }
    } else {
        fill_matrix(root->child_1, mat);
        fill_matrix(root->child_2, mat);
        fill_matrix(root->child_3, mat);
        fill_matrix(root->child_4, mat);
    }
}  // functia umple matricea primita ca parametru cu valorile pixelilor retinuti in fiecare frunza

void destroyTree(Tree root) {
    if (root == NULL) {
        return;
    }
    if (root->zone != NULL) {
        free(root->zone);
    }
    destroyTree(root->child_1);
    destroyTree(root->child_2);
    destroyTree(root->child_3);
    destroyTree(root->child_4);
   free(root);
   root = NULL;
   return;
}  // functia elibereaza memoria alocata pentru quadtree

#endif  //_TEMA_2_H_