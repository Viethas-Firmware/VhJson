/*
    json.h      This is a library to generate strings in JSON format.
    Author      DucVM
    Copyright   Viethas@2013
*/

#ifndef __JSON_H__
#define __JSON_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define JSON_MAX_SIZE       4096
#define JSON_MAX_SIZE_KEY   50
#define JSON_MAX_SIZE_DATA  1024
#define JSON_MAX_SIZE_NUM   20

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
    /**
     * @brief Set the Buffer Size object
     * 
     * @param size 
     */
    void setBufferSize(uint16_t size);
    /**
     * @brief Get the Buffer Size object
     * 
     * @return uint16_t buffer size
     */
    uint16_t getBufferSize(void);
    /**
     * @brief The set() method is used to set value of an integer/string/float/Json. The "field"
     *  parameter specifies the name of the field to set, and the value parameter specifies 
     *  the value to set the field to. The method returns a reference to the Json object, 
     *  so that the caller can chain multiple method calls together.
     * 
     * @param field         The name of the field to set.
     * @param value         The value to set the field to.
     */
    Json& set(const char* field, const char*    value);
    Json& set(const char* field, int            value);
    Json& set(const char* field, float          value);
    Json& set(const char* field, Json&          child);
    /**
     * @brief The set() method in the Json class is used to set the value of a field in a JSON object. 
     * The method takes three argumens:
     * 
     * @param field         The name of the field to set
     * @param values        An array of string/integer/float/Json that contain the values to set for the field.
     * @param size          The numbers of elements in the values array.
     */
    Json& set(const char* field, char*  values[], int size);
    Json& set(const char* field, int    values[], int size);
    Json& set(const char* field, float  values[], int size);
    Json& set(const char* field, Json   values[], int size);
    
    /**
     * @brief The 'get()' method retrieves the value of a field.
     * 
     * @param field         The name of the field to get.
     * @param result        The value of field type string/integer/float/Json
     */
    Json& get(const char* field, char*&  result);
    Json& get(const char* field, int&    result);
    Json& get(const char* field, float&  result);
    Json& get(const char* field, Json&   child);
    /**
     * @brief The 'get()' method retrieves the array of a field.
     * 
     * @param field         The name of the field to get.
     * @param result        The array value of field 
     * @param length        The numbers of elements in the result array.
     */
    Json& get(const char* field, int*&   result, int& length);
    Json& get(const char* field, char**& result, int& length);
    Json& get(const char* field, float*& result, int& length);
    Json& get(const char* field, Json*&  result, int& length);
    

    Json& split(const char* payload);

    void clear(void);

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
     * @brief This method has the function of creating a mapping map for the data.
     * 
     * @param data          the data need to mapping.
     * @param key           when data is key then is 0 else 1
     * @param level         is level in json format
     */
    void create_map(char* data, uint8_t type, uint8_t level);
    /**
     * @brief clear data in memory
     * 
     */
    void clear_map(void);
    void delete_map(void);

    void splitarray(char* str, int*&    result, int& length);
    void splitarray(char* str, char**&  result, int& length);
    void splitarray(char* str, float*&  result, int& length);
    void splitarray(char* str, Json*&   result, int& length);

    void validation(void);

    void check_map(void);
};


#endif