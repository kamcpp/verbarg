/* 
 *   Verbarg: Verbal argument parser for C++
 *   Copyright (C) 2017  Kamran Amini <kam.cpp@gmail.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <algorithm>
#include <vector>
#include <map>


struct Args {
  std::string verb;
  std::map<std::string, std::string> pairs;

  bool HasKey(std::string key) {
    return pairs.find(key) != pairs.end();
  }
};

struct Param {
  std::string key;
  std::vector<std::string> literals;
  std::string description;
  bool optional;
  bool has_value;
};

std::vector<std::string> VERBS = {
  "help"
};

std::map<std::string, std::string> VERB_DESC = {
  {"help", "Print this help message."}
};

std::map<std::string, std::vector<Param>> PARAMS = {
  {"help", {}}
};

#define NOT_OPTIONAL false
#define OPTIONAL true
#define HAS_VALUE true
#define HAS_NO_VALUE false

#define LIST(...) {__VA_ARGS__}
#define PARAM(k, ls, d, o, hv) {k, ls, d, o, hv}
#define DEFINE_VERB(v, vd, ps) \
  VERBS.push_back(v); \
  VERB_DESC[v] = vd; \
  PARAMS[v] = ps; \


void PrintFullHelpMessage(int argc, char **argv, Args &args);
void PrintListOfVerbs(int argc, char **argv, Args &args);
bool IsVerbValid(Args &args);
int ProcessArgs(int argc, char **argv, Args &args);

void PrintFullHelpMessage(int argc, char **argv, Args &args) {
  std::cout << "Usage: bip [verb] [param value] [param value] ..." << std::endl;
  PrintListOfVerbs(argc, argv, args);
}

void PrintListOfVerbs(int argc, char **argv, Args &args) {
  std::cout << "List of verbs:" << std::endl;
  for (auto verb : VERBS) {
    std::cout << "  " << verb << "              " << VERB_DESC[verb] << std::endl;  
  }
}

bool IsVerbValid(std::string verb) {
  return std::find(VERBS.begin(), VERBS.end(), verb) != VERBS.end();
}

int ProcessArgs(int argc, char **argv, Args &args) {
  if (argc < 2) {
    PrintFullHelpMessage(argc, argv, args);
    return 1;
  }
  args.verb = argv[1];
  if (not IsVerbValid(args.verb)) {
    std::cout << "ERROR: '" << args.verb << "' is not a valid verb!" << std::endl;
    PrintListOfVerbs(argc, argv, args);
    return 1;  
  }
  Param param_obj;
  std::string param = "";
  std::string param_value = "";
  bool param_seen = false;  
  for (uint32_t i = 2; i < argc; i++) {
    if (not param_seen) {
      bool found = false;
      for (auto p : PARAMS[args.verb]) {
        if (std::find(p.literals.begin(), p.literals.end(), argv[i]) != p.literals.end()) {
          found = true;
          param_obj = p;
          break;
        }
      }
      if (not found) {
        std::cout << "ERROR: '" << argv[i] << "' is not a valid parameter for verb '" << args.verb << "'" << std::endl;
        return 1;
      }
      param = param_obj.key;
      if (param_obj.has_value) {
        param_seen = true;
      } else {
        args.pairs[param] = "";
      }
    } else {
      param_value = argv[i];
      args.pairs[param] = param_value;
      param_seen = false;
    }
  }
  // TODO(kamran): Validate params.
  return 0;
}