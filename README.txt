This is a quick benchmark comparing find/replace performance of ASCII, UTF-32 and UTF-8. The test loads the full text of Alice in Wonderland into a string in each format, and then performs the find/replace operation. The ASCII and UTF-32 operations simply iterate over each character, replacing '.' with ',' as needed. The UTF-8 operation uses the UTF8-CPP library to decode each code point, and then compares it with '.', and copies over the original code point or ',' as needed.

Because of the variable-width nature of UTF-8, the find/replace cannot be performed in-place without risking O(n^2) performance, so it is performed not-in-place. To compensate for that, the benchmark also performs a not-in-place version of the UTF-32 operation.

My results:
ASCII file loaded: 150899 bytes
UTF8 file loaded: 150899 bytes
UTF-32 file loaded: 603596 bytes
Find/replace (in-place) from '.' to ',' took 674 microseconds using ASCII
Find/replace (in-place) from '.' to ',' took 699 microseconds using UTF-32
Find/replace (not-in-place) from '.' to ',' took 45980 microseconds using UTF-8
Find/replace (not-in-place) from '.' to ',' took 7938 microseconds using UTF-32
