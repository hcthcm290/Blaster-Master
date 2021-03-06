#pragma once
#include <Windows.h>

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <string>


using namespace std;

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);

float RandomFloat(float a, float b);
std::wstring s2ws(const std::string& s);

string IntToString(int x, int length);
string IntToString(int x);