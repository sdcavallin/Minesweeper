#include "Textures.h"

// "redeclare" the variable so this file knows it exists and can use it
unordered_map<string, sf::Texture> Textures::textures;
unordered_map<int, sf::Texture> Textures::nums;

void Textures::load(string name) {
	string path = "images/" + name + ".png";

	textures[name].loadFromFile(path);
}

sf::Texture& Textures::get(string name) {

	if (textures.find(name) == textures.end()) {
		load(name);
	}
	return textures[name];
}

void Textures::loadDigits() {

	for (int i = 0; i <= 10; i++) {
		/*nums[i].create(21, 32);
		nums[i].update(digits, i * 21, 0);*/

		nums[i].loadFromFile("images/digits.png", sf::IntRect(i * 21, 0, 21, 32));
	}
}

sf::Texture& Textures::num(int n) {
	if (nums.find(n) == nums.end()) {
		loadDigits();
	}
	if (n >= 10) {
		n = 10;
	}
	return nums[n];
}

void Textures::clear() {
	textures.clear();
}