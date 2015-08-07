// //////////////////////////////////////////////////////////
// match.h
// Copyright (c) 2015 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
//

#pragma once

// allowed metasymbols:
// ^  -> text must start with pattern (only allowed as first symbol in regular expression)
// $  -> text must end   with pattern (only allowed as last  symbol in regular expression)
// .  -> any literal
// x? -> literal x occurs zero or one time
// x* -> literal x is repeated arbitrarily
// x+ -> literal x is repeated at least once

/// points to first regex match or NULL if not found, text is treated as one line   and ^ and $ refer to its start/end
const char* match      (const char* text, const char* regex);

/// points to first regex match or NULL if not found, text is split into lines (\n) and ^ and $ refer to each line's start/end
const char* matchlines (const char* text, const char* regex);
