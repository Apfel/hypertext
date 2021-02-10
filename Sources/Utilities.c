// Copyright (c) 2020 Apfel
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software.
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Utilities.h"
#include "Internals.h"

#include <stdlib.h>
#include <string.h>

inline int64_t hypertext_utilities_contains_item(hypertext_Header_Field* fields, size_t field_count, const char* name)
{
    if (fields != NULL && field_count >= 2 && name != NULL) for (size_t i = 0; i != field_count; i++) if (strcmp(fields[i].key, name) == 0) return i;

    return -1;
}

const char* hypertext_utilities_cut_text(const char* text, size_t start, size_t end)
{
    if (start >= end) return NULL;

    char* slice = calloc(end - start + 1, sizeof(char));
    memcpy(slice, text + start, sizeof(char) * (end - start));

    return slice;
}

size_t hypertext_utilities_parse_headers(const char* input, hypertext_Header_Field* fields, size_t* field_count)
{
    if (input == NULL || field_count == NULL || strlen(input) <= 1 || input[0] == '\n' || (input[0] == '\r' && input[1] == '\n') || (fields == NULL && *field_count != 0)) return 0;

    size_t padding = 0, count = 1, key_length, value_padding, value_length;
    int64_t position;

    hypertext_Header_Field* i_fields = calloc(count, sizeof(hypertext_Header_Field));

    do
    {
        key_length      = 0;
        value_padding   = 1;
        value_length    = 0;

        if (input[padding] == '\n' || (input[padding] == '\r' && input[padding + 1] == '\n')) break;

        while (input[padding + key_length] != ':') key_length++;
        if (input[padding + key_length + value_padding] == ' ') value_padding++;
        while (input[padding + key_length + value_padding + value_length] != '\n' && (input[padding + key_length + value_padding + value_length] != '\r' && input[padding + key_length + value_padding + value_length + 1] != '\n')) value_length++;

        position = hypertext_utilities_contains_item(i_fields, count - 1, hypertext_utilities_cut_text(input, padding, padding + key_length));
        if (position != -1)
        {
            size_t old_value_length = strlen(i_fields[position].value);
            i_fields[position].value = realloc(i_fields[position].value, old_value_length + value_length + 3);
            strncat(i_fields[position].value, ", ", 3);
            strncat(i_fields[position].value, hypertext_utilities_cut_text(input, padding + key_length + value_padding, padding + key_length + value_padding + value_length), value_length);
        }
        else
        {
            hypertext_Header_Field field =
            {
                .key    = calloc(key_length + 1, sizeof(char)),
                .value  = calloc(value_length + 1, sizeof(char))
            };

            strncpy(field.key, hypertext_utilities_cut_text(input, padding, padding + key_length), key_length);
            strncpy(field.value, hypertext_utilities_cut_text(input, padding + key_length + value_padding, padding + key_length + value_padding + value_length), value_length);

            memcpy(&i_fields[count - 1], &field, sizeof(field));
        }

        if (position == -1)
        {
            count++;
            i_fields = realloc(i_fields, sizeof(hypertext_Header_Field) * count);
        }

        padding += key_length + value_padding + value_length + 1;
        if (input[padding - 1] == '\r') padding++;
    }
    while (padding <= strlen(input));

    count--;

    if (*field_count == 0)
    {
        memcpy(field_count, &count, sizeof(size_t));
        return padding + 1;
    }

    if ((count - 1) > *field_count) return 0;

    memcpy(fields, i_fields, sizeof(hypertext_Header_Field) * count);
    free(i_fields);
    return padding + 1;
}
