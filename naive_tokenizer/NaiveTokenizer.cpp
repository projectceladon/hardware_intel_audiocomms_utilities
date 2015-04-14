/*
 * Copyright (C) 2013-2015 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "NaiveTokenizer.h"
#include <cstring>

char* NaiveTokenizer::getNextToken(char** line)
{
    const char *quotes = "'\""; // single or double quotes
    char separator[2] = " ";
    char first[2];

    if (*line == NULL || (*line)[0] == '\0') {
        return NULL;
    }

    // Copy the first character into its own new string
    first[0] = (*line)[0];
    first[1] = '\0';

    // Check if the first character is a quote
    if (strstr(quotes, first) != NULL) {
        // If so, move forward and set the separator to that quote
        (*line)++;
        strncpy(separator, first, sizeof(separator));
    }
    // If it is not, get the next space-delimited token
    // First, move the cursor forward if the first character is a space
    // This effectively ignores multiple spaces in a row
    else if (strstr(separator, first) != NULL) {
        (*line)++;
        return NaiveTokenizer::getNextToken(line);
    }

    return strsep(line, separator);
}
