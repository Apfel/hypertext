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

uint8_t hypertext_Create_Request(hypertext_Instance* instance, uint8_t method, const char* path, size_t path_length, uint8_t version, hypertext_Header_Field* fields, size_t field_count, const char* body, size_t body_length)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Unknown) return hypertext_Result_Invalid_Instance;
    else if (path == NULL || method == hypertext_Method_Unknown || method >= hypertext_Method_Max || version == hypertext_HTTP_Version_Unknown || version >= hypertext_HTTP_Version_Max) return hypertext_Result_Invalid_Parameters;

    instance->type = hypertext_Instance_Content_Type_Request;

    instance->method = method;
    
    instance->path = calloc(path_length + 1, sizeof(char));
    memcpy(instance->path, path, path_length * sizeof(char));

    if (field_count != 0)
    {
        if (fields == NULL) return hypertext_Result_Invalid_Parameters;

        instance->field_count = field_count;
        instance->fields = calloc(field_count, sizeof(hypertext_Header_Field));
        memcpy(instance->fields, fields, sizeof(hypertext_Header_Field) * field_count);
    }

    if (body_length != 0)
    {
        if (body == NULL) return hypertext_Result_Invalid_Parameters;

        instance->body = calloc(body_length + 1, sizeof(char));
        memcpy(instance->body, body, body_length * sizeof(char));
    }

    instance->version = version;

    return hypertext_Result_Success;
}

uint8_t hypertext_Create_Response(hypertext_Instance* instance, uint8_t version, uint16_t code, hypertext_Header_Field* fields, size_t field_count, const char* body, size_t body_length)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Unknown) return hypertext_Result_Invalid_Instance;
    else if (code == hypertext_Status_Unknown || code >= hypertext_Status_Max || version == hypertext_HTTP_Version_Unknown || version >= hypertext_HTTP_Version_Max) return hypertext_Result_Invalid_Parameters;

    instance->type = hypertext_Instance_Content_Type_Response;

    if (field_count != 0)
    {
        if (fields == NULL) return hypertext_Result_Invalid_Parameters;

        instance->field_count = field_count;
        instance->fields = calloc(field_count, sizeof(hypertext_Header_Field));
        memcpy(instance->fields, fields, sizeof(hypertext_Header_Field) * field_count);
    }

    if (body_length != 0)
    {
        if (body == NULL) return hypertext_Result_Invalid_Parameters;

        instance->body = calloc(body_length + 1, sizeof(char));
        memcpy(instance->body, body, body_length * sizeof(char));
    }

    instance->version = version;

    return hypertext_Result_Success;
}
