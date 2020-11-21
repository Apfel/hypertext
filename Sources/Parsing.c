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

#include <hypertext.h>

#include "Internals.h"
#include "Utilities.h"

#include <stdlib.h>
#include <string.h>

uint8_t hypertext_Parse_Request(hypertext_Instance* instance, const char* input, size_t length)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Unknown) return hypertext_Result_Invalid_Instance;
    else if (input == NULL) return hypertext_Result_Invalid_Parameters;

    instance->type = hypertext_Instance_Content_Type_Request;

    char* methodstr     = calloc(8, sizeof(char));
    uint8_t methodlen   = 0;
    for (; methodlen != 8; methodlen++)
    {
        if (input[methodlen] ==  ' ' || input[methodlen] == 0) break;

        methodstr[methodlen] = input[methodlen];
    }

    instance->method = UINT8_MAX;
    if      (strcmp(methodstr, "OPTIONS") == 0)    instance->method = hypertext_Method_OPTIONS;
    else if (strcmp(methodstr, "GET") == 0)        instance->method = hypertext_Method_GET;
    else if (strcmp(methodstr, "HEAD") == 0)       instance->method = hypertext_Method_HEAD;
    else if (strcmp(methodstr, "POST") == 0)       instance->method = hypertext_Method_POST;
    else if (strcmp(methodstr, "PUT") == 0)        instance->method = hypertext_Method_PUT;
    else if (strcmp(methodstr, "DELETE") == 0)     instance->method = hypertext_Method_DELETE;
    else if (strcmp(methodstr, "TRACE") == 0)      instance->method = hypertext_Method_TRACE;
    else if (strcmp(methodstr, "CONNECT") == 0)    instance->method = hypertext_Method_CONNECT;

    free(methodstr);
    if (instance->method == UINT8_MAX) return hypertext_Result_Invalid_Method;

    uint32_t pathlen = 1;
    for (; pathlen != UINT32_MAX; pathlen++) if (input[methodlen + pathlen] ==  ' ' || input[methodlen + pathlen] == 0) break;

    instance->path = calloc(1, sizeof(char) * pathlen);
    for (int i = 0; i != pathlen - 1; i++) instance->path[i] = input[methodlen + i + 1];

    if (strcmp(hypertext_utilities_append_null(hypertext_utilities_cut_text(input, methodlen + pathlen + 1, methodlen + pathlen + 5), 5), "HTTP/") != 0) return hypertext_Result_Invalid_Parameters;

    char* ver_str = calloc(4, sizeof(char));
    for (int i = 0; i != 3; i++) ver_str[i] = input[methodlen + pathlen + 6 + i];

    if      (strcmp(ver_str, "1.0") == 0) instance->version = hypertext_HTTP_Version_1_0;
    else if (strcmp(ver_str, "1.1") == 0) instance->version = hypertext_HTTP_Version_1_1;
    else
    {
        free(ver_str);
        return hypertext_Result_Invalid_Version;
    }

    free(ver_str);

    size_t padding = methodlen + pathlen + 9;
    if (input[padding] == '\r') padding++;

    if (input[padding] != '\n') return hypertext_Result_Invalid_Parameters;

    padding++;
    if (hypertext_utilities_parse_headers(hypertext_utilities_cut_text(input, padding, strlen(input)), NULL, &instance->field_count) == 0) return hypertext_Result_Invalid_Parameters;
    instance->fields = calloc(instance->field_count, sizeof(hypertext_Header_Field));

    int64_t offset = hypertext_utilities_parse_headers(hypertext_utilities_cut_text(input, padding, strlen(input)), instance->fields, &instance->field_count);
    if (offset == 0) return hypertext_Result_Invalid_Parameters;
    if (length != 0)
    {
        if (length > strlen(input) - offset) length = strlen(input) - offset;

        instance->body = calloc(length + 1, sizeof(char));
        errno_t err = memcpy_s(instance->body, length + 1, hypertext_utilities_cut_text(input, padding + offset, padding + offset + length), length);
        if (err != 0) return hypertext_Result_Invalid_Parameters;        
    }

    return hypertext_Result_Success;
}

uint8_t hypertext_Parse_Response(hypertext_Instance* instance, const char* input, size_t length)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Unknown) return hypertext_Result_Invalid_Instance;
    if (input == NULL) return hypertext_Result_Invalid_Parameters;

    instance->type = hypertext_Instance_Content_Type_Response;

    if (strcmp(hypertext_utilities_append_null(hypertext_utilities_cut_text(input, 0, 5), 5), "HTTP/") != 0) return hypertext_Result_Invalid_Parameters;

    char* ver_str = calloc(4, sizeof(char));
    for (int i = 0; i != 3; i++) ver_str[i] = input[5 + i];;

    if      (strcmp(ver_str, "1.0") == 0) instance->version = hypertext_HTTP_Version_1_0;
    else if (strcmp(ver_str, "1.1") == 0) instance->version = hypertext_HTTP_Version_1_1;
    else
    {
        free(ver_str);
        return hypertext_Result_Invalid_Version;
    }

    char statusstr[4] = { input[9], input[10], input[11], 0 };
    unsigned long status_int = strtoul(statusstr, NULL, 0);
    if (status_int > UINT16_MAX || status_int < 100) return hypertext_Result_Invalid_Parameters;

    instance->code = (uint16_t)status_int;

    uint32_t padding = 13;
    for (; padding != UINT32_MAX; padding++)
    {
        if (input[padding] == '\r') continue;
        else if (input[padding] == '\n')
        {
            padding++;
            break;
        }
    }

    if (hypertext_utilities_parse_headers(hypertext_utilities_cut_text(input, padding, strlen(input)), NULL, &instance->field_count) == 0) return hypertext_Result_Invalid_Parameters;
    instance->fields = calloc(instance->field_count, sizeof(hypertext_Header_Field));

    int64_t offset = hypertext_utilities_parse_headers(hypertext_utilities_cut_text(input, padding, strlen(input)), instance->fields, &instance->field_count);
    if (offset == 0) return hypertext_Result_Invalid_Parameters;
    if (length != 0)
    {
        if (length > strlen(input) - offset) length = strlen(input) - offset;

        instance->body = calloc(length + 1, sizeof(char));
        errno_t err = memcpy_s(instance->body, length + 1, hypertext_utilities_cut_text(input, padding + offset, padding + offset + length), length);
        if (err != 0) return hypertext_Result_Invalid_Parameters;        
    }

    return hypertext_Result_Success;
}
