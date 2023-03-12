#include "json.h"

char* Json::jsonParent  = (char*)malloc(MAX_SIZE * sizeof(char));
char* Json::jsonParam   = (char*)malloc(MAX_SIZE * sizeof(char));

Json::Json()    {
    // allocate memory for storage and map
    storage         = (uint8_t*)malloc(sizeof(uint8_t) * 1024);
    map             = (JsonMap*)malloc(sizeof(JsonMap) * 10);
    error           = false;
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

// void Json::add(char* parent, const char* field, int values[], int size) {
//     char* temp = parent;

//     String str = "";

//     for(int i = 0; i < size; i++) {
//         str += String(values[i]);
//         if(i < (size - 1)) str += ',';
//     }

//     if(parent[0] == '\0') {
//         sprintf(parent, "\"%s\":[%s]", field, str.c_str());
//     }
//     else {
//         sprintf(parent, "%s,\"%s\":[%s]", temp, field, str.c_str());
//     }

//     temp = NULL;
//     free(temp);
// }

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
        // DGB("memory allocation has failed.")
        error = true;
        return;
    }

    int idx         = 0;

    clear_map();

    for(size_t i = 0; i < length; i++) {
        char chr = payload[i];
        if(jsm == JSON_INITIAL) {
            if(chr == '{') jsm = JSON_START;
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
                create_map(t_key);
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
        }
        else if(jsm == JSON_STRING_DATA) {
            if(chr == '\"' || chr == '\'') {
                memset(t_data, 0, JSON_MAX_SIZE_DATA);
                strcpy(t_data, str);
                create_map(t_data);
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
                create_map(t_data);
                // chuyen trang thai
                if(chr == ',') jsm = JSON_START;
                else if(chr == '}') jsm = JSON_END;
                else jsm = JSON_WAIT_DATA;
            }
        }
        else if(jsm == JSON_WAIT_DATA) {
            if(chr == ',') jsm = JSON_START;
            else if(chr == '}') jsm = JSON_END;
        }
        else if(jsm == JSON_END) { /*do nothing */}
    }

    free(str);
    free(num);
    free(key);
    free(t_data);
    free(t_key);
}

void Json::create_map(char* data) {
    JsonMap t_map;
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

void Json::validation(void) {
    // nếu bị lỗi thì thoát hàm
    if(error)       return;
    for(int i = 0; i < cnt; i+=2) {
        JsonMap idx = map[i];
        char* key = (char*)malloc(idx.length + 1);
        if(key) {
            memset(key, 0, idx.length + 1);
            memcpy(key, &storage[idx.pos], idx.length);
            // Serial.print(key);

            idx = map[i+1];
            char* data = (char*)malloc(idx.length + 1);
            if(data) {
                memset(data, 0, idx.length + 1);
                memcpy(data, &storage[idx.pos], idx.length);
                // Serial.print("\t-\t");
                // if(idx.length > 100)
                //     Serial.println(strlen(data));
                // else
                //     Serial.println(data);
                free(data);
            }
            free(key);
        }
    }
}

void Json::check_map(void) {
    // nếu bị lỗi thì thoát hàm
    if(error)       return;
    for(int i = 0; i < cnt; i++) {
        JsonMap idx = map[i];
    } 
}