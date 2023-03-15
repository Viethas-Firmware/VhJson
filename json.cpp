#include "json.h"

char* Json::jsonParent  = (char*)malloc(MAX_SIZE * sizeof(char));
char* Json::jsonParam   = (char*)malloc(MAX_SIZE * sizeof(char));

Json::Json()    {
    // allocate memory for storage and map
    storage         = (uint8_t*)malloc(sizeof(uint8_t) * 1024);
    map             = (JsonMap*)malloc(sizeof(JsonMap) * 10);
    error           = true;
}
Json::~Json()   {   
    delete_map();
}

void Json::add(char* parent, const char* field, int value) {
    char* temp = parent;

    if(parent[0] == '\0') {
        sprintf(parent, "\"%s\":%d", field, value);
    }
    else {
        sprintf(parent, "%s,\"%s\":%d", temp, field, value);
    }
    // giải phóng vùng nhớ con trỏ
    temp = NULL;
    free(temp);
}

void Json::add(char* parent, const char* field, const char* value) {
    char* temp = parent;

    if(parent[0] == '\0') {
        sprintf(parent, "\"%s\":\"%s\"", field, value);
    }
    else {
        sprintf(parent, "%s,\"%s\":\"%s\"", temp, field, value);
    }

    temp = NULL;
    free(temp);
}

void Json::add(char* parent, const char* field, const char* value[], int size) {
    char* temp = parent;
    char* str   = (char*)malloc(2048);

    for(int i = 0; i < size; i++) {
        strcat(str, "\"");
        strcat(str, value[i]);
        strcat(str, "\"");
        if(i != (size - 1)) strcat(str, ", ");
    }

    if(parent[0] == '\0') {
        sprintf(parent, "\"%s\": [ %s ]", field, str);
    }
    else {
        sprintf(parent, "%s, \"%s\": [ %s ]", temp, field, str);
    }

    temp    = NULL;
    str     = NULL;

    free(temp);
    free(str);
}

void Json::add(char* parent, const char* field, int values[], int size) {
    char* temp  = parent;
    char* str   = (char*)malloc(1024);
    char* digit = (char*)malloc(10);

    if(!str)    return;
    if(!digit)  return;

    memset(str, 0, 1024);
    memset(digit, 0, 10);

    for(int i = 0; i < size; i++) {
        sprintf(digit, "%d", values[i]);
        strcat(str, digit);

        if(i != size -1) strcat(str, ", "); 
    }

    if(parent[0] == '\0') {
        sprintf(parent, "\"%s\":[ %s ]", field, str);
    }
    else {
        sprintf(parent, "%s, \"%s\":[%s]", temp, field, str);
    }

    temp    = NULL;
    str     = NULL;
    digit   = NULL;

    free(temp);
    free(str);
    free(digit);
}

void Json::add(char* parent, const char* field, char* multi) {
    char* temp = parent;

    sprintf(parent, "%s,\"%s\":{%s}", temp, field, multi);

    // giải phóng con trỏ
    temp = NULL;
    free(temp);
}

void Json::add(char* parent, char* child) {
    char* temp = parent;

    sprintf(parent, "%s%s", temp, child);

    temp = NULL;
    free(temp);
}

void Json::add(char* parent) {
    char* temp = parent;

    char* buff = (char*)malloc(MAX_SIZE * sizeof(char));
    sprintf(buff, "{%s}", temp);
    strcpy(parent, buff);
    // giải phóng vùng nhớ và con trỏ
    free(buff);
    temp = NULL;
    free(temp);
}

void Json::split(const char* payload) {
    size_t length = strlen(payload);
    JsonStateMachine jsm = JSON_INITIAL;

    char* key       = (char*)malloc(JSON_MAX_SIZE_KEY);
    char* str       = (char*)malloc(JSON_MAX_SIZE_DATA);
    char* num       = (char*)malloc(JSON_MAX_SIZE_NUM);
    char* t_key     = (char*)malloc(JSON_MAX_SIZE_KEY);
    char* t_data    = (char*)malloc(JSON_MAX_SIZE_DATA);
    if(!key || !str || !num || !t_key || !t_data) {
        printf("memory allocation has failed.\n");
        error = true;
        return;
    }

    int idx         = 0;
    int start       = 0;
    int end         = 0;

    clear_map();

    for(size_t i = 0; i < length; i++) {
        char chr = payload[i];
        if(jsm == JSON_INITIAL) {
            if(chr == '{') {
                start++;
                jsm = JSON_START;
            }
        }
        else if(jsm == JSON_START) {
            if(chr == '\"' || chr == '\'') {
                memset(key, 0, JSON_MAX_SIZE_KEY);
                idx = 0;
                // chuyển trạng thái
                jsm = JSON_KEY;
            }
        }
        else if(jsm == JSON_KEY) {
            if(chr == '\"' || chr == '\'') {
                memset(t_key, 0, JSON_MAX_SIZE_KEY);
                strcpy(t_key, key);
                create_map(t_key, KEY, start);
                // chuyển trạng thái 
                jsm = JSON_WAIT_KEY;
            }
            else {
                key[idx++] = chr;
            }
        }
        else if(jsm == JSON_WAIT_KEY) {
            if(chr == ':') jsm = JSON_DATA;
        }
        else if(jsm == JSON_DATA) {
            if(chr == '\"' || chr == '\'') {
                idx = 0;
                memset(str, 0, JSON_MAX_SIZE_DATA);
                // chuyển trạng thái
                jsm = JSON_STRING_DATA;
            }
            else if(chr >= 48 && chr <= 57) {
                idx = 0;
                memset(num, 0, JSON_MAX_SIZE_NUM);
                num[idx++]  = chr;
                // chuyển trạng thái
                jsm = JSON_INT_DATA;
            }
            else if(chr == '{') {
                start++;
                jsm = JSON_START;
            }
            else if(chr == '[') {
                idx = 0;
                memset(str, 0, JSON_MAX_SIZE_DATA);

                str[idx++] = chr;

                jsm = JSON_ARRAY_START;
            }
        }
        else if(jsm == JSON_STRING_DATA) {
            if(chr == '\"' || chr == '\'') {
                memset(t_data, 0, JSON_MAX_SIZE_DATA);
                strcpy(t_data, str);
                create_map(t_data, DATA, start);
                // chuyển trạng thái
                jsm = JSON_WAIT_DATA;
            }
            else {
                str[idx++]  = chr;
            }
        }
        else if(jsm == JSON_INT_DATA) {
            if(chr >= 48 && chr <= 57) {
                num[idx++] = chr;
            }
            else {
                memset(t_data, 0, JSON_MAX_SIZE_DATA);
                strcpy(t_data, num);
                create_map(t_data, DATA, start);
                // chuyen trang thai
                if(chr == ',') jsm = JSON_START;
                else if(chr == '}') {
                    start--;
                    jsm = JSON_END;
                }
                else jsm = JSON_WAIT_DATA;
            }
        }
        else if(jsm == JSON_WAIT_DATA) {
            if(chr == ',') jsm = JSON_START;
            else if(chr == '}') {
                start--;
                jsm = JSON_END;
            }
        }
        else if(jsm == JSON_ARRAY_START) {
            str[idx++] = chr;
            if(chr == ']') {
                memset(t_data, 0, JSON_MAX_SIZE_DATA);
                strcpy(t_data, str);
                create_map(t_data, DATA_ARRAY, start);
                jsm = JSON_ARRAY_END;
            }
        }
        else if(jsm == JSON_ARRAY_END) {
            if(chr == ',') jsm = JSON_START;
            else if(chr == '}') {
                start--;
                jsm = JSON_END;
            }
        }
        else if(jsm == JSON_END) { 
            if(start == 0) {/*do nothing*/}
            else jsm = JSON_START;
        }
    }
    
    error   = false;

    key     = NULL;
    free(key);
    if(str)     free(str);
    if(num)     free(num);
    
    if(t_data)  free(t_data);
    if(t_key)   free(t_key);
}

void Json::create_map(char* data, uint8_t type, uint8_t level) {
    JsonMap t_map;
    t_map.type      = type;
    t_map.level     = level;
    t_map.pos       = pointer;
    t_map.length    = strlen(data);

    map[cnt++]      = t_map;
    // copy data to storage
    memcpy(&storage[pointer], data, strlen(data));
    // update pointer
    pointer         += strlen(data) + 1;
}

void Json::clear_map(void) {
    // clear all memory in storage and map
    memset(storage, 0, sizeof(uint8_t) * 1024);
    memset(map, 0, sizeof(JsonMap) * 10);

    cnt             = 0;
    pointer         = 0;
}

void Json::delete_map(void) {
    free(storage);
    free(map);
}

void Json::get(const char* field, char*& result) {
    char* key       = (char*)malloc(JSON_MAX_SIZE_KEY);
    // nếu error bằng true
    if(error)   {
        return;
    }
    for(int i = 0; i < cnt; i+= 2) {
        JsonMap idx = map[i];
        memset(key, 0, idx.length + 1);
        memcpy(key, &storage[idx.pos], idx.length);
        // so sánh kết quả
        if(!strcmp(key, field)) {
            // nếu đúng thì lấy dữ liệu vào move vào result
            idx = map[i+1]; // data của key
            // allocate memory for result
            result = (char*)malloc(idx.length + 1);
            // clear memory zone and copy data
            memset(result, 0, idx.length + 1);
            memcpy(result, &storage[idx.pos], idx.length);

            free(key);
            return;
        }
    }
    result = NULL;
    free(key);
}

void Json::get(const char* field, int& result) {
    char* key       = (char*)malloc(JSON_MAX_SIZE_KEY);
    // nếu bị lỗi thì sẽ ko tiếp tục
    if(error)       return;
    for(int i = 0; i < cnt; i+=2) {
        JsonMap idx = map[i];
        memset(key, 0, idx.length + 1);
        memcpy(key, &storage[idx.pos], idx.length);
        // so sánh kết quả
        if(!strcmp(key, field)) {
            // nếu đúng lấy dữ liệu và move vào result
            idx = map[i+1];
            // allocate memory for result
            char* temp = (char*)malloc(idx.length + 1);
            // clear memory and copy data
            memset(temp, 0, idx.length + 1);
            memcpy(temp, &storage[idx.pos], idx.length);
            // casting temp to int
            result = atoi(temp);

            free(temp);
            free(key);

            return;
        }
    }
    result = -1;
    free(key);
}

void Json::get(const char* field, int*& result, uint16_t& length) {
    char* key       = (char*)malloc(JSON_MAX_SIZE_KEY);
    if(error)       return;

    for(int i = 0; i < cnt; i++) {
        JsonMap idx = map[i];
        if(idx.type == KEY) {
            memset(key, 0, idx.length + 1);
            memcpy(key, &storage[idx.pos], idx.length);
            // so sanh ket qua
            if(!strcmp(key, field)) {
                // copy data of key
                idx = map[i+1];
                // kiem tra neu la type = DATA_ARRAY
                if(idx.type == DATA_ARRAY) {
                    char* buf = (char*)malloc(idx.length + 1);
                    // get data in map
                    memset(buf, 0, idx.length + 1);
                    memcpy(buf, &storage[idx.pos], idx.length);
                    // split data array
                    splitarray(buf, result, length);
                    // release memory and exit function
                    free(buf);
                    free(key);
                    return;

                }
                else return;
            }
        }
    }
}

void Json::get(const char* field, char**& result, uint16_t& length) {
    char*   key   = (char*)malloc(JSON_MAX_SIZE_KEY);
    if(error)       return;

    for(int i = 0; i < cnt; i++) {
        JsonMap idx = map[i];
        if(idx.type == KEY) {
            memset(key, 0, idx.length + 1);
            memcpy(key, &storage[idx.pos], idx.length);
            // so sanh ket qua
            if(!strcmp(key, field)) {
                // copy data of key
                idx = map[i+1];
                // kiem tra neu la type = DATA_ARRAY
                if(idx.type == DATA_ARRAY) {
                    char* buf = (char*)malloc(idx.length + 1);
                    // get data in map
                    memset(buf, 0, idx.length + 1);
                    memcpy(buf, &storage[idx.pos], idx.length);
                    // split data array
                    splitarray(buf, result, length);
                    // release memory and exit function
                    free(buf);
                    free(key);
                    return;

                }
                else return;
            }
        }
    }
}

void Json::validation(void) {
    // nếu bị lỗi thì thoát hàm
    if(error)       return;

    for(int i = 0; i < cnt; i++) {
        JsonMap idx = map[i];
        JsonMap idx_map = map[i+1];

        if(idx.type == KEY) {
            char* name = (char*)malloc(idx.length + 1);
            if(name) {
                memset(name, 0, idx.length + 1);
                memcpy(name, &storage[idx.pos], idx.length);
                if(idx_map.type == KEY) {
                    printf("%s\n", name);
                }
                else {
                    if(idx.level == 1)
                        printf("%s", name);
                    else if(idx.level == 2)
                        printf("\t%s", name);
                    else if(idx.level == 3)
                        printf("\t\t%s", name);
                }
                free(name);
            }
        }
        else if(idx.type == DATA || idx.type == DATA_ARRAY) {
            char* data = (char*)malloc(idx.length + 1);
            if(data) {
                memset(data, 0, idx.length + 1);
                memcpy(data, &storage[idx.pos], idx.length);
                printf("\t\t-\t%s\n", data);
                free(data);
            }
        }
    }
}

void Json::check_map(void) {
    // nếu bị lỗi thì thoát hàm
    if(error)       return;
    for(int i = 0; i < cnt; i++) {
        JsonMap idx = map[i];
        printf("type: %d\nlevel: %d\npos: %d\nlength: %d\n***\n", idx.type, idx.level, idx.pos, idx.length);
    } 

}

void Json::splitarray(char* str, int*& result, uint16_t& length) {
    JsonArrayStateMachine fsm = JSM_ARRAY_INITIAL;
    int idx     = 0;
    int count   = 0;
    // allocate memory for temp veriable
    char* digit = (char*)malloc(JSON_MAX_SIZE_NUM);
    int* temp   = (int*)malloc(JSON_MAX_SIZE_DATA * sizeof(int));
    // when allocate failed then exit function.
    if(!temp)   return;

    for(int i = 0; i < strlen(str); i++) {
        char chr = str[i];

        if(fsm == JSM_ARRAY_INITIAL) {
            if(chr == '[')  {
                // reset variable temp
                idx = 0;
                memset(digit, 0, JSON_MAX_SIZE_NUM);
                // change state
                fsm = JSM_ARRAY_START;
            }
        }
        else if(fsm == JSM_ARRAY_START) {
            if(chr == ',') {
                // fectch data
                int t           = atoi(digit);
                temp[count++]   = t;
                // clear data in digital memmory
                memset(digit, 0, JSON_MAX_SIZE_NUM);
                idx = 0;
            }
            else if(chr == ']') {
                int t           = atoi(digit);
                temp[count++]   = t;
                // clear data in buffer
                memset(digit, 0, JSON_MAX_SIZE_NUM);
                idx = 0;
                // change state
                fsm = JSM_ARRAY_END;
            }
            else {
                digit[idx++] = chr;
            }
        }
        else if(fsm == JSM_ARRAY_END) { /*do nothing*/ }
    }
    // when result allcated memory then release memory
    if(result)  free(result);
    // allocate memory
    result  = (int*)malloc(count * sizeof(int));
    if(!result) return;
    // copy data from temp to result
    memcpy(result, temp, count * sizeof(int));
    length = count;

    // release memory
    free(digit);
    free(temp);
}

void Json::splitarray(char* str, char**& result, uint16_t& length) {
    JsonArrayStateMachine fsm   = JSM_ARRAY_INITIAL;
    // declare local variable
    int count       = 0;
    int idx         = 0;
    // local variable string for buffer
    char* __str     = (char*)malloc(JSON_MAX_SIZE_DATA * sizeof(char));
    char** __buf    = (char**)malloc(JSON_MAX_SIZE_DATA * sizeof(char*));

    if(!__str || !__buf) return;

    // browse string and get element
    for(int i = 0; i < strlen(str); i++) {
        char chr = str[i];

        if(fsm == JSM_ARRAY_INITIAL) {
            if(chr == '[') {
                // initial value for local variable
                idx     = 0;
                count   = 0; 
                // clear data
                memset(__str, 0, JSON_MAX_SIZE_DATA);
                fsm = JSM_ARRAY_START;
            }
        }
        else if(fsm == JSM_ARRAY_START) {
            if(chr == '\"') fsm = JSM_ARRAY_ELE_STRING_START;
            else if(chr == ']') fsm = JSM_ARRAY_END;
        }
        else if(fsm == JSM_ARRAY_ELE_STRING_START) {
            if(chr == '\"') fsm = JSM_ARRAY_ELE_STRING_END;
            else {
                // write data into temp
                __str[idx++] = chr;
            }
        }
        else if(fsm == JSM_ARRAY_ELE_STRING_END) {
            // closing data
            int t_idx       = count;
            // allocate memory and copy data
            __buf[t_idx]  = (char*)malloc(idx + 1);
            memset(__buf[t_idx], 0, idx);
            strcpy(__buf[t_idx], __str);
            // clear temp
            memset(__str, 0, JSON_MAX_SIZE_DATA);
            idx = 0;
            // when character is ',' change state
            if(chr == ',') fsm = JSM_ARRAY_START;
            else if(chr == ']') fsm = JSM_ARRAY_END;
            // increst local count
            count++;

        }
        else if(fsm == JSM_ARRAY_END) { /* do nothing */ }
    }

    // process result data
    result  = (char**)malloc(count * sizeof(char*));
    for(int i = 0; i < count; i++) {
        // allocate memory
        result[i]   = (char*)malloc(strlen(__buf[i]) + 1);
        // check allocate memory
        memset(result[i], 0, strlen(__buf[i]) + 1);
        // copy data 
        strcpy(result[i], __buf[i]);
    }
    length  = count;
    
    // release memory
    free(__str);
    free(__buf);
}