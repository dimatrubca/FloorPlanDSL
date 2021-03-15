#pragma once

bool isLetter(char ch) {
     return ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z' || ch == '_';
}

bool isDigit(char ch) {
     return ch >= '0' && ch <= '9';
}

bool isAlpha(char ch) {
     return isLetter(ch) || isDigit(ch);
}



