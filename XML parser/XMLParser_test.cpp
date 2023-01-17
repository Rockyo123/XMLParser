#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include <iostream>
#include "catch.hpp"
#include "XMLParser.hpp"

using namespace std;

// TODO Implement tests of your Stack class and XMLParser class here


TEST_CASE( "Test Bag add", "[XMLParser]" )
{
	   INFO("Hint: testing Bag add()");
		// Create a Bag to hold ints
		Bag<int> intBag;
		int testSize = 2;
		int bagSize;
		bool success;
		for (int i=0; i<testSize; i++) {
			success = intBag.add(i);
			REQUIRE(success);
			bagSize = intBag.size();
			success = (bagSize == (i+1));
			REQUIRE(success);
		}
}

TEST_CASE( "Testing stack", "[XMLParser]" )
{
	INFO("Hint: testing Stack push()");
		// Create a Stack to hold strings
		Stack<string> stringStack;
		int testSize = 3;
		stringStack.push("test");
		REQUIRE(stringStack.peek() == "test");
		REQUIRE(stringStack.size() == 1);
		stringStack.pop();
		stringStack.push("string");
		REQUIRE(stringStack.size() == 1);
		REQUIRE(stringStack.peek() == "string");
}



TEST_CASE( "Test XMLParser tokenizeInputString", "[XMLParser]" )
{
	   INFO("Hint: tokenize single element test of XMLParse");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		string testString = "<test>stuff</test>";
		string testStringLong = "<test>stuff</test><test2>stuff</test2>";
		string testString2 = "test>stuff</test>";
		string testString3 = "<test>stuff</test";
		string testString4 = "<test>>stuff</test";
		string testString5 = "<<test>stuff</test>";
		string testString6 = "<test>stuff<</test>";
		string testString7 = "<test>stuff</test>>";	
		bool success;
		success = myXMLParser.tokenizeInputString(testString);
		REQUIRE(success);
		myXMLParser.clear();
		success = myXMLParser.tokenizeInputString(testStringLong);
		REQUIRE(success);
		success = myXMLParser.tokenizeInputString(testString2);	
		REQUIRE(!success);
		success = myXMLParser.tokenizeInputString(testString3);
		REQUIRE(!success);
		success = myXMLParser.tokenizeInputString(testString4);
		REQUIRE(!success);
		success = myXMLParser.tokenizeInputString(testString5);
		REQUIRE(!success);
		success = myXMLParser.tokenizeInputString(testString6);
		REQUIRE(!success);
		success = myXMLParser.tokenizeInputString(testString7);
		REQUIRE(!success); 
}	
	
TEST_CASE( "Testing deleteAttributes and validToken", "[XMLParser]" )
{
	XMLParser myXMLParser;
	string test = "<element>";
	REQUIRE(myXMLParser.checkifValidToken(test));
	test = "</element>";
	REQUIRE(myXMLParser.checkifValidToken(test));
	test = "<?element?>";
	REQUIRE(myXMLParser.checkifValidToken(test));
	test = "<element>";
	REQUIRE(myXMLParser.checkifValidToken(test));
	test = "<element osgisofgoishvos>";
	REQUIRE(myXMLParser.checkifValidToken(test));
	test = "<e*lement>";
	REQUIRE(!myXMLParser.checkifValidToken(test));
	test = "<1element osgisofgoishvos>";
	REQUIRE(!myXMLParser.checkifValidToken(test));
	test = "<eleme'nt osgisofgoishvos>";
	REQUIRE(!myXMLParser.checkifValidToken(test));
	test = "<element 123fhoi*|;:~`>";
	REQUIRE(myXMLParser.checkifValidToken(test));

}

TEST_CASE( "Test XMLParser parseTokenizedInput", "[XMLParser]" )
{
	   INFO("Hint: parseTokenizedInput");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		string testString = "   <test>stuff</test>";
		myXMLParser.tokenizeInputString(testString);
		REQUIRE(myXMLParser.parseTokenizedInput());
		myXMLParser.clear();
		string testStringLong = "<intro><test>stuff</test><tests>stuff</tests></intro>";
		myXMLParser.tokenizeInputString(testStringLong);
		REQUIRE(myXMLParser.parseTokenizedInput());
		
}	


TEST_CASE( "Test XMLParser getFrequency and contains", "[XMLParser]" )
{
	   INFO("Hint: getFrequency and contains");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		string testString = "<test>stuff</test>";
		myXMLParser.tokenizeInputString(testString);
		REQUIRE(myXMLParser.parseTokenizedInput());
		REQUIRE(myXMLParser.frequencyElementName("test") == 1);
		REQUIRE(myXMLParser.containsElementName("test"));
		myXMLParser.clear();
		string testStringLong = "<intro><test>stuff</test><tests>stuff</tests></intro>";
		myXMLParser.tokenizeInputString(testStringLong);
		REQUIRE(myXMLParser.parseTokenizedInput());
		REQUIRE(myXMLParser.frequencyElementName("tests") == 1);
		REQUIRE(myXMLParser.containsElementName("tests"));
}	


TEST_CASE( "Testing declarations", "[XMLParser]" )
{
	   INFO("Hint: testing declarations");
		// Create an instance of XMLParse
		XMLParser myXMLParser;
		string testString = "   <?Declaration?>     <test> stuff </test>    ";
		REQUIRE(myXMLParser.tokenizeInputString(testString));
}	