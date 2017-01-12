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

#include <iostream>
#include <iomanip>
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

static std::vector<std::string> VERBS = {
  "help"
};

static std::map<std::string, std::string> VERB_DESC = {
  {"help", "Print this help message."}
};

static std::map<std::string, std::vector<Param>> PARAMS = {
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


static void PrintUsageMessage(int argc, char **argv, Args &args);
static void PrintListOfVerbs(int argc, char **argv, Args &args);
static void PrintHelpForVerb(int argc, char **argv, Args &args, std::string target_verb);
static bool IsVerbValid(std::string verb);
static int ProcessArgs(int argc, char **argv, Args &args);

void PrintUsageMessage(int argc, char **argv, Args &args) {  
  std::cout << "Usage: " << argv[0] << " [verb] [param value] [param value] ..." << std::endl;
  PrintListOfVerbs(argc, argv, args);
}

void PrintListOfVerbs(int argc, char **argv, Args &args) {
  std::cout << std::endl;
  std::cout << "List of verbs:" << std::endl;
  for (auto verb : VERBS) {
    std::cout << "    " << std::left << std::setw(20) << verb;
    std::cout << "> " << VERB_DESC[verb] << std::endl;
  }
  std::cout << std::endl;
  std::cout << "Please type '" << argv[0] << " help [verb]' to see help for each verb." << std::endl;
  std::cout << std::endl;
}

void PrintHelpForVerb(int argc, char **argv, Args &args, std::string target_verb) {
  uint32_t max_col_width = 50;
  std::cout << std::endl;
  std::cout << "List of parameters for verb '" << target_verb << "':" << std::endl;
  for (auto p : PARAMS[target_verb]) {
    std::string delim = "";
    std::string literals = "";
    for (auto literal : p.literals) {
      literals += delim + literal;
      delim = " | ";
    }
    std::cout << "    " << std::left << std::setw(max_col_width) << literals;
    if (literals.size() >= max_col_width) {
      std::cout << std::endl << "    " << std::right << std::setw(max_col_width + p.description.size() + 2) << "> " + p.description;
    } else {
      std::cout << "> " << p.description;
    }
    if (p.optional) {
      std::cout << " [OPTIONAL]" << std::endl;
    } else {
      std::cout << " [MANDATORY]" << std::endl;
    }
  }
  std::cout << std::endl;
}

bool IsVerbValid(std::string verb) {
  return std::find(VERBS.begin(), VERBS.end(), verb) != VERBS.end();
}

int ProcessArgs(int argc, char **argv, Args &args) {
  if (argc < 2) {
    PrintUsageMessage(argc, argv, args);
    return 1;
  }
  args.verb = argv[1];
  if (args.verb == "help") {
    if (argc == 2) {
      PrintUsageMessage(argc, argv, args);
      return 1;
    } else {
      std::string target_verb = argv[2];
      if (not IsVerbValid(target_verb)) {
        std::cout << "ERROR: '" << target_verb << "' is not a valid verb!" << std::endl;
        PrintListOfVerbs(argc, argv, args);
        return 1;  
      }
      PrintHelpForVerb(argc, argv, args, target_verb);
      return 1;
    }
  }
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
        PrintUsageMessage(argc, argv, args);
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
  for (auto p : PARAMS[args.verb]) {
    if (not p.optional) {
      if (args.pairs.find(p.key) == args.pairs.end()) {
        std::cout << "ERROR: '" << p.key << "' parameter should be provided by one of the literalrs [";
        std::string delim = "";
        for (auto literal : p.literals) {
          std::cout << delim << literal;
          delim = ", ";
        }
        std::cout << "]." << std::endl;
        PrintHelpForVerb(argc, argv, args, args.verb);
        return 1;
      }
    }
  }
  return 0;
}