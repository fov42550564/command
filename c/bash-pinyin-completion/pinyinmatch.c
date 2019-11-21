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

#define DEBUG
#ifdef DEBUG
#define __MYLOG(x, ARGS...) fprintf (stderr, "\n%s:"x"\n", __FUNCTION__, ##ARGS)
#else
#define __MYLOG(x, ARGS...)
#endif //DEBUG

int index_of(const char* str, int offset, char ch) {
    int index = 0;
    while(str[index] != '\0'){
        if(index > offset && str[index] == ch){
            return index;
        }
        index++;
    }
    return -1;
}
bool match_item(char* parent, const char *child) {
    if (parent[0] != child[0]) {
        return false;
    }
    int last_index = -1;
    int count = 0;
    int child_len = strlen(child);
    for (int i = 0; i < child_len; i++) {
        char ch = child[i];
        int index = index_of(parent, last_index, ch);
        if (index > last_index) {
            count++;
        }
        last_index = index;
    }
    return count == child_len;
}
bool match(char** parents, int parent_count, const char *child) {
    for (int i = 0; i < parent_count; i++) {
        if (match_item(parents[i], child)) {
            return true;
        }
    }
    return false;
}
void set_parents_pinyins(char **parents, int parent_count, const char **pinyins, int *count_list, int level) {
    int pre_count = 1;
    for (int i = 0; i < level; i++) {
        pre_count *= count_list[i];
    }
    int current_count = count_list[level];
    for (int i = 0; i < parent_count; i++) {
        strcat(parents[i], pinyins[(i/pre_count)%current_count]);
    }
}
void set_parents_char(char **parents, int parent_count, char ch) {
    for (int i = 0; i < parent_count; i++) {
        parents[i][strlen(parents[i])] = ch;
    }
}
bool match_line(const char *line, int line_length, const char *word) {
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
        parents[i] = (char *)calloc(size * 6 + 1, sizeof(char));
    }

    level = 0;
    while((line_char = utf8vector_next_unichar(line_vector)) != '\0') {
        if (pinyin_ishanzi(line_char)) {
            const char **pinyins;
            pinyin_get_pinyins_by_unicode(line_char, &pinyins);
            set_parents_pinyins(parents, parent_count, pinyins, count_list, level);
            level++;
            free(pinyins);
        } else {
            set_parents_char(parents, parent_count, line_char);
        }
    }
    bool result = match(parents, parent_count, word);
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
        word = "";
    }
    int word_len = strlen(word);
    int select = 0;
    if (word_len > 1 && word[word_len-1] > 48 && word[word_len-1] < 58) { // 使用数字1-9查看第几个，如果这个数字本身存在，则直接找到该选项
        select = word[word_len-1] - 48;
    }

    utf8vector word_vector = utf8vector_create(word, -1);
    int word_size = utf8vector_uni_count(word_vector);
    char *_word = (char *)calloc(word_size * 6 + 1, sizeof(char));
    wchar_t word_char;
    while((word_char = utf8vector_next_unichar(word_vector)) != '\0') {
        if (pinyin_ishanzi(word_char)) {
            const char **pinyins;
            pinyin_get_pinyins_by_unicode(word_char, &pinyins);
            strcat(_word, pinyins[0]);
            free(pinyins);
        } else {
            _word[strlen(_word)] = word_char;
        }
    }

    int count;
    int index = 0;
    linereader reader = linereader_create(STDIN_FILENO);
    bool found = false;
    char ** lines = (char **)calloc(256, sizeof(char *));
    while ((count = linereader_readline(reader)) != -1) {
        const char *line = reader->line_buffer;
        if (index < 256) {
            lines[index++] = strdup(line);
        }
        if (word_len == 0 || match_line(line, count, _word)) {
            found = true;
            printf("%s\n", line);
        }
    }
    if (!found && select > 0) {
        word_len--;
        _word[strlen(_word)-1] = '\0';
        int show_index = 0;
        for (int i=0; i<index; i++) {
            const char *line = lines[i];
            if (word_len == 0 || match_line(line, count, _word)) {
                show_index++;
                if (show_index == select) {
                    printf("%s\n", line);
                    break;
                }
            }
        }
    }
    linereader_free(reader);
    free(_word);
    return false;
}