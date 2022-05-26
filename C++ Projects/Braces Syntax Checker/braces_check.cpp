// Takes a code file as input and checks if all of the braces are syntactially correct

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

std::vector<char> getBracesIndecies(std::string input);
bool isValidBraces(std::vector<char> braces);

const char openBraces[3] = {'(', '[', '{'};
const char closedBraces[3] = {')', ']', '}'};

int main(int argc, char** argv)
{
    std::ifstream ifs(argv[1]);
    std::string input((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
    std::vector<char> braces = getBracesIndecies(input);
    bool result = isValidBraces(braces);
    if (result == 1)
        std::cout << "All braces are valid" << std::endl;
    else
        std::cout << "Braces are invalid" << std::endl;
    return 0;
}

std::vector<char> getBracesIndecies(std::string input)
{
    std::vector<char> braces;
    for(unsigned long int i = 0; i < input.length(); i++)
    {
        if(input[i] == openBraces[0] || input[i] == openBraces[1] || input[i] == openBraces[2] ||
        input[i] == closedBraces[0] || input[i] == closedBraces[1] || input[i] == closedBraces[2])
        {
            braces.push_back(input[i]);
        }
    }
    return braces;
}

bool isValidBraces(std::vector<char> braces) 
{
    std::stack<char> bracesStack;

    for (unsigned long int i = 0; i <= braces.size() - 1; i++)
    {
        const char *charPtr = std::find(openBraces, openBraces + 3, braces[i]);
    
        // if brace is an open brace, add it to the stack
        if (charPtr != openBraces + 3)
        {
            bracesStack.push(braces[i]);
        }
        // if char is not an open brace and the stack is not empty
        else if (charPtr == openBraces + 3 && !bracesStack.empty())
        {
            const char *closedBracesPositionPtr = std::find(closedBraces, closedBraces + 3, braces[i]);
      
            // if char is a closed brace
            if (closedBracesPositionPtr != closedBraces + 3)
            {
                // if the top brace on the stack matches the closing brace, remove the opening brace from the stack
                if (bracesStack.top() == openBraces[closedBracesPositionPtr - closedBraces])
                {
                    bracesStack.pop();
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    // if all open braces had matches
    if (bracesStack.empty()) 
        return true;
    else
        return false;
}
