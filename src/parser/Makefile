CXX      = g++
CXXFLAGS = -std=c++14 -Wall -Wextra
BISON    = bison
FLEX     = flex

TARGET   = deutsch_c

PARSER_SOURCE = parser.cc
PARSER_HEADER = parser.hh
LEXER_SOURCE  = lexer.cc

SOURCES = main.cc Driver.cc $(PARSER_SOURCE) $(LEXER_SOURCE)
OBJECTS = $(SOURCES:.cc=.o)

all: $(TARGET)

# Linken des fertigen Programms
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

$(PARSER_SOURCE) $(PARSER_HEADER): parser.yy
	$(BISON) -d -o $(PARSER_SOURCE) parser.yy

$(LEXER_SOURCE): lexer.ll $(PARSER_HEADER)
	$(FLEX) -o $(LEXER_SOURCE) lexer.ll

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(PARSER_SOURCE) $(PARSER_HEADER) $(LEXER_SOURCE) location.hh main.o

cleanall: clean
	rm -f $(TARGET) $(OBJECTS) $(PARSER_SOURCE) $(PARSER_HEADER) $(LEXER_SOURCE) location.hh

test: all
	./$(TARGET) test.txt