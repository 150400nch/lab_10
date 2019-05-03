#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum MenuItem {
    ADD,
    DELETE,
    EXIT
};

enum EnumCar {
    ID,
    BRANDNAME,
    MODELNAME,
    YEAR,
    MAXSPEED
};

struct Car {
    int id;
    char *brandName;
    char *modelName;
    int year;
    double maxSpeed;
};

struct LinkedCar {
    struct Car *this;
    struct LinkedCar *next;
};

struct LinkedHeadCar {
    int size;
    int maxId;
    struct LinkedCar *linkedCar;
} head;

/*For error response with Message*/
int funErrorResponse(const char *error, int errorcode) {
    printf("[ERROR] %s", error);
    return errorcode;
}

/*Get String Length*/
int funGetStringSize(char *string) {
    int i = 0;
    while (string[i] != '\0') {
        i++;
    }
    return i + 1;
}

/*Initialization String Field*/
char *funInitStringField(char *string) {
    int len = funGetStringSize(string);
    char *str = (char *) malloc(len * sizeof(char));
    strcpy(str, string);
    return str;
}

/*Increment Enum Of Car*/
void funIncEnumOfCar(enum EnumCar *enumCar) {
    *enumCar = (enum EnumCar) (*enumCar + 1);
    if ((*enumCar) == MAXSPEED + 1) {
        (*enumCar) = ID;
    }
}

/*Read Field*/
void funReadFieldFromString(struct Car *car, enum EnumCar enumCar, char *string) {
    switch (enumCar) {
        case ID: {
            (*car).id = atoi(string);
            break;
        }
        case BRANDNAME: {
            (*car).brandName = funInitStringField(string);
            break;
        }
        case MODELNAME: {
            (*car).modelName = funInitStringField(string);
            break;
        }
        case YEAR: {
            (*car).year = atoi(string);
            break;
        }
        case MAXSPEED: {
            (*car).maxSpeed = atof(string);
            break;
        }
    }
}

struct LinkedCar *funGetLastCar() {
    struct LinkedCar *car = head.linkedCar;
    while (car->next != NULL) {
        car = car->next;
    }
    return car;
}

int funSearchMaxId() {
    int maxId = 0;
    struct LinkedCar *car = head.linkedCar;

    while (car->next != NULL) {
        car = car->next;
        if (maxId < car->this->id) {
            maxId = car->this->id;
        }
    }
    return maxId;
}

void funAddCarToEnd(struct Car *car) {
    if (head.linkedCar == NULL) {
        head.linkedCar = (struct LinkedCar *) malloc(sizeof(struct LinkedCar));
        head.linkedCar->next = NULL;
        head.linkedCar->this = car;
    } else {
        struct LinkedCar *linkedCar = (struct LinkedCar *) malloc(sizeof(struct LinkedCar));
        linkedCar->next = NULL;
        linkedCar->this = car;
        funGetLastCar(head)->next = linkedCar;
    }
    head.size++;
    head.maxId = funSearchMaxId();
}

int funDeleteCarById(int id) {
    struct LinkedCar *car = head.linkedCar;
    struct LinkedCar *last = NULL;
    while (car->this->id != id && car->next != NULL) {
        last = car;
        car = car->next;
    }

    if (car->next == NULL && car->this->id != id) {
        return -1;
    } else {
        if (head.maxId == car->this->id) {
            head.maxId = 0;
        }

        if(last != NULL) {
            last->next = car->next;
            free(car->this);
        }
        else {
            car->this = car->next->this;
            car->next = car->next->next;
        }

        head.size--;
        head.maxId = funSearchMaxId();
        int id = car->this->id;
        return id;
    }
}

/*Read from CVS format file*/
void funReadFromFile(FILE *file) {
    enum EnumCar enumCar = ID;
    char scanner[1024];
    while (fgets(scanner, 1024, file)) {
        struct Car *car = (struct Car*) malloc(sizeof(struct Car));
        char *words = strtok(scanner, ";,");
        while (words != NULL) {
            funReadFieldFromString(car, enumCar, words);
            funIncEnumOfCar(&enumCar);
            words = strtok(NULL, ";,");
        }
        funAddCarToEnd(car);
    }
}

/*For New Car Add*/
void funReadNewCar() {
    char string[1024];
    struct Car* car = (struct Car*) malloc(sizeof(struct Car));
    printf("Write car ID: ");
    scanf("%d", &car->id);
    printf("Write car brand: ");
    scanf("%s", &string);
    car->brandName = funInitStringField(string);
    printf("Write car model: ");
    scanf("%s", &string);
    car->modelName = funInitStringField(string);
    printf("Write car year: ");
    scanf("%d", &car->year);
    printf("Write car speed: ");
    scanf("%lf", &car->maxSpeed);
    funAddCarToEnd(car);
}

/*Print list*/
void funPrintLinkedList() {
    printf("ID\tBRANDNAME\tMODELNAME\tYEAR\tMAXSPEED\n");

    struct LinkedCar *car = head.linkedCar;
    while (car->next != NULL) {
        printf("%d\t%s\t\t%s\t\t%d\t%.1f\n", car->this->id,
               car->this->brandName, car->this->modelName,
               car->this->year, car->this->maxSpeed);
        car = car->next;
    }
    printf("%d\t%s\t\t%s\t\t%d\t%.1f\n", car->this->id,
           car->this->brandName, car->this->modelName,
           car->this->year, car->this->maxSpeed);
}

/*Print menu*/
void funShowMenuWithList() {
    system("cls");
    printf("Size: %d\n", head.size);
    printf("MaxID: %d\n", head.maxId);
    printf("Your cars:\n");
    funPrintLinkedList();
    printf("\nMenu:\n");
    printf("1. Add new car\n");
    printf("2. Delete by ID\n");
    printf("3. Exit\n");
    printf("\nCommand: ");
}

int main() {
    FILE *inputFile = fopen("input.csv", "r");
    funReadFromFile(inputFile);
    fclose(inputFile);

    funShowMenuWithList();
    enum MenuItem menu = EXIT;
    do {
        funShowMenuWithList();
        scanf("%d", &menu);
        switch (menu) {
            case ADD + 1: {
                funReadNewCar();
                break;
            }
            case DELETE + 1: {
                int id;
                char c;
                printf("Enter ID: ");
                scanf("%d", &id);
                if(funDeleteCarById(id) == -1) {
                    funErrorResponse("Id not found\n", 101);
                    printf("PRESS ANYTHING NUMBER TO LEAVE");
                    scanf("%d", &c);
                }
                break;
            }
        }
    } while (menu != EXIT + 1);
    return 0;
}
