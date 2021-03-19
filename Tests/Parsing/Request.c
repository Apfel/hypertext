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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* example = "GET /index.html HTTP/1.0\r\nHost: www.example.org\r\nUser-Agent: hypertext-Example\r\nExample: test\r\nExample: test 2\r\n\r\nThis is an example body used to test the parser.";

int main()
{
    hypertext_Instance* instance = hypertext_New();
    if (instance == NULL)
    {
        printf("Error: The resulting instance was null.\n");
        return 1;
    }

    uint8_t code = hypertext_Parse_Request(instance, example, 48);
    switch (code)
    {
    case hypertext_Result_Success:
        printf("Success.\n");
        break;

    case hypertext_Result_Invalid_Parameters:
        printf("Error: hypertext_Parse_Request failed with code \"invalid parameters\".\n");
        break;

    case hypertext_Result_Invalid_Method:
        printf("Error: hypertext_Parse_Request failed with code \"invalid request method\".\n");
        break;

    default:
        printf("An error occurred that isn't handled separately; code %d.\n", code);
        break;
    }

    if (code != 0)
    {
        hypertext_Destroy(instance);
        free(instance);
        return code;
    }

    size_t length = 0;
    hypertext_Output_Request(instance, NULL, &length, true);

    char* output = calloc(length + 1, sizeof(char));
    hypertext_Output_Request(instance, output, &length, true);

    if (strcmp(example, output) == 0) printf("Warning: hypertext_Output_Request that's the same as the input.\n");

    hypertext_Destroy(instance);
    free(instance);

    return 0;
}
