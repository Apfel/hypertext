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

#include <string.h>

uint8_t hypertext_Fetch_Method(hypertext_Instance* instance, uint8_t* output)
{
    return hypertext_Result_Success;
}

uint8_t hypertext_Fetch_Path(hypertext_Instance* instance, char** output)
{
    return hypertext_Result_Success;
}

uint8_t hypertext_Fetch_Version(hypertext_Instance* instance, uint8_t* output)
{
    return hypertext_Result_Success;
}

uint8_t hypertext_Fetch_Code(hypertext_Instance* instance, uint8_t* output)
{
    return hypertext_Result_Success;
}

uint8_t hypertext_Fetch_Header_Field(hypertext_Instance* instance, hypertext_Header_Field* output, const char* key_name)
{
    return hypertext_Result_Success;
}

uint8_t hypertext_Fetch_Body(hypertext_Instance* instance, char** output)
{
    return hypertext_Result_Success;
}
