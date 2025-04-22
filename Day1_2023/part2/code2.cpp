#include <iostream>
#include <map>
#include <ranges>
#include <algorithm>
#include <fstream>
#include <string>

// This is an application of X-MACROS
#define FOR_ALL_TOKEN_TYPES(DO) \
    DO(0, DIGIT)                \
    DO(1, WORDED_NUMBER)        \
    DO(2, INVALID)              \
    DO(3, END)

#define DEFINE_ENUM(id, name) name = id,
enum class TokenType
{
    FOR_ALL_TOKEN_TYPES(DEFINE_ENUM)
};

#define INSERT_ELEMENT(id, name) {static_cast<TokenType>(id), #name},
std::map<TokenType, std::string_view> TokenType_STR = {
    FOR_ALL_TOKEN_TYPES(INSERT_ELEMENT)};

using Token_t = struct
{
    TokenType type;
    char value;
    size_t pos;
};

using NumDict = std::map<std::string, char>;

const NumDict Valid_Worded_Numbers = {
    {"zero", '0'}, {"one", '1'}, {"two", '2'}, {"three", '3'}, {"four", '4'}, {"five", '5'}, {"six", '6'}, {"seven", '7'}, {"eight", '8'}, {"nine", '9'}};

void printToken(Token_t &token)
{
    std::cout << "[Token] type: " << TokenType_STR[token.type] << ", value: '" << token.value << "', pos: " << token.pos << "\n";
}

std::pair<std::string, char> get_first_match_value(const NumDict &map, const std::string &substr)
{
    std::pair<std::string, char> result{};
    for (const auto &[key, value] : map)
    {
        if (key.find(substr) != std::string::npos)
        {
            result = {key, value};
            return result;
        }
    }
    result = {"", '\0'};
    return result;
}

class Lexer
{
public:
    Lexer() : cursor(0), bot(0), temp{""} {}
    ~Lexer() {}

    void init(std::string_view data)
    {
        content = data;
        cursor = 0;
        bot = 0;
        temp = "";
    }

    size_t getCursor()
    {
        return cursor;
    }

    Token_t next()
    {
        Token_t token{.type = TokenType::END, .value = '\0', .pos = 0};

        if (cursor > content.size())
        {
            token.pos = cursor;
            return token;
        }

        if (temp.size() == 3)
        {
            auto matched = get_first_match_value(Valid_Worded_Numbers, temp);
            if (matched.second != '\0') // match found
            {
                auto matched_substr = content.substr(bot, matched.first.length());
                if (matched_substr == matched.first) // is matched substr fully spelt in content?
                {
                    token.type = TokenType::WORDED_NUMBER;
                    token.value = matched.second;
                    token.pos = cursor - 3;      // look_ahead value (3)
                    cursor += (matched.first.length() - temp.length() - 1); // move cursor to the beginning of next token | -1 => fixes word overlap e.g. 'twone'
                    bot = cursor;
                    temp = "";
                    return token;
                }
            }
            bot++;
            cursor = bot;
            temp = "";
            return next();
        }

        char item = content[cursor];

        if (std::isdigit(item))
        {
            token.type = TokenType::DIGIT;
            token.value = item;
            token.pos = cursor;
            cursor++;
            bot = cursor;
            temp = "";
            return token;
        }
        else
        {
            temp += item;
            cursor++;
            return next();
        }

        cursor++;
        temp = "";
        return next();
    }

private:
    size_t cursor;
    size_t bot; // beginning of token
    std::string_view content;
    std::string temp;
};

void testLexer(std::string_view text)
{
    Lexer lexer;
    Token_t token{};
    lexer.init(text);
    do
    {
        token = lexer.next();
        printToken(token);
        std::cout << "\tCursor: " << lexer.getCursor() << "\n";
    } while (token.type != TokenType::END);
    std::cout << std::endl;
}

int main()
{

    std::string filename{"../puzzle_input.txt"};
    std::string filename_out{"puzzle_out.txt"};

    std::fstream fs{filename, fs.in};
    if (!fs.is_open())
    {
        std::cerr << "Failed to open file: '" << filename << "'" << std::endl;
        return 99;
    }
    std::ofstream ofs{filename_out};
    if (!ofs.is_open())
    {
        std::cerr << "Failed to open file: '" << filename_out << "'" << std::endl;
        return 89;
    }

    Lexer lexer;
    std::string data{};
    std::string numstr = "";
    std::string tmp = "";
    Token_t token{};
    int sum = 0;

    while (std::getline(fs, data))
    {
        lexer.init(data);
        do
        {
            token = lexer.next();
            if (token.value != '\0')
            {
                numstr.append(1, token.value);
                // numstr += token.value;
            }
            // printToken(token);
        } while (token.type != TokenType::END);
        tmp.append(1, numstr[0]);
        tmp.append(1, numstr[numstr.size() - 1]);
        // tmp += numstr[0];
        // tmp += numstr[numstr.length()-1];
        int calibration_num = std::stoi(tmp);
        sum += calibration_num;

        ofs << data << " => " << "Numstr: " << numstr << " => " << tmp << "\n";

        tmp = "";
        numstr = "";
    }

    ofs << "========================\nCalibration Sum: " << sum << std::endl;
    std::cout << "Calibration Sum: " << sum << std::endl;

    // testLexer("nsztxcjeightplmcdkhbnxfclbqqq55vqxqv7");
    // testLexer("czd6dngcnssxfjrqxgjksktonevrljgzbsrgg");
    // testLexer("two1nine");
    // std::cout << "===================" << std::endl;
    // testLexer("eightwothree");
    // std::cout << "===================" << std::endl;
    // testLexer("abcone2threexyz");
    // std::cout << "===================" << std::endl;
    // testLexer("xtwone3four");
    // std::cout << "===================" << std::endl;
    // testLexer("4nineeightseven2");
    // std::cout << "===================" << std::endl;
    // testLexer("zoneight234");
    // std::cout << "===================" << std::endl;
    // testLexer("7pqrstsixteen");

    return 0;
}
