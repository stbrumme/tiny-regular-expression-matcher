// //////////////////////////////////////////////////////////
// test.c
// Copyright (c) 2015 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#include "match.h"

#include <stdio.h>


/// count matches
int count(const char* text, const char* regex)
{
  int result = 0;
  while (text = match(text, regex))
  {
    result++;

    if (*text == 0)
      break;
    text++;
  }
  return result;
}


/// count matching lines
int lines(const char* text, const char* regex)
{
  int result = 0;
  while (text = matchlines(text, regex))
  {
    result++;

    if (*text == 0)
      break;

    do
      text++;
    while (*text != 0 && *text != '\n');
  }

  return result;
}


int numErrors = 0;
void error(const char* msg)
{
  printf("failed case %s\n", msg);
  numErrors++;
}


// --------------------------------
// original code by Rob Pike which my lib is based on
int pike_matchhere(char *regexp, char *text);
int pike_matchstar(int c, char *regexp, char *text);
// in addition, returns -1 if these unsupported symbols are found in regex: +, ?, \ 
/* pike_match: search for regexp anywhere in text */
int pike_match(char *regexp, char *text)
{
  /* my code: reject unsupported symbols in regex */
  char* scan = regexp;
  while (*scan != '\0')
  {
    if (*scan == '+' || *scan == '?' || *scan == '\\')
      return 0;
    scan++;
  }
  /* end of my code */

  if (regexp[0] == '^')
    return pike_matchhere(regexp+1, text);
  do {  /* must look even if string is empty */
    if (pike_matchhere(regexp, text))
      return 1;
  } while (*text++ != '\0');
  return 0;
}

/* pike_matchhere: search for regexp at beginning of text */
int pike_matchhere(char *regexp, char *text)
{
  if (regexp[0] == '\0')
    return 1;
  if (regexp[1] == '*')
    return pike_matchstar(regexp[0], regexp+2, text);
  if (regexp[0] == '$' && regexp[1] == '\0')
    return *text == '\0';
  if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
    return pike_matchhere(regexp+1, text+1);
  return 0;
}

/* pike_matchstar: search for c*regexp at beginning of text */
int pike_matchstar(int c, char *regexp, char *text)
{
  do {  /* a * matches zero or more instances */
    if (pike_matchhere(regexp, text))
      return 1;
  } while (*text != '\0' && (*text++ == c || c == '.'));
  return 0;
}


int main(int argc, char* argv[])
{
//#define match(text, regex) pike_match(regex, text)
  if (!match("", ""))     error("Empty1"); // RobPike: ok
  if (!match("abc", ""))  error("Empty2"); // RobPike: ok
  if (!match("", "a?"))   error("Empty3"); // RobPike: unsupported, failed
  if (!match("", ".?"))   error("Empty4"); // RobPike: unsupported, failed
  if (!match("", "a*"))   error("Empty5"); // RobPike: ok
  if (!match("", ".*"))   error("Empty6"); // RobPike: ok
  if (!match("", "^"))    error("Empty7"); // RobPike: ok
  if (!match("", "$"))    error("Empty8"); // RobPike: ok
  if (!match("", "^$"))   error("Empty9"); // RobPike: ok
  if (!match("", "^.?"))  error("Empty10"); // RobPike: unsupported, failed
  if (!match("", "^.*"))  error("Empty11"); // RobPike: ok
  if (!match("", ".?$"))  error("Empty12"); // RobPike: unsupported, failed
  if (!match("", ".*$"))  error("Empty13"); // RobPike: ok
  if (!match("", "^.?$")) error("Empty14"); // RobPike: unsupported, failed
  if (!match("", "^.*$")) error("Empty15"); // RobPike: ok
  if ( match("", "."))    error("Empty16"); // RobPike: ok
  if ( match("", "^."))   error("Empty17"); // RobPike: ok
  if ( match("", ".$"))   error("Empty18"); // RobPike: ok
  if ( match("", "^.$"))  error("Empty19"); // RobPike: ok

  if (!match("abc", "abc"))    error("Exact1"); // RobPike: ok
  if (!match("abc", "ab"))     error("Exact2"); // RobPike: ok
  if (!match("abc", "bc"))     error("Exact3"); // RobPike: ok
  if (!match("abc", "^a"))     error("Exact4"); // RobPike: ok
  if (!match("abc", "^ab"))    error("Exact5"); // RobPike: ok
  if (!match("abc", "^abc"))   error("Exact6"); // RobPike: ok
  if (!match("abc", "^abc$"))  error("Exact7"); // RobPike: ok
  if (!match("abc", "abc$"))   error("Exact8"); // RobPike: ok
  if (!match("abc", "bc$"))    error("Exact9"); // RobPike: ok
  if (!match("abc", "c$"))     error("Exact10"); // RobPike: ok
  if ( match("abc", "Abc"))    error("Exact11"); // RobPike: ok
  if (!match("aabc", "abc"))   error("Exact12"); // RobPike: ok
  if (!match("aabcc", "abc"))  error("Exact13"); // RobPike: ok
  if (!match("abcc", "abc"))   error("Exact14"); // RobPike: ok
  if ( match("aabc", "^abc"))  error("Exact15"); // RobPike: ok
  if (!match("aabc", "abc$"))  error("Exact16"); // RobPike: ok
  if ( match("abcc", "abc$"))  error("Exact17"); // RobPike: ok
  if (!match("abcc", "^abc"))  error("Exact18"); // RobPike: ok

  if (!match("a",   "a*"))    error("Star1"); // RobPike: ok
  if (!match("aa",  "a*"))    error("Star2"); // RobPike: ok
  if (!match("b",   "a*"))    error("Star3"); // RobPike: ok
  if (!match("ab",  "a*"))    error("Star4"); // RobPike: ok
  if (!match("aab", "a*"))    error("Star5"); // RobPike: ok
  if (!match("a",   "^a*"))   error("Star6"); // RobPike: ok
  if (!match("aa",  "^a*"))   error("Star7"); // RobPike: ok
  if (!match("b",   "^a*"))   error("Star8"); // RobPike: ok
  if (!match("ab",  "^a*"))   error("Star9"); // RobPike: ok
  if (!match("aab", "^a*"))   error("Star10"); // RobPike: ok
  if (!match("a",   "a*$"))   error("Star11"); // RobPike: ok
  if (!match("aa",  "a*$"))   error("Star12"); // RobPike: ok
  if (!match("b",   "a*$"))   error("Star13"); // RobPike: ok
  if (!match("ba",  "a*$"))   error("Star14"); // RobPike: ok
  if (!match("baa", "a*$"))   error("Star15"); // RobPike: ok
  if (!match("a",   "^a*$"))  error("Star16"); // RobPike: ok
  if (!match("aa",  "^a*$"))  error("Star17"); // RobPike: ok
  if ( match("b",   "^a*$"))  error("Star18"); // RobPike: ok
  if (!match("a",   "a*a"))   error("Star19"); // RobPike: ok
  if (!match("aa",  "a*a"))   error("Star20"); // RobPike: ok

  if (!match("a",   "a+"))    error("Plus1"); // RobPike: unsupported, failed
  if (!match("aa",  "a+"))    error("Plus2"); // RobPike: unsupported, failed
  if ( match("b",   "a+"))    error("Plus3"); // RobPike: unsupported, ok
  if (!match("ab",  "a+"))    error("Plus4"); // RobPike: unsupported, failed
  if (!match("aab", "a+"))    error("Plus5"); // RobPike: unsupported, failed
  if (!match("a",   "^a+"))   error("Plus6"); // RobPike: unsupported, failed
  if (!match("aa",  "^a+"))   error("Plus7"); // RobPike: unsupported, failed
  if ( match("b",   "^a+"))   error("Plus8"); // RobPike: unsupported, ok
  if (!match("ab",  "^a+"))   error("Plus9"); // RobPike: unsupported, failed
  if (!match("aab", "^a+"))   error("Plus10"); // RobPike: unsupported, failed
  if (!match("a",   "a+$"))   error("Plus11"); // RobPike: unsupported, failed
  if (!match("aa",  "a+$"))   error("Plus12"); // RobPike: unsupported, failed
  if ( match("b",   "a+$"))   error("Plus13"); // RobPike: unsupported, ok
  if (!match("ba",  "a+$"))   error("Plus14"); // RobPike: unsupported, failed
  if (!match("baa", "a+$"))   error("Plus15"); // RobPike: unsupported, failed
  if (!match("a",   "^a+$"))  error("Plus16"); // RobPike: unsupported, failed
  if (!match("aa",  "^a+$"))  error("Plus17"); // RobPike: unsupported, failed
  if ( match("b",   "^a+$"))  error("Plus18"); // RobPike: unsupported, ok
  if ( match("a",   "a+a"))   error("Plus19"); // RobPike: unsupported, ok
  if (!match("aa",  "a+a"))   error("Plus20"); // RobPike: unsupported, failed

  if (!match("a",  "."))     error("Dot1"); // RobPike: unsupported, ok
  if (!match(".",  "."))     error("Dot2"); // RobPike: unsupported, ok
  if (!match("ab", "."))     error("Dot3"); // RobPike: unsupported, ok
  if (!match("a",  ".?"))    error("Dot4"); // RobPike: unsupported, failed
  if (!match("ab", ".?"))    error("Dot5"); // RobPike: unsupported, failed
  if (!match("a",  "^."))    error("Dot6"); // RobPike: unsupported, ok
  if (!match("ab", "^."))    error("Dot7"); // RobPike: unsupported, ok
  if (!match("a",  ".$"))    error("Dot8"); // RobPike: unsupported, ok
  if (!match("ab", ".$"))    error("Dot9"); // RobPike: unsupported, ok
  if (!match("a",  "^.$"))   error("Dot10"); // RobPike: unsupported, ok
  if ( match("ab", "^.$"))   error("Dot11"); // RobPike: unsupported, ok

  if (!match(".",  "\\."))   error("Escape1"); // RobPike: unsupported, failed
  if ( match("a",  "\\."))   error("Escape2"); // RobPike: unsupported, ok
  if (!match("a",  "\\.?"))  error("Escape3"); // RobPike: unsupported, failed
  if (!match("a",  "\\.*"))  error("Escape4"); // RobPike: unsupported, failed
  if ( match("a",  "\\.+"))  error("Escape5"); // RobPike: unsupported, ok
  if (!match("*",  "\\*"))   error("Escape6"); // RobPike: unsupported, failed
  if ( match("a",  "\\*"))   error("Escape7"); // RobPike: unsupported, ok
  if (!match("a",  "\\*?"))  error("Escape8"); // RobPike: unsupported, failed
  if (!match("a",  "\\**"))  error("Escape9"); // RobPike: unsupported, failed
  if ( match("a",  "\\*+"))  error("Escape10"); // RobPike: unsupported, ok
  if (!match("+",  "\\+"))   error("Escape11"); // RobPike: unsupported, failed
  if ( match("a",  "\\+"))   error("Escape12"); // RobPike: unsupported, ok
  if (!match("a",  "\\+?"))  error("Escape13"); // RobPike: unsupported, failed
  if (!match("a",  "\\+*"))  error("Escape14"); // RobPike: unsupported, failed
  if ( match("a",  "\\++"))  error("Escape15"); // RobPike: unsupported, ok
  if (!match("^",  "\\^"))   error("Escape16"); // RobPike: unsupported, failed
  if ( match("a",  "\\^"))   error("Escape17"); // RobPike: unsupported, ok
  if (!match("a",  "\\^?"))  error("Escape18"); // RobPike: unsupported, failed
  if (!match("a",  "\\^*"))  error("Escape19"); // RobPike: unsupported, failed
  if ( match("a",  "\\^+"))  error("Escape20"); // RobPike: unsupported, ok
  if (!match("$",  "\\$"))   error("Escape21"); // RobPike: unsupported, failed
  if ( match("a",  "\\$"))   error("Escape22"); // RobPike: unsupported, ok
  if (!match("a",  "\\$?"))  error("Escape23"); // RobPike: unsupported, failed
  if (!match("a",  "\\$*"))  error("Escape24"); // RobPike: unsupported, failed
  if ( match("a",  "\\$+"))  error("Escape25"); // RobPike: unsupported, ok
  if (!match("?",  "\\?"))   error("Escape26"); // RobPike: unsupported, failed
  if ( match("a",  "\\?"))   error("Escape27"); // RobPike: unsupported, ok
  if (!match("a",  "\\??"))  error("Escape28"); // RobPike: unsupported, failed
  if (!match("a",  "\\?*"))  error("Escape29"); // RobPike: unsupported, failed
  if ( match("a",  "\\?+"))  error("Escape30"); // RobPike: unsupported, ok
  if ( match("\\",  "\\"))   error("Escape31"); // RobPike: unsupported, ok
  if (!match("\\",  "\\\\")) error("Escape32"); // RobPike: unsupported, failed
  if (!match("a\n", "\n"))   error("Escape33"); // RobPike: ok

  if ( match("^",  "^^"))    error("Begin1"); // RobPike: failed
  if ( match("a",  "a^"))    error("Begin2"); // RobPike: misinterpreted, ok
  if (!match("^",  "\\^"))   error("Begin3"); // RobPike: unsupported, failed
  if (!match("^",  "^\\^"))  error("Begin4"); // RobPike: unsupported, failed
  if (!match("^a", "^\\^"))  error("Begin5"); // RobPike: unsupported, failed
  if ( match("^",  "\\^^"))  error("Begin6"); // RobPike: unsupported, ok

  if ( match("$",  "$$"))    error("End1"); // RobPike: failed
  if ( match("a",  "$a"))    error("End2"); // RobPike: misinterpreted, ok
  if (!match("$",  "\\$"))   error("End3"); // RobPike: unsupported, failed
  if (!match("$",  "\\$$"))  error("End4"); // RobPike: unsupported, failed
  if (!match("a$", "\\$$"))  error("End5"); // RobPike: unsupported, failed
  if ( match("$",  "$\\$"))  error("End6"); // RobPike: unsupported, ok

  // ---------------------
  // note: count() cannot be used for ^ matching (but $ is working fine)

  if (count("",    "a" ) != 0) error("CountExact1");
  if (count("a",   "a" ) != 1) error("CountExact2");
  if (count("aa",  "a" ) != 2) error("CountExact3");
  if (count("aaa", "a" ) != 3) error("CountExact4");

  if (count("aaa", "a*") != 4) error("CountStar1");

  if (count("aaa", "a+") != 3) error("CountPlus1");

  if (count("aaa", "."     ) != 3) error("CountDot1");
  if (count("aba", "."     ) != 3) error("CountDot2");
  if (count("aba", ".."    ) != 2) error("CountDot3");
  if (count("aba", "..."   ) != 1) error("CountDot4");
  if (count("aba", "...."  ) != 0) error("CountDot5");
  if (count("aba", "....?" ) != 1) error("CountDot6");
  if (count("aba", "..?.." ) != 1) error("CountDot7");
  if (count("aba", "..?..?") != 2) error("CountDot8");
  if (count("",    "."     ) != 0) error("CountDot9");
  if (count("",    ".?"    ) != 1) error("CountDot10");

  if (count("aaa", "a$") != 1) error("CountEnd1");
  if (count("aaa", "$" ) != 1) error("CountEnd2");

  // ---------------------
  if (lines("",      ""    ) != 1) error("LinesExact1");
  if (lines("\n",    ""    ) != 2) error("LinesExact2");

  return numErrors;
}
