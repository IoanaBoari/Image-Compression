/*BOARI Ioana-Ruxandra - 312CD*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tema2.h"

int main(int argc, char const *argv[]) {
    if (argc == 5) {  // rezolva cerina 1 sau 2
    FILE *fp;
    char tip[3];
    unsigned int width, height, max;
    int factor;
    factor = atoi((char *)argv[2]);  // extrage factorul primit ca parametru
    fp = fopen(argv[3], "r"); // deschide fisierul dat ca argument
    if (fp == NULL) { // verifica daca fisierul s-a deschis cu succes
        printf("Eroare la deschiderea fisierului.\n");
        exit(1);
    }
    fscanf(fp, "%s %u %u %u", tip, &width, &height, &max);
    fseek(fp, 1, SEEK_CUR);
    // citeste antetul din fisier si pozitioneaza cursorul la inceputul matricei de pixeli
    Pixel **mat = (Pixel **) malloc(height * sizeof(Pixel *));
    for (int i = 0; i < height; i++) {
        mat[i] = (Pixel *) malloc(width * sizeof(Pixel));
    }  // se aloca memorie pentru matricea de pixeli
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fread(&mat[i][j].red, sizeof(unsigned char), 1, fp);
            fread(&mat[i][j].green, sizeof(unsigned char), 1, fp);
            fread(&mat[i][j].blue, sizeof(unsigned char), 1, fp);
        }
    }  // se introduc datele din fisier in matrice
    Tree root = createTree(mat, height, factor);
    // se creeaza arborele cuaternar
    int nr_niv = 1 + maxDepth(root);  // se aduna 1 deoarece maxDepth considera root la nivelul 0
    int level = firstLeafLevel(root, 0);  // se gaseste primul nivel ce contine un nod frunza
    int maxLat = height;  // se porneste de la dimensiunea initiala a imaginii
    for (int i = 0; i < level; i++) {
        maxLat = maxLat/2;
    } // pentru fiecare nivel latura se imparte la jumatate
    if (strcmp(argv[1], "-c1") == 0) {
        FILE *out = fopen(argv[4], "wt");
        fprintf(out,"%d\n", nr_niv);
        fprintf(out, "%d\n", nrFrunze(root));
        fprintf(out, "%d\n", maxLat);
        fclose(out);
    } else if (strcmp(argv[1], "-c2") == 0) {
        FILE *out = fopen(argv[4], "wb");
        fwrite(&height, sizeof(unsigned int), 1, out);
        printLevelOrder(root, out);
        fclose(out);
    }
    fclose(fp);  // inchide fisierul
    for (int i = 0; i < height; i++) {
    free(mat[i]);
    }
    free(mat);
    // se elibereaza memoria alocata pentru matricea de pixeli
    destroyTree(root);
    // se elibereaza memoria alocata pentru quadtree
} else if (argc == 4) {
    if (strcmp(argv[1], "-d") == 0) {
        FILE *fp = fopen(argv[2], "rb");
        unsigned int size;
        if (fp == NULL) {  // verifica daca fisierul s-a deschis cu succes
        printf("Eroare la deschiderea fisierului.\n");
        exit(1);
        }
        fread(&size, sizeof(unsigned int), 1, fp);  // se citeste dimensiunea imaginii
        FILE *out = fopen(argv[3], "w");
        fprintf(out, "P6\n");  // se precizeaza in enunt
        fprintf(out, "%u %u\n", size, size);
        fprintf(out, "255\n");  // se precizeaza in enunt
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);  // se calculeaza nr de octeti din fisier
        long nr_elements = (file_size-4) / sizeof(unsigned char);  // se calculeaza numarul de elemente ce urmeaza sa fie citite
        fseek(fp, 4, SEEK_SET);
        unsigned char *read_vec = malloc(nr_elements * sizeof(unsigned char));
        fread(read_vec, sizeof(unsigned char), nr_elements, fp);  // se citesc informatiile din fisier intr-un vector
        Pixel **mat_remake = (Pixel **) malloc(size * sizeof(Pixel *));
        for (int i = 0; i < size; i++) {
            mat_remake[i] = (Pixel *) malloc(size * sizeof(Pixel));
        }  // se aloca memorie pentru matricea de pixeli ce urmeaza sa fie reconstruita
        Queue *queue_type = createQueue();  // coada ce va retine tipurile de noduri
        Queue *queue_colors = createQueue();  // coada ce va retine culorile pentru nodurile frunza
        for (int i = 0; i < nr_elements; i++) {
            if (read_vec[i] == 0) {
                enqueue(queue_type, read_vec[i]);
            } else if (read_vec[i] == 1) {
                enqueue(queue_type, read_vec[i]);
                i++;
                enqueue(queue_colors, read_vec[i]);
                i++;
                enqueue(queue_colors, read_vec[i]);
                i++;
                enqueue(queue_colors, read_vec[i]);
            }
        }
        Tree root = recreateTree(queue_type, queue_colors, size, 0, 0);
        // se creeaza arborele pentru decompresie
        fill_matrix(root, mat_remake);
        // se umple matricea de pixeli cu valorile corespunzatoare
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                fwrite(&(mat_remake[i][j].red), sizeof(unsigned char), 1, out);
                fwrite(&(mat_remake[i][j].green), sizeof(unsigned char), 1, out);
                fwrite(&(mat_remake[i][j].blue), sizeof(unsigned char), 1, out);
            }
        }  // valorile pentru pixeli se scriu in fisierul final
        for (int i = 0; i < size; i++) {
            free(mat_remake[i]);
        }
        free(mat_remake);
        // se elibereaza memoria alocata pentru matricea de pixeli
        free(read_vec);
        // se elibereaza memoria alocata pentru vectorul de citire
        destroyQueue(queue_type);
        // se elibereaza memoria alocata pentru coada queue_type
        destroyQueue(queue_colors);
        // se elibereaza memoria alocata pentru coada queue_colors
        destroyTree(root);
        // se elibereaza memoria alocata pentru quadtree
        fclose(out);
        fclose(fp);  // inchide fisierul
    }
}
    return 0;
}