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

const char* example = "HTTP/1.1 200 OK\r\nContent-Type: text\r\nContent-Length: 3\r\nConnection: close\r\n\r\n";

int main()
{
    hypertext_Instance* instance = hypertext_New();
    if (instance == NULL)
    {
        printf("Error: The resulting instance was null.\n");
        return 1;
    }

    uint8_t code = hypertext_Parse_Response(instance, example, 0);
    switch (code)
    {
    case hypertext_Result_Success:
        printf("Success.\n");
        break;

    case hypertext_Result_Invalid_Parameters:
        printf("Error: hypertext_Parse_Response failed with code \"invalid parameters\".\n");
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
    hypertext_Output_Response(instance, NULL, &length, true, true);

    char* output = calloc(length + 1, sizeof(char));
    hypertext_Output_Response(instance, output, &length, true, true);

    if (strcmp(example, output) != 0) printf("Warning: hypertext_Output_Response returned data that isn't the same as the input.\n");

    free(output);
    hypertext_Destroy(instance);
    free(instance);

    return code;
}
