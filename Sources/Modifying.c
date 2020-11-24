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

#include <stdlib.h>
#include <string.h>

uint8_t hypertext_Add_Field(hypertext_Instance* instance, hypertext_Header_Field* input)
{
    if (!hypertext_utilities_is_valid_instance(instance)) return hypertext_Result_Invalid_Instance;
    else if (input == NULL || input->key == NULL || input->value == NULL) return hypertext_Result_Invalid_Parameters;

    for (size_t i = 0; i != instance->field_count; i++) if (instance->fields[i].key == input->key) return hypertext_Result_Already_Present;

    instance->field_count++;

    instance->fields = realloc(instance->fields, sizeof(hypertext_Header_Field) * instance->field_count);

    memcpy(&instance->fields[instance->field_count], input, sizeof(hypertext_Header_Field));

    return hypertext_Result_Success;
}

uint8_t hypertext_Remove_Field(hypertext_Instance* instance, const char* input)
{
    if (!hypertext_utilities_is_valid_instance(instance)) return hypertext_Result_Invalid_Instance;
    else if (input == NULL) return hypertext_Result_Invalid_Parameters;

    bool found = false;
    for (size_t i = 0; i != instance->field_count; i++) if (instance->fields[i].key == input)
    {
        found = true;
        break;
    }

    if (!found) return hypertext_Result_Not_Found;

    hypertext_Header_Field* field_copy = calloc(instance->field_count - 1, sizeof(hypertext_Header_Field));

    size_t pos = 0;
    for (size_t pos2 = 0; pos2 != instance->field_count; pos2++)
    {
        if (instance->fields[pos2].key == input) continue;

        memcpy(&field_copy[pos], &instance->fields[pos2], sizeof(hypertext_Header_Field));
        pos++;
    }

    instance->field_count--;

    instance->fields = realloc(instance->fields, sizeof(hypertext_Header_Field) * instance->field_count);
    for (size_t i = 0; i != instance->field_count; i++) memcpy(&instance->fields[i], &field_copy[i], sizeof(hypertext_Header_Field));

    free(field_copy);

    return hypertext_Result_Success;
}

uint8_t hypertext_Set_Body(hypertext_Instance* instance, const char* body, size_t length)
{
    if (!hypertext_utilities_is_valid_instance(instance)) return hypertext_Result_Invalid_Instance;
    else if (body == NULL || length == 0) return hypertext_Result_Invalid_Parameters;

    if (strlen(instance->body) < length) instance->body = realloc(instance->body, sizeof(char) * length);

    strncpy(instance->body, body, length);

    return hypertext_Result_Success;
}

uint8_t hypertext_Set_Code(hypertext_Instance* instance, uint16_t code)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Response) return hypertext_Result_Invalid_Instance;
    else if (code < 100) return hypertext_Result_Invalid_Parameters;

    instance->code = code;

    return hypertext_Result_Success;
}

uint8_t hypertext_Set_Method(hypertext_Instance* instance, uint8_t method)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Request) return hypertext_Result_Invalid_Instance;
    else if (method == hypertext_Method_Unknown || method >= hypertext_Method_Max) return hypertext_Result_Invalid_Parameters;

    instance->method = method;

    return hypertext_Result_Success;
}

uint8_t hypertext_Set_Path(hypertext_Instance* instance, const char* path, size_t length)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Request) return hypertext_Result_Invalid_Instance;
    else if (path == NULL) return hypertext_Result_Invalid_Parameters;

    if (strlen(instance->path) < length) instance->path = realloc(instance->path, sizeof(char) * length);

    strncpy(instance->path, path, length);

    return hypertext_Result_Success;
}

uint8_t hypertext_Set_Version(hypertext_Instance* instance, uint8_t version)
{
    if (!hypertext_utilities_is_valid_instance(instance)) return hypertext_Result_Invalid_Instance;
    else if (version == hypertext_HTTP_Version_Unknown || version >= hypertext_HTTP_Version_Max) return hypertext_Result_Invalid_Parameters;

    instance->version = version;

    return hypertext_Result_Success;
}
