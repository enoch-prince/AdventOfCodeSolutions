#include <iostream>
#include <map>
#include <ranges>
#include <algorithm>

// This is an application of X-MACROS
#define FOR_ALL_TOKEN_TYPES(DO) \
    DO(0, DIGIT) \
    DO(1, WORDED_NUMBER) \
    DO(2, INVALID) \
    DO(3, END) \

#define DEFINE_ENUM(id, name) name = id,
enum class TokenType {
    FOR_ALL_TOKEN_TYPES(DEFINE_ENUM)
};

#define INSERT_ELEMENT(id, name) {static_cast<TokenType>(id), #name},
std::map<TokenType, std::string_view> TokenType_STR = {
    FOR_ALL_TOKEN_TYPES(INSERT_ELEMENT)
};


using Token_t = struct {
    TokenType type;
    char value;
    size_t pos;
};

using NumDict = std::map<std::string, char>;

const NumDict Valid_Worded_Numbers = {
    {"zero", '0'}, {"one", '1'}, {"two", '2'}, {"three", '3'}, {"four", '4'}, {"five", '5'},
    {"six", '6'}, {"seven", '7'}, {"eight", '8'}, {"nine", '9'}
};

void printToken(Token_t& token) {
    std::cout << "[Token] type: " << TokenType_STR[token.type] << 
                ", value: '" << token.value << "', pos: " << 
                token.pos << std::endl; 
}

std::pair<std::string, char> get_first_match_value(const NumDict& map, const std::string& substr) {
    std::pair<std::string, char> result{};                   
    for (const auto& [key, value] : map) {
        if (key.find(substr) != std::string::npos) {
            result = {key, value};
            return result;
        }
    }
    result = {"", '\0'};
    return result;
}

class Lexer {
    public:
        Lexer() : cursor(0), bot(0), temp{""}  {}
        ~Lexer() {}

        void init(std::string_view data) {
            content = data;
            cursor = 0;
            bot = 0;
            temp = "";
        }

        Token_t next() {
            Token_t token{.type = TokenType::END, .value = '\0', .pos = 0};

            if (cursor >= content.size()) {
                token.pos = cursor;
                return token;
            }
            char item = content[cursor];

            if (std::isdigit(item)) {
                token.type = TokenType::DIGIT;
                token.value = item;
                token.pos = cursor;
                bot = cursor;
                cursor++;
                temp = "";
               return token;
            }
            else if (temp.size() < 3) {
                temp += item;
                cursor++;
                return next();
            }

            auto matched = get_first_match_value(Valid_Worded_Numbers, temp);
            if (matched.second == '\0') {
                cursor = ++bot; 
                temp = "";
                return next();
            }
            else {
                token.type = TokenType::WORDED_NUMBER;
                token.value = matched.second;
                token.pos = cursor-3; // look_ahead value (3)
                cursor += (matched.first.length() - temp.length()); // move cursor to the beginning of next token
                bot = cursor; 
                temp = "";
                return token;
            }
            
            cursor++;
            return next();
        }
    
    private:
        size_t cursor;
        size_t bot; //beginning of token
        std::string_view content;
        std::string temp;
};

int main() {
    Lexer lexer;
    // lexer.init("three78sixtwo");
    lexer.init("fdlv8jmjtgdfhskkpbmzkpbvfivevlsbrgfxpq8mzdnzglf7");
    Token_t token{};
    do {
        token = lexer.next();
        printToken(token);
    } while (token.type != TokenType::END);

    // std::cout << "two = " << get_first_match_value(Valid_Worded_Numbers,"two").second << std::endl;

    return 0;
}