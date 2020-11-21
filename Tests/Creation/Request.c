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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    hypertext_Instance* instance = hypertext_New();
    if (instance == NULL)
    {
        printf("Error: The resulting instance was null.\n");
        return 1;
    }

    const char* path = "/index.html";

    hypertext_Header_Field field_host       = { "Host", "example.org" };
    hypertext_Header_Field field_user_agent = { "User-Agent", "hypertext-Example" };
    hypertext_Header_Field fields[2]        = { field_host, field_user_agent };

    int code = 1;
    switch (hypertext_Create_Request(instance, hypertext_Method_GET, path, strlen(path), fields, sizeof(fields) / sizeof(hypertext_Header_Field), NULL, 0))
    {
    case hypertext_Result_Success:
        code = 0;
        printf("Success.\n");
        break;

    case hypertext_Result_Invalid_Parameters:
        printf("Error: hypertext_Create_Request failed with code \"invalid parameters\".\n");
        break;

    case hypertext_Result_Invalid_Method:
        printf("Error: hypertext_Create_Request failed with code \"invalid request method\".\n");
        break;
    }

    hypertext_Destroy(instance);
    free(instance);

    return code;
}
