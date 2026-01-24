#ifndef PASSWORDQL_H
#define PASSWORDQL_H

#include <string>

void TextColor3(int x);

void passInput(std::string &pass);

bool Manager(const std::string& inputUsername, const std::string& inputPass);

int resetPass(const std::string& currentPass, const std::string& newPass, const std::string& retypePass);

#endif
