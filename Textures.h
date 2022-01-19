#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

using namespace std;

class Textures {
	static unordered_map<string, sf::Texture> textures;
	static unordered_map<int, sf::Texture> nums;
	static void load(string name);
	static void loadDigits();
public:
	static sf::Texture& get(string name);
	static sf::Texture& num(int n);
	static void clear();
};

