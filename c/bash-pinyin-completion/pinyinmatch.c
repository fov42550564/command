#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "pinyin.h"
#include "utf8vector.h"
#include "linereader.h"

#define true 1
#define false 0
typedef int bool;

// #define DEBUG
#ifdef DEBUG
#define __MYLOG(x, ARGS...) fprintf (stderr, x"\n", ##ARGS)
#else
#define __MYLOG(x, ARGS...)
#endif //DEBUG

char * strcat_uppper(char *str, const char* str1) {
    char *_str = (char *)str1;
    while (*_str) {
        str[strlen(str)] = pinyin_uppercase(*_str);
        _str++;
    }
    return str;
}
int index_of(const char* str, int offset, char ch, bool is_case) {
    int index = 0;
    while(str[index] != '\0'){
        if (is_case) {
            if(index > offset && str[index] == ch){
                return index;
            }
        } else {
            if(index > offset && (str[index] == ch || pinyin_lowercase(str[index]) == ch)){
                return index;
            }
        }
        index++;
    }
    return -1;
}
bool match_item(char* parent, const char *word, bool is_case, bool is_serial) {
    if (is_case) {
        if (parent[0] != word[0]) {
            return false;
        }
    } else {
        if (pinyin_lowercase(parent[0]) != word[0]) {
            return false;
        }
    }
    int last_index = -1;
    int child_len = strlen(word);
    if (strlen(parent) < child_len) {
        return false;
    }
    for (int i = 0; i < child_len; i++) {
        char ch = word[i];
        if (is_serial) {
            if (is_case) {
                if (parent[i] != ch) {
                    return false;
                }
            } else {
                if (parent[i] != ch  && pinyin_lowercase(parent[i]) != ch) {
                    return false;
                }
            }
        } else {
            last_index = index_of(parent, last_index, ch, is_case);
            if (last_index == -1) {
                return false;
            }
        }
    }
    return true;
}
bool match(char** parents, int parent_count, const char *word, bool is_case, bool is_full, bool is_serial) {
    for (int i = 0; i < parent_count; i++) {
        if (match_item(parents[i], word, is_case, is_serial)) {
            __MYLOG("[match]: parents[%d]=%s word=%s is_case=%d is_full=%d is_serial=%d", i, parents[i], word, is_case, is_full, is_serial);
            return true;
        } else {
            __MYLOG("parents[%d]=%s word=%s is_case=%d is_full=%d is_serial=%d", i, parents[i], word, is_case, is_full, is_serial);
        }
    }
    return false;
}
void set_parents_pinyins(char **parents, int parent_count, const char **pinyins, int *count_list, int level, bool is_full) {
    int pre_count = 1;
    for (int i = 0; i < level; i++) {
        pre_count *= count_list[i];
    }
    int current_count = count_list[level];
    for (int i = 0; i < parent_count; i++) {
        if (is_full) {
            strcat_uppper(parents[i], pinyins[(i/pre_count)%current_count]);
        } else {
            parents[i][strlen(parents[i])] = pinyin_uppercase(pinyins[(i/pre_count)%current_count][0]);
        }
    }
}
void set_parents_char(char **parents, int parent_count, char ch) {
    for (int i = 0; i < parent_count; i++) {
        parents[i][strlen(parents[i])] = ch;
    }
}
bool match_line(const char *line, int line_length, const char *word, bool is_case, bool is_full, bool is_serial) {
    __MYLOG("[match_line]: line=%s word=%s is_case=%d is_full=%d is_serial=%d",line, word, is_case, is_full, is_serial);
    wchar_t line_char;
    utf8vector line_vector = utf8vector_create(line, line_length);
    int size = utf8vector_uni_count(line_vector);

    int parent_count = 1;
    int *count_list = (int *)calloc(size, sizeof(int));
    int level = 0;
    while((line_char = utf8vector_next_unichar(line_vector)) != '\0') {
        if (pinyin_ishanzi(line_char)) {
            int count = get_pinyin_count(line_char);
            parent_count *= count;
            count_list[level] = count;
            level++;
        }
    }
    utf8vector_reset(line_vector);
    char **parents = (char **)calloc(parent_count, sizeof(char *));
    for (int i = 0; i < parent_count; i++) {
        parents[i] = (char *)calloc(size * (is_full ? 6 : 1 ) + 1, sizeof(char));
    }

    level = 0;
    while((line_char = utf8vector_next_unichar(line_vector)) != '\0') {
        if (pinyin_ishanzi(line_char)) {
            const char **pinyins;
            pinyin_get_pinyins_by_unicode(line_char, &pinyins);
            set_parents_pinyins(parents, parent_count, pinyins, count_list, level, is_full);
            level++;
            free(pinyins);
        } else {
            set_parents_char(parents, parent_count, is_case ? line_char : pinyin_lowercase(line_char));
        }
    }
    bool result = match(parents, parent_count, word, is_case, is_full, is_serial);
    utf8vector_free(line_vector);
    for (int i = 0; i < parent_count; i++) {
        free(parents[i]);
    }
    free(parents);
    free(count_list);

    return result;
}
int main(int argc, char **argv) {
    char *word = argv[1];
    if (NULL == word) {
        return 1;
    }
    int word_len = strlen(word);
    int select = 0;
    if (word_len > 1 && word[word_len-1] > 48 && word[word_len-1] < 58) { // 使用数字1-9查看第几个，如果这个数字本身存在，则直接找到该选项
        select = word[word_len-1] - 48;
    }
    utf8vector word_vector = utf8vector_create(word, -1);
    int word_size = utf8vector_uni_count(word_vector);
    char *_word = (char *)calloc(word_size * 6 + 1, sizeof(char)); // 存放所有字母
    char *_sword = (char *)calloc(word_size  + 1, sizeof(char)); // 存放首字母
    char *_wordu = (char *)calloc(word_size * 6 + 1, sizeof(char)); // 存放区分大小写所有字母
    char *_swordu = (char *)calloc(word_size  + 1, sizeof(char)); // 存放区分大小写首字母
    wchar_t word_char;
    while((word_char = utf8vector_next_unichar(word_vector)) != '\0') {
        if (pinyin_ishanzi(word_char)) {
            const char **pinyins;
            pinyin_get_pinyins_by_unicode(word_char, &pinyins);
            strcat_uppper(_word, pinyins[0]);
            _sword[strlen(_sword)] = pinyin_uppercase(pinyins[0][0]);
            strcat_uppper(_wordu, pinyins[0]);
            _swordu[strlen(_swordu)] = pinyin_uppercase(pinyins[0][0]);
            free(pinyins);
        } else {
            _word[strlen(_word)] = pinyin_lowercase(word_char);
            _sword[strlen(_sword)] = pinyin_lowercase(word_char);
            _wordu[strlen(_wordu)] = word_char;
            _swordu[strlen(_swordu)] = word_char;
        }
    }

    __MYLOG("_word=%s _sword=%s _wordu=%s _swordu=%s select=%d", _word, _sword, _wordu, _swordu, select);
    int count;
    int index = 0;
    linereader reader = linereader_create(STDIN_FILENO);
    bool found = false;
    char ** lines = (char **)calloc(1024, sizeof(char *));
    // 全字区分大小写连续匹配
    while ((count = linereader_readline(reader)) != -1) {
        const char *line = reader->line_buffer == NULL ? "" : reader->line_buffer;
        if (index < 1024) {
            lines[index++] = strdup(line);
        }
        if (word_len == 0 || match_line(line, count, _wordu, true, true, true)) {
            found = true;
            printf("%s\n", line);
        }
    }
    if (found) {
        goto end;
    }
    // 全字连续匹配
    for (int i=0; i<index; i++) {
        const char *line = lines[i];
        if (match_line(line, count, _word, false, false, true)) {
            found = true;
            printf("%s\n", line);
        }
    }
    if (found) {
        goto end;
    }

    // 拼音首字母区分大小写连续匹配
    for (int i=0; i<index; i++) {
        const char *line = lines[i];
        if (match_line(line, count, _swordu, true, false, true)) {
            found = true;
            printf("%s\n", line);
        }
    }
    if (found) {
        goto end;
    }
    // 拼音首字母连续匹配
    for (int i=0; i<index; i++) {
        const char *line = lines[i];
        if (match_line(line, count, _sword, false, false, true)) {
            found = true;
            printf("%s\n", line);
        }
    }
    if (found) {
        goto end;
    }

    // 拼音首字母区分大小写跳跃匹配
    for (int i=0; i<index; i++) {
        const char *line = lines[i];
        if (match_line(line, count, _swordu, true, false, false)) {
            found = true;
            printf("%s\n", line);
        }
    }
    if (found) {
        goto end;
    }
    // 拼音首字母跳跃匹配
    for (int i=0; i<index; i++) {
        const char *line = lines[i];
        if (match_line(line, count, _sword, false, false, false)) {
            found = true;
            printf("%s\n", line);
        }
    }
    if (found) {
        goto end;
    }

    // 全字区分大小写跳跃匹配
    for (int i=0; i<index; i++) {
        const char *line = lines[i];
        if (match_line(line, count, _wordu, true, true, false)) {
            found = true;
            printf("%s\n", line);
        }
    }
    if (found) {
        goto end;
    }
    // 全字跳跃匹配
    for (int i=0; i<index; i++) {
        const char *line = lines[i];
        if (match_line(line, count, _word, false, true, false)) {
            found = true;
            printf("%s\n", line);
        }
    }
    if (found) {
        goto end;
    }

    if (select > 0) {
        // 全字区分大小写连续匹配带数字
        _word[strlen(_word)-1] = '\0';
        int show_index = 0;
        for (int i=0; i<index; i++) {
            const char *line = lines[i];
            if (match_line(line, count, _wordu, true, true, true)) {
                show_index++;
                if (show_index == select) {
                    printf("%s\n", line);
                    goto end;
                }
            }
        }
        // 全字连续匹配带数字
        _word[strlen(_word)-1] = '\0';
        show_index = 0;
        for (int i=0; i<index; i++) {
            const char *line = lines[i];
            if (match_line(line, count, _word, false, true, true)) {
                show_index++;
                if (show_index == select) {
                    printf("%s\n", line);
                    goto end;
                }
            }
        }

        // 拼音首字母区分大小写连续匹配带数字
        _sword[strlen(_sword)-1] = '\0';
        show_index = 0;
        for (int i=0; i<index; i++) {
            const char *line = lines[i];
            if (match_line(line, count, _swordu, true, false, true)) {
                show_index++;
                if (show_index == select) {
                    printf("%s\n", line);
                    goto end;
                }
            }
        }
        // 拼音首字母连续匹配带数字
        _sword[strlen(_sword)-1] = '\0';
        show_index = 0;
        for (int i=0; i<index; i++) {
            const char *line = lines[i];
            if (match_line(line, count, _sword, false, false, true)) {
                show_index++;
                if (show_index == select) {
                    printf("%s\n", line);
                    goto end;
                }
            }
        }

        // 拼音首字母区分大小写跳跃匹配带数字
        show_index = 0;
        for (int i=0; i<index; i++) {
            const char *line = lines[i];
            if (match_line(line, count, _swordu, true, false, false)) {
                show_index++;
                if (show_index == select) {
                    printf("%s\n", line);
                    goto end;
                }
            }
        }
        // 拼音首字母跳跃匹配带数字
        show_index = 0;
        for (int i=0; i<index; i++) {
            const char *line = lines[i];
            if (match_line(line, count, _sword, false, false, false)) {
                show_index++;
                if (show_index == select) {
                    printf("%s\n", line);
                    goto end;
                }
            }
        }

        // 全字区分大小写跳跃匹配带数字
        show_index = 0;
        for (int i=0; i<index; i++) {
            const char *line = lines[i];
            if (match_line(line, count, _wordu, true, true, false)) {
                show_index++;
                if (show_index == select) {
                    printf("%s\n", line);
                    goto end;
                }
            }
        }
        // 全字跳跃匹配带数字
        show_index = 0;
        for (int i=0; i<index; i++) {
            const char *line = lines[i];
            if (match_line(line, count, _word, false, true, false)) {
                show_index++;
                if (show_index == select) {
                    printf("%s\n", line);
                    goto end;
                }
            }
        }
    }
end:
    linereader_free(reader);
    free(_word);
    free(_sword);
    free(lines);
    return 0;
}
