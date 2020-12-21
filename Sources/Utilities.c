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

char* hypertext_utilities_cut_text(const char* text, size_t start, size_t end)
{
    if (start >= end) return NULL;

    char* slice = calloc(end - start + 1, sizeof(char));
    memcpy(slice, text + start, sizeof(char) * (end - start + 1));

    return slice;
}

size_t hypertext_utilities_parse_headers(const char* input, hypertext_Header_Field* fields, size_t* field_count)
{
    if (input == NULL || strlen(input) <= 1 || input[0] == '\n' || (input[0] == '\r' && input[1] == '\n') || (fields == NULL && *field_count != 0)) return 0;

    size_t padding = 0, count = 0;

    hypertext_Header_Field* i_fields = NULL;
    if (*field_count != 0) i_fields = calloc(*field_count, sizeof(hypertext_Header_Field));

    do
    {
        if (input[padding] == '\n' || (input[padding] == '\r' && input[padding + 1] == '\n')) break;

        size_t key_length = 0, value_padding = 1, value_length = 0;

        while (input[padding + key_length] != ':') key_length++;
        if (input[padding + key_length + value_padding] == ' ') value_padding++;

        while (true)
        {
            if (input[padding + key_length + value_padding + value_length] == '\n' || (input[padding + key_length + value_padding + value_length] == '\r' && input[padding + key_length + value_padding + value_length + 1] == '\n')) break;
            value_length++;
        }

        if (i_fields != NULL)
        {
            if (count > *field_count) continue;

            hypertext_Header_Field field =
            {
                .key    = calloc(key_length + 1, sizeof(char)),
                .value  = calloc(value_length + 1, sizeof(char))
            };

            memcpy((char*)field.key, hypertext_utilities_cut_text(input, padding, padding + key_length), key_length);
            memcpy((char*)field.value, hypertext_utilities_cut_text(input, padding + key_length + value_padding, padding + key_length + value_padding + value_length), value_length);

            memcpy(&i_fields[count], &field, sizeof(field));
        }

        count++;
        padding += key_length + value_padding + value_length + 1;
        if (input[padding - 1] == '\r') padding++;
    }
    while (padding <= strlen(input));

    if (*field_count == 0)
    {
        memcpy(field_count, &count, sizeof(size_t));
        return padding + 1;
    }

    if (count > *field_count) return 0;

    for (size_t i = 0; i != count; i++) memcpy(&fields[i], &i_fields[i], sizeof(i_fields[i]));

    free(i_fields);    

    return padding + 1;
}
