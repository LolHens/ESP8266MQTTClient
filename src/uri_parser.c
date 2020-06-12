/*
URI Parser
Copyright (c) 2016 Tuan PM (tuanpm@live.com)
Inspired by Hirochika Asai, http://draft.scyphus.co.jp/lang/c/url_parser.html
License (MIT license):
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
#include "uri_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
static __inline__ int
_is_scheme_char(int c)
{
    return (!isalpha(c) && '+' != c && '-' != c && '.' != c) ? 0 : 1;
}

#define JUMP_NEXT_STATE(var, state) { *curr_ptr = 0; curr_ptr ++; var = curr_ptr; parse_state = state; break;}
parsed_uri_t *parse_uri(const char *url)
{
    parsed_uri_t *puri;
    char *curr_ptr;
    int bracket_flag;
    enum parse_state_t {
        PARSE_SCHEME = 0,
        PARSE_HOST,
        PARSE_PORT,
        PARSE_PATH,
        PARSE_QUERY
    } parse_state = 0;
    puri = (parsed_uri_t *)malloc(sizeof(parsed_uri_t));
    if(NULL == puri) {
        //malloc failed, don't try to memset it
        return NULL;
    }
    memset(puri, 0, sizeof(parsed_uri_t));
    puri->_uri_len = strlen(url);
    puri->_uri = (char*) malloc(puri->_uri_len + 1);
    memset(puri->_uri, 0, puri->_uri_len + 1);
    if(puri->_uri == NULL) {
        free_parsed_uri(puri);
        return NULL;
    }
    strcpy(puri->_uri, url);
    puri->_uri[puri->_uri_len] = 0;
    puri->scheme = NULL;
    puri->host = NULL;
    puri->port = NULL;
    puri->path = NULL;
    puri->query = NULL;

    curr_ptr = puri->_uri;
    puri->scheme = curr_ptr;
    parse_state = PARSE_SCHEME;
    bracket_flag = 0;
    while(*curr_ptr) {
        // *curr_ptr = tolower((unsigned char)*curr_ptr);
        switch(parse_state) {
            case PARSE_SCHEME: /* parse scheme */
                if(curr_ptr + 3 < (puri->_uri + puri->_uri_len) && memcmp(curr_ptr, "://", 3) == 0) {
                    *curr_ptr++ = 0;
                    *curr_ptr++ = 0;
                    *curr_ptr++ = 0;
                    puri->host = curr_ptr;
                    parse_state = PARSE_HOST; //next is host
                    break;
                }
                // if(!_is_scheme_char(*curr_ptr)) {
                //  free_parsed_uri(puri);
                //  return NULL;
                // }
                curr_ptr ++;
                break;
            case PARSE_HOST: /* host */
                if('[' == *curr_ptr && bracket_flag == 0) {
                    bracket_flag = 1;
                } else if(']' == *curr_ptr && bracket_flag == 1) {
                    bracket_flag = 0;
                }
                if(bracket_flag == 0 && *curr_ptr == ':') {
                    JUMP_NEXT_STATE(puri->port, PARSE_PORT);
                } else if(bracket_flag == 0 && *curr_ptr == '/') {
                    puri->port = NULL;
                    JUMP_NEXT_STATE(puri->path, PARSE_PATH);
                }
                curr_ptr ++;
                break;
            case PARSE_PORT: /* port */
                if(*curr_ptr == '/') {
                    JUMP_NEXT_STATE(puri->path, PARSE_PATH);
                } else if(*curr_ptr == '?') {
                    JUMP_NEXT_STATE(puri->query, PARSE_QUERY);
                }
                curr_ptr ++;
                break;
            case PARSE_PATH: /* path */
                if(*curr_ptr == '?') {
                    // JUMP_NEXT_STATE(puri->query, PARSE_QUERY);
                }
                curr_ptr ++;
            case PARSE_QUERY: /* query */
                curr_ptr ++;
                break;
        }

    }
    if (puri->path && puri->path[0]!= 0){
        char *temp = (char*)malloc(strlen(puri->path) + 2);
        sprintf(temp, "/%s", puri->path);
        puri->path = temp;
    } else {
        puri->path = malloc(2);
        puri->path[0] = '/';
        puri->path[1] = 0;
    }
    return puri;
}
void parse_uri_info(parsed_uri_t *puri)
{
    printf( "scheme addr: %x\n"
            "host addr: %x\n"
            "port addr: %x\n"
            "path addr: %x\n"
            "extension addr: %x\n"
            "host_ext addr: %x\r\n",
            (int)puri->scheme,
            (int)puri->host,
            (int)puri->port,
            (int)puri->path,
            (int)puri->extension,
            (int)puri->host_ext);

    if(puri->scheme && puri->scheme[0] != 0) {
        printf("scheme: %s\n", puri->scheme);
    }
    if(puri->host && puri->host[0] != 0) {
        printf("Host: %s\n", puri->host);
    }
    if(puri->path && puri->path[0] != 0) {
        printf("path: %s\n", puri->path);
    }
    if(puri->port && puri->port[0] != 0) {
        printf("port: %s\n", puri->port);
    }
    if(puri->extension && puri->extension[0] != 0) {
        printf("extension: %s\n", puri->extension);
    }
    if(puri->host_ext && puri->host_ext[0] != 0) {
        printf("host_ext: %s\n", puri->host_ext);
    }
}
void free_parsed_uri(parsed_uri_t *puri)
{
    if(NULL != puri) {
        if(puri->path && puri->path[0] != 0) {
            free(puri->path);
        }
        if(NULL != puri->_uri) {
            free(puri->_uri);
        }
        free(puri);
    }
}
