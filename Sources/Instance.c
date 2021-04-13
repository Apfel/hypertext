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

#include <stdlib.h>

static inline void hypertext_utilities_free_and_null(void* data)
{
    free(data);
    data = NULL;
}

hypertext_Instance* hypertext_New()
{
    hypertext_Instance* instance = calloc(1, sizeof(hypertext_Instance));
    if (instance == NULL) return NULL;

    instance->type = hypertext_Instance_Content_Type_Unknown;

    return instance;
}

void hypertext_Destroy(hypertext_Instance* instance)
{
    if (instance == NULL) return;

    instance->code          = 0;
    instance->field_count   = 0;
    instance->method        = hypertext_Method_Unknown;
    instance->version       = 0;
    instance->type          = hypertext_Instance_Content_Type_Unknown;

    if (instance->body      != NULL) hypertext_utilities_free_and_null(instance->body);
    if (instance->fields    != NULL) hypertext_utilities_free_and_null(instance->fields);
    if (instance->path      != NULL) hypertext_utilities_free_and_null(instance->path);
}
