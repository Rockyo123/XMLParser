// Project 3 -- XML Parsing Project

/** XML parsing class implementation.
    @file XMLParser.cpp */

#include <string>
#include <assert.h>
#include "XMLParser.hpp"

XMLParser::XMLParser()
{
	parseStack = new Stack<string>;
	elementNameBag = new Bag<string>;
}  // end default constructor

XMLParser::~XMLParser()
{
	clear();
	delete parseStack;
	parseStack = nullptr;
	delete elementNameBag;
	elementNameBag = nullptr;
}  // end destructor


static std::string deleteAttributes(std::string input)
{
	string temp = input;
	
	temp.erase(0, 1);
	temp.erase(temp.length()-1, 1);
	int cut = temp.length();
	if(temp[0] == '/' || temp[0] == '?')
	{
		temp.erase(0, 1);
	}
	if(temp[temp.length() - 1] == '?' || temp[temp.length() - 1] == '/')
	{
		temp.erase(temp.length()-1, 1);
	}
	for(int i = 0; i < temp.length(); i++)
	{
		if(temp[i] == ' ')
		{
			cut = i - 1;
			break;
		}
	}
	temp = temp.substr(0, cut+1);
	return temp;	
}

bool XMLParser::tokenizeInputString(const std::string &inputString)
{
	string currentTag = "";
	string currentContent = "";
	bool isContent = false;
	int i = 0;
	while(inputString[i] == ' ')
	{
		i++;
	}

	string curString = inputString;
	int j = curString.length()-1;
	while(curString[j] == ' ')
	{
		curString.erase(j,1);
		j--;
	}
	
	//checking to see that inputString begins and ends with tags
	if((curString[i] != '<') || (curString[curString.length()-1] != '>'))
	{
		clear();
		return false;
	}
	//parsing through string and seperating it into tokens, making sure its valid
	while(i < curString.length())
	{
		//in a tag, will parse through tag and add it to tokenizedInputVector
		if(curString[i] == '<')
		{
			currentTag = "<";
			i++;
			while(curString[i] != '>' && i < curString.length())
			{
				if(curString[i] == '<')
				{
					clear();
					return false;
				}
				else
				{
				currentTag = currentTag + curString[i];
				i++;
				}
			}
			if(curString[i] != '>')
			{
				clear();
				return false;
			}
			currentTag = currentTag + '>';
			if(!(checkifValidToken(currentTag)))
			{
				clear();
				return false;
			}
			TokenStruct newToken;
			newToken.tokenString = currentTag;

			//check what type of Tag newToken is
			if(newToken.tokenString[1] == '/')
			{
				newToken.tokenType = END_TAG;
			}
			else if(newToken.tokenString[1] == '?')
			{
				newToken.tokenType = DECLARATION;
			}
			else if(newToken.tokenString[currentTag.length()-2] == '/')
			{
				newToken.tokenType = EMPTY_TAG;
			}
			else
			{
				newToken.tokenType = START_TAG;
			}
			if(!(newToken.tokenType == DECLARATION))
			{
				newToken.tokenString = deleteAttributes(newToken.tokenString);
			}
			else if(newToken.tokenType == DECLARATION)
			{
				newToken.tokenString.erase(0,1);
				newToken.tokenString.erase(0,1);
				newToken.tokenString.pop_back();
				newToken.tokenString.pop_back();
			}
			tokenizedInputVector.push_back(newToken);
			currentTag = "";
			i++;
		}
		//is not in tag, go through content and add it as content
		while(curString[i] != '<' && curString[i] != '>' && i < curString.size())
		{
			currentContent = currentContent + curString[i];
			isContent = true;
			i++;
		}
		if(isContent)
		{
			TokenStruct newToken;
			newToken.tokenString = currentContent;
			newToken.tokenType = CONTENT;
			bool push = true;
			string test = newToken.tokenString;
			for(int i = 0; i< test.length(); i++)
			{
				if(test[i] == ' ')
				{
					test.erase(i);
				}
			}
				if(test.find_first_not_of("\n") == std::string::npos)
				{
					push = false;
				}
			if(push)
			{			
				tokenizedInputVector.push_back(newToken);
			}
			currentContent = "";
			isContent = false;
		}
		//cannot have > outside of tag, also checks for two < at end
		if((curString[i] == '>' && i != (curString.length() - 1)) || (curString[i] == '>' && (curString[i-1] == '>')))
		{
			clear();
			return false;
		}
	}		
	return true;
}  // end


bool XMLParser::parseTokenizedInput()
{
	if(tokenizedInputVector.size() == 0)
	{
		return false;
	}
	//making sure doesn't start with end tag
	if(tokenizedInputVector[0].tokenType != START_TAG && tokenizedInputVector[0].tokenType != DECLARATION)
	{
		return false;
	}
	string name = "";
	for(int i = 0; i < tokenizedInputVector.size(); i++)
	{
		if(tokenizedInputVector[i].tokenType == START_TAG)
		{
			name = tokenizedInputVector[i].tokenString;
			elementNameBag -> add(name);
			parseStack -> push(name);
		}
		if(tokenizedInputVector[i].tokenType == END_TAG)
		{
			
			name = tokenizedInputVector[i].tokenString;
			if(!(parseStack -> peek() == name))
			{
				clear();
				return false;
			}
			parseStack -> pop();
		}
		if(tokenizedInputVector[i].tokenType == EMPTY_TAG)
		{
			name = tokenizedInputVector[i].tokenString;
			elementNameBag -> add(name);
		}	
		if(tokenizedInputVector[i].tokenType == DECLARATION)
		{
			if(i != 0)
			{
				clear();
				return false;
			}
			else
			{
				name = tokenizedInputVector[i].tokenString;
				elementNameBag -> add(name);
			}
			if(name != "Declaration")
			{
				return false;
			}

		}
	}
	if(!(parseStack->isEmpty()))
	{
		clear();
		return false;
	}	
	return true;	
}

void XMLParser::clear()
{
	elementNameBag -> clear();
	parseStack -> clear();	
	tokenizedInputVector.clear();
}

vector<TokenStruct> XMLParser::returnTokenizedInput() const
{
	return tokenizedInputVector;
}

bool XMLParser::containsElementName(const std::string &inputString) const
{
	if(elementNameBag->isEmpty()) throw std::logic_error("tokenizeInputString or parseTokenizedInput returned false");
		if(elementNameBag -> contains(inputString))
		{
			return true;
		}
		else
		{
			return false;
		}
}

int XMLParser::frequencyElementName(const std::string &inputString) const
{
	if(elementNameBag->isEmpty()) throw std::logic_error("tokenizeInputString or parseTokenizedInput returned false");
	if(!(elementNameBag->isEmpty()))
	{
		return elementNameBag -> getFrequencyOf(inputString);
	}
	else
	{
		return 0;
	}
}

bool XMLParser::checkifValidToken(string &token)
{
	if(token[1] == ' ')
	{
		return false;
	}
	if(token[1] == '?' && token[token.length()-2] != '?')
	{
		return false;
	}	
	string name = deleteAttributes(token);

	bool contains_bad = name.find_first_of("!#$%&()*+,/;<=>?@[]^\\\"\'`{|}~") != std::string::npos;;
	if(contains_bad)
	{
		return false;
	}
	if(name[0] == '-' || name[0] == '.' || name[0] == '0' || name[0] == '1' || name[0] == '2' || name[0] == '3' || name[0] == '4' || name[0] == '5' || name[0] == '6' || name[0] == '7' || name[0] == '8' || name[0] == '9' || name[0] == ' ')
	{
	return false;
	}

	return true;
}