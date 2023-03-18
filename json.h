#ifndef __JSON_H__
#define __JSON_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



#define JSON_MAX_SIZE       4096
#define JSON_MAX_SIZE_KEY   50
#define JSON_MAX_SIZE_DATA  1024
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

enum JsonGetChild {
    JSM_GET_CHILD_INITIAL,
    JSM_GET_CHILD_KEY,
    JSM_GET_CHILD_DATA,
    JSM_GET_CHILD_CHILD_KEY,
    JSM_GET_CHILD_END,
};

struct JsonMap {
    uint8_t     type  : 2;
    uint8_t     level : 2;
    uint16_t    pos   : 12;
    uint16_t    length;

    void print(void) {
        printf("======\n");
        printf("type:\t%d\n", type);
        printf("level:\t%d\n", level);
        printf("pos:\t%d\n", pos);
        printf("len:\t%d\n", length);
    }
};

class Json {
public:
    Json();
    ~Json();

    void setBufferSize(uint16_t size);

    Json& set(const char* field, const char* value);
    Json& set(const char* field, int value);

    Json& set(const char* field, char* values[], int size);
    Json& set(const char* field, int values[], int size);
    Json& set(const char* fiedl, Json values[], int size);

    Json& set(const char* field, Json& child);

    void get(const char* field, char*& result);
    void get(const char* field, int& result);

    void get(const char* field, int*& result, uint16_t& length);
    void get(const char* field, char**& result, uint16_t& length);

    void get(const char* field, Json& child);

    void split(const char* payload);

    void validation(void);

    void check_map(void);

    const char* c_str(void);
    
private:
    uint8_t*    __storage       = NULL;
    JsonMap*    __map           = NULL;
    char*       __json_str      = NULL;

    bool        __error         = false;

    uint16_t    __cnt           = 0;
    uint16_t    __pointer       = 0;
    uint16_t    __buffer_size   = 0;

    

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