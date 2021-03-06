// This file is part of the hypertext project.
//
// Copyright (c) 2020-2021 Apfel
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

#include <string.h>

uint8_t hypertext_Fetch_Method(hypertext_Instance* instance, uint8_t* output)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Request) return hypertext_Result_Invalid_Instance;
    else if (output == NULL) return hypertext_Result_Invalid_Parameters;

    memcpy(output, &instance->method, sizeof(uint8_t));

    return hypertext_Result_Success;
}

uint8_t hypertext_Fetch_Path(hypertext_Instance* instance, char* output, size_t* length)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Request) return hypertext_Result_Invalid_Instance;
    else if (length == NULL) return hypertext_Result_Invalid_Parameters;

    if (output == NULL)
    {
        size_t path_length = strlen(instance->path);
        memcpy(length, &path_length, sizeof(size_t));
        return hypertext_Result_Success;
    }

    strncpy(output, instance->path, *length > strlen(instance->path) ? *length : strlen(instance->path));

    return hypertext_Result_Success;
}

uint8_t hypertext_Fetch_Version(hypertext_Instance* instance, uint8_t* output)
{
    if (!hypertext_utilities_is_valid_instance(instance)) return hypertext_Result_Invalid_Instance;
    else if (output == NULL) return hypertext_Result_Invalid_Parameters;

    memcpy(output, &instance->version, sizeof(uint8_t));

    return hypertext_Result_Success;
}

uint8_t hypertext_Fetch_Code(hypertext_Instance* instance, uint16_t* output)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Response) return hypertext_Result_Invalid_Instance;
    else if (output == NULL) return hypertext_Result_Invalid_Parameters;

    memcpy(output, &instance->code, sizeof(uint16_t));

    return hypertext_Result_Success;
}

uint8_t hypertext_Fetch_Header_Field(hypertext_Instance* instance, hypertext_Header_Field* output, const char* key_name)
{
    if (!hypertext_utilities_is_valid_instance(instance)) return hypertext_Result_Invalid_Instance;
    else if (output == NULL || key_name == NULL || strlen(key_name) == 0) return hypertext_Result_Invalid_Parameters;

    for (size_t i = 0; i != instance->field_count; i++) if (strcmp(instance->fields[i].key, key_name) == 0)
    {
        memcpy(output, &instance->fields[i], sizeof(hypertext_Header_Field));
        return hypertext_Result_Success;
    }

    return hypertext_Result_Not_Found;
}

uint8_t hypertext_Fetch_Header_Field_Count(hypertext_Instance* instance, size_t* count)
{
    if (!hypertext_utilities_is_valid_instance(instance)) return hypertext_Result_Invalid_Instance;
    else if (count == NULL) return hypertext_Result_Invalid_Parameters;

    memcpy(count, &instance->field_count, sizeof(size_t));

    return hypertext_Result_Success;
}

uint8_t hypertext_Fetch_Body(hypertext_Instance* instance, char* output, size_t* length)
{
    if (!hypertext_utilities_is_valid_instance(instance)) return hypertext_Result_Invalid_Instance;
    else if (length == NULL) return hypertext_Result_Invalid_Parameters;
    else if (instance->body == NULL) return hypertext_Result_No_Body;

    if (output == NULL)
    {
        size_t body_length = strlen(instance->body);
        memcpy(length, &body_length, sizeof(size_t));
        return hypertext_Result_Success;
    }

    strncpy(output, instance->body, *length > strlen(instance->path) ? *length : strlen(instance->path));

    return hypertext_Result_Success;
}

uint8_t hypertext_Fetch_Type(hypertext_Instance* instance, uint8_t* type)
{
    if (instance == NULL) return hypertext_Result_Invalid_Instance;
    else if (type == NULL) return hypertext_Result_Invalid_Parameters;

    memcpy(type, &instance->type, sizeof(uint8_t));

    return hypertext_Result_Success;
}
