/* Start Header ------------------------------------------------------- 
Copyright (C) 2013 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of 
DigiPen Institute of Technology is prohibited. 
File Name: Parser.h
Purpose: Utility functions for parsing strings.
Language: C++
Platform: Windows, Mircosoft C++ compiler
Project: Kevin.Joseph 80003910, Kevin.Joseph_CS300_1
Author: Kevin Gray, Kevin.Joseph 80003910
Creation date: January 28, 2011
End Header --------------------------------------------------------*/


#pragma once
#include "Parser.h"

namespace Parser
{
  //Delimeter is space ' '
  void ParseString(std::string &line, std::vector<std::string> &tokens)
  {
    unsigned start_token = 0;
    unsigned off_set = 1;
    bool in_token = false;
    for(unsigned i = 0; i < line.size(); i++)
    {
      if(line[i] == ' ' || line[i] == '/')
      {
        //reached the end of a token, get the substring
        if(in_token == true)
        {
          std::string token = line.substr(start_token, off_set);
          tokens.push_back(token);
          in_token = false;
        }
        //else just continue
      }
      else
      {
        //move the offset if we are in the token
        if(in_token == true)
        {
          off_set++;
        }
        //start of a token
        else
        { 
          start_token = i;
          off_set = 1;
          in_token = true;
        }
      }
    }
    if(in_token)
    {
      std::string token = line.substr(start_token, off_set);
      tokens.push_back(token);
    }
  }
}