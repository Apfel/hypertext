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
        sprintf(method_str, "OPTIONS");
        break;

    case hypertext_Method_GET:
        sprintf(method_str, "GET");
        break;

    case hypertext_Method_HEAD:
        sprintf(method_str, "HEAD");
        break;

    case hypertext_Method_POST:
        sprintf(method_str, "POST");
        break;

    case hypertext_Method_PUT:
        sprintf(method_str, "PUT");
        break;

    case hypertext_Method_DELETE:
        sprintf(method_str, "DELETE");
        break;

    case hypertext_Method_TRACE:
        sprintf(method_str, "TRACE");
        break;

    case hypertext_Method_CONNECT:
        sprintf(method_str, "CONNECT");
        break;
    }

    size_t out_len = strlen(method_str) + strlen(instance->path) + 12;
    if (keep_compat) out_len++;

    if (instance->field_count != 0 && instance->fields != 0) for (size_t i = 0; i != instance->field_count; i++)
    {
        out_len += strlen(instance->fields[i].key) + strlen(instance->fields[i].value) + 2;
        if (keep_compat) out_len += 2;
    }

    out_len += keep_compat ? 3 : 2;

    if (instance->body != NULL) out_len += strlen(instance->body);

    if (*length == 0) memcpy(length, &out_len, sizeof(size_t));
    else if (*length != out_len) return hypertext_Result_Invalid_Parameters;

    if (output != NULL)
    {
        char* out_str = calloc(out_len + 1, sizeof(char));

        char* term = calloc((keep_compat ? 2 : 1) + 1, sizeof(char));
        keep_compat ? sprintf(term, "\r\n") : sprintf(term, "\n");

        char* ver_str = calloc(4, sizeof(char));

        switch (instance->version)
        {
        case hypertext_HTTP_Version_1_0:
            sprintf(ver_str, "1.0");
            break;

        case hypertext_HTTP_Version_1_1:
            sprintf(ver_str, "1.1");
            break;
        }

        sprintf(out_str, "%s %s HTTP/%s%s", method_str, instance->path, ver_str, term);

        if (instance->field_count != 0 && instance->fields != 0) for (size_t i = 0; i != instance->field_count; i++) sprintf(out_str, "%s%s:%s%s%s", out_str, instance->fields[i].key, keep_compat ? " " : "", instance->fields[i].value, term);

        sprintf(out_str, "%s%s", out_str, term);

        if (instance->body != NULL) sprintf(out_str, "%s%s", out_str, instance->body);

        memcpy(output, out_str, out_len);

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
            sprintf(description, "Continue");
            break;

        case hypertext_Status_Switching_Protocols:
            sprintf(description, "Switching Protocols");
            break;

        case hypertext_Status_OK:
            sprintf(description, "OK");
            break;

        case hypertext_Status_Created:
            sprintf(description, "Created");
            break;

        case hypertext_Status_Accepted:
            sprintf(description, "Accepted");
            break;

        case hypertext_Status_Non_Authoritative_Information:
            sprintf(description, "Non-Authoritative Information");
            break;

        case hypertext_Status_No_Content:
            sprintf(description, "No Content");
            break;

        case hypertext_Status_Reset_Content:
            sprintf(description, "Reset Content");
            break;

        case hypertext_Status_Partial_Content:
            sprintf(description, "Partial Content");
            break;

        case hypertext_Status_Multiple_Choices:
            sprintf(description, "Multiple Choices");
            break;

        case hypertext_Status_Moved_Permanentely:
            sprintf(description, "Moved Permanentely");
            break;

        case hypertext_Status_Found:
            sprintf(description, "Found");
            break;

        case hypertext_Status_See_Other:
            sprintf(description, "See Other");
            break;

        case hypertext_Status_Not_Modified:
            sprintf(description, "Not Modified");
            break;

        case hypertext_Status_Use_Proxy:
            sprintf(description, "Use Proxy");
            break;

        case hypertext_Status_Unused_306:
            sprintf(description, "Unused");
            break;

        case hypertext_Status_Temporary_Redirect:
            sprintf(description, "Temporary Redirect");
            break;

        case hypertext_Status_Bad_Request:
            sprintf(description, "Bad Request");
            break;

        case hypertext_Status_Unauthorized:
            sprintf(description, "Unauthorized");
            break;

        case hypertext_Status_Payment_Required:
            sprintf(description, "Payment Required");
            break;

        case hypertext_Status_Forbidden:
            sprintf(description, "Forbidden");
            break;

        case hypertext_Status_Not_Found:
            sprintf(description, "Not Found");
            break;

        case hypertext_Status_Method_Not_Allowed:
            sprintf(description, "Method Not Allowed");
            break;

        case hypertext_Status_Not_Acceptable:
            sprintf(description, "Not Acceptable");
            break;

        case hypertext_Status_Proxy_Authentication_Required:
            sprintf(description, "Proxy Authentication Required");
            break;

        case hypertext_Status_Request_Timeout:
            sprintf(description, "Request Timeout");
            break;

        case hypertext_Status_Conflict:
            sprintf(description, "Conflict");
            break;

        case hypertext_Status_Gone:
            sprintf(description, "Gone");
            break;

        case hypertext_Status_Length_Required:
            sprintf(description, "Length Required");
            break;

        case hypertext_Status_Precondition_Failed:
            sprintf(description, "Precondition Failed");
            break;

        case hypertext_Status_Request_Entity_Too_Large:
            sprintf(description, "Request Entity Too Large");
            break;

        case hypertext_Status_Request_URI_Too_Long:
            sprintf(description, "Request URI Too Long");
            break;

        case hypertext_Status_Unsupported_Media_Type:
            sprintf(description, "Unsupported Media Type");
            break;

        case hypertext_Status_Requested_Range_Not_Satisfiable:
            sprintf(description, "Requested Range Not Satisfiable");
            break;

        case hypertext_Status_Expectation_Failed:
            sprintf(description, "Expectation Failed");
            break;

        case hypertext_Status_Not_Implemented:
            sprintf(description, "Not Implemented");
            break;

        case hypertext_Status_Bad_Gateway:
            sprintf(description, "Bad Gateway");
            break;

        case hypertext_Status_Service_Unavailable:
            sprintf(description, "Service Unavailable");
            break;

        case hypertext_Status_Gateway_Timeout:
            sprintf(description, "Gateway Timeout");
            break;

        case hypertext_Status_HTTP_Version_Not_Supported:
            sprintf(description, "HTTP Version Not Supported");
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

    if (*length == 0) memcpy(length, &out_len, sizeof(size_t));
    else if (*length != out_len) return hypertext_Result_Invalid_Parameters;

    if (output != NULL)
    {
        char* out_str = calloc(out_len + 1, sizeof(char));

        char* term = calloc((keep_compat ? 2 : 1) + 1, sizeof(char));
        keep_compat ? sprintf(term, "\r\n") : sprintf(term, "\n");

        char* ver_str = calloc(4, sizeof(char));

        switch (instance->version)
        {
        case hypertext_HTTP_Version_1_0:
            sprintf(ver_str, "1.0");
            break;

        case hypertext_HTTP_Version_1_1:
            sprintf(ver_str, "1.1");
            break;
        }

        sprintf(out_str, "HTTP/%s %d", ver_str, instance->code);
        if (keep_desc) sprintf(out_str, "%s %s ", out_str, description);

        sprintf(out_str, "%s%s", out_str, term);

        if (instance->field_count != 0 && instance->fields != 0) for (size_t i = 0; i != instance->field_count; i++) sprintf(out_str, "%s%s:%s%s%s", out_str, instance->fields[i].key, keep_compat ? " " : "", instance->fields[i].value, term);

        sprintf(out_str, "%s%s", out_str, term);

        if (instance->body != NULL && strlen(instance->body) > 0) sprintf(out_str, "%s%s", out_str, instance->body);

        memcpy(output, out_str, out_len);

        free(ver_str);
        free(term);
        free(out_str);
    }

    if (keep_desc) free(description);

    return hypertext_Result_Success;
}
