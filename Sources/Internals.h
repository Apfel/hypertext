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

#ifndef hypertext_INTERNALS
#define hypertext_INTERNALS

#include <hypertext.h>

struct hypertext_Instance
{
    char*                   body;
    uint16_t                code;
    hypertext_Header_Field* fields;
    size_t                  field_count;
    uint8_t                 method;
    char*                   path;
    uint8_t                 type;
    uint8_t                 version;
};

inline bool hypertext_utilities_is_valid_instance(hypertext_Instance* instance)
{
    if (instance == NULL) return false;
    else if (instance->type == hypertext_Instance_Content_Type_Request || instance->type == hypertext_Instance_Content_Type_Response) return true;

    return false;
}

#endif
