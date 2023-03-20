#include "json.h"

Json::Json()    {
    // allocate memory for __storage and __map
    __buffer_size   = JSON_MAX_SIZE;
    __json_str      = (char*)malloc(__buffer_size * sizeof(char));
    memset(__json_str, 0, JSON_MAX_SIZE);

    __storage         = (uint8_t*)malloc(sizeof(uint8_t) * 1024);
    __map             = (JsonMap*)malloc(sizeof(JsonMap) * 10);
    __error           = true;
}
Json::~Json()   {
    if(__json_str)  free(__json_str);
    __json_str = NULL;
    delete_map();
}

void Json::setBufferSize(uint16_t size) {
    if(size != 0 && size != this->__buffer_size) {
        // re-allocate memory for json string
        this->__json_str = (char*)realloc(this->__json_str, size);
        // clear all memory
        memset(this->__json_str, 0, size);
        // update size in buffer size.
        this->__buffer_size   = size;
    }
}

uint16_t Json::getBufferSize(void) {
    return __buffer_size;
}

const char* Json::c_str(void) {
    return (const char*)this->__json_str;
}

void Json::clear(void) {
    clear_map();
    
    memset(__json_str, 0, __buffer_size);

    __error = true;
}

Json& Json::set(const char* field, const char* value) {

    if(this->__json_str[0] == '\0') {
        sprintf(this->__json_str, "{\"%s\": \"%s\"}", field, value);
    }
    else {
        // allocate memory for temp variable
        char* t     = (char*)malloc(strlen(this->__json_str) + 1);
        memset(t, 0, strlen(this->__json_str) + 1);
        memcpy(t, &this->__json_str[1], strlen(this->__json_str) - 2);

        sprintf(this->__json_str, "{%s, \"%s\": \"%s\"}", t, field, value);
        // release memory
        free(t);
    }
    // split json data 
    split(__json_str);

    return *this;
}

Json& Json::set(const char* field, int value) {

    if(this->__json_str[0] == '\0') {
        sprintf(this->__json_str, "{\"%s\": %d}", field, value);
    }
    else {
        // allocate memory for temp variable
        char* t     = (char*)malloc(strlen(this->__json_str) + 1);
        memcpy(t, &this->__json_str[1], strlen(this->__json_str) - 2);

        sprintf(this->__json_str, "{%s, \"%s\": %d}", t, field, value);
        // release memory
        free(t);
    }
    // split json data
    split(__json_str);

    return *this;
}

Json& Json::set(const char* field, float value) {
    if(this->__json_str[0] == '\0') {
        sprintf(this->__json_str, "{\"%s\": \"%f\"}", field, value);
    }
    else {
        // allocate memory for local variable
        char* t = (char*)malloc(strlen(this->__json_str) + 1);
        memcpy(t, &this->__json_str[1], strlen(this->__json_str) - 2);

        sprintf(this->__json_str, "{%s, \"%s\": \"%f\"}", t, field, value);
        // release memory
        free(t);
    }

    split(__json_str);

    return *this;
}

Json& Json::set(const char* field, char* values[], int size) {
    // allocate memory for local pointer
    char* str = (char*)malloc(JSON_MAX_SIZE);

    memset(str, 0, JSON_MAX_SIZE);

    for(int i = 0; i < size; i++) {
        strcat(str, "\"");
        strcat(str, values[i]);
        strcat(str, "\"");
        if(i != (size - 1)) strcat(str, ", ");
    }

    if(__json_str[0] == '\0') {
        sprintf(__json_str, "{\"%s\": [ %s ]}", field, str);
    }
    else {
        char* t = (char*)malloc(strlen(__json_str) + 1);
        memcpy(t, &__json_str[1], strlen(__json_str) - 2);

        sprintf(__json_str, "{%s, \"%s\": [ %s ]}", t, field, str);
        // release memory for local variable
        free(t);
    }
    // release memory for local variable
    free(str);
    str = NULL;
    // split json data
    split(__json_str);
    
    return *this;
}

Json& Json::set(const char* field, int values[], int size) {
    char* str = (char*)malloc(JSON_MAX_SIZE);
    char* digit = (char*)malloc(JSON_MAX_SIZE_NUM);

    memset(str, 0, JSON_MAX_SIZE);
    memset(digit, 0, JSON_MAX_SIZE_NUM);

    for(int i = 0; i < size; i++) {
        sprintf(digit, "%d", values[i]);
        strcat(str, digit);
        if(i != size - 1) strcat(str, ", ");
    }

    if(__json_str[0] == '\0') {
        sprintf(__json_str, "\"%s\": [ %s ]", field, str);
    }
    else {
        char* t = (char*)malloc(strlen(__json_str) + 1);
        memcpy(t, &__json_str[1], strlen(__json_str) - 2);

        sprintf(__json_str, "{%s, \"%s\": [ %s ]}", t, field, str);
        // release memory for local variable
        free(t);
    }
    // release memory for local variable
    free(str);
    str = NULL;
    // split data json
    split(__json_str);

    return *this;
}

Json& Json::set(const char* field, Json values[], int size) {
    char* str = (char*)malloc(JSON_MAX_SIZE);
    
    memset(str, 0, JSON_MAX_SIZE);

    for(int i = 0; i < size; i++) {
        strcat(str, values[i].c_str());
        if(i != (size - 1)) strcat(str, ", ");
    }

    if(__json_str[0] == '\0') {
        sprintf(__json_str, "{\"%s\": [ %s ]}", field, str);
    }
    else {
        char* t = (char*)malloc(strlen(__json_str) + 1);
        memcpy(t, &__json_str[1], strlen(__json_str) - 2);

        sprintf(__json_str, "{%s, \"%s\": [ %s ]}", t, field, str);
        // release memory for local pointer
        free(t);
    }

    //release memory for local pointer
    free(str);
    str = NULL;
    // split data json
    split(__json_str);

    return *this;
}

Json& Json::set(const char* field, Json& child) {
    if(__json_str[0] == '\0') {
        sprintf(__json_str, "{\"%s\": %s}", field, (char*)child.c_str());
    }
    else {
        char* t = (char*)malloc(strlen(__json_str) + 1);
        memcpy(t, &__json_str[1], strlen(__json_str) - 2);

        sprintf(__json_str, "{%s, \"%s\": %s}", t, field, (char*)child.c_str());
        // release memory for local variable
        free(t);
    }
    // split data json
    split(__json_str);

    return *this;
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
        __error = true;
        return;
    }

    int idx         = 0;
    int start       = 0;
    int end         = 0;

    clear_map();
    // copy data json string
    strcpy(__json_str, payload);

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
                // change state
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
    
    __error   = false;

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
    t_map.pos       = __pointer;
    t_map.length    = strlen(data);

    __map[__cnt++]      = t_map;
    // copy data to __storage
    memcpy(&__storage[__pointer], data, strlen(data));
    // update __pointer
    __pointer         += strlen(data) + 1;
}

void Json::clear_map(void) {
    // clear all memory in __storage and __map
    memset(__storage, 0, sizeof(uint8_t) * 1024);
    memset(__map, 0, sizeof(JsonMap) * 10);

    __cnt             = 0;
    __pointer         = 0;
}

void Json::delete_map(void) {
    if(__storage)   {
        free(__storage);
        __storage = NULL;
    }
    if(__map){ 
        free(__map);
        __map = NULL;
    }
}

void Json::get(const char* field, char*& result) {
    char* key       = (char*)malloc(JSON_MAX_SIZE_KEY);
    // nếu __error bằng true
    if(__error)     return;
    for(int i = 0; i < __cnt; i+= 2) {
        JsonMap idx = __map[i];
        if(idx.type == KEY) {
            memset(key, 0, idx.length + 1);
            memcpy(key, &__storage[idx.pos], idx.length);
            // so sánh kết quả
            if(!strcmp(key, field)) {
                // nếu đúng thì lấy dữ liệu vào move vào result
                idx = __map[i+1]; // data của key

                if(idx.type == DATA) {
                    // allocate memory for result
                    result = (char*)malloc(idx.length + 1);
                    // clear memory zone and copy data
                    memset(result, 0, idx.length + 1);
                    memcpy(result, &__storage[idx.pos], idx.length);

                    free(key);
                    return;
                }
            }
        }
    }
    result = NULL;
    free(key);
}

void Json::get(const char* field, int& result) {
    char* key       = (char*)malloc(JSON_MAX_SIZE_KEY);
    // nếu bị lỗi thì sẽ ko tiếp tục
    if(__error)       return;
    for(int i = 0; i < __cnt; i++) {
        JsonMap idx = __map[i];
        if(idx.type == KEY) {
            memset(key, 0, idx.length + 1);
            memcpy(key, &__storage[idx.pos], idx.length);
            // so sánh kết quả
            if(!strcmp(key, field)) {
                // nếu đúng lấy dữ liệu và move vào result
                idx = __map[i+1];

                if(idx.type == DATA) {
                    // allocate memory for result
                    char* temp = (char*)malloc(idx.length + 1);
                    // clear memory and copy data
                    memset(temp, 0, idx.length + 1);
                    memcpy(temp, &__storage[idx.pos], idx.length);
                    // casting temp to int
                    result = atoi(temp);
                    // release memory
                    free(temp);
                    free(key);

                    return;
                }
            }
        }
    }
    result = -1;
    free(key);
}

void Json::get(const char* field, float& result) {
    char* key       = (char*)malloc(JSON_MAX_SIZE_KEY);

    if(__error)       return;
    for(int i = 0; i < __cnt; i++) {
        JsonMap idx = __map[i];
        if(idx.type == KEY) {
            memset(key, 0, idx.length + 1);
            memcpy(key, &__storage[idx.pos], idx.length);
            // compare result
            if(!strcmp(key, field)) {
                idx = __map[i+1];
                // check type is DATA
                if(idx.type == DATA) {
                    char* buf = (char*)malloc(idx.length + 1);
                    // copy dat for result
                    memset(buf, 0, idx.length + 1);
                    memcpy(buf, &__storage[idx.pos], idx.length);
                    // cast const char* to float
                    result = atof(buf);
                    // release memory
                    free(buf);
                    free(key);

                    return;
                }
            }
        }
    }
    result = -1.0;
    free(key);
}

void Json::get(const char* field, int*& result, uint16_t& length) {
    char* key       = (char*)malloc(JSON_MAX_SIZE_KEY);
    if(__error)       return;

    for(int i = 0; i < __cnt; i++) {
        JsonMap idx = __map[i];
        if(idx.type == KEY) {
            memset(key, 0, idx.length + 1);
            memcpy(key, &__storage[idx.pos], idx.length);
            // so sanh ket qua
            if(!strcmp(key, field)) {
                // copy data of key
                idx = __map[i+1];
                // kiem tra neu la type = DATA_ARRAY
                if(idx.type == DATA_ARRAY) {
                    char* buf = (char*)malloc(idx.length + 1);
                    // get data in __map
                    memset(buf, 0, idx.length + 1);
                    memcpy(buf, &__storage[idx.pos], idx.length);
                    // split data array
                    splitarray(buf, result, length);
                    // release memory and exit function
                    free(buf);
                    free(key);
                    return;

                }
            }
        }
    }
    free(key);
}

void Json::get(const char* field, char**& result, uint16_t& length) {
    char*   key   = (char*)malloc(JSON_MAX_SIZE_KEY);
    if(__error)       return;

    for(int i = 0; i < __cnt; i++) {
        JsonMap idx = __map[i];
        if(idx.type == KEY) {
            memset(key, 0, idx.length + 1);
            memcpy(key, &__storage[idx.pos], idx.length);
            // so sanh ket qua
            if(!strcmp(key, field)) {
                // copy data of key
                idx = __map[i+1];
                // kiem tra neu la type = DATA_ARRAY
                if(idx.type == DATA_ARRAY) {
                    char* buf = (char*)malloc(idx.length + 1);
                    // get data in __map
                    memset(buf, 0, idx.length + 1);
                    memcpy(buf, &__storage[idx.pos], idx.length);
                    // split data array
                    splitarray(buf, result, length);
                    // release memory and exit function
                    free(buf);
                    free(key);
                    return;

                }
            }
        }
    }
    free(key);
}

void Json::get(const char* field, float*& result, uint16_t& length) {
    char* key   = (char*)malloc(JSON_MAX_SIZE_KEY);
    
    if(__error) return;
    for(int i = 0; i < __cnt; i++) {
        JsonMap idx = __map[i];
        if(idx.type == KEY) {
            memset(key, 0, idx.length + 1);
            memcpy(key, &__storage[idx.pos], idx.length);

            if(!strcmp(key, field)) {
                // copy data of key
                idx = __map[i+1];
                // check type is DATA_ARRAY
                if(idx.type == DATA_ARRAY) {
                    char* buf = (char*)malloc(idx.length + 1);
                    // get data in map
                    memset(buf, 0, idx.length + 1);
                    memcpy(buf, &__storage[idx.pos], idx.length);
                    // split data array
                    splitarray(buf, result, length);
                    // release memory and exit function
                    free(buf);
                    free(key);
                    return;
                }
            }
        }
    }
}

void Json::get(const char* field, Json& child) {
    char* key       = (char*)malloc(JSON_MAX_SIZE_KEY);

    if(__error)       return;
    for(int i = 0; i < __cnt; i++) {
        JsonMap idx = __map[i];
        if(idx.type == KEY) {
            memset(key, 0, idx.length + 1);
            memcpy(key, &__storage[idx.pos], idx.length);

            if(!strcmp(key, field)) {
                // kiem tra cac thanh phan ke tiep la level 2 thi get
                int j               = i + 1;
                idx                 = __map[j];

                JsonMap idx_next    = __map[j+1];
                // get first location
                uint16_t pos    = idx.pos;
                uint16_t length = 1;

                char*   t_key       = NULL;
                char*   t_key_child = NULL;
                char*   t_data      = NULL;
                Json    t_child;

                int     old_level   = idx.level;
                bool    is_child    = false;
                bool    has_child   = false;

                while(idx.level > 1) {

                    if(idx.type == KEY) {
                        if(idx_next.type == KEY) {
                            t_key_child = (char*)malloc(idx.length + 1);
                            memset(t_key_child, 0, idx.length + 1);
                            memcpy(t_key_child, &__storage[idx.pos], idx.length);
                        }
                        else {
                            t_key = (char*)malloc(idx.length + 1);
                            memset(t_key, 0, idx.length + 1);
                            memcpy(t_key, &__storage[idx.pos], idx.length);
                        }
                    }
                    else if(idx.type == DATA) {
                        t_data = (char*)malloc(idx.length + 1);
                        memset(t_data, 0, idx.length + 1);
                        memcpy(t_data, &__storage[idx.pos], idx.length);

                        if(is_child) {
                            t_child.set(t_key, t_data);
                        }
                        else {
                            child.set(t_key, t_data);
                        }
                    }
                    else if(idx.type == DATA_ARRAY) {
                        char** t_result  = NULL;
                        uint16_t t_length = 0;
                        t_data = (char*)malloc(idx.length + 1);
                        memset(t_data, 0, idx.length + 1);
                        memcpy(t_data, &__storage[idx.pos], idx.length);

                        splitarray(t_data, t_result, t_length);

                        if(is_child) {
                            t_child.set(t_key, t_result, t_length);
                        }
                        else {
                            child.set(t_key, t_result, t_length);
                        }
                        // release pointer 
                        for(int i = 0; i < t_length; i++)
                            free(t_result[i]);
                        free(t_result);
                    }

                    if(idx_next.level > idx.level) is_child = true;
                    else if(idx.level > idx_next.level) {
                        if(is_child) {
                            child.set(t_key_child, t_child);
                        }
                        is_child = false;
                    }
                    else {/* do nothing */}

                    // old_level   = idx.level;
                    j++;
                    idx     = __map[j];
                    if(j < __cnt) idx_next = __map[j+1];
                }

                if(is_child) {
                    child.set(t_key_child, t_child);
                }

                free(t_key);
                free(t_data);
                free(t_key_child);
            }
        }
    }

    free(key);
}

void Json::validation(void) {
    // nếu bị lỗi thì thoát hàm
    if(__error)       return;

    for(int i = 0; i < __cnt; i++) {
        JsonMap idx = __map[i];
        JsonMap idx_map = __map[i+1];

        if(idx.type == KEY) {
            char* name = (char*)malloc(idx.length + 1);
            if(name) {
                memset(name, 0, idx.length + 1);
                memcpy(name, &__storage[idx.pos], idx.length);
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
                memcpy(data, &__storage[idx.pos], idx.length);
                printf("\t\t-\t%s\n", data);
                free(data);
            }
        }
    }
}

void Json::check_map(void) {
    // nếu bị lỗi thì thoát hàm
    if(__error)       return;
    for(int i = 0; i < __cnt; i++) {
        JsonMap idx = __map[i];
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

void Json::splitarray(char* str, float*& result, uint16_t& length) {
    JsonArrayStateMachine fsm = JSM_ARRAY_INITIAL;
    int idx     = 0;
    int count   = 0;
    // allocate memory for local variable
    char* digit = (char*)malloc(JSON_MAX_SIZE_NUM);
    float* temp = (float*)malloc(JSON_MAX_SIZE_DATA * sizeof(float));
    // when allocated failed then exit function
    if(!temp)   return;

    for(int i = 0; i < strlen(str); i++) {
        char chr = str[i];

        if(fsm == JSM_ARRAY_INITIAL) {
            if(chr == '[') {
                // reset local variable
                idx = 0;
                memset(digit, 0, JSON_MAX_SIZE_NUM);
                // change state
                fsm = JSM_ARRAY_START;
            }
        }
        else if(fsm == JSM_ARRAY_START) {
            if(chr == ',') {
                float t         = atof(digit);
                temp[count++]   = t;
                // clear data in digit memory
                memset(digit, 0, JSON_MAX_SIZE_NUM);
                idx = 0;
            }
            else if(chr == ']') {
                float t         = atof(digit);
                temp[count++]   = t;
                // clear data in buffer
                memset(digit, 0, JSON_MAX_SIZE_NUM);
                idx = 0;
                // change state
                fsm = JSM_ARRAY_END;
            }
            else {
                digit[idx++]    = chr;
            }
        }
        else if(fsm == JSM_ARRAY_END)   { /*do nothing*/ }
    }

    // when result allocated memory then release memory
    if(result)  free(result);
    // allocate memory
    result      = (float*)malloc(count * sizeof(float));
    if(!result) return;

    memcpy(result, temp, count * sizeof(float));
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
            if(chr == ',') {
            // increst local count
                count++;
                fsm = JSM_ARRAY_START;
            }
            else if(chr == ']') fsm = JSM_ARRAY_END;
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

    for(int i = 0; i < count; i++)
        free(__buf[i]);
    free(__buf);
}