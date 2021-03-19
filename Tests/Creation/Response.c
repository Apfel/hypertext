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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    hypertext_Instance* instance = hypertext_New();
    if (instance == NULL)
    {
        printf("Error: The resulting instance was null.\n");
        return 1;
    }

    hypertext_Header_Field field_type   = { "Content-Type", "text" };
    hypertext_Header_Field field_length = { "Content-Length", "3" };
    hypertext_Header_Field field_conn   = { "Connection", "close" };
    hypertext_Header_Field fields[3]    = { field_type, field_length, field_conn };

    const char* body = "Hi!";

    uint8_t code = hypertext_Create_Response(instance, hypertext_HTTP_Version_1_1, 200, fields, sizeof(fields) / sizeof(hypertext_Header_Field), body, strlen(body));
    switch (code)
    {
    case hypertext_Result_Success:
        printf("Success.\n");
        break;

    case hypertext_Result_Invalid_Parameters:
        printf("Error: hypertext_Create_Request failed with code \"invalid parameters\".\n");
        break;

    case hypertext_Result_Invalid_Method:
        printf("Error: hypertext_Create_Request failed with code \"invalid request method\".\n");
        break;

    default:
        printf("An error occurred that isn't handled separately; code %d.\n", code);
        break;
    }

    hypertext_Destroy(instance);
    free(instance);

    return code;
}
