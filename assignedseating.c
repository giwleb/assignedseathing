//P1 - Assigned Seating
//Ash Hutchinson

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITSIZE 10
#define MAXLEN 50
#define MAXROWS 100001

typedef struct order {
    int s_seat;
    int e_seat;
    char* name;
}   order;

typedef struct theaterrow {
    order** list_orders;
    int max_size;
    int cur_size;
} theaterrow;

//Declare functions
order* make_order(int start, int end, char* this_name);
theaterrow* make_empty_row();
int conflict (order* order1, order* order2);
int can_add_order(theaterrow* this_row, order* this_order);
int add_order(theaterrow* this_row, order* this_order);
char* get_owner(theaterrow** theater, int row, int seat_num);
char* get_row_owner(theaterrow* this_row, int seat_num);
int contains(order* myorder, int seat_no);
void free_order(order* this_order);
void free_row(theaterrow* this_row);

int main()
{
    //Declare variables that will be used
    int row, start, end, seat;
    char name[51], op[7], input[10000];

    //Allocating memory for theater
    theaterrow** theater = calloc (MAXROWS, sizeof(theaterrow*));
    for (int i = 0; i < MAXROWS; i++){
        theater[i] = make_empty_row();
    }

    //Asking for input until QUIT
    while (strcmp(op, "QUIT") != 0){
        scanf("%s", op);

        if (strcmp(op, "BUY") == 0){
            //Input row, seat start, seat end, and name of customer
            printf("BUY entered\n");
            scanf("%d%d%d%s", &row, &start, &end, name);

            printf("%d %d %d %s\n", row, start, end, name);

            //Create order
            struct order* this_order = make_order(start, end, name);

            //Run add_order to determine if the order is successful
            if (add_order(theater[row], this_order) == 1){
                //If so print SUCCESSFUL
                printf("SUCCESSFUL\n");
            }

            else {
                //If not print FAILURE and free order
                free_order(this_order);
                printf("FAILURE");
            }
        }

        else if (strcmp(op, "LOOKUP") == 0){
            printf("LOOKUP entered\n");
            scanf("%d", &seat);
            scanf("%d", &row);

            printf("Seat: %d\nRow: %d\n", seat, row);

            if(get_owner(theater, row, seat) == NULL){
                printf("No one");
            }
            else{
                printf("%s", get_owner(theater, row, seat));
            }
        }
    }

    printf("Exited\n");
    for (int i = 0; i < MAXROWS; i++)
        free_row(theater[i]);
    free(theater);


    return 0;
}

/* Returns a pointer to a dynamically allocated order storing the given
start row, end row, and the name this_name. Note: strcpy should be
used to copy the contents into the struct after its name field is
dynamically allocated.*/
order* make_order(int start, int end, char* this_name)
{
    printf("make_order entered\n");

    order* this_order = malloc(sizeof(order));

    printf("Memory allocated\n");

    printf("%d", sizeof(this_order -> name));

    strcpy(this_order -> name, this_name);
    printf("%s\n", this_order -> name);
    this_order -> s_seat = start;
    printf("%d\n", this_order -> s_seat);
    this_order -> e_seat = end;
    printf("%d\n", this_order -> e_seat);

}

/* This function will allocate the memory for one theaterrow, including
allocating its array of orders to a default maximum size of 10, and
setting its current size of 0. */
theaterrow* make_empty_row()
{
    theaterrow* this_row = malloc(sizeof(theaterrow));
    this_row->list_orders = calloc (INITSIZE, sizeof(theaterrow));
    this_row -> max_size = INITSIZE;
    this_row -> cur_size = 0;
}

/* Assuming that order1 and order2 point to orders on the same row, this
function returns 1 if the orders, conflict, meaning that they share
at least 1 seat in common, and returns 0 otherwise*/
int conflict(order* order1, order* order2)
{
    printf("conflict entered\n");
    int range1 = (order1 -> e_seat - order1 -> s_seat) + 1;
    int array1[range1];
    int range2 = (order2 -> e_seat - order2 -> s_seat) + 1;
    int array2[range2];

    for(int i = 0; i < range1; i++){
        array1[i] = order1 -> s_seat + i;
    }
    for(int j = 0; j < range2; j++){
        array2[j] = order2 -> s_seat + j;
    }

    int i = 0;
    int j = 0;

    while (i < range1 && j < range2){
        if (array1[i] == array2[j]){
            return 1;
        }
        else if (array1[i] < array2[j]){
            i++;
        }
        else{
            j++;
        }
    }
    return 0;
}

/* Returns 1 if the order pointed to by this_order can be added to the
row pointed to by this_row. Namely, 1 is returned if and only if
this_order does NOT have any seats in it that are part of any order
already on this_row. */
int can_add_order(theaterrow* this_row, order* this_order)
{
    printf("can_add_order entered\n");
    if (this_row -> cur_size == 0){
        return 1;
    }

    else if (this_row -> cur_size != 0 ){

        if (conflict(this_order, this_row -> list_orders[this_row -> cur_size]) == 0){

            return 1;
        }
    }

    return 0;
}

/* This function tries to add this_order to this_row. If successful,
the order is added and 1 is returned. Otherwise, 0 is returned and
no change is made to this_row. If the memory for this_row is full,
this function will double the maximum # of orders allocated for the
row (via realloc), before adding this_order, and adjust max_size and
cur_size of this_row. */
int add_order(theaterrow* this_row, order* this_order)
{
    printf("add_order entered\n");
    int new_max = this_row -> max_size * 2;

    if (can_add_order(this_row, this_order) == 1){
        printf("Can_add_order is true\n");

        if (realloc(this_row, MAXROWS) == NULL){
            realloc(this_row, new_max);
            this_row -> max_size = new_max;
        }

        this_row -> list_orders[this_row -> cur_size] = this_order;
        printf("Checking for error\n");
        this_row -> cur_size ++;
        printf("Checking for error\n");

        return 1;
    }
    return 0;
}

/* If seat_num seat number in row number row is occupied, return a
pointer to the owner’s name. Otherwise, return NULL. */
char* get_owner(theaterrow** theater, int row, int seat_num)
{
    printf("get_owner entered\n");
    char* owner = get_row_owner(theater[row], seat_num);

    printf("%s", owner);

    if (owner != NULL){
        return owner;
    }

    return NULL;
}

/* If seat_num in the row pointed to by this_row is occupied, return a
pointer to the string storing the owner’s name. If no one is sitting
in this seat, return NULL. */
char* get_row_owner(theaterrow* this_row, int seat_num)
{
    printf("get_row_owner entered\n");

    if (contains(this_row -> list_orders[this_row -> cur_size], seat_num) == 1){
        printf("contains works\n");
        //Returning order** from a function with incompatible return type char*
        return this_row -> list_orders[this_row -> cur_size] -> name;
    }

    return NULL;
}

/* This function returns 1 if the seat number seat_no is contained in
the range of seats pointed to by myorder, and returns 0 otherwise. */
int contains(order* myorder, int seat_no)
{
    printf("contains entered\n");
    int start = myorder -> s_seat;
    int end = myorder -> e_seat;

    printf("Start: %d\nEnd: %d\nSeat: %d\n", start, end, seat_no);

    if (start <= seat_no && seat_no <= end){
        return 1;
    }

    return 0;
}

// Frees all memory associated with this_order.
void free_order(order* this_order)
{
    free(this_order);
}

// Frees all memory associated with this_row.
void free_row(theaterrow* this_row)
{
    for (int i = 0; i < this_row->cur_size; i++){
        free_order(this_row -> list_orders[i]);
    }
    free(this_row->list_orders);
    free(this_row);
}
