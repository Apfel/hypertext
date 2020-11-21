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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t hypertext_Output_Request(hypertext_Instance* instance, char* output, size_t* length, bool keep_compat)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Request) return hypertext_Result_Invalid_Instance;

    char* method_str = calloc(9, sizeof(char));
    switch (instance->method)
    {
    case hypertext_Method_OPTIONS:
        sprintf_s(method_str, 8, "OPTIONS");
        break;

    case hypertext_Method_GET:
        sprintf_s(method_str, 8, "GET");
        break;

    case hypertext_Method_HEAD:
        sprintf_s(method_str, 8, "HEAD");
        break;

    case hypertext_Method_POST:
        sprintf_s(method_str, 8, "POST");
        break;

    case hypertext_Method_PUT:
        sprintf_s(method_str, 8, "PUT");
        break;

    case hypertext_Method_DELETE:
        sprintf_s(method_str, 8, "DELETE");
        break;

    case hypertext_Method_TRACE:
        sprintf_s(method_str, 8, "TRACE");
        break;

    case hypertext_Method_CONNECT:
        sprintf_s(method_str, 8, "CONNECT");
        break;
    }

    size_t out_len = strlen(method_str) + strlen(instance->path) + 12;
    if (keep_compat) out_len++;

    if (instance->field_count != 0 && instance->fields != 0) for (size_t i = 0; i != instance->field_count; i++)
    {
        out_len += strlen(instance->fields[i].key) + strlen(instance->fields[i].value) + 2;
        if (keep_compat) out_len += 2;
    }

    out_len += keep_compat ? 2 : 1;

    if (instance->body != NULL) out_len += strlen(instance->body);

    if (*length == 0) memcpy_s(length, sizeof(size_t), &out_len, sizeof(size_t));
    else if (*length != out_len) return hypertext_Result_Invalid_Parameters;

    if (output != NULL)
    {
        char* out_str = calloc(out_len + 1, sizeof(char));

        char* term = calloc((keep_compat ? 2 : 1) + 1, sizeof(char));
        keep_compat ? sprintf_s(term, 3, "\r\n") : sprintf_s(term, 2, "\n");

        char* ver_str = calloc(4, sizeof(char));

        switch (instance->version)
        {
        case hypertext_HTTP_Version_1_0:
            sprintf_s(ver_str, 4, "1.0");
            break;

        case hypertext_HTTP_Version_1_1:
            sprintf_s(ver_str, 4, "1.1");
            break;
        }

        sprintf_s(out_str, out_len, "%s %s HTTP/%s%s", method_str, instance->path, ver_str, term);

        if (instance->field_count != 0 && instance->fields != 0) for (size_t i = 0; i != instance->field_count; i++) sprintf_s(out_str, out_len, "%s%s:%s%s%s", out_str, instance->fields[i].key, keep_compat ? " " : "", instance->fields[i].value, term);

        sprintf_s(out_str, out_len, "%s%s", out_str, term);

        if (instance->body != NULL) sprintf_s(out_str, out_len, "%s%s", out_str, instance->body);

        memcpy_s(output, out_len + 1, out_str, out_len + 1);

        free(ver_str);
        free(term);
        free(out_str);
    }

    free(method_str);

    return hypertext_Result_Success;
}

uint8_t hypertext_Output_Response(hypertext_Instance* instance, char* output, size_t* length, bool keep_desc, bool keep_compat)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Response) return hypertext_Result_Invalid_Instance;

    char* description = NULL;

    if (keep_desc)
    {
        description = calloc(512, sizeof(char));

        switch (instance->code)
        {
        case hypertext_Status_Continue:
            sprintf_s(description, 512, "Continue");
            break;

        case hypertext_Status_Switching_Protocols:
            sprintf_s(description, 512, "Switching Protocols");
            break;

        case hypertext_Status_OK:
            sprintf_s(description, 512, "OK");
            break;

        case hypertext_Status_Created:
            sprintf_s(description, 512, "Created");
            break;

        case hypertext_Status_Accepted:
            sprintf_s(description, 512, "Accepted");
            break;

        case hypertext_Status_Non_Authoritative_Information:
            sprintf_s(description, 512, "Non-Authoritative Information");
            break;

        case hypertext_Status_No_Content:
            sprintf_s(description, 512, "No Content");
            break;

        case hypertext_Status_Reset_Content:
            sprintf_s(description, 512, "Reset Content");
            break;

        case hypertext_Status_Partial_Content:
            sprintf_s(description, 512, "Partial Content");
            break;

        case hypertext_Status_Multiple_Choices:
            sprintf_s(description, 512, "Multiple Choices");
            break;

        case hypertext_Status_Moved_Permanentely:
            sprintf_s(description, 512, "Moved Permanentely");
            break;

        case hypertext_Status_Found:
            sprintf_s(description, 512, "Found");
            break;

        case hypertext_Status_See_Other:
            sprintf_s(description, 512, "See Other");
            break;

        case hypertext_Status_Not_Modified:
            sprintf_s(description, 512, "Not Modified");
            break;

        case hypertext_Status_Use_Proxy:
            sprintf_s(description, 512, "Use Proxy");
            break;

        case hypertext_Status_Unused_306:
            sprintf_s(description, 512, "Unused");
            break;

        case hypertext_Status_Temporary_Redirect:
            sprintf_s(description, 512, "Temporary Redirect");
            break;

        case hypertext_Status_Bad_Request:
            sprintf_s(description, 512, "Bad Request");
            break;

        case hypertext_Status_Unauthorized:
            sprintf_s(description, 512, "Unauthorized");
            break;

        case hypertext_Status_Payment_Required:
            sprintf_s(description, 512, "Payment Required");
            break;

        case hypertext_Status_Forbidden:
            sprintf_s(description, 512, "Forbidden");
            break;

        case hypertext_Status_Not_Found:
            sprintf_s(description, 512, "Not Found");
            break;

        case hypertext_Status_Method_Not_Allowed:
            sprintf_s(description, 512, "Method Not Allowed");
            break;

        case hypertext_Status_Not_Acceptable:
            sprintf_s(description, 512, "Not Acceptable");
            break;

        case hypertext_Status_Proxy_Authentication_Required:
            sprintf_s(description, 512, "Proxy Authentication Required");
            break;

        case hypertext_Status_Request_Timeout:
            sprintf_s(description, 512, "Request Timeout");
            break;

        case hypertext_Status_Conflict:
            sprintf_s(description, 512, "Conflict");
            break;

        case hypertext_Status_Gone:
            sprintf_s(description, 512, "Gone");
            break;

        case hypertext_Status_Length_Required:
            sprintf_s(description, 512, "Length Required");
            break;

        case hypertext_Status_Precondition_Failed:
            sprintf_s(description, 512, "Precondition Failed");
            break;

        case hypertext_Status_Request_Entity_Too_Large:
            sprintf_s(description, 512, "Request Entity Too Large");
            break;

        case hypertext_Status_Request_URI_Too_Long:
            sprintf_s(description, 512, "Request URI Too Long");
            break;

        case hypertext_Status_Unsupported_Media_Type:
            sprintf_s(description, 512, "Unsupported Media Type");
            break;

        case hypertext_Status_Requested_Range_Not_Satisfiable:
            sprintf_s(description, 512, "Requested Range Not Satisfiable");
            break;

        case hypertext_Status_Expectation_Failed:
            sprintf_s(description, 512, "Expectation Failed");
            break;

        case hypertext_Status_Not_Implemented:
            sprintf_s(description, 512, "Not Implemented");
            break;

        case hypertext_Status_Bad_Gateway:
            sprintf_s(description, 512, "Bad Gateway");
            break;

        case hypertext_Status_Service_Unavailable:
            sprintf_s(description, 512, "Service Unavailable");
            break;

        case hypertext_Status_Gateway_Timeout:
            sprintf_s(description, 512, "Gateway Timeout");
            break;

        case hypertext_Status_HTTP_Version_Not_Supported:
            sprintf_s(description, 512, "HTTP Version Not Supported");
            break;
        }
    }

    size_t out_len = 13 + (keep_desc ? strlen(description) : 0);
    if (keep_compat) out_len++;

    if (instance->field_count != 0 && instance->fields != 0) for (size_t i = 0; i != instance->field_count; i++)
    {
        out_len += strlen(instance->fields[i].key) + strlen(instance->fields[i].value) + 2;
        if (keep_compat) out_len += 2;
    }

    out_len += keep_compat ? 3 : 2;

    if (instance->body != NULL) out_len += strlen(instance->body);

    if (*length == 0) memcpy_s(length, sizeof(size_t), &out_len, sizeof(size_t));
    else if (*length != out_len) return hypertext_Result_Invalid_Parameters;

    if (output != NULL)
    {
        char* out_str = calloc(out_len + 1, sizeof(char));

        char* term = calloc((keep_compat ? 2 : 1) + 1, sizeof(char));
        keep_compat ? sprintf_s(term, 3, "\r\n") : sprintf_s(term, 2, "\n");

        char* ver_str = calloc(4, sizeof(char));

        switch (instance->version)
        {
        case hypertext_HTTP_Version_1_0:
            sprintf_s(ver_str, 4, "1.0");
            break;

        case hypertext_HTTP_Version_1_1:
            sprintf_s(ver_str, 4, "1.1");
            break;
        }

        sprintf_s(out_str, out_len, "HTTP/%s %d", ver_str, instance->code);
        if (keep_desc) sprintf_s(out_str, out_len, "%s %s ", out_str, description);

        sprintf_s(out_str, out_len, "%s%s", out_str, term);

        if (instance->field_count != 0 && instance->fields != 0) for (size_t i = 0; i != instance->field_count; i++) sprintf_s(out_str, out_len, "%s%s:%s%s%s", out_str, instance->fields[i].key, keep_compat ? " " : "", instance->fields[i].value, term);

        sprintf_s(out_str, out_len, "%s%s", out_str, term);

        if (instance->body != NULL && strlen(instance->body) > 0) sprintf_s(out_str, out_len, "%s%s", out_str, instance->body);

        memcpy_s(output, out_len, out_str, out_len);

        free(ver_str);
        free(term);
        free(out_str);
    }

    if (keep_desc) free(description);

    return hypertext_Result_Success;
}
