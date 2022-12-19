// Copyright Serban Bianca-Sanziana 2021

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "utils.h"

#define MAX_INPUT_LINE_SIZE 300
#define COMMAND_SIZE 100
#define TRUE 1

struct Dir;
struct File;

typedef struct Dir
{
    char *name;
    struct Dir *parent;
    linked_list_t *dir_list;
    linked_list_t *file_list;
} Dir;

typedef struct File
{
    char *name;
    struct Dir *parent;
} File;

void touch(Dir *parent, char *name)
{
    linked_list_t *list;
    // daca nu exista niciun fisier de tip file
    if (parent->file_list == NULL)
    {
        // creez un nou file
        File *file = malloc(sizeof(*file));
        DIE(file == NULL, "Allocation error");

        file->name = malloc(sizeof(char) * strlen(name) + 1);
        DIE(file->name == NULL, "Allocation error");

        strcpy(file->name, name);
        file->parent = parent;

        // creez o lista noua si adaug fisierul
        list = ll_create(sizeof(File));
        ll_add_nth_node(list, 0, file);
        parent->file_list = list;
        free(file);
        return;
    }

    // verific daca exista fisierul cu numele dorit in lista
    int ok = 0;
    list = parent->file_list;
    ll_node_t *node = list->head;

    while (node != NULL)
    {
        File *file = node->data;
        if (strcmp(name, file->name) == 0)
        {
            ok = 1;
        }
        node = node->next;
    }

    if (ok == 1)
    {
        printf("File already exists\n");
        return;
    }

    // daca nu exista il adaug la finalul listei de fisiere
    File *file = malloc(sizeof(*file));
    DIE(file == NULL, "Allocation error");

    file->name = malloc(sizeof(char) * strlen(name) + 1);
    DIE(file->name == NULL, "Allocation error");

    strcpy(file->name, name);
    file->parent = parent;
    ll_add_nth_node(list, list->size, file);
    free(file);
}

void mkdir(Dir *parent, char *name)
{
    linked_list_t *list;
    // daca nu exista niciun director
    if (parent->dir_list == NULL)
    {
        // creez un nou director
        Dir *dir = malloc(sizeof(*dir));
        DIE(dir == NULL, "Allocation error");

        dir->name = malloc(sizeof(char) * strlen(name) + 1);
        DIE(dir->name == NULL, "Allocation error");

        strcpy(dir->name, name);
        dir->parent = parent;
        dir->file_list = NULL;

        // creez o lista noua
        list = ll_create(sizeof(Dir));
        ll_add_nth_node(list, 0, dir);
        parent->dir_list = list;

        free(dir);

        return;
    }

    // verific daca exista directorul cu numele dorit in lista
    int ok = 0;
    list = parent->dir_list;
    ll_node_t *node = list->head;

    while (node != NULL)
    {
        Dir *dir = node->data;
        if (strcmp(name, dir->name) == 0)
        {
            ok = 1;
        }
        node = node->next;
    }

    if (ok == 1)
    {
        printf("Directory already exists\n");
        return;
    }

    // daca nu exista il adaug la finalul listei de directoare
    Dir *dir = malloc(sizeof(*dir));
    DIE(dir == NULL, "Allocation error");

    dir->name = malloc(sizeof(char) * strlen(name) + 1);
    DIE(dir->name == NULL, "Allocation error");

    strcpy(dir->name, name);
    dir->parent = parent;
    ll_add_nth_node(list, list->size, dir);
    free(dir);
}

void ls(Dir *parent)
{
    // parcurg lista de directoare si afisez fiecare director
    if (parent->dir_list != NULL)
    {
        linked_list_t *list2 = parent->dir_list;
        ll_node_t *node2 = list2->head;

        while (node2 != NULL)
        {
            Dir *dir = node2->data;
            printf("%s\n", dir->name);
            node2 = node2->next;
        }
    }

    // parcurg lista de fisiere si afisez fiecare fisier
    if (parent->file_list != NULL)
    {
        linked_list_t *list = parent->file_list;
        ll_node_t *node = list->head;

        while (node != NULL)
        {
            File *file = node->data;
            printf("%s\n", file->name);
            node = node->next;
        }
    }
}

void rm(Dir *parent, char *name)
{
    // daca nu exista niciun fisier afisez mesajul corespunzator
    if (parent->file_list == NULL)
    {
        printf("Could not find the file\n");
        return;
    }

    int pos = 0;
    linked_list_t *list = parent->file_list;
    ll_node_t *node = list->head;

    // parcurg lista si verific daca exista un
    // fisier cu numele dorit si il sterg
    int ok = 0;
    while (node != NULL)
    {
        File *file = node->data;
        if (strcmp(file->name, name) == 0)
        {
            free(file->name);
            free(file);
            file = NULL;
            ll_node_t *node_removed = ll_remove_nth_node(list, pos);
            node_removed->data = NULL;
            free(node_removed);
            node_removed = NULL;
            ok = 1;
            return;
        }
        pos++;
        node = node->next;
    }

    // daca nu gasesc niciun fisier cu numele corespunzator
    // afisez mesajul corespunzator
    if (ok == 0)
    {
        printf("Could not find the file\n");
        return;
    }
}

// functie care sterge recursiv directorul dorit
void rmdir_rec(Dir *parent)
{
    if (parent->dir_list)
    {
        linked_list_t *list = parent->dir_list;
        ll_node_t *node = list->head;

        while (node != NULL)
        {
            // sterg intai directorul dorit si eliberez memoria
            rmdir_rec((Dir *)node->data);

            Dir *dir = (Dir *)ll_remove_nth_node(list, 0)->data;
            free(dir->name);
            free(dir);
            free(node);

            node = list->head;
        }

        ll_free(&parent->dir_list);
    }

    if (parent->file_list)
    {
        linked_list_t *list = parent->file_list;
        ll_node_t *node = list->head;

        while (node)
        {
            // sterg lista de fisiere din directorul dorit
            // si eliberez memoria
            File *file = (File *)ll_remove_nth_node(list, 0)->data;
            free(file->name);
            free(file);
            free(node);

            node = list->head;
        }

        ll_free(&parent->file_list);
    }
}

void rmdir(Dir *parent, char *name)
{
    // daca nu exista niciun director afisez mesajul corespunzator
    if (parent->dir_list == NULL)
    {
        printf("Could not find the dir\n");
        return;
    }

    int pos = 0;
    linked_list_t *list = parent->dir_list;
    ll_node_t *node = list->head;

    // parcurg lista si verific daca exista un
    // director cu numele dorit si il sterg,
    // folosindu-ma si de functia recursiva rmdir_rec
    int ok = 0;
    while (node != NULL)
    {
        Dir *dir = node->data;
        if (strcmp(dir->name, name) == 0)
        {
            rmdir_rec(dir);

            free(dir->name);
            free(dir);
            dir = NULL;
            ll_node_t *node_removed = ll_remove_nth_node(list, pos);
            node_removed->data = NULL;
            free(node_removed);
            node_removed = NULL;
            ok = 1;
            return;
        }
        pos++;
        node = node->next;
    }
    // daca nu am gasit niciun director cu numele corespunzator
    if (ok == 0)
    {
        printf("Could not find the dir\n");
        return;
    }
}

void cd(Dir **target, char *name)
{
    // schimb directorul curent in directorul parinte
    // daca dupa cd este '..'
    if (strcmp("..", name) == 0)
    {
        if ((*target)->parent == NULL)
        {
            return;
        }
        *target = (*target)->parent;
        return;
    }

    int ok = 0;
    linked_list_t *list = (*target)->dir_list;
    ll_node_t *node = list->head;

    // iterez prin lista de directoare si verific numele si
    // daca il gasesc schimb directorul curent
    while (node != NULL)
    {
        Dir *dir = node->data;
        if (strcmp(name, dir->name) == 0)
        {
            Dir *dir = node->data;
            (*target) = dir;
            ok = 1;
            break;
        }
        node = node->next;
    }

    // daca nu s-a gasit niciun director cu numele corespunzator
    if (ok == 0)
    {
        printf("No directories found!\n");
        return;
    }
}

char *pwd(Dir *target)
{
    // creez o lista noua in care voi tine numele fiecarui
    // director
    linked_list_t *list_pwd = ll_create(sizeof(char *));

    // cat timp exista un director parinte adaug fisierul
    // curent la inceputul listei
    Dir *current = target;
    while (current->parent != NULL)
    {
        ll_add_nth_node(list_pwd, 0, current->name);
        current = current->parent;
    }

    // parcurg lista si determin lungimea size-ului, adaugand 
    // de fiecare data lungimea numelui directorului
    ll_node_t *node = list_pwd->head;
    int size = 0;
    while (node != NULL)
    {
        size = size + strlen(node->data) + 1;
        node = node->next;
    }
    
    // aloc path
    char *pwd_path = malloc(sizeof(char) * (size + 1) + sizeof("/home"));
    DIE(pwd_path == NULL, "Allocation error");

    pwd_path[0] = '\0';
    strcat(pwd_path, "/");
    strcat(pwd_path, "home");

    // parcurg lista si adaug numele directorului curent
    node = list_pwd->head;
    while (node != NULL)
    {
        strcat(pwd_path, "/");
        strcat(pwd_path, node->data);
        node = node->next;
    }

    // eliberez memoria pentru lista alocata
    ll_free(&list_pwd);

    return pwd_path;
}

void stop(Dir *target)
{
    // sterg recursiv tot ce contine directorul home
    rmdir_rec(target);

    // eliberez memoria pentru nume si director
    free(target->name);
    free(target);
    target = NULL;
}

void tree(Dir *target, int level) {}

void mv(Dir *parent, char *oldname, char *newname) {}

int main()
{
    char *input = malloc(sizeof(char) * MAX_INPUT_LINE_SIZE);
    DIE(input == NULL, "Allocation error");

    // creez directorul root
    Dir *home_director = malloc(sizeof(*home_director));
    DIE(home_director == NULL, "Allocation error");

    home_director->name = malloc(sizeof(char) * 5);
    DIE(home_director->name == NULL, "Allocation error");

    strcpy(home_director->name, "home");
    home_director->parent = NULL;
    home_director->dir_list = NULL;
    home_director->file_list = NULL;

    // creez o copie care va reprezenta directorul curent
    Dir *copy_director = home_director;

    while (TRUE)
    {
        // citesc linia de input
        fgets(input, MAX_INPUT_LINE_SIZE, stdin);

        // obtin comanda si argumentul
        char command_input[COMMAND_SIZE];
        char arg[MAX_INPUT_LINE_SIZE - COMMAND_SIZE];
        sscanf(input, "%s %[^\t\n]s", command_input, arg);

        // interpretez fiecare comanda
        if (strcmp(command_input, "touch") == 0)
        {
            touch(copy_director, arg);
        }

        else if (strcmp(command_input, "mkdir") == 0)
        {
            mkdir(copy_director, arg);
        }

        else if (strcmp(command_input, "ls") == 0)
        {
            ls(copy_director);
        }

        else if (strcmp(command_input, "rm") == 0)
        {
            rm(copy_director, arg);
        }

        else if (strcmp(command_input, "rmdir") == 0)
        {
            rmdir(copy_director, arg);
        }

        else if (strcmp(command_input, "cd") == 0)
        {
            cd(&copy_director, arg);
        }

        else if (strcmp(command_input, "pwd") == 0)
        {
            char *pwd_name = pwd(copy_director);
            printf("%s\n", pwd_name);
            free(pwd_name);
        }

        else if (strcmp(command_input, "stop") == 0)
        {
            free(input);
            stop(home_director);
            break;
        }
    }
    return 0;
}
