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
#include "Utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t hypertext_Output_Request(hypertext_Instance* instance, char* output, size_t* length, bool keep_compat)
{
    if (instance == NULL || instance->type != hypertext_Instance_Content_Type_Request) return hypertext_Result_Invalid_Instance;
    else if (length == NULL) return hypertext_Result_Invalid_Parameters;

    char* method_str = calloc(9, sizeof(char));
    switch (instance->method)
    {
    case hypertext_Method_OPTIONS:
        snprintf(method_str, 9, "OPTIONS");
        break;

    case hypertext_Method_GET:
        snprintf(method_str, 9, "GET");
        break;

    case hypertext_Method_HEAD:
        snprintf(method_str, 9, "HEAD");
        break;

    case hypertext_Method_POST:
        snprintf(method_str, 9, "POST");
        break;

    case hypertext_Method_PUT:
        snprintf(method_str, 9, "PUT");
        break;

    case hypertext_Method_DELETE:
        snprintf(method_str, 9, "DELETE");
        break;

    case hypertext_Method_TRACE:
        snprintf(method_str, 9, "TRACE");
        break;

    case hypertext_Method_CONNECT:
        snprintf(method_str, 9, "CONNECT");
        break;
    }

    size_t out_len = strlen(method_str) + strlen(instance->path) + (keep_compat ? 12 : 11);

    if (instance->field_count != 0 || instance->fields != NULL) for (size_t i = 0; i != instance->field_count; i++) out_len += strlen(instance->fields[i].key) + strlen(instance->fields[i].value) + (keep_compat ? 4 : 2);

    out_len += keep_compat ? 2 : 1;

    if (instance->body != NULL) out_len += strlen(instance->body);

    if (*length == 0) memcpy(length, &out_len, sizeof(size_t));
    else if (*length != out_len) return hypertext_Result_Invalid_Parameters;

    if (output != NULL)
    {
        char* out_str = calloc(out_len + 1, sizeof(char));

        char* term = calloc((keep_compat ? 3 : 2), sizeof(char));
        if (keep_compat) snprintf(term, 3, "\r\n");
        else snprintf(term, 2, "\n");

        char* ver_str = calloc(4, sizeof(char));

        switch (instance->version)
        {
        case hypertext_HTTP_Version_1_0:
            snprintf(ver_str, 4, "1.0");
            break;

        case hypertext_HTTP_Version_1_1:
            snprintf(ver_str, 4, "1.1");
            break;
        }

        snprintf(out_str, out_len + 1, "%s %s HTTP/%s%s", method_str, instance->path, ver_str, term);

        if (instance->field_count != 0 && instance->fields != NULL) for (size_t i = 0; i != instance->field_count; i++) snprintf(out_str, out_len + 1, "%s%s:%s%s%s", out_str, instance->fields[i].key, keep_compat ? " " : "", instance->fields[i].value, term);

        snprintf(out_str, out_len + 1, "%s%s", out_str, term);

        if (instance->body != NULL && strlen(instance->body) > 0) snprintf(out_str, out_len + 1, "%s%s", out_str, instance->body);

        memcpy(output, out_str, sizeof(char) * out_len);

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
        description = calloc(256, sizeof(char));

        switch (instance->code)
        {
        case hypertext_Status_Continue:
            snprintf(description, 256, "Continue");
            break;

        case hypertext_Status_Switching_Protocols:
            snprintf(description, 256, "Switching Protocols");
            break;

        case hypertext_Status_OK:
            snprintf(description, 256, "OK");
            break;

        case hypertext_Status_Created:
            snprintf(description, 256, "Created");
            break;

        case hypertext_Status_Accepted:
            snprintf(description, 256, "Accepted");
            break;

        case hypertext_Status_Non_Authoritative_Information:
            snprintf(description, 256, "Non-Authoritative Information");
            break;

        case hypertext_Status_No_Content:
            snprintf(description, 256, "No Content");
            break;

        case hypertext_Status_Reset_Content:
            snprintf(description, 256, "Reset Content");
            break;

        case hypertext_Status_Partial_Content:
            snprintf(description, 256, "Partial Content");
            break;

        case hypertext_Status_Multiple_Choices:
            snprintf(description, 256, "Multiple Choices");
            break;

        case hypertext_Status_Moved_Permanentely:
            snprintf(description, 256, "Moved Permanentely");
            break;

        case hypertext_Status_Found:
            snprintf(description, 256, "Found");
            break;

        case hypertext_Status_See_Other:
            snprintf(description, 256, "See Other");
            break;

        case hypertext_Status_Not_Modified:
            snprintf(description, 256, "Not Modified");
            break;

        case hypertext_Status_Use_Proxy:
            snprintf(description, 256, "Use Proxy");
            break;

        case hypertext_Status_Unused_306:
            snprintf(description, 256, "Unused");
            break;

        case hypertext_Status_Temporary_Redirect:
            snprintf(description, 256, "Temporary Redirect");
            break;

        case hypertext_Status_Bad_Request:
            snprintf(description, 256, "Bad Request");
            break;

        case hypertext_Status_Unauthorized:
            snprintf(description, 256, "Unauthorized");
            break;

        case hypertext_Status_Payment_Required:
            snprintf(description, 256, "Payment Required");
            break;

        case hypertext_Status_Forbidden:
            snprintf(description, 256, "Forbidden");
            break;

        case hypertext_Status_Not_Found:
            snprintf(description, 256, "Not Found");
            break;

        case hypertext_Status_Method_Not_Allowed:
            snprintf(description, 256, "Method Not Allowed");
            break;

        case hypertext_Status_Not_Acceptable:
            snprintf(description, 256, "Not Acceptable");
            break;

        case hypertext_Status_Proxy_Authentication_Required:
            snprintf(description, 256, "Proxy Authentication Required");
            break;

        case hypertext_Status_Request_Timeout:
            snprintf(description, 256, "Request Timeout");
            break;

        case hypertext_Status_Conflict:
            snprintf(description, 256, "Conflict");
            break;

        case hypertext_Status_Gone:
            snprintf(description, 256, "Gone");
            break;

        case hypertext_Status_Length_Required:
            snprintf(description, 256, "Length Required");
            break;

        case hypertext_Status_Precondition_Failed:
            snprintf(description, 256, "Precondition Failed");
            break;

        case hypertext_Status_Request_Entity_Too_Large:
            snprintf(description, 256, "Request Entity Too Large");
            break;

        case hypertext_Status_Request_URI_Too_Long:
            snprintf(description, 256, "Request URI Too Long");
            break;

        case hypertext_Status_Unsupported_Media_Type:
            snprintf(description, 256, "Unsupported Media Type");
            break;

        case hypertext_Status_Requested_Range_Not_Satisfiable:
            snprintf(description, 256, "Requested Range Not Satisfiable");
            break;

        case hypertext_Status_Expectation_Failed:
            snprintf(description, 256, "Expectation Failed");
            break;

        case hypertext_Status_Not_Implemented:
            snprintf(description, 256, "Not Implemented");
            break;

        case hypertext_Status_Bad_Gateway:
            snprintf(description, 256, "Bad Gateway");
            break;

        case hypertext_Status_Service_Unavailable:
            snprintf(description, 256, "Service Unavailable");
            break;

        case hypertext_Status_Gateway_Timeout:
            snprintf(description, 256, "Gateway Timeout");
            break;

        case hypertext_Status_HTTP_Version_Not_Supported:
            snprintf(description, 256, "HTTP Version Not Supported");
            break;
        }
    }

    size_t out_len = (keep_desc ? strlen(description) : 0) + 15 + (keep_compat ? 2 : 1);

    if (instance->field_count != 0 && instance->fields != NULL) for (size_t i = 0; i != instance->field_count; i++) out_len += strlen(instance->fields[i].key) + strlen(instance->fields[i].value) + (keep_compat ? 4 : 2);

    out_len += keep_compat ? 2 : 1;

    if (instance->body != NULL) out_len += strlen(instance->body);

    if (*length == 0) memcpy(length, &out_len, sizeof(size_t));
    else if (*length != out_len) return hypertext_Result_Invalid_Parameters;

    if (output != NULL)
    {
        char* out_str = calloc(out_len + 1, sizeof(char));

        char* term = calloc((keep_compat ? 3 : 2), sizeof(char));
        if (keep_compat) snprintf(term, 3, "\r\n");
        else snprintf(term, 2, "\n");

        char* ver_str = calloc(4, sizeof(char));

        switch (instance->version)
        {
        case hypertext_HTTP_Version_1_0:
            snprintf(ver_str, 4, "1.0");
            break;

        case hypertext_HTTP_Version_1_1:
            snprintf(ver_str, 4, "1.1");
            break;
        }

        snprintf(out_str, out_len + 1, "HTTP/%s %d", ver_str, instance->code);
        if (keep_desc) snprintf(out_str, out_len + 1, "%s %s", out_str, description);

        snprintf(out_str, out_len + 1, "%s%s", out_str, term);

        if (instance->field_count != 0 && instance->fields != 0) for (size_t i = 0; i != instance->field_count; i++) snprintf(out_str, out_len + 1, "%s%s:%s%s%s", out_str, instance->fields[i].key, keep_compat ? " " : "", instance->fields[i].value, term);

        snprintf(out_str, out_len + 1, "%s%s", out_str, term);

        if (instance->body != NULL && strlen(instance->body) > 0) snprintf(out_str, out_len + 1, "%s%s", out_str, instance->body);

        memcpy(output, out_str, sizeof(char) * out_len);

        free(ver_str);
        free(term);
        free(out_str);
    }

    if (keep_desc) free(description);

    return hypertext_Result_Success;
}
