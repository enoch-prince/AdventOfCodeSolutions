/**
 * @file code.cpp
 * @author Enoch Prince (@enoch.prince)
 * @brief
 * @version 0.1
 * @date 2025-04-09
 *
 * [Advent of Code - 2023 - Day](https://adventofcode.com/2023/day/1)
 * Task: On each line, the calibration value can be found by combining
 * the first digit and the last digit (in that order) to form a single
 * two-digit number. Sum up all the two digit numbers to solve the puzzle.
 *
 * Plan
 * ----
 * 1. Read a text file
 * 2. Determine appropriate data structure for the input
 * 3. Determine appropriate algorithm to extract the digits
 * 4. Sum the digits up
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <numeric>


//Side Note: string_view is used instead of const& std::string for performance

int main()
{
   std::string filename{"../puzzle_input.txt"};
   std::string filename_out{"puzzle_out.txt"};

   std::fstream fs{filename, fs.in};
   if (!fs.is_open()) {
      std::cerr << "Failed to open file: '" << filename << "'" << std::endl;
      return 99;
   }
   std::ofstream ofs{filename_out};
   if (!ofs.is_open()) {
      std::cerr << "Failed to open file: '" << filename_out << "'" << std::endl;
      return 89;
   }

   std::string data{};
   std::string numstr = "";
   std::string numstr_tmp = "";

   std::array<int, 1000> calibration_values{0};
   int array_index = 0;

   while (std::getline(fs, data)) {
      // extract digits from data
      for (auto itr = data.begin(); itr != data.end(); itr++) {
         if (std::isdigit(*itr)) {
            numstr += *itr;
         }
      }
      
      if (numstr != "") {
         // extract the first & last digits
         numstr_tmp += numstr[0];
         numstr_tmp += numstr[numstr.size()-1];
         // convert extracted digits to number + store in data structure
         calibration_values[array_index] = std::stoi(numstr_tmp);
         // print result into the output file
         ofs << data << " => " << "Numstr: " << numstr_tmp << " => " << calibration_values[array_index] << "\n";
      }

      // reset variables
      numstr = ""; 
      numstr_tmp = "";
      array_index++;
   }

   int calibration_sum = std::accumulate(calibration_values.begin(), calibration_values.end(), 0);
   ofs << "========================\nCalibration Sum: " << calibration_sum << std::endl;
   std::cout << "Calibration Sum: " << calibration_sum << std::endl;
   
   return 0;
}