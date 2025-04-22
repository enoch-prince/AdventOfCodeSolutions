def solve_part2(file_path, output_file):
    digit_words = {
        'zero': '0',
        'one': '1',
        'two': '2',
        'three': '3',
        'four': '4',
        'five': '5',
        'six': '6',
        'seven': '7',
        'eight': '8',
        'nine': '9'
    }
    total = 0
    with open(file_path, 'r') as file, open(output_file, 'w') as outfile:
        for line in file:
            first_digit = None
            last_digit = None
            # Find first digit
            for i in range(len(line)):
                if line[i].isdigit():
                    first_digit = line[i]
                    break
                found_word = False
                for word in digit_words:
                    if line.startswith(word, i):
                        first_digit = digit_words[word]
                        found_word = True
                        break
                if found_word:
                    break
            # Find last digit
            for i in range(len(line)-1, -1, -1):
                if line[i].isdigit():
                    last_digit = line[i]
                    break
                found_word = False
                for word in digit_words:
                    if i - len(word) + 1 >= 0 and line.startswith(word, i - len(word) + 1):
                        last_digit = digit_words[word]
                        found_word = True
                        break
                if found_word:
                    break
            if first_digit is not None and last_digit is not None:
                line = line.rstrip("\n")
                outfile.write(f"{line} => {first_digit + last_digit}\n")
                total += int(first_digit + last_digit)
        outfile.write(f"===========\nCalibration: {total}")
    return total

print(solve_part2("puzzle_input.txt", "py_out.txt"))