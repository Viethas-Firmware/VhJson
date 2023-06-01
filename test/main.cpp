#include <stdio.h>
#include <conio.h>
#include <json.h>


int main() {
    Json doc;
    Json child[3];
    Json json;

    Json *result;
    
    int length;

    child[0].set("name", "Vu Minh Duc");
    child[0].set("id", "id_123456");
    child[1].set("name", "Huynh Ngoc Trinh");
    child[1].set("id", "id_145456");
    child[2].set("name", "Huynh Lan Anh");
    child[2].set("id", "id_135456");

    doc.set("student", child, 3);

    printf("%s\n", doc.c_str());

    json.split(doc.c_str());

    json.get("student", result, length);

    for(int i = 0; i < length; i++) {
        char *name;
        char *id;

        result[i].get("name",   name);
        result[i].get("id",     id);

        printf("name: %s\tid: %s\n", name, id);

        free(name);
        free(id);
    }


    return 0;
}