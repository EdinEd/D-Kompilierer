#pragma once

struct SourceLocation {
    int line_begin = 0;
    int column_begin = 0;
    int line_end = 0;
    int column_end = 0;

    SourceLocation() = default;

    SourceLocation(int lb, int cb, int le, int ce) 
        : line_begin(lb), column_begin(cb), line_end(le), column_end(ce) {}
};