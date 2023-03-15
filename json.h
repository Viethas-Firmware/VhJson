#ifndef __JSON_H__
#define __JSON_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



#define MAX_SIZE            4096
#define JSON_MAX_SIZE_KEY   50
#define JSON_MAX_SIZE_DATA  256
#define JSON_MAX_SIZE_NUM   10

#define JSON_TYPE_KEY       1
#define JSON_TYPE_DATA      0

#define KEY                 0
#define DATA                1
#define DATA_ARRAY          2

enum JsonStateMachine {
    JSON_INITIAL,
    JSON_START,
    JSON_WAIT_KEY,
    JSON_KEY,
    JSON_WAIT_DATA,
    JSON_DATA,
    JSON_STRING_DATA,
    JSON_INT_DATA,
    JSON_ARRAY_START,
    JSON_ARRAY_END,
    JSON_END,
};

enum JsonArrayStateMachine {
    JSM_ARRAY_INITIAL,
    JSM_ARRAY_START,
    JSM_ARRAY_ELE_STRING_START,
    JSM_ARRAY_ELE_STRING_END,
    JSM_ARRAY_END,
};

struct JsonMap {
    uint8_t     type  : 2;
    uint8_t     level : 2;
    uint16_t    pos   : 12;
    uint16_t    length;
};

class Json {
public:
    static char *jsonParent;
    static char *jsonParam;

    Json();
    ~Json();

    void add(char* parent, const char* field, const char* value);
    void add(char* parent, const char* field, int value);

    void add(char* parent, const char* field, const char* value[], int size);
    void add(char* parent, const char* field, int values[], int size);

    void add(char* parent, const char* field, char* multi);
    void add(char* parent, char* child);
    void add(char* parent);

    void get(const char* field, char*& result);
    void get(const char* field, int& result);

    void get(const char* field, int*& result, uint16_t& length);
    void get(const char* field, char**& result, uint16_t& length);

    void validation(void);

    void check_map(void);

    void split(const char* payload);
    
private:
    uint8_t*    storage;
    JsonMap*    map;

    bool        error       = false;
    uint16_t    cnt;
    uint16_t    pointer;

    /**
     * @brief Sẽ tạo bản đổ ánh xạ cho dữ liệu
     * 
     * @param data          dữ liệu cần được tạo ánh xạ
     * @param key           nếu đó là key thì bàng 1 ngược lại 0.
     * @param level         cap bac cung tung thanh phan trong chuoi json
     */
    void create_map(char* data, uint8_t type, uint8_t level);
    /**
     * @brief xóa dữ liệu trong bản đồ ánh xạ và reset các biến
     * 
     */
    void clear_map(void);
    void delete_map(void);

    void splitarray(char* str, int*& result, uint16_t& length);
    void splitarray(char* str, char**& result, uint16_t& length);
};


#endif