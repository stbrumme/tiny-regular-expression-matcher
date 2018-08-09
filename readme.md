# Tiny Regular Expression Matcher

Chapter 1 of the excellent book "Beautiful Code" discusses Rob Pike's very short and elegant regex parser code.
It doesn't come with all the bells and whistles of a full Perl regular expression matcher but is quite sufficient for many purposes.

I extended Rob's C code in such a way that it can handle the following regex meta-symbols:
- `.` - match any character
- `*` - the previous character is arbitrarily repeated
- `+` - the previous character occurs at least once
- `?` - the previous character occurs once or not at all
- `^` - match regular expression only to the beginning of the text
- `$` - match regular expression only to the end of the text
- `\` - treat next character as a literal, even if it is a regex symbol

My library consists of just one `.h` and one `.c` file - that's why it's called "tiny".

See my website https://create.stephan-brumme.com/tiny-regular-expression-matcher/ for a live demo, code examples and a simple test suite.
