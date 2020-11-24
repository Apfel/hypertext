// Copyright (c) 2020 The hypertext authors.
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

/**
 * \file hypertext.h
 * \brief Main header for hypertext.
 */

#ifndef hypertext_INCLUDE
#define hypertext_INCLUDE

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_WIN32)
#define hypertext_EXPORT __declspec(dllexport)
#elif defined(__GNUC__) || defined(COMPILER_GCC) || defined(__APPLE__)
#define hypertext_EXPORT __attribute__((visibility("default")))
#else
#error "Your compiler and/or platform might not be supported."
#endif

/// A normal header field.
typedef struct
{
    const char* key; /// The key (name) for this field. Should be capitalized.
    const char* value; /// The value of this field. It must not contain newlines.
} hypertext_Header_Field;

/// An instance stored as an opaque structure; contains any required data.
typedef struct hypertext_Instance hypertext_Instance; 

/// Different types of contents held within an instance.
enum hypertext_Instance_Content_Type
{
    hypertext_Instance_Content_Type_Request, /// Used to set an instance as a "request" instance.
    hypertext_Instance_Content_Type_Response, /// Used to set an instance as a "response" instance.

    hypertext_Instance_Content_Type_Unknown = UINT8_MAX /// Unknown or unset instance type; mostly used within a freshly created instance.
};

/// Different HTTP methods.
enum hypertext_Method
{
    hypertext_Method_Unknown, /// Unknown or unset method; mostly used within a freshly created instance.

    hypertext_Method_OPTIONS, /// Used to discuss what communication options are available and how to use those across the request/response chain.
    hypertext_Method_GET, /// Used to retrieve elements.
    hypertext_Method_HEAD, /// Similar to GET, though the server doesn't need to answer.
    hypertext_Method_POST, /// Used to add or append elements.
    hypertext_Method_PUT, /// Used to add or replace elements.
    hypertext_Method_DELETE, /// Used to delete elements.
    hypertext_Method_TRACE, /// Used to figure out the recipient's identity.
    hypertext_Method_CONNECT, /// Used for proxying / switching to being a tunnel (e.g. SSL tunneling).

    hypertext_Method_Max /// Used for error checking.
};

/** \brief Different return codes. */
enum hypertext_Result
{
    hypertext_Result_Success, /// No error occurred.

    /** \brief Invalid instance.
     * 
     * The supplied instance was already fed with data either via a creation or a parsing method. Use hypertext_destroy to destroy its contents.
     * 
     * \see hypertext_Destroy
     */
    hypertext_Result_Invalid_Instance,

    hypertext_Result_Invalid_Parameters, /// Invalid parameters were supplied; i.e the instance was NULL, the input contained invalid line endings, invalid format, version and so on.
    hypertext_Result_Invalid_Method, /// The request used an unsupported method.
    hypertext_Result_Invalid_Version, /// The version set in the request is invalid or not supported.
    hypertext_Result_Not_Found, /// The specific header field doesn't exist.
    hypertext_Result_Already_Present, /// An another header field with the same key exists already.
    hypertext_Result_No_Body, /// The instance does not contain a body.

    hypertext_Result_Unknown = UINT8_MAX /// Unknown or unset result; mostly used within a freshly created instance.
};

/** \brief Different HTTP status codes.
 * 
 * Codes are always 3 digits long.
 * 
 * 1xx - Informational codes | 2xx - Successful codes | 3xx - Redirection | 4xx - Client Error | 5xx - Server error
 */
enum hypertext_Status
{
    hypertext_Status_Unknown, /// Unknown or unset status. Mostly used inside a freshly created instance.

    hypertext_Status_Continue = 100, /// Tells the client that it should continue its request.
    hypertext_Status_Switching_Protocols = 101, /// The server is willing to switch the protocol.
    hypertext_Status_OK = 200, /// The request succeeded.
    hypertext_Status_Created = 201, /// The request has been fulfilled and resulted in a new resource being created.
    hypertext_Status_Accepted = 202, /// The request has been accepted, but not fully processed yet.
    hypertext_Status_Non_Authoritative_Information = 203, /// The returned information in the "entity-header" is not the definitive set as available from the origin server, but is gathered from a local or a third-party copy.
    hypertext_Status_No_Content = 204, /// The request has been fulfilled, but no content has been returned.
    hypertext_Status_Reset_Content = 205, /// The server successfully fulfilled the request and the client should reset its cached data. Mainly used for user input.
    hypertext_Status_Partial_Content = 206, /// The server successfully fulfilled a part of the request, but isn't done entirely. This request must have a Range header field.
    hypertext_Status_Multiple_Choices = 300, /// The client has multiple choices to pick from to get the resource requested.
    hypertext_Status_Moved_Permanentely = 301, /// The requested resource was found under a permanentally changed URI.
    hypertext_Status_Found = 302, /// The requested resource was found at another URI, because it has been temporarily moved.
    hypertext_Status_See_Other = 303, /// The requested resource may be found somewhere else if desired.
    hypertext_Status_Not_Modified = 304, /// The requested resource hasn't been modified. Mostly used to tell the client that the cached contents are still up-to-date.
    hypertext_Status_Use_Proxy = 305, /// The requested resource can only be accessed via the proxy in the "Location" header field.
    hypertext_Status_Unused_306 = 306, /// Unused but reserved code.
    hypertext_Status_Temporary_Redirect = 307, /// The requested resource resides temporarily under a different URI.
    hypertext_Status_Bad_Request = 400, /// The request was invalid.
    hypertext_Status_Unauthorized = 401, /// The request was understood by the remote, but the client needs to authorize itself.
    hypertext_Status_Payment_Required = 402, /// Reserved for future use.
    hypertext_Status_Forbidden = 403, /// The requested resource is not accessible.
    hypertext_Status_Not_Found = 404, /// The requested resource doesn't exist.
    hypertext_Status_Method_Not_Allowed = 405, /// The resource was found, but the request method was invalid.
    hypertext_Status_Not_Acceptable = 406, /// The "Accept" header field was invalid.
    hypertext_Status_Proxy_Authentication_Required = 407, /// The proxy requires authentication.
    hypertext_Status_Request_Timeout = 408, /// The request took too long to arrive at the server.
    hypertext_Status_Conflict = 409, /// The current request requested a resource that's currently in a conflicting state.
    hypertext_Status_Gone = 410, /// The requested resource is no longer available.
    hypertext_Status_Length_Required = 411, /// The request needs a "Content-Length" field. Mostly used for POST requests.
    hypertext_Status_Precondition_Failed = 412, /// The given precondition ended up false.
    hypertext_Status_Request_Entity_Too_Large = 413, /// The given request entity is larger than what the server is willing to process.
    hypertext_Status_Request_URI_Too_Long = 414, /// The request URI is too long.
    hypertext_Status_Unsupported_Media_Type = 415, /// The entity of the request is in an unsupported format.
    hypertext_Status_Requested_Range_Not_Satisfiable = 416, /// The given Range header field and the content of the request do not overlap correctly.
    hypertext_Status_Expectation_Failed = 417, /// A given expectation in the Expect header field couldn't be met.
    hypertext_Status_Not_Implemented = 501, /// The request couldn't be fulfilled because the server's implementation lacks the functionality to do so.
    hypertext_Status_Bad_Gateway = 502, /// The upstream server did not deliver a proper response.
    hypertext_Status_Service_Unavailable = 503, /// The server is currently not able to satisfy the request.
    hypertext_Status_Gateway_Timeout = 504, /// The upstream server failed to respond within the given time frame.
    hypertext_Status_HTTP_Version_Not_Supported = 505, /// The version needed by this request is either not, or the server refuses to support that version.

    hypertext_Status_Max, /// Used for error checking.
};

/// Different HTTP versions supported by hypertext.
enum hypertext_HTTP_Version
{
    hypertext_HTTP_Version_1_0,
    hypertext_HTTP_Version_1_1
};

/** \brief Creates a new instance.
 * 
 * \return Returns NULL if an error occurred; otherwise it'll be a usable instance.
 */
hypertext_EXPORT hypertext_Instance* hypertext_New();

/// Destroys the instance's content. Useful in order to re-use a cleaned instance for a new request or response.
hypertext_EXPORT void hypertext_Destroy(hypertext_Instance* instance);

/** \brief Initializes the instance as a new request.
 * 
 * \param instance The instance to use.
 * \param method The method used in the request.
 * \param path The absolute path to the file.
 * \param path_length The length of the path.
 * \param fields All header fields.
 * \param field_count The field count.
 * \param body The body's content.
 * \param body_length The body's length.
 * 
 * \note If any length is set to 0, the respective value will be skipped.
 * \note field_count must be the amount of fields, not the size of a field array.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Create_Request(hypertext_Instance* instance, uint8_t method, const char* path, size_t path_length, hypertext_Header_Field* fields, size_t field_count, const char* body, size_t body_length);

/** \brief Initializes the instance as a new request.
 * 
 * \param instance The instance to use.
 * \param code The response code.
 * \param fields All header fields.
 * \param field_count The field count.
 * \param body The body's content.
 * \param body_length The body's length.
 * 
 * \note If any length is set to 0, the respective value will be skipped.
 * \note field_count must be the amount of fields, not the size of a field array.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Create_Response(hypertext_Instance* instance, uint16_t code, hypertext_Header_Field* fields, size_t field_count, const char* body, size_t body_length);

/** \brief Parses a raw request coming from a UTF-8 character array.
 * 
 * \param instance The instance to use.
 * \param input The input character array to parse.
 * \param length The maximum length to parse if a null terminator isn't met before the set length.
 * 
 * \note The length counts the amount of letters and not the length of the body in bytes.
 * \note The length only applies to the body.
 * \note Set length to 0 to parse until a null terminator is met.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Parse_Request(hypertext_Instance* instance, const char* input, size_t length);

/** \brief Parses a raw response coming from a UTF-8 character array.
 * 
 * \param instance The instance to use.
 * \param input The input character array to parse.
 * \param length The maximum length to parse if a null terminator isn't met before the set length.
 * 
 * \note The length counts the amount of letters and not the length of the body in bytes.
 * \note The length only applies to the body.
 * \note Set length to 0 to parse until a null terminator is met.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Parse_Response(hypertext_Instance* instance, const char* input, size_t length);

/** \brief Takes the request contents stored within the instance and pushes it into "output".
 * 
 * \param instance The instance to use.
 * \param output The output variable.
 * \param length The maximum length to parse if a null terminator isn't met before the set length.
 * \param keep_compat Whether to keep full compatibility with the HTTP standard, RFC 2616, or to reduce size by skipping some values set in place.
 * 
 * \note output can be null.
 * \note length cannot be null.
 * \note if keep_compat is false, only a linefeed will be used, instead of a carriage return followed a linefeed.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Output_Request(hypertext_Instance* instance, char* output, size_t* length, bool keep_compat);

/** \brief Takes the response contents stored within the instance and pushes it into "output".
 * 
 * \param instance The instance to use.
 * \param output The output variable.
 * \param length The maximum length to parse if a null terminator isn't met before the set length.
 * \param keep_desc Whether to add the description for the status code or not. hypertext doesn't need this during parsing, so this might save some bandwidth.
 * \param keep_compat Whether to keep full compatibility with the HTTP standard, RFC 2616, or to reduce size by skipping some values set in place.
 * 
 * \note output can be null.
 * \note length cannot be null.
 * \note if keep_compat is false, only a linefeed will be used, instead of a carriage return followed a linefeed.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Output_Response(hypertext_Instance* instance, char* output, size_t* length, bool keep_desc, bool keep_compat);

/** \brief Adds a header field to the instance.
 * \param instance The instance to use.
 * \param input The header field to add.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Add_Field(hypertext_Instance* instance, hypertext_Header_Field* input);

/** \brief Removes a header field from the instance.
 * \param instance The instance to use.
 * \param input The name of the field to remove.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Remove_Field(hypertext_Instance* instance, const char* input);

/** \brief Returns the request's method.
 * \param instance The instance to use.
 * \param output The output variable.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Fetch_Method(hypertext_Instance* instance, uint8_t* output);

/** \brief Returns the path's length.
 * \param instance The instance to use.
 * \param length The output to copy the length to.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Fetch_Path_Length(hypertext_Instance* instance, size_t* length);

/** \brief Returns the request's path.
 * \param instance The instance to use.
 * \param output The output variable.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Fetch_Path(hypertext_Instance* instance, char* output, size_t length);

/** \brief Returns the used version of the response/request stored inside.
 * \param instance The instance to use.
 * \param output The output variable.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Fetch_Version(hypertext_Instance* instance, uint8_t* output);

/** \brief Returns the response's return code.
 * \param instance The instance to use.
 * \param output The output variable.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Fetch_Code(hypertext_Instance* instance, uint16_t* output);

/** \brief Fetches a header field based on its key.
 * \param instance The instance to use.
 * \param output The output variable.
 * \param key_name The name to search for.
 *
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Fetch_Header_Field(hypertext_Instance* instance, hypertext_Header_Field* output, const char* key_name);

/** \brief Returns the amount of header fields.
 * \param instance The instance to use.
 * \param count The output variable.
 *
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Fetch_Header_Field_Count(hypertext_Instance* instance, size_t* count);

/** \brief Returns the body of the request/response.
 * \param instance The instance to use.
 * \param output The output variable.
 * \param length The length of the body to return or to process.
 * 
 * \note If output is NULL, the length will be overwritten. Use this to fetch the length.
 * 
 * \return A normal return code.
 * \sa hypertext_Result.
 */
hypertext_EXPORT uint8_t hypertext_Fetch_Body(hypertext_Instance* instance, char* output, size_t* length);

#ifdef __cplusplus
}
#endif
#endif
