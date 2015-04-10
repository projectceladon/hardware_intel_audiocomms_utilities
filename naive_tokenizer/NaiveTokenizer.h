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

class NaiveTokenizer {
public:
    /** tokenize a space-separated string, handling quotes
     *
     * The input is tokenized, using " " (space) as the tokenizer; multiple
     * spaces are regarded as a single separator.  If the input begins with a
     * quote (either single (') or double (")), the next token will be all the
     * characters (including spaces) until the next identical quote.
     * Warning: This function modifies its argument in-place !
     *
     * It aims at reproducing the parsing of a shell.
     *
     * @param[inout] line The string to be tokenized. Warning: modified in-place
     * @return Pointer to the next token (which is actually the original value of 'line'
     */
    static char* getNextToken(char** line);
};
