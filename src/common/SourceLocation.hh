#pragma once
#include <ostream>
struct SourceLocation {
    int line_begin = 0;
    int column_begin = 0;
    int line_end = 0;
    int column_end = 0;

    SourceLocation() = default;

    SourceLocation(int lb, int cb, int le, int ce) 
        : line_begin(lb), column_begin(cb), line_end(le), column_end(ce) {}

    friend std::ostream &operator<<(std::ostream &os, const SourceLocation &sl)
    {
        os << "Line: " << sl.line_begin << ":" << sl.column_begin
           << " to " << sl.line_end << ":" << sl.column_end;
        return os;
    }
};